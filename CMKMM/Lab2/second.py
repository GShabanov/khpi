#!/usr/bin/env python3

"""
    Laboratory work 2

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

def GetEntropy(value1, value2, value3):
    #
    # Entropy(S) = - ∑ pᵢ * log₂(pᵢ) ; i = 1 to n
    #
    total = value1 + value2 + value3

    return -(value1 / total * math.log(value1 / total, 2) + value2 / total * math.log(value2 / total, 2)
             + value3 / total * math.log(value3 / total, 2))

def AttributeEntropy(dataset):

    #
    # pᵢ is the probability of class "i" or the ratio of "the number of rows with class i in the target column" to the "total number of rows" in the data set.
    #
    data_array = np.array(dataset)

    Entropy = 0
    Sum = 0

    #
    # return possibility for values in this data array
    #
    for val in ['Iris-setosa', 'Iris-versicolor', 'Iris-virginica']:
        p = 0
        for elem in data_array:
            if elem[2] == val:
                p = p + 1

        possibility = p / len(dataset)

        #
        # IG(S, A) = Entropy(S) - ∑((|Sᵥ| / |S|) * Entropy(Sᵥ))
        #
        if p > 0:
            Entropy += (-possibility * math.log(possibility, 2))
            Sum += Entropy

    return Sum / 2




def InformationGain(current, target):
    return (target - current)

def findMaxGain(infoSet):
    max = 0
    name = ''

    for gain in infoSet:
        if gain[0] >= max:
            max = gain[0]
            name = gain[1]

    print('[*] Max gain is: ' + str(max) + ', set ' + name)
    return (max, name)



def Splitting(dataFrame, train):

    print(train.Species.value_counts())
    value1 = train.Species.value_counts()[0]
    value2 = train.Species.value_counts()[1]
    value3 = train.Species.value_counts()[2]

    entropy = GetEntropy(value1, value2, value3)

    print('Total entry =', entropy)
    print('\n')

    gains = []

    #
    # 2. Construct a binary tree. To do this, calculate the information increment for the traits with a step: for a flower width of 0.8 cm and for a flower length of 2.5.
    #

    print('Split by information gain')
    #
    # flower length of 2.5.
    #
    for value in (2.5, 5):
        #
        # left value
        #
        leftBranch = train[(dataFrame.PetalLengthCm < value)]
        leftBranch.name = 'PetalLengthCm < ' + str(value)

        #
        # rigth value
        #
        rigthBranch = train[(dataFrame.PetalLengthCm >= value)]
        rigthBranch.name = 'PetalLengthCm >= ' + str(value)

        gains.append((InformationGain(AttributeEntropy(leftBranch),
                entropy), 
             leftBranch.name))

        gains.append(
            (InformationGain(AttributeEntropy(rigthBranch), entropy), rigthBranch.name))

    #
    # for a flower width of 0.8 cm 
    #
    for value in (0.8, 1.6):
        #
        # left value
        #
        leftBranch = train[(dataFrame.PetalWidthCm < value)]
        leftBranch.name = 'PetalWidthCm < ' + str(value)

        #
        # rigth value
        #
        rigthBranch = train[(dataFrame.PetalWidthCm >= value)]
        rigthBranch.name = 'PetalWidthCm >= ' + str(value)

        gains.append(
            (InformationGain(AttributeEntropy(leftBranch), entropy), leftBranch.name))
        gains.append(
            (InformationGain(AttributeEntropy(rigthBranch), entropy), rigthBranch.name))


    #
    # 3. Make a node of the decision tree using the function with the maximum profit of information (nodes with the highest profit are at the top of the tree)
    #
    findMaxGain(gains)



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


    print('Divide data to train (80%) and test (20%) randomly')

    #
    # Divide data to train (80%) and test (20%) randomly.
    #
    train, test = model(data, test_size = 0.2, random_state = 1)

    Splitting(df, train)





# Run main code
main()
