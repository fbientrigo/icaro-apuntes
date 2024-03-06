
**Deep Generative Modeling. J Martinez Palomera, J.S. Bloom, E. S. Abrahams (2022)**
- Se utiliza el espacio de baja dimensionalidad para introducir variaciones minimas que mantengan la forma de la data
	- Generando asi nueva data

**Deep NonLinear Autocneoders. G. Hinton, R. Salakhutdinov**
- Paso a paso ir comprimiendo mediante varias capas de manera que el modelo aprenda pasos a pasos a comprimir la informacion
___
[[u_LaSerena/Slides/autoencoders.pdf]]
____
- Es posible realizar clustering en este espacio bajo dimensional
- Mapea data alta dimensional a bajas dimensiones
- Generaliza aprendiendo features abstractas en una forma unsupervisada

___
## Segmentation
Autoencoder me permite entonces correr un CNN con Pooling para asi conseguir una segmentacion de la imagen.
- No es necesario tener un training label
	- Es posible comparar la segmentacion obtenida con una segmentacion obtenida de otro algoritmo
## Aprovechar la Representacion bajo dimensional
Podemos entrenar una red neuronal que aprenda la representacion de bajas dimensiones;
- El encoder mantiene las features principales
- reduciendo el trabajo de clasificion
___
## Training
Se utiliza una L2 Loss function comparando los pixeles de la imagen
$$
L_2 = |\vec x - \vec x'| ^2
$$

existe una llamada: *reconstruction loss function*
$$
E = \sum |\vec x  - \vec x'|
$$
- Se desea fijar los pesos del autoencoder, de manera que sea simetrico
$$
W' = W^T
$$
### Hyperparameters
- Code size: Size of bottleneck
- depth: numero de layers, controls complexity
- nodes per layer
- reconstruction loss $E$

**Un autoencoder posee demasiada capacidad, asi que necesitamos realizar regularizacion**
	[[0809 - CNN - Regularization, Saliency Map and Interpretation]]





___
## Diferencia con un PCA
Un *PCA* se trata de un mapa lineal; mienras que el autoencoder puede contener mapeos no lineales
- PCA Lineal
- autoencoder no Lineal

