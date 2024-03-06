The classic scientific method 
$$
\text{Hipotesis} \rightarrow \text{Data Collection} \rightarrow \text{Analysis}
$$
On the present there's **too many data**; so:
$$
\text{Data} \rightarrow \text{Exploration, Validate Hypothesis}
$$
Still important
- Formal Hypothesis
- Know the Meta Data
	- precision
	- missing values
	- where's from

**Data has become to big to contain into a file**

___
## Data Management
- Access
- Shared
- Safe

## Metadata
- Type/Format
- Meaning of Data
- Provenance

## Data
- Ever growing data

___

# DBMS (Data Base Management System)
Ever changing architecture to organize
- Schema
- Query's

# RDBMS (Relational DBMS)
## Comfort Zone (Safe for RDBMS)
- Complete data, homogeneous
- data in the same server
- vertical scaling is possible

## Components
- Data Strcuture
- Operations (Language like SQL)
- Integrity Constrains

___
# SQL

### Connecting 2 databases


### Grouping and Aggregation
Having a groupby everything needs to contain and aggregation function; like doing means or max. As a way to synthetize data into this smaller groups
```sql
SELECT strftime('%Y', day) as year, 
	   max(max) as year_max_wheaterStation
FROM daily_measyres
GROUP BY strftime('%Y',day)
```
In this example is posible to see:
- we can define column names in the select part, using existing columns
- we can give alias to functios

# SQL Cost's
### Querying
- Projection and selection
	- search $\mathcal O(1) \rightarrow \mathcal O(\log n) \rightarrow \mathcal O(n)$
- Joins
	- $\mathcal O(n) \rightarrow \mathcal O(n^k)$
	- Because is so big; when joining to big data is recommended to contact the manager's so they can review it and implement another quicker algorithm
- Grouping and aggregation
	- $\mathcal O(n \log n)$

Somethings mentioned:
- Cascade Deletion
- Integrity Constrains when Updating

## Q&A
- Pandas can read querys
	- https://pandas.pydata.org/docs/reference/api/pandas.read_sql.html
- MongoDB is used, but is pythonic so it was annoying for the workers

