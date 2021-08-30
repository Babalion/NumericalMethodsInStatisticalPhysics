# Projects for the lecture Numerical Methods of Statistical Physics

The lecture was held by Markus Quandt and Giuseppe Burgio in summer term 2020 at University of Tübingen.

The used language for simulations is C++20. The IDE is CLion.
The used build-system is Debian Buster.
It should also work on other platforms with small modifications in CMakeLists.txt.

## Testing different random-number-generators (RNGs)

![Diagram of Convergence](./Projects/RngTests/Genauigkeit_vs_n_1E10.png)

I did a pi-Test with 2 different random number generators (RNGs). The program can run in a fully parallelized mode.

The RNGs were used from standard C++ header `Random.h`.

We can see the convergence of the **Mersenne-Twister MT19937** algorithm with
up to 1E10 random numbers per calculated point. Convergence doesn't seem to slow down until 1E10, so I assume we can
calculate pi more exactly with more time. The maximum period of random numbers of MT19937 is pow(2,19937)-1, so this
would be a thinkable limit.

The **linear congruential RNG** (LCG) is limited in randomness and periodicity which results in limited convergence. With
this RNG it is not possible to determine pi more exactly than up to 1E-2.

## Forest Fire Simulations
There are some interesting Simulations of Forest-Fire-Cellular-Automatons:

https://people.sc.fsu.edu/~jburkardt/c_src/forest_fire_simulation/forest_fire_simulation.html

https://perso.u-cergy.fr/~ahonecker/bs/software/forest2d.html

## Sand Piles
This project is available under: https://github.com/Babalion/sandpiles

## Calculate spectral dimensions of fractal structures
This project is available under: https://github.com/Babalion/Fractal-Dimensions

## Solve the travelling-salesman-problem (tsp) with a simulated annealing algorithm
This project is available under: https://github.com/Babalion/simulated-annealing-tsp

## Ising Model
### 2 Level System
This project is available under: https://github.com/Babalion/Ising2level

### Sigma-Model
This is an Ising-Spin-System on a square lattice where every spin can direct in an arbitrary angle in 2D.
One can thus parametrize each spin via angle `theta`. You can also view the simulation live via openCV. Example below:

....