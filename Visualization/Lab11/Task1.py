import matplotlib.pyplot as plt
import numpy as np
from sklearn import datasets
from sklearn.decomposition import PCA
from sklearn.decomposition import TruncatedSVD
from sklearn.preprocessing import StandardScaler

iris = datasets.load_iris()
X = iris.data
y = iris.target

colors = ["r", "g", "b"]
target_names = iris.target_names

plt.figure()

for color, i, target_name in zip(colors, [0, 1, 2], target_names):
    plt.scatter(X[y == i, 0], X[y == i, 1], color=color, label=target_name)
plt.legend(loc='best', shadow=False, scatterpoints=1)
plt.title('Iris data')

#
# PCA з 2 компонентами для 2D візуалізації
#
pca_2d = PCA(n_components=2)
X_pca_2d = pca_2d.fit_transform(X)

plt.figure()


for color, i, target in zip(["r", "g", "b"], [0, 1, 2], iris.target_names):
    plt.scatter(X_pca_2d[y == i, 0], X_pca_2d[y == i, 1], color=color, label=target)

plt.legend(loc='best', shadow=False, scatterpoints=1)
plt.title('2D PCA Iris dataset')

#
# PCA з 3 компонентами для 3D візуалізації
#
pca_3d = PCA(n_components=3)
X_pca_3d = pca_3d.fit_transform(X)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
for color, i, target in zip(["r", "g", "b"], [0, 1, 2], iris.target_names):
    ax.scatter(X_pca_3d[y == i, 0], X_pca_3d[y == i, 1], X_pca_3d[y == i, 2], color=color, label=target)

ax.legend(loc='best', shadow=False, scatterpoints=1)
plt.title('3D PCA Iris dataset')

#
# Стандартизація даних
#
X_norm = StandardScaler().fit_transform(X)
u, s, vh = np.linalg.svd(X_norm)
sorted_s = np.sort(s)[::-1]
plt.figure()
plt.plot(sorted_s, 'ro-', linewidth=2)
#print("s standart      " + str(sorted_s))
plt.title('Графік власних значень від їх номеру (стандарт)')

#
# Без стандартизації
#
u1, s1, vh1 = np.linalg.svd(X)
sorted_s1 = np.sort(s1)[::-1]
plt.figure()
plt.plot(sorted_s1, 'ro-', linewidth=2)
#print("s not standart  " + str(sorted_s1))
plt.title('Графік власних значень від їх номеру (не стандарт)')

#
# суму всіх власних значень
#
total = sum(s ** 2)
sorted_s_values = sorted(s ** 2, reverse=True)

# знайти таке d, що сума перших d власних значень складає не менше 80% від total
current_sum = 0
d = 0
for value in sorted_s_values:
    current_sum = current_sum + value
    d = d + 1
    if current_sum / total >= 0.8:
        break

#print(f"Мінімальне значення розміру простору d: {d}")

#
# Занулити власні значення, які не відповідають першим d компонентам
#
s_d = s.copy()
s_d[d:] = 0

#
# Створити матрицю з діагональними елементами s_d
#
s_d_matrix = np.zeros((X_norm.shape[0], X_norm.shape[1]))
for i in range(min(X_norm.shape)):
    s_d_matrix[i, i] = s_d[i]

#
# зворотне перетворення
#
X_res = u.dot(s_d_matrix).dot(vh)

plt.figure()
for color, i, target_name in zip(colors, [0, 1, 2], target_names):
    plt.scatter(X_res[y == i, 0], X_res[y == i, 1], color=color, label=target_name)

plt.legend(loc='best', shadow=False, scatterpoints=1)
plt.title('Iris data після зворотного перетворення')


#
# суму всіх власних значень
#
total1 = sum(s1 ** 2)
sorted_s_values1 = sorted(s1 ** 2, reverse=True)

#
# знайти таке d, що сума перших d власних значень складає не менше 80% від total
#
current_sum1 = 0
d1 = 0
for value in sorted_s_values1:
    current_sum1 = current_sum1 + value
    d1 = d1 + 1
    if current_sum1 / total1 >= 0.8:
        break

print(f"Мінімальне значення розміру простору d (без стандарт.): {d1}")

#
# Занулити власні значення, які не відповідають першим d компонентам
#
s_d1 = s1.copy()
s_d1[d1:] = 0

#
# Створити матрицю з діагональними елементами s_d
#
s_d_matrix1 = np.zeros((X.shape[0], X.shape[1]))

for i in range(min(X.shape)):
    s_d_matrix1[i, i] = s_d1[i]

#
# зворотне перетворення
#
X_res1 = u1.dot(s_d_matrix1).dot(vh1)

plt.figure()
for color, i, target_name in zip(colors, [0, 1, 2], target_names):
    plt.scatter(X_res1[y == i, 0], X_res1[y == i, 1], color=color, label=target_name)

plt.legend(loc='best', shadow=False, scatterpoints=1)
plt.title('Iris data після зворотного перетворення (без стандартизації)')
plt.show()

#
# ----------------------------------------------------------------------------------------------------------------------
# сума всіх власних значень без стандартизації
#
total1 = sum(s1 ** 2)
sorted_s1_values = sorted(s1 ** 2, reverse=True)

#
# знайти таке d1, що сума перших d1 власних значень складає не менше 80% від total1
#
current_sum1 = 0
d1 = 0
for value in sorted_s1_values:
    current_sum1 = current_sum1 + value
    d1 = d1 + 1
    if current_sum1 / total1 >= 0.8:
        break

#print(f"Мінімальне значення розміру простору d1: {d1}")

plt.show()
