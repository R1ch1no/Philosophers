# Philosophers

Philosophers is a project of 42.

Project deals with philosphers problem and shared resources.
Each philospher eats, sleeps and thinks (always in this order).
In order to eat, each Philosopher needs two forks but each has only one. 
This means that forks are a shared recourse that they have to share.
Philosophers are not allowed to communicate, there can only be monitor who checks if everyone has eaten amount of times (if specified, this argument is optional) or if some philospher dies of starvation.
If times to eat is not specified and values are sufficient for everyone to survive, program runs indefinitely.

Arguments for the program are : number of philosopher, time to eat(how long a philosopher eats), time to sleep, times to eat (optional).
If number of philosophers is 1, he will die as there is not enough of forks for him to eat.
Code is written in C. 
