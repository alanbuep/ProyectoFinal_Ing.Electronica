import numpy as np
import cmath
import matplotlib.pyplot as plt
import scipy.fftpack as fftpack


#x = np.arange(0, 10, 0.5)
x = np.genfromtxt('FFTcolpitts_frec_spice.txt', delimiter='')
x = x.tolist()

y_real = np.genfromtxt('FFTcolpitts_real_spice.txt', delimiter='')
y_real = y_real.tolist()

y_imag = np.genfromtxt('FFTcolpitts_imag_spice.txt', delimiter='')
y_imag = y_imag.tolist()

for i in range(len(x)):
    x[i] = x[i]/1000

for i in range(len(y_imag)):
    y_imag[i] = complex(y_real[i],y_imag[i])

print(len(y_imag))
print(len(x))

y = np.abs(y_imag)

plt.figure(figsize=(10,5))
plt.plot(x,y)
plt.title('FFT generador de señal', fontsize=15)
plt.xlabel('Frecuencia (kHz)', fontsize= 11)
plt.ylabel('Amplitud (V)', fontsize= 11)
plt.xlim(0, 5000)
plt.grid()
plt.show()