import numpy as np

g = 1
k = 1
m = 1

#Calculo r en base a las coordenadas, esto lo hago con otra funcion creada
def get_r(x,y):
    return np.sqrt(x*2 + y*2)

#Defino la funcion que utilizare
def get_yuka(r,k=1):
    return -g**2 * (np.exp(-k*m*r))/(r)

def calcularYuka():
    x_pos = input("Ingrese coordenada x: ")
    y_pos = input("Ingrese coordenada y: ")
    
    r = get_r(x_pos,y_pos)
    yukaw = get_yuka(r)
    
    print(yukaw)
