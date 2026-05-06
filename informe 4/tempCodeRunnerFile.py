import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos.xlsx', header=None)

x=df.iloc[:, 0]
y=df.iloc[:, 1]

pendiente1, intercepto, r_value, p_value, std_err = stats.linregress(x, y)

x_teorico = np.linspace(min(x), max(x), 100)
y_teorico1 = pendiente1 * x_teorico + intercepto

plt.scatter(x, y,  color='darkblue', marker='o', s=50)
plt.plot(x_teorico, y_teorico1, label=f'Regresión experimental;  y = {pendiente1:.4f}x + ({intercepto:.4f})', color='darkblue', linewidth=2.5)

texto=(
    
    f"   R²_medido = {r_value**2:.4f} \n"

)

plt.text(0.14, 0.77, texto, transform=plt.gca().transAxes, fontsize=9, 
         verticalalignment='bottom', horizontalalignment='center', bbox=dict(facecolor='white', alpha=0.5, linewidth=0.5, edgecolor='grey', boxstyle='round'))    
plt.xlabel('Duty Cycle (0–255)', fontweight='bold')
plt.ylabel('RPM', fontweight='bold')
plt.title('Duty Cycle vs RPM', fontweight='bold')
plt.legend(fontsize=9)

plt.tight_layout()
plt.show()