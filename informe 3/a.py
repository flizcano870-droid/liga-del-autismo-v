import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos.xlsx', header=None)

y_teorico=df.iloc[:, 2]
x_baud=df.iloc[:, 0]
y_medido=df.iloc[:, 1]

x_baud_log=np.log10(x_baud)
y_teorico_log=np.log10(y_teorico)
y_medido_log=np.log10(y_medido)

pendiente1, intercepto1, r_value1, p_value1, std_err1 = stats.linregress(x_baud_log, y_medido_log)
pendiente2, intercepto2, r_value2, p_value2, std_err2 = stats.linregress(x_baud_log, y_teorico_log)

x_teorico = np.logspace(np.log10(min(x_baud)), np.log10(max(x_baud)), 100)
y_medido_reg = 10**(pendiente1 * np.log10(x_teorico) + intercepto1)
y_teorico_reg = 10**(pendiente2 * np.log10(x_teorico) + intercepto2)


plt.figure(figsize=(10, 6))

plt.semilogx(x_baud, y_medido, 'o', color='darkblue', label='t medido', 
             markersize=8, markerfacecolor='darkblue', markeredgecolor='darkblue')

plt.semilogx(x_baud, y_teorico, 's', color='coral', label='t teórico', 
             markersize=8, markerfacecolor='white', markeredgecolor='coral')

plt.semilogx(x_teorico, y_medido_reg, color='darkblue', linestyle='-', label='Ajuste t medido')
plt.semilogx(x_teorico, y_teorico_reg, color='coral', linestyle='-', label='Ajuste t teórico')

plt.xlabel('Baudrate (baud)', fontweight='bold')
plt.ylabel('Tiempo de transmisión (µs)', fontweight='bold')
plt.title('t medido vs t teórico por baudrate', fontweight='bold')
plt.legend(fontsize=9)

plt.tight_layout()
plt.show()