from functools import reduce

import numpy as np
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import math
from scipy.stats import chi2


def calc_probs(u):
    probs = dict()
    values, counts = np.unique(u, return_counts=True)

    for num, count in zip(values, counts):
        probability = count / len(u)
        probs[num] = probability

    return probs


def count_elems(u):
    element_count = {}
    for element in u:
        if element in element_count:
            element_count[element] += 1
        else:
            element_count[element] = 1

    return element_count


def math_exp(probs):
    me = 0
    for x in probs:
        me += x * probs[x]
    return me


def st_deviation(u, n):
    average = 0
    for x in u:
        average += x
    average /= n

    sigma = 0
    for x in u:
        sigma += (x - average) * (x - average)
    sigma = np.sqrt(sigma / n)

    return sigma


def graph_me(u):
    xs = [i * 500 for i in range(1, 21)]
    ys = []
    for x in xs:
        ys.append(math_exp(calc_probs(u[0:x])))

    absolute = abs(ys[9] - ys[-1])
    relative = absolute / ys[9] * 100
    print('Относительная погрешность измерения математического ожидания для выборки из {} элементов: '
          '{}%'.format(xs[9], round(relative, 3)))

    df = pd.DataFrame({'Объём выборки': xs, 'Математическое ожидание': ys})

    sns.scatterplot(data=df, x='Объём выборки', y='Математическое ожидание')
    plt.show()


def graph_sigma(u):
    xs = [i * 500 for i in range(1, 21)]
    ys = []
    for x in xs:
        ys.append(st_deviation(u[0:x], x))

    absolute = abs(ys[9] - ys[-1])
    relative = absolute / ys[9] * 100
    print('Относительная погрешность измерения среднеквадратичного отклонения для выборки из {} элементов: '
          '{}%'.format(xs[9], round(relative, 3)))

    df = pd.DataFrame({'Объём выборки': xs, 'Cреднеквадратичное отклонение': ys})

    sns.scatterplot(data=df, x='Объём выборки', y='Cреднеквадратичное отклонение')
    plt.show()


def chi_square(observed=None, expected=None, k=None, alpha=0.05):
    if observed is None:
        observed = np.array(list(count_elems(u).values()))
    if k is None:
        k = observed.shape[0]
    if expected is None:
        expected = np.array([n / k for _ in range(k)])

    diff = observed - expected
    squared_diff = np.square(diff)
    chi_squared = np.sum(squared_diff / expected)

    critical = chi2.ppf(1 - alpha, k - 1)
    # print(chi_squared, critical)
    return chi_squared <= critical


def series(d=8):
    observed = np.array((d * d) * [0])
    for i in range(n // 2):
        q, r = math.floor(u[2 * i] * d), math.floor(u[2 * i + 1] * d)
        observed[q * d + r] += 1

    expected = np.array(d ** 2 * [n / (2 * d ** 2)])

    return chi_square(observed, expected, d * d)


def intervals(d=16):
    j, s, count = -1, 0, 8 * [0]
    num_of_intervals = n / 10

    while s != num_of_intervals and j != n:
        j, r = j + 1, 0

        while j != n and u[j] * d < d / 2:
            j, r = j + 1, r + 1

        count[min(r, 7)] += 1
        s += 1

    if j == n:
        raise Exception("Последовательность исчерпана, найдено %d из %d отрезков." % (s, num_of_intervals))

    pd = 0.5
    expected = [num_of_intervals * pd * (1.0 - pd) ** r for r in range(7)] + [num_of_intervals * (1.0 - pd) ** 7]

    return chi_square(np.array(count), np.array(expected), 8)


def stirling(n, k):
    n1 = n
    k1 = k
    if n <= 0:
        return 1

    elif k <= 0:
        return 0

    elif n == 0 and k == 0:
        return -1

    elif n != 0 and n == k:
        return 1

    elif n < k:
        return 0

    else:
        temp1 = stirling(n1 - 1, k1)
        temp1 = k1 * temp1
        return (k1 * (stirling(n1 - 1, k1))) + stirling(n1 - 1, k1 - 1)


def poker(d=16):
    observed = 5 * [0]
    for i in range(n // 5):
        hand = [math.floor(v * d) for v in u[i * 5:i * 5 + 5]]
        unique = [v in hand for v in range(d)]
        distinct = reduce(lambda a, b: a + b, unique, 0)
        observed[distinct - 1] += 1

    k = 5
    expected = 5 * [0]
    for r in range(1, 6):
        p = 1.0
        for i in range(r):
            p *= d - i
        expected[r - 1] = (n / 5) * (p / d ** k) * stirling(k, r)

    return chi_square(np.array(observed), np.array(expected), k)


def permutation(t=4, d=1024):
    t_fact = math.factorial(t)
    observed = t_fact * [0]
    for i in range(n // t):
        v = [v * d for v in u[t * i:t * i + t]]
        c = t * [0]
        r = t

        while r > 0:
            s = 0
            for j in range(r):
                if v[j] > v[s]:
                    s = j
            c[r - 1] = s
            v[r - 1], v[s] = v[s], v[r - 1]
            r -= 1

        f = 0
        for j in range(t - 1):
            f = (f + c[j]) * (j + 2)
        f += c[t - 1]
        observed[f] += 1

    expected = t_fact * [n / t / t_fact]

    return chi_square(np.array(observed), np.array(expected), t_fact)


def run(d=1024):
    last, ln = u[0] * d, 1
    observed = 7 * [0]
    for i in u[1:]:
        y = i * d
        if y > last:
            last, ln = y, ln + 1
        else:
            observed[min(ln, 6)] += 1
            last, ln = y, 1

    observed[min(ln, 6)] += 1

    a = [[4529.4, 9044.9, 13568.0, 18091.0, 22615.0, 27892.0],
         [9044.9, 18097.0, 27139.0, 36187.0, 45234.0, 55789.0],
         [13568.0, 27139.0, 40721.0, 54281.0, 67852.0, 83685.0],
         [18091.0, 36187.0, 54281.0, 72414.0, 90470.0, 111580.0],
         [22615.0, 45234.0, 67852.0, 90470.0, 113262.0, 139476.0],
         [27892.0, 55789.0, 83685.0, 111580.0, 139476.0, 172860.0]]
    b = [1.0 / 6.0, 5.0 / 24.0, 11.0 / 120.0, 19.0 / 720.0, 29.0 / 5040.0, 1.0 / 840.0]

    expected = 0.0
    for i in range(6):
        for j in range(6):
            expected += (observed[i + 1] - n * b[i]) * (observed[j + 1] - n * b[j]) * a[i][j]
    expected /= n

    # К данному критерию не применяется хи-квадрат, вместо этого сравнивается статистика этого критерия
    return 1.653 <= expected <= 12.59


def collision():
    m = 128 * n
    v = u.copy()
    v.sort()
    collisions = 0
    for i in range(n - 1):
        if abs(v[i] - v[i + 1]) <= 1 / m:
            print(v[i], v[i + 1])
            collisions += 1

    print(collisions)

    # a = n * [0]
    # a[1] = 1
    # j0, j1 = 1, 1
    # for _ in range(n - 1):
    #     j1 += 1
    #     for j in range(j1, j0, -1):
    #         a[j] = (j / m) * a[j] + ((1 + 1 / m) - (j / m)) * a[j - 1]
    #         if a[j] < 10e-10:
    #             a[j] = 0
    #             if j == j1:
    #                 j1 -= 1
    #             elif j == j0:
    #                 j0 += 1
    # print(a)
    # t_table = [0.01, 0.05, 0.25, 0.5, 0.75, 0.95, 0.99, 1.0]
    # p, t = 0, 1
    # j = j0 - 1
    # while t != len(t_table) - 1 and p <= t_table[t]:
    #     j += 1
    #     p = p + a[j]
    #     t += 1
    #
    # print(1 - p, n - j - 1)


if __name__ == '__main__':
    rnd_name = input('Введите имя файла с ПСЧ ')
    with open(rnd_name, 'r') as infile:
        u = [int(i) for i in infile.read()[:-1].split(',')]
        mx = max(u) + 1
        u = [i / mx for i in u]
        n = len(u)

    graph_me(u)
    graph_sigma(u)

    print('Критерий хи-квадрат', chi_square())
    print('Критерий серий', series())
    print('Критерий интервалов', intervals())
    print('Критерий разбиений', poker())
    print('Критерий перестановок', permutation())
    print('Критерий монотонности', run())
