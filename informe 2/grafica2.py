import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos2.xlsx', header=None)

x = df.iloc[:, 0]
y_directa = df.iloc[:, 1]
y_amplificada = df.iloc[:, 2] 

plt.scatter(x, y_directa, color='darkblue', marker='.', s=50, label='Voltaje Directo')