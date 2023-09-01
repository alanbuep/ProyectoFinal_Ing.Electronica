import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft
from scipy.signal import periodogram
from scipy.optimize import curve_fit
from scipy.stats import norm
import statistics
import pandas as pd

df = pd.read_csv('C:\\Users\\alanb\OneDrive\\Documentos\\Proyecto suelto\\Mediciones - Copy of Sheet2.csv')
data = df['S2'].to_numpy()

mu, std = norm.fit(data)
promedio = np.mean(data)

# Plot the histogram.
plt.figure(figsize=(10, 5))
plt.hist(data, bins=10, density=True, alpha=0.6, color='blue')
plt.grid()
# Plot the PDF.
xmin, xmax = plt.xlim()
x = np.linspace(xmin, xmax, 100)
p = norm.pdf(x, mu, std)

# Valor de sigma
sigma_calc = std
print('La desviacion estandar o sigma es:', std)

# Error
error = sigma_calc / np.sqrt(len(data))
print('El error estadistico es:',error)

# Agregar texto con el valor de la desviación estándar y el error
# text = "Desviación estándar (Hz): {:.2f}\nError estadístico (Hz): {:.2f}".format(std, error)
text = "Desviación estándar: {:.2f}Hz\nPromedio: {:.2f}Hz\nError estadístico: {:.2f}Hz".format(std, promedio, error)
plt.text(0.7, 0.96, text, transform=plt.gca().transAxes, fontsize=10,
        verticalalignment='top', bbox=dict(facecolor='white', alpha=0.9))

plt.plot(x, p, 'k', linewidth=2, color='red')
# title = "Fit Values: {:.2f} and {:.2f}".format(mu, std)
# plt.title(title)
plt.title('Ajuste Gaussiano del Histograma')
plt.ylabel('Densidad de probabilidad')
plt.xlabel('Frecuencia (Hz)')
plt.show()