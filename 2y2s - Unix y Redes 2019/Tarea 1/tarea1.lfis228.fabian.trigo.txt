#!/bin/bash

#hacer un for para eliminar las lineas del archivo, para ello
#ingresamos todo lo que se debe borrar en un arreglo
declare -a borrar=(7 36 49 61 62 63 64 65 74 79 95 96 102 103 104 105 107 110 \
128 134 135 136 139 140 142 146 153 161 170 181 183 191 198 215 217 218 230 231 \
236 238 240 241 249 259)

cp poblacion.csv poblacion_procesado.csv
#limpia el archivo csv para solo incluir datos
sed -i '1,4d' poblacion_procesado.csv

#las lineas se reemplazan por null, de borrarse el numbeor de lineas cambiaria
#EXISTE ALGUNA FORMA DE BORRAR UN ARREGLO DE LINEAS A LA VEZ?
#COSA DE NO MODIFICAR EL NUMERO DE LINEAS AL BORRAR LA PRIMERA
for k in "${borrar[@]}"; do
	sed -i "$k s/.*//" poblacion_procesado.csv
done

#ahora borramos del fichero las comillas y comas
sed -i 's/,/;/g' poblacion_procesado.csv
sed -i 's/\"//g' poblacion_procesado.csv

#hasta este punto el fichero carece de lo innecesario y de las lineas amarillas

#2. Modificar las columnas
awk -F ";" '{print $1 ";" $2 ";" $42 ";" $62 ";" }' poblacion_procesado.csv>poblacion3.csv
#creamos un fichero temporal, poblacion3.csv

#lo cual nos deja las lineas que borramos con 4 ; seguidas
sed -i '/;;;;/d' poblacion3.csv

#ahora que el fichero esta mas despejado, podemos trabajar de mejor manera
#crearemos una nueva columna para las filas con datos de 1997 != 0
awk -F ";" '{ if ($3 > 0) {
	print $0, 100*($4-$3)/$3
}
}' poblacion3.csv>poblacion4.csv
#awk no puede modificar archivos en el lugar, solo crea archivos temporales
rm poblacion3.csv

awk -F ";" '{print $0 , ";" , $3+$4}' poblacion4.csv>crecimiento_poblacion.csv
rm poblacion4.csv

#se cambian los titulos
sed -i '1 s/1.0015/Crecimiento 20 years/' crecimiento_poblacion.csv
sed -i '1 s/4014/Total/' crecimiento_poblacion.csv

#disfrute su fichero organizado!
