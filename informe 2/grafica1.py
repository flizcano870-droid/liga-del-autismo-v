import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos1.xlsx', header=None)

x=df.iloc[:, 0]
y=df.iloc[:, 1]

pendiente, intercepto, r_value, p_value, std_err = stats.linregress(x, y)
print(f"   RESULTADOS DEL AJUSTE:")
print(f"   Ecuación: y = {pendiente:.4f}x + {intercepto:.4f}")
print(f"   R² = {r_value**2:.4f}")


x_teorico = np.linspace(min(x), max(x), 100)
y_teorico = pendiente * x_teorico + intercepto

plt.scatter(x, y, label='Datos', color='blue', marker='o', s=80)
plt.plot(x_teorico, y_teorico, label='Recta teórica', color='blue', linewidth=2)

plt.xlabel('raw data', fontweight='bold')
plt.ylabel('Voltage (V)', fontweight='bold')
plt.title('Voltaje ADC vs Posición del Potenciómetro', fontweight='bold')
plt.legend()

plt.show()