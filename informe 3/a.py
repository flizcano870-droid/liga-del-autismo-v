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

plt.xlabel('Baudrate (log scale)', fontweight='bold')
plt.ylabel('Tiempo de transmisión (log scale)', fontweight='bold')
plt.title('Voltaje ADC vs Posición del Potenciómetro', fontweight='bold')
plt.legend(fontsize=9)

plt.tight_layout()
plt.show()