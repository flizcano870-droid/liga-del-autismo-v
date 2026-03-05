import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_excel('datos2.xlsx', header=None)

x_ambiente = df.iloc[0,0]
y_ambiente = df.iloc[0,1]

x_60 = df.iloc[1:128,0]
y_60directa = df.iloc[1:128,1]

x_frio = df.iloc[128:177,0]
y_friodirecta= df.iloc[128:177,1]

x_eq = df.iloc[177:236,0]
y_eqdirecta = df.iloc[177:236,1]

plt.scatter(x_ambiente, y_ambiente, color='mediumseagreen', marker='.', s=50, label='Temperatura Ambiente')

plt.xlabel('Tiempo (s)', fontweight='bold')
plt.ylabel('Temperatura (°C)', fontweight='bold')
plt.title('Temperatura LM35 vs Tiempo ', fontweight='bold')
plt.legend(fontsize=9)

plt.tight_layout()
plt.show()

