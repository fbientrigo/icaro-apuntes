BERT is a multiheaded self attention model;
it's use because its pretrained; google already used then thousands dollars for training
	Its very slow and expensive to train

Opensource and easy to take and adapt to individual task
- Good for transfer learning

Flavors of BERT:
- Base (Google trained and give it to us)
	- 12 encoders (12 layers)
		- 12 heads for each one
	- 768 dimension for vectors
- Large BERT
	- 24 encoders
		- 16 heads
	- 1024 dimension

## Technical Details
- Inputs needs special tokens
	- $[MASK]$ masking tokens
	- $[SEP]$ separator token
	- $[CLS]$ classifier tokens

# Training BERT
Semi supervised pre training (we don't need labels, only predict one word given the others) (pre training means transfer learning)
**We have to train the model for this 2 task at the same time:**
- Masked Language Model
	- " Shivas was hit by a [MASK] as he was crossing the street"
- Next Sentence Prediciton

We give it only text; and it will play with the text;
no labels or anything, it will only eat text

## Masked Language Model
### Forward
- Give it a sentence
	- Mask something
- Get attention vectors
- Use a FFNN + softmax
	- predict masked word
	- compare it with real word

### Con's
Compares the Masked word very well; but is very bad to predict other words
- We pick other words that are unmasked, change them to a random word so *it can't use the identity matrix, like remembering the word*

### Masking details for Training
This percentages are Hyperparameter Tuning
- Take 15% of input token and *ask* the model the predict it
	- from those selected 80% will be masked
	- 10% random words
	- 10% same words

## Next Sentence Prediction
- Use *negative sampling*
	- scramble the data

___
# How to use BERT
- BERT will give context embeddings
	- then we are able to do anything with them; because they area *extracted information*
- Classification Task
- Named Entity Recognition
- Question Answering Task







