**Attention is all you Need**
$$
z_i =  \alpha_{ij} v^j
$$
where $\alpha$ is related to the similarity of words, a normalized dot product
$$
\alpha_{ij} = \text{softmax} \frac{v^j \cdot v^i}{|v^j||v^i|}
$$
- we can use softmax for getting a normalized

## Attention System is the Matrix $\alpha$
Existing the weights for pre transforming the vectors before applying the simmilarity idea
![[Pasted image 20230810121242.png]]
![[Pasted image 20230810121715.png]]

- it becomes a trainable thing: $W$
$$
Q = W_Q V
$$
$$
K = W_K V
$$
values weights is for the weights that I multiply $\color{magenta}\Lambda = \alpha_{ij}$ with
$$
\color{cyan}U = W_V V
$$
then:
$$
\color{magenta}\Lambda = \text{softmax}(\frac{Q K^T}{\sqrt{d_k}})
$$
- Scaled dot product
	- $d_k$ is the dimension of key and query vectors

So the output of this will be the $\color{yellow} Z$ context embedding
$$
\textcolor{yellow}Z = \textcolor{magenta}\Lambda \textcolor{cyan} U
$$
This is then used as an embedding $E_t$ for the classical $\text{LLM}$
$$
E_t \rightarrow Z
$$
[[0810 - Language Models LM]]
___
### Attention Issues
- Limited contextual mapping
- No positional information

For fixing we introduce: *Multi head Attention Blocks*
## Multiheaded Block
We introduce the multiple vector output to a *NN* to get the context prediction
![[Pasted image 20230810122832.png]]
- The skip connection is for the vanishing gradient problem

___
## Q&A
- Ca we do TimeSeries
	- AstroMer (Transformer for Time Series)
	- We replace CrossEntropy (classes) with MSE (values)

___
## We just built the Encoder
![[Pasted image 20230810123837.png]]


___
# Positional Encoding
- Adding a simple trained vector is too expensive to train
	- because we add it before training; so it has too long of a path for the derivatives to get there
- Using the orthogonality of sin's and cosine's we can use the *dimensionality* of the model
![[Pasted image 20230810124357.png]]




