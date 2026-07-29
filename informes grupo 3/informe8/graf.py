import matplotlib.pyplot as plt
import numpy as np

# Tus datos
N_labels = ['1', '4', '16', '64', '256']
teorico = [1.00, 0.50, 0.25, 0.125, 0.0625]
medido = [1.00, 0.469, 0.305, 0.188, 0.091]

# Posiciones en el eje X
x = np.arange(len(N_labels))

# Configuración de la figura
fig, ax = plt.subplots(figsize=(8, 6))

# Serie 1: Datos experimentales (Barras)
ax.bar(x, medido, width=0.5, label='Medido (σ_over / σ_cruda)', color='bisque', edgecolor='sandybrown')

# Serie 2: Curva teórica (Línea)
ax.plot(x, teorico, marker='.', color='darkorange', linestyle='-', linewidth=2, label='Teórico (1/√N)')


ax.set_xlabel('Factor de Oversampling (N)', fontsize=12, fontweight='bold')
ax.set_ylabel('Ratio de Ruido', fontsize=12, fontweight='bold')
ax.set_title('Reducción de ruido por oversampling', fontsize=15, fontweight='bold')
ax.set_xticks(x)
ax.set_xticklabels(N_labels)
ax.legend()
ax.tick_params(axis='both', which='major', labelsize=14)

for i in range(len(medido)):
    ax.text(x[i], medido[i] + 0.02, f'{medido[i]:.3f}', ha='center', va='bottom', fontsize=9)


plt.ylim(0, 1.2) 
plt.tight_layout()

plt.show()