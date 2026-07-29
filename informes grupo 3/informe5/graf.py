import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos.xlsx', header=None)

x_lineal=df.iloc[3:8, 0]
y_lineal=df.iloc[3:8, 1]

pendiente1, intercepto, r_value, p_value, std_err = stats.linregress(x_lineal, y_lineal)

x_teorico = np.linspace(min(x_lineal), max(x_lineal), 100)
y_teorico1 = pendiente1 * x_teorico + intercepto

x_muerto=df.iloc[0:2, 0]
y_muerto=df.iloc[0:2, 1]
x_start=df.iloc[2, 0]
y_start=df.iloc[2, 1]
x_resto=df.iloc[8:, 0]
y_resto=df.iloc[8:, 1]
x_p1=df.iloc[3, 0]
y_p1=df.iloc[3, 1]
x_p2=df.iloc[7, 0]
y_p2=df.iloc[7, 1]

x_lineal_sin_especiales = x_lineal[~((x_lineal == df.iloc[3, 0]) | (x_lineal == df.iloc[7, 0]))]
y_lineal_sin_especiales = y_lineal[~((x_lineal == df.iloc[3, 0]) | (x_lineal == df.iloc[7, 0]))]

plt.scatter(x_p1, y_p1, color='cornflowerblue', marker='o', s=50, label='P1.(75, 183)')
plt.scatter(x_p2, y_p2, color='cornflowerblue', marker='o', s=50, label='P2.(175, 607)')

plt.scatter(x_resto, y_resto, color='darkblue', marker='o', s=50)
plt.scatter(x_muerto, y_muerto, color='deeppink', marker='o', s=50, label='Punto muerto')
plt.scatter(x_start, y_start, color='darkorchid', marker='o', s=50, label='Punto de arranque')
plt.scatter(x_lineal_sin_especiales, y_lineal_sin_especiales, color='darkblue', marker='o', s=50)
plt.plot(x_teorico, y_teorico1, label=f'Regresión experimental;  y = {pendiente1:.4f}x + ({intercepto:.4f})', 
         color='darkblue', linewidth=1.5)

plt.xlabel('Duty Cycle (0-255)', fontweight='bold')
plt.ylabel('RPM', fontweight='bold')
plt.title('Duty Cycle vs RPM', fontweight='bold')
plt.legend(fontsize=7)
print(f'r_value: {r_value:.4f}')
plt.tight_layout()
plt.show()