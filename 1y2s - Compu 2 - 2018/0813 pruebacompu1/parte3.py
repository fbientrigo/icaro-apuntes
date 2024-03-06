from potencial import get_yuka 
import matplotlib.pyplot as plt
import numpy as np

#Ahora se nos pide hacer un grafico, una curva de este, defino r como un array
rad = np.linspace(0.01,0.1,100)

plt.figure()

for j in range(10):
    
    k = 10 * (j+1)
    potenciales = get_yuka(rad,k)
    plt.plot(rad,potenciales)
    plt.legend(str(k))

    

plt.show()
                     
