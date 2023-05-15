import numpy as np
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt


def calc_probs(rnd):
    probs = dict()
    values, counts = np.unique(rnd, return_counts=True)

    for num, count in zip(values, counts):
        probability = count / len(rnd)
        probs[num] = probability

    return probs


def math_exp(probs):
    me = 0
    for x in probs:
        me += x * probs[x]
    return me


def st_deviation(rnd, n):
    average = 0
    for x in rnd:
        average += x
    average /= n

    sigma = 0
    for x in rnd:
        sigma += (x - average) * (x - average)
    sigma = np.sqrt(sigma / n)

    return sigma


def graph_me(rnd):
    xs = [i * 500 for i in range(1, 21)]
    ys = []
    for x in xs:
        ys.append(math_exp(calc_probs(rnd[0:x])))

    absolute = abs(ys[9] - ys[-1])
    relative = absolute / ys[9] * 100
    print('Относительная погрешность измерения математического ожидания для выборки из {} элементов: '
          '{}%'.format(xs[9], round(relative, 3)))

    df = pd.DataFrame({'Объём выборки': xs, 'Математическое ожидание': ys})

    sns.scatterplot(data=df, x='Объём выборки', y='Математическое ожидание')
    plt.show()


def graph_sigma(rnd):
    xs = [i * 500 for i in range(1, 21)]
    ys = []
    for x in xs:
        ys.append(st_deviation(rnd[0:x], x))

    absolute = abs(ys[9] - ys[-1])
    relative = absolute / ys[9] * 100
    print('Относительная погрешность измерения среднеквадратичного отклонения для выборки из {} элементов: '
          '{}%'.format(xs[9], round(relative, 3)))

    df = pd.DataFrame({'Объём выборки': xs, 'Cреднеквадратичное отклонение': ys})

    sns.scatterplot(data=df, x='Объём выборки', y='Cреднеквадратичное отклонение')
    plt.show()


if __name__ == '__main__':
    with open('rnd.dat', 'r') as infile:
        rnd = [int(i) / 1024 for i in infile.read()[:-1].split(',')]
        n = len(rnd)

    graph_me(rnd)
    graph_sigma(rnd)
