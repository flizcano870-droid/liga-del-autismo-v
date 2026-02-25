import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats

df= pd.read_excel('datoslab2.xlsx')
fig, ax= plt.subplots( figsize=(10,10))


X1=df.iloc[:,0].dropna().tolist()
Y1=df.iloc[:,1].dropna().tolist()     

X2=df.iloc[:,2].dropna().tolist()
Y2=df.iloc[:,3].dropna().tolist()   

X3=df.iloc[:,4].dropna().tolist()   
Y3=df.iloc[:,5].dropna().tolist()

X4=df.iloc[:,6].dropna().tolist()
Y4=df.iloc[:,7].dropna().tolist()

valores_x = [10, 100, 200, 500]
ax.set_xticks(valores_x)

ax.scatter(X1, Y1, color='blue', alpha=0.6, linewidth=0.5, s=50, marker= 'o', label='delay 10')
ax.scatter(X2, Y2, color='orange', alpha=0.6, linewidth=0.5, s=50, marker= 'o', label='delay 100')
ax.scatter(X3, Y3, color='green', alpha=0.6, linewidth=0.5, s=50, marker= 'o', label='delay 200')
ax.scatter(X4, Y4, color='red', alpha=0.6, linewidth=0.5, s=50, marker= 'o', label='delay 500')

ax.set_xlabel('Delay', fontsize=15, labelpad=10)
ax.set_ylabel('Microsegundos', fontsize=15, labelpad=10)
ax.set_title('Comparación de tiempos de respuesta para diferentes delays', fontweight='bold', fontsize=16,
             y=1.05)

plt.show()

