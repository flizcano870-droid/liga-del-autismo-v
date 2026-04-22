import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos.xlsx', header=None)

y_teorico=df.iloc[:, 2]
x_baud=df.iloc[:, 0]
y_medido=df.iloc[:, 1]

plot.semilogx(x_baud, y_medido, 'o', label='Medido')
plot.semilogx(x_baud, y_teorico, 'r-', label='Teórico')


plt.xlabel('raw data (0–1023)', fontweight='bold')
plt.ylabel('Voltage (V)', fontweight='bold')
plt.title('Voltaje ADC vs Posición del Potenciómetro', fontweight='bold')
plt.legend(fontsize=9)

plt.tight_layout()
plt.show()