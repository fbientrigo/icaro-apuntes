At backpropagation the multiplication of many terms
$$
\lim_{N \rightarrow \infty}\Pi_i^{N} g_i = \begin{cases} 
0 \; \; \forall g_i<1
\\
\infty \; \; \forall g_i>1
\end{cases}
$$
- $g_i$ is the gradient at one layer

AlexNet uses special architecture to avoid exploding gradients

___
## Quick Fixes for Vanishing Gradient
- Add an skip connection
___
## Dangers:
- Initial weight assignments with too large losses can get exploding gradient
	- Nina's recommendation is to not initilize weights at random
- Weights can get very big after updating; so they overflow and is impossible to keep training

___
## Identify Vanishing Gradient
- Model weights may become 0
- Parameters at top layers change too much and parameters at lower layers not much (our gradient vanished)
- The training stagnates *after few iterations*

## Identify Exploding Gradient
- Model weights may become *Nan*, overflowing and stop training
- *Avalanche Learning*

___
# Overcome this Issues
### Weight Initialization 
#### Xavier Init
- Xavier Initialization *Considering Sigmoid Activation Function*
	- X. Glorot & Y. Benigo (2010)
	- **Variance of outputs of each layer should be equal to its inputs**
		- So the variance should not increase 
	- **Gradients should have equal variance in forward and backward direction**
		- This is only possible if the number of neurons on layer is equal to layers output

#### He Init


### Regularization
$$
E(\theta) = \frac{1}{n} \sum_{i}^m E_i(y,\hat y) + \textcolor{magenta}{\lambda R(\theta)}
$$
Lets remember that the network architecture play's a role; for preventing overffiting:
- Limit hidden neurons
- Early Stopping
- *DropOut*:
	- uses less computer resources (ignores calculations)
	- creates generalized learning

#### Weights Decay
Limit number of weights -> make more weights close to 0 (*decay*)
- $\lambda \approx 0.1$ 
$$
C = E + \textcolor{cyan}{\frac{\lambda}{2} \sum_i w_i^2}
$$
$$
\partial_{w_i}C=\partial_{w_i}E + \textcolor{cyan}{\lambda w_i}
$$
Benefits:
- Improved generalization
- reduces complexity encouraging a sparse learning representation
- improves convergence by having smaller weights

#### Data Augmentation
Increasing training set by:
- Rotation
- Add noise
- Smooth
- Mirror
- Rotate and crop

#### Batch Normalization
Act's as a regularization for the data
- provides *covariance shift*

