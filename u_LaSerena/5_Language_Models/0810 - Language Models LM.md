$$
\mathbb P[x_{t+1}|x_t, x_{t-1},\dots, x_0;\theta]
$$
- Estimate the next word given history and model parameters $\theta$

### Work Order
- Input preprocessing
	- Lemmatization and Stemming
- Embedding
	- Words become vectors
		- *word2vec* or *one-hot encoding*
	- Similar words become similar vectors
- Feature Extraction
	- Take my data and summarize it with a latent space
	- NN are good at finding features to do something
- Task
	- Predicting next word or classifying
		- *Softmax* for predicting a probability of the next word

___
## Tiding UP
- RNN
	- A word token comes in
	- NN predicts:
		- Hidden State (Short Memory)
		- Output

____
# Embedding
We have 2 layers of LSTM, so we have 2 layers capable of creating a hidden state
Train with text; get the hidden states from going left ot right and the input embedding
$$
E_t = \{ x_t, h_t^{1l}, h_t^{2l},h_t^{1r},h_t^{2r}\}
$$
- Input one word we get 5 vectors
- can rename the hidden state from the left and right 
$h_t^1 = \{h_t^{1l}, h_t^{1r}\}$
$h_t^2 = \{h_t^{2l}, h_t^{2r}\}$
$h_t^0=x_t$
$$
E_t = \{h_t^0, h_t^1,h_t^2 \}
$$
- This embedding can be concatenated to the input

### Bidirectional LLM Embedding secret Sauce
$$
\text{LLE}_t = \gamma \sum_j^L s_j h_t^j
$$
$s_j$ are the weights/proportions for the ingridients

