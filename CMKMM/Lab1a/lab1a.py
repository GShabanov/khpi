#!/usr/bin/env python3

"""
    Laboratory work 1a

    Implemented by: Gennady Shabanov
    Group: IKM-M222k
"""

import sys
import pandas
import re
import numpy as np
import seaborn
import matplotlib.pyplot as plt
from sklearn.preprocessing import OneHotEncoder

def main():

    name = 'Energy_and_Water_Data_Disclosure_for_Local_Law_84_2017__Data_for_Calendar_Year_2016_.csv'

    if sys.argv[1:]:
        name = int(sys.argv[1])

    data = pandas.read_csv(name, index_col='Order')

    print("1. Data cleansing \n")
	#
	# 1. The The whole set contains 60 features. missing data inserted into the set looks like a line with the entry “Not Available”. 
	# You should analyze data and convert number types into float, if value is “Not Available” convert into   «not a number»)
	#
    print(f'Raw data information: \n{data.info()}')
    data.replace('Not Available', np.nan, inplace = True)

    #
    # normalize data values to the same type
    #
    for column in data.columns:
        for pattern in ['Score', 'ft', 'kBtu', 'Metric Tons', 'kWh', 'gal', 'therms']:

            #
            # check column for this pattern
            #
            if pattern in column:
                data[column] = data[column].astype(float)

    print(f'Data after rebuild: {data.info()} \n')

	#
	# 2. Find Missing Data and Outliers
	#
    print("2. Find Missing Data and Outliers \n")

    #
    # a) You need to estimate how much data in columns is missing. If feature has more than 50% gaps, remove it.
    #
    removed_colunms = []

    for column in data.columns:
        percentOfEmptyData = data[column].isnull().sum() / data.shape[0]

        if percentOfEmptyData >= 0.5:
            removed_colunms.append(column)


    print(f'Columns with more than 50% gaps\n {removed_colunms} \n Count:' + str(len(removed_colunms)))

    data.drop(removed_colunms, axis=1, inplace=True)

    print(f'After removing missing data: {data.info()} \n')

    #
    # b) Then it is worth getting rid of the outliers. They can be associated with typos, errors in units of measurement,
    # or be correct but too extreme values. For getting rid of outliers remove anomalous values based on this criterion:
    #
    for column in data.columns:

        for pattern in ['Score', 'ft', 'kBtu', 'kWh', 'therms', 'gal', 'Metric']:
            #
            # check column for this pattern
            #
            if pattern in column:

                firstQuartile = data[column].describe()['25%']
                thirdQuartile = data[column].describe()['75%']

                #
                # interquartile
                #
                iq = thirdQuartile - firstQuartile

                print(' Processing:  ')
                print(data[column].name)

                #
                # fences calculation
                #
                column_data = data[(data[column] > (firstQuartile - 3 * iq)) & (data[column] < (thirdQuartile + 3 * iq))]
                # print(column_data.info)
                print(' ******** ')


    firstQuartile = data['ENERGY STAR Score'].describe()['25%']
    thirdQuartile = data['ENERGY STAR Score'].describe()['75%']

    iq = thirdQuartile - firstQuartile
    column_data = data[(data['ENERGY STAR Score'] > (firstQuartile - 3 * iq))
                 & (data['ENERGY STAR Score'] < (thirdQuartile + 3 * iq))]

    #
    # The box plot is a useful graphical display for describing the behavior of the data in the middle as well as at the ends of the distributions. 
    #
    plt.hist(column_data['ENERGY STAR Score'].dropna(), bins = 15, color = 'green', edgecolor = 'black')
    plt.title('ENERGY STAR Score')


    #
    # 3. Conduct Exploratory Data Analysis
    #
    print("3. Conduct Exploratory Data Analysis \n")

    #
    # а) Finding relationships between various features. Build Density plot (seaborn) between that features (Energy Star Scores) by 
    # Building Type, Borough. What feature shows better difference between type?
    #
    building = data['Largest Property Use Type'].value_counts()
    building = list(building[building.values > 100].index)

    fig2 = plt.figure()

    for propertyType in building:
        subset = data[data['Largest Property Use Type'] == propertyType]
        seaborn.kdeplot(subset['ENERGY STAR Score'].dropna(), label = propertyType, fill = True)

    plt.title('Density plot of ENERGY STAR Score by property Use Type')
    plt.legend()

    district = data['Borough'].value_counts()
    district = list(district[district.values > 7].index)

    fig3 = plt.figure()

    for districtType in district:
        subset = data[data['Borough'] == districtType]
        seaborn.kdeplot(subset['ENERGY STAR Score'].dropna(), label = districtType, fill = True)

    plt.title('Density plot of ENERGY STAR Score by Borough')
    plt.legend()

    #
    # b) Find the biggest and the lowest correlation between features and (Energy Star Scores)
    #
    print("b) Find the biggest and the lowest correlation between features and (Energy Star Scores)")
    correlations = column_data.corr()['ENERGY STAR Score'].sort_values()

    # 
    # print first and last values
    #
    print(correlations.head(3), '\n')
    print(correlations.tail(4), '\n')
    print(' ********')


    #
    # 4. Feature Engineering and Selection
    #
    print("4. Feature Engineering and Selection")
    print("a) One-hot coding of categorical features (district and building type)\n")

    oneHotEnc = OneHotEncoder(handle_unknown = 'ignore')
    transformed = oneHotEnc.fit_transform([data['Largest Property Use Type'], data['Borough']])
    print(transformed, '\n')

    print('b) Taking the logarithm (natural) of numerical data')
    #
    # Taking the logarithm (natural) of numerical data.
    #
    for column in data.columns:

        for pattern in ['Score', 'ft', 'kBtu', 'kWh', 'therms', 'gal', 'Metric']:

            if pattern in column:
                values = data[column].value_counts().sort_values()
                print(values, '\n')

    #
    # Features that are strongly correlated with each other are known as collinear and removing one of the variables in these pairs of
    # features can often help a machine learning model generalize and be more interpretable. (It should point out we are talking about
    # correlations of features with other features, not correlations with the target, which help our model!)
    #
    for column in data.columns:
    
        if (data[column].dtype == float) and (data[column].describe()['max'] > 1000000):
            print('convert data to logarith form for column: \'' + column + '\'\n')

            columData = data[column]            
            result = np.where(columData == np.nan)

            for i in range(0, len(columData.array)):

                value = columData.array[i]
                #
                # check value
                #
                if np.isnan(value) or value <= 0:
                    columData.array[i] = np.log(1)
                else:
                    columData.array[i] = np.log(value)


    print(' ********')

    print('c) Selection')

    correlation = data.corr().abs()

    #
    # You should use correlation coefficient to identify and remove collinear features. We will drop one of a pair of
    # features if the correlation coefficient between them is greater than 0.6.
    #

    #
    # find high correlation pair
    #
    highCorelation = np.where(correlation > 0.6)

    highPairs = []

    for x, y in zip(*highCorelation):
        if x != y and x < y:
            highPairs.append((correlation.index[x], correlation.index[y]))


    print(f'Pair of features with corr > 0.6 \n');
    for i in range(0, len(highPairs)):
        print(f' collinear pairs {highPairs[i]}')


    print(f'drop columns from first part of pairs\n');


    #
    # build array for drop columns
    #
    removed_colunms = []

    for i in range(0, len(highPairs)):
        removed_colunms.append(highPairs[i][0])

    data_cleared = data.drop(removed_colunms, axis=1, inplace=False, errors='ignore')

    #
    # build array for drop columns
    #
    print('cleared colums\n')

    for column in data_cleared.columns:
        for pattern in ['ft', 'kBtu', 'kWh', 'therms', 'gal', 'Metric']:
    
            if pattern in column:
                column_correlations2 = data_cleared.corr()[column].sort_values()
                print(column_correlations2, '\n')
                print(' ********')


    print(data_cleared.info())

    plt.show()



# Run main code
main()
