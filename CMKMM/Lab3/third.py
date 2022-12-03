#!/usr/bin/env python3

"""
    Laboratory work 3

    Implemented by: Gennady Shabanov
    Group: IKM-M222k
"""

import sys
import math
import pandas
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split as model

def findDominant(pairs):
    max_value = 0
    name = ''
    for value in pairs:
        if value[1] >= max_value:
            max_value = value[1]
            name = value[0]

    return name

def EuclideanDistance(point1, point2):
    return math.sqrt((point1[0] - point2[0]) ** 2 + (point1[1] - point2[1]) ** 2)

def PlotPicture(df):
    plt.scatter(df.PetalLengthCm, df.PetalWidthCm)
    sns.set_style('whitegrid')
    sns.FacetGrid(df, hue='Species').map(plt.scatter, 'PetalLengthCm', 'PetalWidthCm').add_legend()
    plt.show()


def knnClassify(data, k = 5):

    dataArray = np.array(data)

    mistakes = 0
    transformedSet = []

    for point in dataArray:

        #
        # build distance array
        #
        distances = []
        for element in dataArray:
            #
            # 4.1.	Calculate the distance between the object coordinate data and each  row of training data using the Euclidean distance method (determine the length of the vector for each neighbor).
            #
            distance = EuclideanDistance(element, point), element[2]
            distances.append(distance)

        #
        # 4.2. Based on the distance value, sort them in ascending order (choose any data sorting method).
        #
        distances.sort()

        #
        # 4.3. You must select the top k rows from the sorted array.
        #
        nearestK = distances[1:k]

        setosa = 0
        versicolor = 0
        virg = 0

        nameCountPair = []

        for elem in nearestK:

            name = elem[1]

            if name == 'Iris-setosa':
                    setosa += 1
            elif name == 'Iris-versicolor':
                    versicolor += 1
            elif name == 'Iris-virginica':
                    virg += 1
            else:
                raise "invalid data"


        nameCountPair.append(('Iris-setosa', setosa))
        nameCountPair.append(('Iris-versicolor', versicolor))
        nameCountPair.append(('Iris-virginica', virg))

        #
        # 4.4. Assign a class (iris type) to a checkpoint based on the most common class of these strings.
        #
        dominantSpecies = findDominant(nameCountPair)

        transformPoint = (point[0], point[1], dominantSpecies)
        transformedSet.append(transformPoint)

        if point[2] != transformPoint[2]:
            mistakes += 1

            print(f'foreign element: {point}')
            print(f'current point: {transformPoint}')
            print('i-Sentosa: ' + str(setosa) + ', I-Versicolor: ' + str(versicolor) + ', I-Verginica: ' + str(virg))
            print('\n')

    print('mistakes: ' + str(mistakes))

    print('reliability:' + str(100 - (mistakes / len(dataArray)) * 100))
    return transformedSet


def main():

    name = 'Iris.csv'


    if sys.argv[1:]:
        name = int(sys.argv[1])

    df = pandas.read_csv(name)

    #
    # 1. Select 2 column attributes for classification: length (‘PetalLengthCm’) and width (‘PetalWidthCm’) of the flower.
    #
    data = df[['PetalLengthCm', 'PetalWidthCm', 'Species']]

    print(f'Data: {data.info()} \n')

    #
    # 2. Next you need to select the value of k, ie the nearest data points. k can be any integer. (recommended k = 5).
    #
    k = 7

    print('Divide data to train (80%) and test (20%) randomly')

    #
    # 3. Divide the sample into training (80%) and test (20%) randomly, because the data is organized. 
    #
    train, test = model(data, test_size = 0.2, random_state = 1)

    #
    # 4. For each point in the test data, do the following: 
    #
    knnClassify(train, k)
    knnClassify(test, k)

    PlotPicture(train)
    PlotPicture(test)




# Run main code
main()
