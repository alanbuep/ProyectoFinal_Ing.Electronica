import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack as fftpack


x = np.genfromtxt('tabla_time_spice.txt', delimiter='')
x = x.tolist()

y = np.genfromtxt('tabla_out_spice.txt', delimiter='')
y = y.tolist()

for i in range(len(x)):
    x[i] = x[i]*100000

print(len(y))
print(len(x))

plt.figure(figsize=(10,5))
plt.plot(x,y)
plt.title('Señal de salida del mezclador', fontsize=15)
plt.xlabel('Tiempo (mS)', fontsize= 11)
plt.ylabel('Amplitud (V)', fontsize= 11)
plt.xlim(0, 1)

plt.xticks(np.arange(0, 1, 0.1), fontsize= 9)
plt.yticks(fontsize= 9)

plt.grid()
plt.show()