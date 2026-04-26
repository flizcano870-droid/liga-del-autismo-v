import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats
df = pd.read_excel('datos1_reto.xlsx', header=None)

x = df.iloc[:, 0]
y = df.iloc[:, 1]

pendiente, intercepto, r_value, p_value, std_err  = stats.linregress(x, y)
x_teorico = np.linspace(min(x), max(x), 100)
y_teorico = pendiente * x_teorico + intercepto

plt.scatter(x, y,  color='darkblue', marker='o', s=50)
plt.plot(x_teorico, y_teorico, label=f'Regresión lineal;  y = {pendiente:.4f}x + ({intercepto:.4f})', color='darkblue', linewidth=2.5)

plt.xlabel('Distancia (cm)', fontweight='bold')
plt.ylabel('Resistencia_LDR (Ohms)', fontweight='bold')
plt.title('R_LDR vs d', fontweight='bold')
plt.legend(fontsize=9)
plt.tight_layout()
plt.show()

