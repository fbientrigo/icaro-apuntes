### Quick references:
- https://modelzoo.co/ get per trained models

# Transfer Learning
Transfer learning for image; follows the idea that a big image data set will provide a general learning of shapes and fetures; then it can be trained to be more specific with the specific dataset
- Use training for a large data set
- keep training with smaller but specific data set

### Idea of Freezing
Freeze Extraction Layers; and Train the Classification Layers
- At training we freeze the first layers which will be the more general
	- Unfreeze the later layers which may be related to more specific task

### When it dosen't work?
To dataset's with too different types, example
- using an image trained in animals and trying to transfer to astronomy data

Whenever there are different low level features this will not work correctly

Then for changing the low level extraction we need to retrain, do fine tuning
## Differential Fine Tuning
1.- Freeze convolutional layer
2.- Train fully conected network used in classification
3.- Unfreeze later layers to train less general features

- Layers closer to input may learn general features
	- Each layer can be trained at 1/3 or 1/10 the learning rate comparing with layer for the next layer
	- Learning will occur quicker at later layer
	- Learning will be more carefull at the more general layers

# Transfer vs FineTuning?
- Simmilaritys between the *source* and *target* data; we can **Transfer**
	- we can add  new layer at the end and train for new features
- Depending of simmilarity:
	- mid layer feature extraction (slightly differents) (cat vs dogs)
	- full model fine tunning (very differents) (cat vs human)









