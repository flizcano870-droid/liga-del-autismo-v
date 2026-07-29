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

pendiente1, intercepto1, r1, _, _ = stats.linregress(x_baud_log, y_medido_log)
pendiente2, intercepto2, r2, _, _ = stats.linregress(x_baud_log, y_teorico_log)

x_teorico = np.logspace(np.log10(min(x_baud)), np.log10(max(x_baud)), 100)
y_medido_reg = 10**(pendiente1 * np.log10(x_teorico) + intercepto1)
y_teorico_reg = 10**(pendiente2 * np.log10(x_teorico) + intercepto2)


plt.figure(figsize=(10, 8))


plt.loglog(x_baud, y_medido, 'o', color='darkblue', label='t medido', 
             markersize=8, markerfacecolor='darkblue', markeredgecolor='darkblue')

plt.loglog(x_baud, y_teorico, 's', color='coral', label='t teórico', 
             markersize=8, markerfacecolor='white', markeredgecolor='coral')

plt.loglog(x_teorico, y_medido_reg, color='darkblue', linestyle='-', label='Ajuste t medido')
plt.loglog(x_teorico, y_teorico_reg, color='coral', linestyle='-', label='Ajuste t teórico')

texto=(
    
    f"   R²_medido = {r1**2:.4f}\n"
    f"   R²_teórico = {r2**2:.4f}"

)

plt.text(0.91, 0.83, texto, transform=plt.gca().transAxes, fontsize=9, 
         verticalalignment='top', horizontalalignment='center', bbox=dict(facecolor='white', alpha=0.5, linewidth=0.5, edgecolor='grey', boxstyle='round'))    

plt.xlabel('Baudrate (baud)', fontweight='bold')
plt.ylabel('Tiempo de transmisión (µs)', fontweight='bold')
plt.title('t medido vs t teórico por baudrate (escala log-log)', fontweight='bold')
plt.legend(fontsize=9)

plt.tight_layout()
plt.show()