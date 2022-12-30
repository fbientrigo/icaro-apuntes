import math as m
print("Trabajo de Fabian Trigo")
while 1==1:
	t = float(input("Ingrese el valor del angulo en grados: "))
	r = float(input("Ingrese la magnitud del vector: "))

	t = m.radians(t)
	x = r*m.cos(t)
	y = r*m.sin(t)
	if(m.fabs(x) == m.fabs(m.sin(m.pi))):
		x=0
	elif (m.fabs(y) == m.fabs(m.sin(m.pi))):
		y=0
	print(x,y)
