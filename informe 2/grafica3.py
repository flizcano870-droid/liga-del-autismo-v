import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats


datos = {
    'Flancos espurios': [0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2]
}

# Histograma básico
n, bins, patches=plt.hist(datos['Flancos espurios'], bins=[-0.5, 0.5, 1.5, 2.5, 3.5], color='mistyrose', edgecolor='crimson', align='mid')
plt.xticks(np.arange(0,5,1)) 
plt.yticks(np.arange(0,22,4))  
labels = [f'{h:.0f}' if h > 0 else '' for h in n]
plt.bar_label(patches, labels=labels, fontsize=8, padding=3)
plt.title('Histograma de Flancos Espurios por Pulsación', fontweight='bold', fontsize=14)
plt.xlabel('Flancos espurios', fontweight='bold')
plt.ylabel('Frecuencia', fontweight='bold')
plt.grid(axis='y', linestyle='--', alpha=0.3)
plt.show()
