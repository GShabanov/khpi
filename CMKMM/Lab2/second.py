#!/usr/bin/env python3

"""
    Laboratory work 2

    Implemented by: Gennady Shabanov
    Group: IKM-M222k
"""
import sys
import math
import numpy as np
import pandas
import seaborn as sns
import random
from pprint import pprint
from sklearn.model_selection import train_test_split as model
import matplotlib.pyplot as plt


COLUMN_STEPS_CONSTS = {
    "PetalWidthCm"  : 0.8,
    "PetalLengthCm" : 2.5
}




def GetEntropy(data):

    target = data.iloc[:, -1]

    #
    # Entropy(S) = - ∑ pᵢ * log₂(pᵢ) ; i = 1 to n
    #
    p = target.value_counts(normalize = True).to_numpy()

    return sum(p * -np.log2(p))



def GetInformationGain(data, name):

    values = data[name].value_counts(normalize=True)
    entropy = 0

    #
    # IG(S, A) = Entropy(S) - ∑((|Sᵥ| / |S|) * Entropy(Sᵥ))
    #
    for v, f in values.items():
        entropy1 = GetEntropy(data[data[name] == v])
        entropy += f * entropy1

    return GetEntropy(data) - entropy


def GetBestGainFeature(data):
    target = data.iloc[:, -1]

    names = data.columns

    infoGains = []

    for name in names:
        if name == target.name:
            continue

        infoGains.append(GetInformationGain(data, name))

    print("Current gains: " + str(infoGains))
    return names[np.argmax(infoGains)]


def Split(data, node, value):

    column = data[data[node] == value]

    return column.drop(node, axis = 1)

def GetCurrentDecision(column):

    print("Species in group:")
    print(column.value_counts())
    maxValues = column.value_counts().idxmax()

    print("Maximum values for: ", maxValues)
    print("\n")

    return maxValues


def BuildTree(data):

    print(data.shape)

    speciesName = data.iloc[:, -1].name

    #
    # 3. Make a node of the decision tree using the function with the maximum profit of information (nodes with the highest profit are at the top of the tree)
    #
    bestGainFeature = GetBestGainFeature(data)

    #
    # cature arrays of unique values
    #
    features = data[bestGainFeature].unique()

    tree = {bestGainFeature: {}}

    for feature in features:
        #
        # split by the best gain
        #
        newData = Split(data, bestGainFeature, feature)

        print("BestFeature:  " + bestGainFeature, feature)
        print("Array split: ", newData.shape)


        #
        # only one element here
        #
        if len(newData[speciesName].unique()) == 1:

            #
            # node leaf
            #

            #
            # 4. If all rows belong to the same class, make the current node a sheet node with the class as its label.
            #
            tree[bestGainFeature][feature] = GetCurrentDecision(newData[speciesName])

        else:

            if len(newData.columns) > 1:

                #
                # local root node
                #

                #
                # more than one column - continue splitting
                #
                tree[bestGainFeature][feature] = BuildTree(newData)

            else:
                #
                # node leaf
                #

                #
                # 4. If all rows belong to the same class, make the current node a sheet node with the class as its label.
                #
                tree[bestGainFeature][feature] = GetCurrentDecision(newData[speciesName])

    return tree


def Predict(row, tree):
    for node in tree.keys():
        value = row[node]
        tree = tree[node][value]

        if type(tree) is dict:
            return Predict(row, tree)
        else:
            return tree


def BuildRanges(data):

    data = data.copy()

    for name, step in COLUMN_STEPS_CONSTS.items():

        for row_index, row_value in data[name].items():

            new_row_value = round(math.ceil(row_value / step) * step, 1)
            data.at[row_index, name] = f"<= {new_row_value}"

    return data


def CheckResults(data, tree):

    columnName = data.iloc[:, -1].name
    
    data["Type prediction"] = data.apply(Predict, axis = 1, args=(tree,))

    data["Is correct"] = data["Type prediction"] == data[columnName]

    print(data.iloc[:])

    return (data["Is correct"].mean(), data)



def main():

    name = 'Iris.csv'


    if sys.argv[1:]:
        name = int(sys.argv[1])

    df = pandas.read_csv(name)


    speciesName = df.iloc[:, -1].name

    print(f'Data: {df.info()} \n')


    #
    # 1. Select 2 column attributes for classification: length (‘PetalLengthCm’) and width (‘PetalWidthCm’) of the flower.
    #
    data = df[['PetalLengthCm', 'PetalWidthCm', 'Species']]

    print('Divide data to train (80%) and test (20%) randomly')

    #
    # Divide data to train (80%) and test (20%) randomly.
    #
    train, test = model(data, test_size = 0.2, random_state = 1)

    #ax = plt.subplots(2, 2, figsize = [10, 8])
    fig, axs = plt.subplots(nrows = 2, ncols = 2, figsize = [10, 8])

    #petalFig1 = plt.figure(figsize = [10, 8])
    sns.scatterplot(x = "PetalWidthCm", y = "PetalLengthCm", hue = speciesName, data = train, ax = axs[0][0])

    #petalFig2 = plt.figure(figsize = [10, 8])
    sns.scatterplot(x = "PetalWidthCm", y = "PetalLengthCm", hue = speciesName, data = test, ax = axs[0][1])

    #
    # 2. Construct a binary tree. To do this, calculate the information increment for the traits with a step: for a flower width of 0.8 cm and for a flower length of 2.5.
    #
    train = BuildRanges(train)
    test = BuildRanges(test)

    id3 = BuildTree(train)




    print("\n")
    print("===========================================================")

    pprint(id3)
    
    print("===========================================================")
    print("\n")

    reliability = CheckResults(test, id3)

    print("\n")
    print("Reliability: ", reliability[0])

    #petalFig3 = plt.figure(figsize = [10, 8])

    test["prediction"] = test.apply(Predict, axis = 1, args=(id3,))
    df["correct"] = test["prediction"] == test[speciesName]
    sns.scatterplot(x = "PetalWidthCm", y = "PetalLengthCm", hue = "correct", data = df, ax = axs[1][1])

    plt.show()









# Run main code
main()
