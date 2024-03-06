Big ecosystem for LLM and many chatGPT extensions
- Med-PaLM 2 (*Medical*)
- Bloomberg GPT (*Finance*)
- KhanAcademy GTP (*Education*)

## Possibilities
- Education
	- Correcting Homework / Quiz Prompts
	- Syllabus Creation
	- Summarization of complicate topics
- Travel
- Retail and E-commerce
- Marketing
- ...
- *Human like responses*
	- changing how we interact with computers and information

## RLHF
Reinforced Learning from Human Feedback;
the 3 steps:
- Supervised fine tuning model
	- Prompt sampled from dataset's (*Questions and Answers*)
- Training a reward model
	- a correct answer is not obvious
	- The steps are:
		- Prompt data set
		- Generate Several Text
		- Human Rank the Answers
		- **Then train a NN to give the same reward as an human**
		- Loss function for comparing two models in a binary way:
$$
\mathcal L\propto-\mathbb E[\log[\sigma[ r_{qi} - r_{qj}]]]
$$
- Fine tune the model
	So the finetune of the models; we consider the an action of updating the generative model so it gets good rewards from the reward model
	- **Proximal Policy Optimization** (PPO)
$$ \mathcal L \propto E [\log[\mathbb P[a_t|s_t] A_t]]$$
- *Clipped surrogative objective*
	- It uses this clipping so the model dosent change that much

- They tried making answer 100% true
	- It stopped giving answer

We finished looking at the engine; now we will drive
# Prompt Engineering
- Start simple
	- Begin with a simpler sub task
- Use instructions for what to do
- Avoid saying what *not to do*
	- Center in what to do
- Be specific and precise
	- Set limits on the answer
	- output text in specific format
		- *in Markdown*
- Assign a role to the model
	- "Write in proffesional matter"

# Advance Prompt Engineering
- Zero shot
- Chain of thought
- Graph prompt
- ...

	### Zero shot prompting
	Give a feed task without a training example
	- it's recommended to give examples for the model
	### Few shot prompting
	Enables in-context learning *I give an example and ask for an aswer*
	### Chain of thought
	break down the answer into steps; so it's answers are better
	### Leverage the Memory
	"Lets continue the explanation of ... give an answer of 500 words"
	### Get First Wrong Answer
	Then manually fact check it;
	will get a good path

### Interesting articles or ideas
- "How to create the Full Stack with 10 Prompts"
- How to find relevant papers with chatGPT
- How to train less expensive ways:
	- Subset of data to train (a lot of data is redundant)
	- *Lora aggregation*
		- Cut training by half
	- 
# LLM Boom
- The generation is in few companies
- The LLama Effect (Meta accidentaly releases their model as public)
	- **Koala**
		- Academic Dialogue
	- **StackLLaMa**
		- Stack overflow
	- Dolly
	- **StableVicuna**
		- Opensource Chat LLM
	- PaLM2
		- Google for Medicine,...
	- ChatLLaMa
		- ChatGPT with own data
- AutoGPT
- OpenAssistant
- DarkChatGPT
- 
