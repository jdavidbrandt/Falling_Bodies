
# Falling_Bodies
Investigation of falling bodies using raylib, and my own RK4 Numerical Integrator
====================================================================================

We all know the algebraic form of equations of one-dimensinal, falling motion: $y(t)=y_{0}+ \nu_{0}t + \frac{1}{2}at^{2}$

where, on Earth, $a = -g = -9.81 \frac{m}{s^{2}}$

Today, we are going to investigate this with the Runge-Kutta 4th Order Numerical Method

Let's imagine 3 separate, 2-dimensional rectangles. The red rectangle, is computed algebraically, with mass $m_{r} = 1.0 kg$

The blue and orange rectangle will be computed using 4th Order RK-4, with mass $m_{b} = 1.0 kg$ \hspace{2.5cm} and \hspace{2.5cm} mass $m_{o} = 2.0 kg$

I'd like to demonstrate two things with these graphs

## 1: Smaller masses are negligable in a planet's gravitational field

Therefore, all objects that are relatively "small" (non-planet or moon-sized) fall at the same rate. Let's investigate what this depends on.

Isaac Newton proved that

$F = ma$

and, more importantly for the planet mass M, and in our case, much smaller mass m, with universal gravitational constant G, and the radius of the planet, R

$F = \frac{GMm}{R^{2}}$

This mathematical statement is saying the Force being exerted upon an object, is the right side of the equation, and that's why this object falls toward the planet.

But pluggin in $F$ \hspace{1cm} from above

$\frac{m}{a} = \frac{GMm}{R^{2}}

As one can see, dividing both sides by $m$

$a = \frac{GM}{R^{2}} = -9.81 m/s^{2}$

## 2: With smaller step-sizes, RK-4 is a very powerful method of Numerical Integration for smooth, continous solutions

### h = 1

![til](./out/build/linux-default/plots/delta1.png)






