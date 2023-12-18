# Capacidad del equipo sin muestra
capacidad_equipo = 790.6859e-12
error_equipo = 50.29e-15
# Capacidad de la muestra y el equipo
capacidad_muestra_equipo = 800.7665e-12
error_muestra_equipo = 29.0448e-15

# Diferencia de capacidad. Capacidad de la muestra
capacidad_muestra = capacidad_muestra_equipo - capacidad_equipo

# Diferencia de capacidad maxima
diferencia_maxima = (capacidad_muestra_equipo + error_muestra_equipo) - (capacidad_equipo - error_equipo)

# Incerteza
incerteza = error_equipo + error_muestra_equipo

print("Capacidad de la muestra: ", capacidad_muestra)
print("Incerteza de la muestra: ", incerteza)