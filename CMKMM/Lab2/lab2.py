import math
import pandas
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split


def init():
    df_base = pandas.read_csv('C:/Users/oksana.lashchenko/Downloads/Lab_2,_Lab_3/Iris.csv')
    return df_base[['PetalLengthCm', 'PetalWidthCm', 'Species']]


def divide_data(data_frame):
    print('Divide data to train (80%) and test (20%) randomly')
    train, test = train_test_split(data_frame, test_size=0.2, random_state=1)
    print(train.shape)
    print(test.shape)
    return train, test


def build_picture(data_frame):
    plt.scatter(data_frame.PetalLengthCm, data_frame.PetalWidthCm)
    sns.set_style('whitegrid')
    sns.FacetGrid(data_frame, hue='Species') \
        .map(plt.scatter, 'PetalLengthCm', 'PetalWidthCm') \
        .add_legend()
    plt.show()


def count_total_entropy(value1, value2, value3):
    total = value1 + value2 + value3
    return -(value1 / total * math.log(value1 / total, 2) + value2 / total * math.log(value2 / total, 2)
             + value3 / total * math.log(value3 / total, 2))


def count_set_entropy(value1, value2):
    total = value1 + value2
    return -(value1 / total * math.log(value1 / total, 2) + value2 / total * math.log(value2 / total, 2))


def count_attribute_entropy(dataset):
    data_array = np.array(dataset)
    target_attribute_values = ['Iris-setosa', 'Iris-versicolor', 'Iris-virginica']
    data_entropy = 0
    entropy_sum = 0
    for val in target_attribute_values:
        p = 0
        for elem in data_array:
            if elem[2] == val:
                p = p + 1
        possibility_of_value = p / len(dataset)
        if p > 0:
            data_entropy += -possibility_of_value * math.log(possibility_of_value, 2)
            entropy_sum = entropy_sum + data_entropy
    return entropy_sum / 2


def count_information_gain(attribute_entropy, set_entropy):
    return set_entropy - attribute_entropy


def find_max_information_gain(information_gain_set):
    max_value = 0
    name = ''
    for information_gain in information_gain_set:
        if information_gain[0] >= max_value:
            max_value = information_gain[0]
            name = information_gain[1]
    print('Maximum Information Gain is ', max_value, 'in set', name)
    return max_value, name


def do_splitting(train):
    print(train.Species.value_counts())
    value1 = train.Species.value_counts()[0]
    value2 = train.Species.value_counts()[1]
    value3 = train.Species.value_counts()[2]
    entropy_total = count_total_entropy(value1, value2, value3)
    print('entropy_total =', entropy_total)
    print('_______________________________________________________________________________')

    print('First Split')
    inf_gains = []
    for value in (2.5, 5):
        training_set_l = train[(df.PetalLengthCm < value)]
        training_set_l.name = 'PetalLengthCm < ' + str(value)
        training_set_r = train[(df.PetalLengthCm >= value)]
        training_set_r.name = 'PetalLengthCm >= ' + str(value)
        inf_gains.append(
            (count_information_gain(count_attribute_entropy(training_set_l), entropy_total), training_set_l.name))
        inf_gains.append(
            (count_information_gain(count_attribute_entropy(training_set_r), entropy_total), training_set_r.name))
    for value in (0.8, 1.6):
        training_set_l = train[(df.PetalWidthCm < value)]
        training_set_l.name = 'PetalWidthCm < ' + str(value)
        training_set_r = train[(df.PetalWidthCm >= value)]
        training_set_r.name = 'PetalWidthCm >= ' + str(value)
        inf_gains.append(
            (count_information_gain(count_attribute_entropy(training_set_l), entropy_total), training_set_l.name))
        inf_gains.append(
            (count_information_gain(count_attribute_entropy(training_set_r), entropy_total), training_set_r.name))
    print(find_max_information_gain(inf_gains))
    print('_______________________________________________________________________________')

    print('Second Split - root - PetalWidthCm >= 0.8')
    df_root = train[(df.PetalWidthCm >= 0.8) & (df.PetalLengthCm >= 2.5)]
    print(df_root.Species.value_counts())
    entropy_total2 = count_set_entropy(df_root.Species.value_counts()[0], df_root.Species.value_counts()[1])
    print('entropy_total2 =', entropy_total2)
    print('_______________________________________________________________________________')

    df_root_length1 = df_root[(df.PetalLengthCm < 5)]
    df_root_length1.name = 'PetalLengthCm < 5'
    df_root_length2 = df_root[(df.PetalLengthCm >= 5)]
    df_root_length2.name = 'PetalLengthCm >= 5'
    df_root_width1 = df_root[(df.PetalWidthCm < 1.6)]
    df_root_width1.name = 'PetalWidthCm < 1.6'
    df_root_width2 = df_root[(df.PetalWidthCm >= 1.6)]
    df_root_width2.name = 'PetalWidthCm >= 1.6'

    information_gains_root = [
        (count_information_gain(count_attribute_entropy(df_root_length1), entropy_total2), df_root_length1.name),
        (count_information_gain(count_attribute_entropy(df_root_length2), entropy_total2), df_root_length2.name),
        (count_information_gain(count_attribute_entropy(df_root_width1), entropy_total2), df_root_width1.name),
        (count_information_gain(count_attribute_entropy(df_root_width2), entropy_total2), df_root_width2.name)]

    print(find_max_information_gain(information_gains_root))
    print('_______________________________________________________________________________')

    print('Third Split 1 branch - yes - PetalLengthCm >= 5')
    df_first_branch = df_root[(df.PetalLengthCm >= 5)]
    print(df_first_branch.Species.value_counts())
    value1 = df_first_branch.Species.value_counts()[0]
    value2 = df_first_branch.Species.value_counts()[1]
    entropy_total3 = count_set_entropy(value1, value2)
    print('entropy_total3 =', entropy_total3)
    print('_______________________________________________________________________________')

    df_first_branch_width1 = df_first_branch[(df.PetalWidthCm <= 1.6)]
    df_first_branch_width1.name = 'PetalWidthCm < 1.6'
    df_first_branch_width2 = df_first_branch[(df.PetalWidthCm > 1.6)]
    df_first_branch_width2.name = 'PetalWidthCm >= 1.6'

    information_gains_3 = [
        (count_information_gain(count_attribute_entropy(df_first_branch_width1), entropy_total3),
         df_first_branch_width1.name),
        (count_information_gain(count_attribute_entropy(df_first_branch_width2), entropy_total3),
         df_first_branch_width2.name)]

    print(find_max_information_gain(information_gains_3))
    print('_______________________________________________________________________________')

    print('Fourth Split - 1 branch - no - PetalLengthCm < 5')
    df_first_branch_no = df_root[(df.PetalLengthCm < 5)]
    print(df_first_branch_no.Species.value_counts())
    entropy_total4 = count_set_entropy(df_first_branch_no.Species.value_counts()[0],
                                       df_first_branch_no.Species.value_counts()[1])
    print('entropy_total4 =', entropy_total3)
    print('_______________________________________________________________________________')

    df_first_branch_no_width1 = df_first_branch_no[(df.PetalWidthCm <= 1.6)]
    df_first_branch_no_width1.name = 'PetalWidthCm < 1.6'
    df_first_branch_no_width2 = df_first_branch_no[(df.PetalWidthCm > 1.6)]
    df_first_branch_no_width2.name = 'PetalWidthCm >= 1.6'

    information_gains_4 = [
        (count_information_gain(count_attribute_entropy(df_first_branch_no_width1), entropy_total4),
         df_first_branch_no_width1.name),
        (count_information_gain(count_attribute_entropy(df_first_branch_no_width2), entropy_total4),
         df_first_branch_no_width2.name)]

    print(find_max_information_gain(information_gains_4))
    print('_______________________________________________________________________________')

    print('Fifth Split 2 branch - yes - PetalWidthCm <= 1.6')
    df_set5 = df_first_branch_no[(df.PetalWidthCm <= 1.6)]
    df_set5.name = 'PetalWidthCm <= 1.6'
    print(df_set5.Species.value_counts())
    count_attribute_entropy(df_set5)
    print('_______________________________________________________________________________')

    print('6th Split- 2 branch - no - PetalWidthCm > 1.6')
    df_set6 = df_first_branch_no[(df.PetalWidthCm > 1.6)]
    df_set6.name = 'PetalWidthCm > 1.6'
    print(df_set6.Species.value_counts())
    count_attribute_entropy(df_set6)
    print('_______________________________________________________________________________')


def build_id_tree(dataset):
    array = np.array(dataset)
    transformed_array = []
    for element in array:
        if element[1] < 0.8:
            element[2] = 'Iris-setosa'
            current_tuple = element[0], element[1], element[2]
            transformed_array.append(current_tuple)
        else:
            if element[0] >= 5:
                element[2] = 'Iris-virginica'
                current_tuple = element[0], element[1], element[2]
                transformed_array.append(current_tuple)
            else:
                if element[1] <= 1.6:
                    element[2] = 'Iris-versicolor'
                    current_tuple = element[0], element[1], element[2]
                    transformed_array.append(current_tuple)
                else:
                    element[2] = 'Iris-virginica'
                    current_tuple = element[0], element[1], element[2]
                    transformed_array.append(current_tuple)
    return transformed_array


def test_data_set(dataset):
    array = np.array(dataset)
    test_array = build_id_tree(dataset)
    count_mistakes = 0
    for i in range(len(array)):
        value = array[i]
        iris_type = value[2]
        test_value = test_array[i]
        test_iris_type = test_value[2]
        if test_iris_type != iris_type:
            count_mistakes = count_mistakes + 1
    return (100 - (count_mistakes / len(array)) * 100), count_mistakes


df = init()
train, test = divide_data(df)
do_splitting(train)
build_id_tree(train)
build_picture(train)
build_picture(test)
test_data_set(test)
print('Accuracy of the ID-tree:', test_data_set(test))
