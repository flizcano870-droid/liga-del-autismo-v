import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos1.xlsx', header=None)

y=df.iloc[:, 0]
x=df.iloc[:, 1]
y_2=df.iloc[:, 2]

pendiente1, intercepto, r_value, p_value, std_err = stats.linregress(x, y)
pendiente2, intercepto2, r_value2, p_value2, std_err2 = stats.linregress(x, y_2)

x_teorico = np.linspace(min(x), max(x), 100)
y_teorico1 = pendiente1 * x_teorico + intercepto
y_teorico2= pendiente2 * x_teorico + intercepto2

plt.scatter(x, y, label='Datos', color='darkblue', marker='o', s=50)
plt.scatter(x, y_2, label='Datos 2', color='tomato', marker='o', s=15)
plt.plot(x_teorico, y_teorico1, label=f'Recta teórica', color='darkblue', linewidth=2)
plt.plot(x_teorico, y_teorico2, label=f'Recta teórica 2', color='tomato', linewidth=1)

texto=(
    f"   y_medido = {pendiente1:.4f}x + ({intercepto:.4f}) \n"
    f"   R²_medido = {r_value**2:.4f} \n"
    f"   y_teorico = {pendiente2:.4f}x + ({intercepto:.4f}) \n"
    f"   R²_teorico = {r_value2**2:.4f}"

)

plt.text(0.2, 0.9, texto, transform=plt.gca().transAxes, fontsize=8, 
         verticalalignment='top', horizontalalignment='center', bbox=dict(facecolor='white', alpha=0.5, linewidth=0.5))    
plt.xlabel('raw data', fontweight='bold')
plt.ylabel('Voltage (V)', fontweight='bold')
plt.title('Voltaje ADC vs Posición del Potenciómetro', fontweight='bold')

plt.show()