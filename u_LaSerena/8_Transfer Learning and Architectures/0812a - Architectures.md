- Different architectures provide new features and new weakness
- ImageNet is a huge data set of labeled images

### Image Recognition Speak
- New model architectures ImageNet outperform humans
	- https://paperswithcode.com/
# State of the art Architectures:
- LeNet-5
	- *Gradient Base Learning applied for Document Recognition. (1998)*
- AlexNet
	- *Alex Krizhevsky (2012)*
	- Augmented data set
	- Dropout regularization
- VGG
	- *Simonyan and Zisserman (2014)*
	- Localized labels
- GoogLeNet
	- *Szegedy (2014)*
	- Inception Module repeated many times
		- Parallel Filter Operations with different sizes
	- 1x1 Kernel Convolution
		- Mixes channel without introducing many parameters
		- Keeps Spatial Information
	- Problem:
		- Many Many Computations
		- Solution:
			- **bottleneck layers**
		- May layers cause exploding vanishing gradients
		- Solution:
			- **Auxiliary classifiers**
			- Introducing more Losses source helps to produce more gradient in between of the network
- ResNet
	- *Microsoft 2015*
	- Contains Residuals in a Residual Block
- DenseNet
	- *Huan 2016*
	- Many interconnected residual layers
		- Allow maximum information (gradient flow) by connecting every layer
	- Does not sum the outputs of every layer
		- Stacks the outputs
$$
a^{[l]} = g([ a^{[0]},a^{[1]}, \dots ])
$$
