import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft
from scipy.signal import periodogram
from scipy.optimize import curve_fit
import pandas as pd

PI = np.pi

# Inductor
L = 9e-6

# Frecuencia promedio y error de la muestra
f_prom = 22761
error_gauss = 16
# Frecuencia y error del AD9833
f_ad9833 = 1852000
error_ad9833 = 1

# Frecuencia Colpitts
f_colpitts = f_prom + f_ad9833

# Frecuencia Colpitts con incerteza
f_prom_err_mas = f_prom + error_gauss
f_ad9833_err_mas = f_ad9833 + error_ad9833
f_prom_err_menos = f_prom - error_gauss
f_ad9833_err_menos = f_ad9833 - error_ad9833

f_muestra_mas = f_prom_err_mas + f_ad9833_err_mas
f_muestra_menos = f_prom_err_menos + f_ad9833_err_menos

# Capacidad muestra
C_muestra = 1 / (4 * (PI**2) * (f_colpitts**2) * L)

# Incerteza de la muestra
C1 = 1 / (4 * (PI**2) * (f_muestra_mas**2) * L)
C2 = 1 / (4 * (PI**2) * (f_muestra_menos**2) * L)

capa_error = C2 - C1

print("Capacidad muestra: ", C_muestra)
print("Incertaza de la capacidad: ", capa_error)