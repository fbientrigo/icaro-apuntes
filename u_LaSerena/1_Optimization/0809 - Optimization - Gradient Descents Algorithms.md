# Optimization
## Gradient Descent
A single update visits all data; highly expensive
$$
\vec v_k = \eta \nabla_\theta E(\vec \theta_k)
$$
$$
\vec \theta_{k+1} = \vec \theta_{k} - \vec v_k
$$
- requirements and safety notes
	- sensitive to initial conditions
	- stuck in local minima
	- learning rate heavily affects performance

## Stochastic Gradient Descent
Separates the training set, selecting a random minibatch for updating; 
- using only one data point of a minibatch for updating
$$
\vec \theta_{k+1} = \vec \theta_{k} - \eta \nabla_\theta E_{mb}(\vec \theta_k)
$$
- a full iteration of all minibatches is an epoch
	- we ended up picking a subset and doing smaller averages for updating

Pros:
- Less chance to end up in *local minima*

Cons:
- Heavily affected by *outliers*; giving them more power
- More time for convergence comparing SGD with GD

## Minibatch Gradient Descent
Visits all points in a minibatch as the classic GD; getting the best of best worlds

Pro:
- Faster convergence than SGD

Cons:
- Prone to overfitting, because could have repeated data

## SGD with Momentum
Implements and inertia terms which saves the last direction it took
$$
\vec v_k = \textcolor{orange}{\gamma \vec v_{k-1}}  + \eta \nabla_\theta E_m (\vec \theta_k)
$$
The factor is close to $\gamma =0.9$
### There's a Nesterov Momentum Update:
slow's down as approaching optima
$$

\vec v_k = 
\textcolor{orange}{\gamma \vec v_{k-1}} 
 + \eta \nabla_\theta E_m (\vec \theta_k - \textcolor{orange}{ \gamma \vec v_{k-1} } )
$$






