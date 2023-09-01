# Leemos la señal en paquete de datos
# Calculamos la FFT para cada paquete
# Calculamos la frecuencia dominante

import matplotlib
import winsound
import scipy.fftpack as fourier
import scipy.io.wavfile as waves
import matplotlib.pyplot as plt
import numpy as np
import pyaudio as pa
import struct

matplotlib.use('TKAgg')

# Seteo parámetros para el procesamiento de audio
Frames = 1024 * 164                       # Tamaño del paquete a procesar
Format = pa.paInt16                       # Formato de lectura Int 16 bits
Channels = 1
Fs = 192000                               # Frecuencia de muestreo tipica de audio

p =pa.PyAudio()

# Abrir Stream de audio para lectura y escritura
stream = p.open(format= Format,
                channels= Channels,
                rate= Fs,
                input= True,
                output= True,
                frames_per_buffer= Frames)

# Crear gráfico con 2 subgráficos
fig, (ax1, ax2) = plt.subplots(2, figsize=(12, 6))

x_audio = np.arange(0, Frames, 1)
x_fft = np.linspace(0, Fs, Frames)

line, = ax1.plot(x_audio, np.random.rand(Frames), 'r')
line_fft, = ax2.semilogx(x_fft, np.random.rand(Frames), 'b')

ax1.set_ylim(-3000, 3000)
ax1.ser_xlim = (0, Frames)

Fmin = 1
Fmax = 40000
ax2.set_xlim(Fmin, Fmax)
fig.suptitle('Entrada de datos en tiempo real', fontsize= 12)
ax1.set_xlabel('Muestras por trama')
ax1.set_ylabel('Amplitud (V)')
ax2.set_xlabel('Frecuencia (Hz)')
ax2.set_ylabel('Amplitud (dB)')
ax1.grid()
ax2.grid()
fig.show()

# Crear vector de frecuencia para encontrar frecuencia dominante
F = (Fs/Frames) * np.arange(0, Frames//2)

while True:
    # Leer paquetes de longitud Frames
    data = stream.read(Frames)
    # Convertir datos del paquete en bytes
    dataInt = struct.unpack(str(Frames) + 'h', data)
    # Asignar datos a variación de curva temporal
    line.set_ydata(dataInt)
    # Calcular FFT y magnitud de FFT para paquete
    M_gk = abs(fourier.fft(dataInt) / Frames)

    ax2.set_ylim(0, np.max(M_gk + 10 ))
    # Asignar magnitud de FFT a curva de espectro
    line_fft.set_ydata(M_gk)
    # Tomar la mitad del espectro
    M_gk = M_gk[0:Frames//2]
    Posm = np.where(M_gk == np.max(M_gk))
    # Encontrar frecuencia correspondiente a magnitud máxima
    F_fund = F[Posm]

    print (int(F_fund))

    fig.canvas.draw()
    fig.canvas.flush_events()
