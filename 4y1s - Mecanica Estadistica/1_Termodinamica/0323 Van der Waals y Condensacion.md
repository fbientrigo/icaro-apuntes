Mediante el modelo de interacción, tenemos particulas como esferas de diametro $\sigma$, pueden interactuar entre ellas
![[Pasted image 20230323191052.png]]
interacción de 2  esferas con diametro $\sigma$ 
- la región oscurecida esta ocupada por este par, por tanto para un mol habra una exclusión de : vo
$$
\frac{4\pi }{3}\sigma^2 [\text{vol excluido por}/2 \text{ particulas}]
$$
osea que por particula, en un mol tendriamos unaexclusición de
$$
V_0=\frac{2\pi }{3}\sigma^2 \; N = b' N
$$


teneindoa si fuerzas de exclusión y atracción (no derivada aqui, ya que requiere mecanica estadistica):

$$
(P + \frac{a' N^2}{V^2})(V-Nb') = Nk_B T

$$

N: numero de particular

tambien siendo posible escribirla en moles $n = N/N_A$

$$
(P + \frac{a n^2}{V^2})(V-nb) = n R T

$$

___
# podemos comparar comportamiento al ideal

Tenemos las fuerzas de van der Waals:
- repulsivas: al comprimir las moleculas entregan una fuerte repulsion que evita esto. *a distancias cortas las moleculas se repelen por sus nubes electronicas*
- atractivas: a grandes distancias las *fluctuaciones estadisticas* pueden dar lugar a una atraccion universal entre las moleculas; a mayor cantidad de electrones y peso molecular mayor atracción.
	- mientras domine la energia termica: permanecera en gas
	- a bajas temperaturas domina la condensación en liquido

![[Pasted image 20230323192338.png]]

Como se observa a mayor peso atomico, tenemos mayor atracción;
claro que eventualmente cuando tenemos mucha presión las moleculas interactuan fuertemente repeliendose.

*Recordemos: a presiones bajas y muchas cargas, tenemos mucha atracción. A presiones muy altas, buscaran repelerse*

### efecto de temperatura
![[Pasted image 20230323193629.png]]
a mayor temperatura, mayor movimiento termico -> por tanto no hay mucha tranquilidad como para que ocurra la atracción. Osea que acabara ganando la repulsión.

a menor temperatura es mas facil tener atracción

- Temperatura de Boyle, a esta temperatura en un buen rango las fuerzas de atraccion y repulsion se balancean, dependiente para cada material

___
# condensado en gases reales
![[Pasted image 20230323192154.png]]

en isotermas mas bajas a la hora de comprimir, cualquier intento simplemente forma liquido cuya mayor densidad (mas masa en menos volumen) balancea perfectamente esta menor cantida de volumen entregada

- De acuerdo a lo que vimos previamente que sucede aqui con las isotermas mas frias?
	- -> tenemos que a menores temperaturas se convierte en liquido; lo que vemos es una aplicación directa de como las fuerzas de atracción ganan en este intervalo de temperatura.

Lo que vemos es un slice de las isotermas
![[Pasted image 20230323194001.png]]

Si miramos la ecuacion de van der Waals graficada como isotermas:
![[Pasted image 20230323194232.png]]
nos entrega soluciones no fisicas, mas aun, existe en este intervalo
- 3 volumenes que nos entrega la misma presion

*Contradice la observacion experimental que con estos 3 estados, Presion, Volumen y Temperatura, podemos determinar una correspondencia unica del estado del sistema*

Y no fisicas pues
$$
(\frac{\partial P}{\partial V})_T <0
$$
al disminuir el volumen, paradojicamente disminuye la presion, por tanto podemos unirlo al coeficiente de compresibilidad:
$$
\kappa = - \frac{1}{V} (\frac{\partial V}{\partial P})_T 
$$
la cual en el caso usual es positiva, donde el incremento de presion provoca un decremento de volumen;
entonces podemos reescribir:
$$
V\kappa = -  (\frac{\partial V}{\partial P})_T  = - 1/ (\frac{\partial P}{\partial V})_T >0
$$
osea que aplicando el signo menos:
$$
\rightarrow (\frac{\partial P}{\partial V})_T <0
$$
en el caso normal, un decremento del volumen debe causar un aumento de la presion.

Se dice que un un sistema con compresibilidad $\kappa <0 \rightarrow (\frac{\partial P}{\partial V})_T >0$ no puede describir a un sistema en equilibrio.

Maxwell arreglo este problema con la construccion de una isoterma que no tenia estos problemas:
[[0324 -f Construccion de Maxwell]]






