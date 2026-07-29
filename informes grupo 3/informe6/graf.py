import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

df= pd.read_csv('datos_s6_20260506_095930.csv')

x=df.iloc[0:, 0]
y=df.iloc[0:, 1]

plt.scatter(x, y,  color='crimson', marker='.', s=50)

plt.xlabel('t (ms)', fontweight='bold')
plt.ylabel('Valor ADC (0-1023)', fontweight='bold')
plt.title('t vs raw_ldr', fontweight='bold')

plt.tight_layout()
plt.show()