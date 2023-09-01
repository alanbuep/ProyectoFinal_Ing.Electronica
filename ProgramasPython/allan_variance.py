import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft
from scipy.signal import periodogram
from scipy.optimize import curve_fit
import pandas as pd
import allantools


df = pd.read_csv('C:\\Users\\alanb\OneDrive\\Documentos\\Proyecto suelto\\Mediciones_v2 - Sheet4.csv')

data = allantools.Dataset(data=df['A'])
data.compute("mdev")

data.write_results("output.dat")

# Crear un objeto Plot de allantools
b = allantools.Plot()

# Graficar los datos con barras de error y grilla
b.plot(data, errorbars=True, grid=True)

# Personalizar el eje x
b.ax.set_xlabel("Tau (s)")

# Obtener la figura actual
fig = plt.gcf()

# Personalizar el tamaño de la figura
fig.set_size_inches(10, 5)

# Personalizar el título del gráfico
plt.title('Desviación de Allan')

# Mostrar el gráfico
plt.show()