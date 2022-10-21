#!/usr/bin/env python3

"""
    Laboratory work 1

    Implemented by: Gennady Shabanov
    Group: IKM-M222k
"""

import sys
import pandas
import re

def main():

    name = 'titanic.csv'

    if sys.argv[1:]:
        name = int(sys.argv[1])

    data = pandas.read_csv(name, index_col='PassengerId')

    data_frame_records = data['Sex'].value_counts()

    print("total records were read: " + str(len(data)))

    #
    # Sex
    #
    print("1. Male and female records:")
    print("Male: " + str(data_frame_records['male']))
    print("Female: " + str(data_frame_records['female']))

    #
    # survived percentage
    #
    print("2. survived percentage")
    is_survived = data['Survived'].value_counts()

    print("Survived: " + str(is_survived[1]))
    print("Died: " + str(is_survived[0]))

    print("Survived percentage: " + str(round((is_survived[1] / len(data)) * 100)) + " %")


    #
    # class statistics
    #
    print("3. First class")
    passenger_class = data['Pclass'].value_counts()

    print("percentage: " + str(round((passenger_class[1] / len(data)) * 100)) + " %")

    #
    # age statistics
    #
    print("4. How old were the passengers")
    ages = data['Age']

    ages_sum = 0
    ages_count = 0

    for passenger in ages.items():
        if not pandas.isna(passenger[1]):
            ages_sum += passenger[1]
            ages_count += 1

    print("madian age: " + str(ages.median()))
    print("average age: " + str(round(ages_sum / ages_count)))

    #
    # siblings and parent children correlation
    #
    print("5. parent childers correlations")

    siblings = data['SibSp']
    parent_child = data['Parch']

    correlations = siblings.corr(parent_child, method='pearson')

    print("Siblings correlation: " + str(correlations))

    passenger_names = data['Name']

    female_names = []

    #
    # extract female names
    #
    for name in passenger_names.items():
        if not pandas.isna(name[1]):
            #
            # extract name for Miss
            #
            if (re.search(".Miss\..", name[1]) != None):

                first_name = re.split(r'\W+', name[1])

                if len(first_name) >= 3:
                    female_names.append(first_name[2])
                else:
                    print("[!] Unknown pattern *" + name[1])

            #
            # extract name for Missis
            #
            if (re.search(".Mrs\..", name[1]) != None) or (re.search(".Lady\..", name[1]) != None):

                first_name = re.search(r'(\(\w+)', name[1])

                #
                # a simple case where we have name in brackets
                #
                if first_name:
                    first_name = first_name.group(0)
                    first_name = str.removeprefix(first_name, '(')
                    female_names.append(first_name)

                #
                # a complex case where the name after Mrs.
                #
                else:
                    first_name = re.search(r'.Mrs\..+', name[1])

                    if first_name:
                        first_name = first_name.group(0).split(r'(Mrs\.) \W+')

                        #
                        # first word after Mrs.
                        #
                        if first_name:
                            first_name = first_name[0]
                            first_name = str.replace(first_name, 'Mrs.', '').lstrip().split()

                            #
                            # an array of words where we take the first word
                            #
                            female_names.append(first_name[0])

                    else:
                        print("[!] Unknown pattern *" + name[1])

    if len(female_names) > 0:

        names = pandas.Series(female_names)

        #
        # just for output array
        #
        # for name in female_names:
        #    print("Name: " + name)

        popular_name = names.value_counts()


        print("6. Most popular name: " + popular_name.idxmax() + ", entrance: " + str(popular_name[0]))

    else:
        print("[!] array of names is empty")

# Run main code
main()
