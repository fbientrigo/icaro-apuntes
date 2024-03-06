if I have a state
$\ket{a,b}$
where I constructed them with the angular momentum, 
$$
J^2 \ket{a,b}=  a\hbar^2 \ket{a,b}
$$

I used the J_z to diagonalize at the same time with J^2
$$
J_z \ket{a,b}=  b \hbar \ket{a,b}
$$

Then I define some raising and lowering operators:
$$
J_+ = J_x + i J_y
$$

$$
J_- = J_x - i J_y
$$

I used the condition
$$
0 \leq J_x^2 + J_y^2 = J^2 - J_z^2
$$

and with that condition I got to
$$
0 \leq \bra{a,b} J^2 - J_z^2 \ket{a,b}  = a \hbar^2 - b^2  \hbar^2 
$$

on which I got that there exist a maximum value of b
$$
a \geq b^2
$$

Understanding how:
$$
J_+ \ket{a,b_{max}} = 0
$$

___

The condition you obtained, $a \geq b^2$, implies that there exists a maximum value of $b$, which we can call $b_{\rm max}$. This is because $b$ is a real number and $a$ is a non-negative number, so $b^2$ can never be greater than $a$. Therefore, $b_{\rm max}$ is the largest real number that satisfies $a \geq b_{\rm max}^2$.

Now, let's consider the action of the raising operator $J_+$ on the state $\ket{a,b_{\rm max}}$. Using the commutation relations $[J_z,J_+]=J_+$ and $[J_z,J_-]=-J_-$, we can show that:

$$
J_z J_+ \ket{a,b_{max}} = (b_{max} + 1) J_+ \ket{a,b_{max}} 
$$


Therefore, $J_+\ket{a,b_{\rm max}}$ is an eigenvector of $J_z$ with eigenvalue $(b_{\rm max}+1)\hbar$. However, $b_{\rm max}$ is already the maximum eigenvalue of $J_z$ for the state $\ket{a,b_{\rm max}}$, so the state $J_+\ket{a,b_{\rm max}}$ cannot be normalized and is not part of the Hilbert space spanned by the basis vectors $\ket{a,b}$ with $b \leq b_{\rm max}$.

This means that the action of $J_+$ on $\ket{a,b_{\rm max}}$ takes us out of the Hilbert space of physical states. This is not a physical state that we can measure or observe. In fact, we can show that $J_+\ket{a,b_{\rm max}}$ is proportional to the zero vector, which means it represents a state with zero probability of occurrence. Therefore, the maximum value of $b$ is an artifact of the mathematics and does not correspond to a physical state.

In summary, the existence of a maximum value of $b$ is a consequence of the condition $a \geq b^2$, but the action of the raising operator $J_+$ on the state $\ket{a,b_{\rm max}}$ takes us out of the Hilbert space of physical states, so it does not correspond to a physical state.

#### why is non normalizable?
We say that $J_+\ket{a,b_{\rm max}}$ cannot be normalized because it is proportional to the zero vector.

To see this, we can use the fact that $J_+ = J_x + iJ_y$ and the commutation relations for the angular momentum operators $[J_x,J_y] = i\hbar J_z$, $[J_y,J_z]=i\hbar J_x$, and $[J_z,J_x]=i\hbar J_y$. We have:
$$
\begin{align} J_+\ket{a,b_{\rm max}} &= (J_x + iJ_y)\ket{a,b_{\rm max}}\\ &= (\hbar\sqrt{a-b_{\rm max}^2} \ket{a,b_{\rm max}+1} + \hbar b_{\rm max}\ket{a,b_{\rm max}})/\sqrt{(a-b_{\rm max}^2)(b_{\rm max}+1)} \\ &= \frac{\hbar}{\sqrt{a-b_{\rm max}^2}}\ket{a,b_{\rm max}+1} + \frac{\hbar b_{\rm max}}{\sqrt{(a-b_{\rm max}^2)(b_{\rm max}+1)}}\ket{a,b_{\rm max}}. \end{align}
$$

The coefficient of $\ket{a,b_{\rm max}}$ is nonzero, so $J_+\ket{a,b_{\rm max}}$ is not proportional to the zero vector. However, since $b_{\rm max}$ is the maximum eigenvalue of $J_z$ for the state $\ket{a,b_{\rm max}}$, the coefficient of $\ket{a,b_{\rm max}+1}$ must be zero. Therefore, we have:
$$
\begin{align} J_+\ket{a,b_{\rm max}} &= 0\cdot\ket{a,b_{\rm max}+1} + \frac{\hbar b_{\rm max}}{\sqrt{(a-b_{\rm max}^2)(b_{\rm max}+1)}}\ket{a,b_{\rm max}}\\ &= \frac{\hbar b_{\rm max}}{\sqrt{(a-b_{\rm max}^2)(b_{\rm max}+1)}}\ket{a,b_{\rm max}}. \end{align}
$$
Since $J_+\ket{a,b_{\rm max}}$ is proportional to $\ket{a,b_{\rm max}}$, it cannot be normalized. Therefore, it is not a physical state that we can observe or measure.