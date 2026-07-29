import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df_rojo= pd.read_csv('medicion_20260527_104729.csv')
df_verde= pd.read_csv('medicion_20260527_105147.csv')

x_rojo=df_rojo.iloc[0:, 2]
y_rojo=df_rojo.iloc[0:, 3]

x_verde=df_verde.iloc[0:, 2]
y_verde=df_verde.iloc[0:, 3]

plt.scatter(x_rojo, y_rojo,  color='crimson', marker='1', s=60, label='LED rojo')
plt.scatter(x_verde, y_verde,  color='limegreen', marker='3', s=50, label='LED verde')

plt.xlabel('Voltaje LED (V)', fontweight='bold')
plt.ylabel('I (mA)', fontweight='bold')
plt.title('Curva I-V de LED rojo y verde superpuestas', fontweight='bold')
plt.legend()

plt.tight_layout()
plt.show()