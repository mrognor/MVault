import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import csv

records = [1, 10, 100, 1000, 10000, 100000, 1000000]
vault = {}
redis = {}


def load_data(file_name, arr):
    with open(file_name, "r") as data_file:
        readed = csv.reader(data_file)
        counter = 0
        for row in readed:
            if counter > 0:
                arr[int(row[0])] = int(row[1])

            counter += 1


load_data("../../Build/MVaultBenchFill.csv", vault)
load_data("../../Build/RedisBenchFill.csv", redis)


def plot(arr):
    y = []

    for i in records:
        y.append(arr[i])

    plt.plot(records, y, marker='o', markersize=5)



def draw_graph():
    plot(vault)
    plot(redis)

    plt.title("Добавление записей")
    plt.legend(["MVault", "Redis"], loc=2)
    plt.grid()
    plt.xlabel("Записи, ед")
    plt.ylabel("Время, мкс")
    

plt.xlim(-10000, 50000)
plt.ylim(-1000, 12000)
draw_graph()
plt.figure()

draw_graph()

plt.show()