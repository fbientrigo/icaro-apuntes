Seq2Seq
[[0809 - LSTM]]
- Encoder RNN 
	- Get words into vectors also with the information about connection with the words
		- hidden layers
		- tokenize information
- Decoder RNN
	- Vectors into a sentence

Seq2Seq Attention
- Add attention for the encoders

Attention is all you need
[[0810 - Transformers LM]]
- Ditch the encoder RNN, just leave the attention and then a feed forward NN

___
### Architecture
- Bidirectional Encoder Representations from Transformers *BERT*
	- Take the text and do anything (except text generation)