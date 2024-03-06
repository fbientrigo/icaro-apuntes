## Unsupervised Learning
Dimensionality Reduction Techniques:
- PCA
- tSNE
- uMap
![[2023_LSSDS_Unsupervised.pdf]]
at the end we did some excercises on python

## Supervised Learning
![[2023_LSSDS_Supervised.pdf]]
Type 1 and 2 errors are really important; if we have umbalanced data sets we have to be careful. A model could learn to say always healthy to a disease that's very rare.

Precision:
$$
P =\frac{TP}{TP + FP}
$$

Recall:
$$
R = \frac{TP}{TP + FN}
$$

de esa manera hacemos un catch de los errores que conversamos
- un modelo que siempre tira positivo tendra $R=1$ pero un $P$ muy bajo
- y viceversa


