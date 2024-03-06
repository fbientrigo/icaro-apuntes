#Parte 2

from potencial import get_yuka#Importo la funcion definida antes en el programa anterior
import matplotlib.pylab as plt
import numpy as np


#Ahora se nos pide hacer un grafico, una curva de este, defino r como un array
rad = np.linspace(0.01,0.1,100)

plt.plot(rad,get_yuka(rad))
plt.show()



