import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft
from scipy.signal import periodogram
import pandas as pd


df = pd.read_csv('Mediciones.csv')

data = df['A'].to_numpy()

end_range = len(data)

x = np.arange(0,end_range,1)

plt.figure(figsize=(12,7))
plt.plot(x, data)
plt.title('Mediciones de frecuencias dominantes', fontsize=15)
plt.xlabel('Muestra', fontsize= 11)
plt.ylabel('Frecuencia (Hz)', fontsize= 11)
plt.xlim(0, 700)
# plt.ylim(22700, 25700)

#plt.xticks(np.arange(0, 1, 0.1), fontsize= 9)
plt.yticks(fontsize= 9)

plt.grid()
plt.show()