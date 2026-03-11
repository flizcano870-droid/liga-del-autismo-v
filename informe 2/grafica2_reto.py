import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.optimize import curve_fit

df= pd.read_excel('datos2_reto.xlsx', header=None)   

x=df.iloc[:, 0]
y= df.iloc[:, 1]

def enfriamiento(t, T0, T_amb,k):
    return T_amb + (T0 - T_amb) * np.exp(-k * t)

parametros,covarianza = curve_fit(enfriamiento, x, y)
x_ajuste=np.linspace(min(x), max(x), 100)
y_ajuste=enfriamiento(x_ajuste, *parametros)    

plt.plot(x_ajuste, y_ajuste, color='orangered', linewidth=2.5, label=f'Ajuste: T(t) = {parametros[1]:.2f} + ({parametros[0]:.2f} - {parametros[1]:.2f}) * exp(-0.02 * t)')
plt.scatter(x, y, color='orangered', marker='o', s=20)
plt.xlabel('Tiempo (s)', fontweight='bold')  
plt.ylabel('Temperatura (°C)', fontweight='bold')        
plt.legend(fontsize=9)
plt.title('Enfriamiento de Newton: T vs tiempo', fontweight='bold')


plt.show()
