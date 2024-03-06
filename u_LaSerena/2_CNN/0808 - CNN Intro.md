# Operacion:
$$
Y_{ij} = \sum_l \sum_k X_{i+l, j+m} W_{lm}
$$
tecnicamente es una cross relacion,
### Costo computacional
$$
\mathcal O(I^2 F^2)
$$
## Output Size
$I \times I$ : Image Size
$F \times F$ : Filter Size
$S$ : Stride (how much it moves, may be 1 pixel)

Output size:
$$
O=\lfloor \frac{I-F}{S}\rfloor+1
$$
$O\times O$ : Output Size

### ejemplo
![[Pasted image 20230808150559.png]]
Imagen $5\times 5$ con un filtro $3\times3$ y un stride de 1 pixel.
$$
O=\lfloor \frac{5-3}{1}\rfloor+1=2 +1 = 3
$$

# Zero Padding
Agregarlo 0's alrededor del tensor de la imagen, com si tuviesemos secciones vacias. Eso permite que la convolucion no se vuelva mas pequeña

# The Power of Channels
![[Pasted image 20230808152135.png]]

About receptive receptors:
- Receptive Size is important
- Dialated Receptor
- Receptive Field Arithmetic

Some links:
https://www.mdpi.com/2079-6374/12/1/22
https://www.mdpi.com/1424-8220/21/2/450

___

# Pooling
Down the Dimension Number
![[Pasted image 20230808154129.png]]

