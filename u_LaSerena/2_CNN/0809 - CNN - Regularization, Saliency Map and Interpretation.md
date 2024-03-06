- What can go wrong with classification?

Resumen:

- Parameters: 
	- Convolution Layers and Pooling Layers extract features.
		- CNN -> Learned Filters
		- Poolign Layers -> Reduce Network Size and surpress noise

Loss Computation:
How much our output deviates from target output
- instance loss:
$$
e_i = 
$$
Then total Loss:
$$
E = \frac{1}{T} \sum_{i=1}^T e
$$
Then parameters $\theta$ get updated by gradient descent
![[Pasted image 20230809165446.png]]

**non convex optimization problem**: You can get trapped in a local minima

___
### Concepts:
- Loss Landscape
	- https://losslandscape.com/galley/

___

## Optimization Section
For optimization
[[0809 - Optimization - Gradient Descents Algorithms]]
[[0809 - Learning Rate]]

## Learning and Optmization Section
[[0809 - Vanishing and Exploding Gradients and Regularization]]

___
# Visualize the CNN
### Filters
It's possible to see what are the learned filters: (*plotting with Keras*)
![[Pasted image 20230809175500.png]]
This is donde with **Gradient Ascent**
	- Starts with random image and evolves it until it maximizes the score for a particular class
		- Is this done filter wise?

### Saliency Map
Pixel attribution methods;
- highlights what pixels where relevant for the classification
![[Pasted image 20230809175733.png]]
If we see that it uses a lot of the water, maybe it learned to identify water rather than the ship
- will it work in the construction hangar?
- will it work with a ship in the grass?
	- According to the saliency map it may not have much luck

#### How to create a Saliency Map?
- Deconvolutional Network Approach
	- *(Zeiler & Fergus (2013))*
- Gradient Based
	- *(Simonyan et al. (2013))*
- class activation mapping (CAM)
	- *Which image triggers the most a particular class*



