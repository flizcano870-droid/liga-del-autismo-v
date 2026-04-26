import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats
import seaborn as sns

df= pd.read_excel('datos2.xlsx', header=None)

x_ambiente = df.iloc[0,0]
y_ambientedirec = df.iloc[0,1]
y_ambienteamplificada = df.iloc[0,2]

x_60 = df.iloc[1:128,0]
y_60directa = df.iloc[1:128,1]
y_60amplificada = df.iloc[1:128,2]

x_frio = df.iloc[128:177,0]
y_friodirecta= df.iloc[128:177,1]
y_frioamplificada = df.iloc[128:177,2]

x_eq = df.iloc[177:236,0]
y_eqdirecta = df.iloc[177:236,1]
y_eqamplificada = df.iloc[177:236,2]

plt.scatter(x_ambiente, y_ambientedirec, color='darkred', marker='.', s=50, label='Temperatura directa Ambiente')
plt.scatter(x_60, y_60directa, color='mediumturquoise', marker='.', s=50, label='Temperatura directa 60s')
plt.scatter(x_frio, y_friodirecta, color='limegreen', marker='.', s=50, label='Temperatura directa enfriamiento')
plt.scatter(x_eq, y_eqdirecta, color='royalblue', marker='.', s=50, label='Temperatura directa en equilibrio')
plt.scatter(x_ambiente, y_ambienteamplificada, color='lime', marker='h', s=30, label='Temperatura amplificada Ambiente')
plt.scatter(x_60, y_60amplificada, color='crimson', marker='h', s=30, label='Temperatura amplificada 60s')
plt.scatter(x_frio, y_frioamplificada, color='deeppink', marker='h', s=30, label='Temperatura amplificada enfriamiento')
plt.scatter(x_eq, y_eqamplificada, color='coral', marker='h', s=30, label='Temperatura amplificada en equilibrio')   


plt.xlabel('Tiempo (ms)', fontweight='bold')
plt.ylabel('Temperatura (°C)', fontweight='bold')
plt.title('Temperatura LM35 vs Tiempo ', fontweight='bold')
plt.legend(fontsize=7)  

plt.tight_layout()
plt.show()

