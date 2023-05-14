import numpy as np


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


if __name__ == '__main__':
    with open('rnd.dat', 'r') as infile:
        rnd = [int(i) / 1024 for i in infile.read()[:-1].split(',')]
        n = len(rnd)

    probs = calc_probs(rnd)
    print(st_deviation(rnd, n))



