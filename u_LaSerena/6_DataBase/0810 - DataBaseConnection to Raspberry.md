# Proceso
- Nos conectamos al servidor que esta en la RaspberryPi
- Se conecto a la Wifi de ese server:
    - Wifi, Wifi Pass
- Se ingreso al servidor levantado
    - ServerName, ServerAdress

Serverworking:
- Postgress
```python
import psycopg2

con = psycopg2.connect(
	host='192.168.0.100', database='betelgeuse_2', 
	user='betelgeuse', password='esueglet')

cur = con.cursor()

```
![[Pasted image 20230810145829.png]]

Localworking:
- Sqlite

___
# Step by Step
- connected to wifi of raspberry pi
- Ran the `introdb2023_postgresql.ipynb`


Diferencia entre executar y pandas query
```python
# Solo ejecuta cambiando la DB
cur.execute("DELETE * from daily_measures")

# solo lee
total_measures = pd.read_sql_query("SELECT count(*) FROM daily_measures", con)

```


### Preguna 1.
existen sensores de viento en estaciones que no son LSC?
```python
pd.read_sql_query(
	"SELECT * FROM sensors WHERE station_code!='LSC' and type='wind speed'", con)
```

### Importancia de Join Conditions
Al especificar las condiciones de que deben ser la misma estacion y mismo codigo de sensor, para que entonces se trate de la misma medida:
```python
vv_measures = pd.read_sql_query("""

    SELECT sensors.station_code, day, max FROM daily_measures, sensors
    WHERE sensors.type='wind speed'
    and sensors.sensor_code = daily_measures.sensor_code
    and sensors.station_code = daily_measures.station_code
    """, con)

vv_measures
```

### Pregunta 2
Como encontrar los tipos de sensores que se encuentran en las 3 estaciones
- se compara a la tabla de sensor consigo misma
```python
# join sensor's with itself

pd.read_sql_query("""

    SELECT DISTINCT s1.type

    FROM sensors as s1, sensors as s2, sensors as s3

    WHERE s1.type = s2.type and s2.type = s3.type and s1.type=s3.type

        and s1.station_code != s2.station_code

        and s3.station_code != s2.station_code

        and s1.station_code != s3.station_code

    """, con)
```

