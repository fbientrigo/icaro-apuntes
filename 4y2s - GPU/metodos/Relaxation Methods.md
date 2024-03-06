Los metodos de relajamiento requieren que conozcamos un buen estimado de la solución, y luego se utiliza la ecuación diferencial para hacer evolucionar el sistema;

los metodos se basan en partir con ecuaciones diferenciales y dejarlas en Finite Differential Equations o FDE;

de maner que quede propuesto como un problema de algebra lineal
$$
\rm{A} \, \rm{ x } = \rm b
$$
descomponiendo la matrix
$$
\rm{A} = \rm{L_*} + \rm{U}
$$
![[fe6016c394a88dd99523b2bb9eefeeec68c8434c.svg]]

la solución sera obtenida:
$$
\rm{L_*} x^{(k+1)} = b - U x^{(k)}
$$
analiticamente deseamos tener:
