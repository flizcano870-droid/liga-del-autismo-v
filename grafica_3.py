import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy import stats

# Datos 
df= pd.read_excel('datos3.xlsx', header=None)

y=df.iloc[:,0].values
x=df.iloc[:,1].values
# Hacer regresión lineal con scipy
pendiente, interseccion, r_value, p_value, std_err = stats.linregress(x, y)

print(f"Ecuación: y = {pendiente:.2f}x + {interseccion:.2f}")
print(f"R²: {r_value**2:.3f}")
print(f"P-value: {p_value:.3f}")
print(f"Error estándar: {std_err:.3f}")

# Crear línea de regresión
x_linea = np.linspace(min(x), max(x), 100)
y_linea = pendiente * x_linea + interseccion

# Graficar
plt.figure(figsize=(10, 6))
plt.scatter(x, y, color='darkslateblue')
plt.plot(x_linea, y_linea, color='darkslateblue', label='Regresión lineal', linewidth=2)
plt.xticks(np.arange(0.1, 0.6, 0.1))
plt.yticks(np.arange(0.5, 2.5, 0.4))



plt.xlabel('Longitud del péndulo (m)', fontweight='bold')
plt.ylabel('Periodo al cuadrado (s^2)', fontweight='bold')
plt.title('T^2 vs L', fontweight='bold', fontsize=16)
plt.legend()

plt.show()

