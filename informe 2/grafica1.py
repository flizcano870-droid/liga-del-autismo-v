import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos1.xlsx', header=None)

y=df.iloc[:, 0]
x=df.iloc[:, 1]
y_2=df.iloc[:, 2]

pendiente, intercepto, r_value, p_value, std_err = stats.linregress(x, y)
print(f"   RESULTADOS DEL AJUSTE:")
print(f"   Ecuación: y = {pendiente:.4f}x + {intercepto:.4f}")
print(f"   R² = {r_value**2:.4f}")


x_teorico = np.linspace(min(x), max(x), 100)
y_teorico = pendiente * x_teorico + intercepto



plt.scatter(x, y, label='Datos', color='darkblue', marker='o', s=50)
plt.plot(x_teorico, y_teorico, label=f'Recta teórica', color='darkblue', linewidth=1.5)

texto=(
    f"   y = {pendiente:.4f}x + ({intercepto:.4f}) \n"
    f"   R² = {r_value**2:.4f}"
)

plt.text(0.2, 0.9, texto, transform=plt.gca().transAxes, fontsize=8, 
         verticalalignment='top', horizontalalignment='center', bbox=dict(facecolor='white', alpha=0.5, linewidth=0.5))    
plt.xlabel('raw data', fontweight='bold')
plt.ylabel('Voltage (V)', fontweight='bold')
plt.title('Voltaje ADC vs Posición del Potenciómetro', fontweight='bold')

plt.show()