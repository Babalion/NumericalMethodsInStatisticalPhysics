# Projects for the lecture Numerical Methods of Statistical Pyhsics
The lecture was held by Markus Quandt and Giuseppe Burgio in summersemester 2020 at University of Tuebingen.

The used language for simulations is C++20. The IDE is CLion.

## Installation requirements
1. CMake https://cmake.org/install/
2. Armadillo http://arma.sourceforge.net/download.html
3. eventually in further releases: Matplotlib-cpp from https://github.com/lava/matplotlib-cpp


## How to compile?

## Tests
Unit-Tests are performed with CTest.
Therefore every projekt has its own cpp-File with the nomenclature: `test{projectName}.cpp` in folder `test/ctest`

## Results of Project 2 RNGs

![Diagram of Convergence](./Projects/RngTests/Genauigkeit_vs_n_1E10.png)

I did a pi-Test with 2 different random number generators (RNGs).
The program can run in a fully parallelized mode.

The RNGs were used from standard C++ header `Random.h`.

We can see the convergence of the Mersenne-Twister MT19937 algorithm with
![O(sqrt(n))](https://latex.codecogs.com/gif.latex?%5Cmathcal%7BO%7D%28%5Csqrt%7Bn%7D%29)
up to 1E10 random numbers per calculated point.
Convergence doesn't seem to slow down until 1E10 so i assume we can calculate pi more exactly with more time.
The maximum period of random numbers of MT19937 is pow(2,19937)-1, so this would be a thinkable limit.

The linear congruential RNG (LCG) is limited in randomness and periodicity wich results in limited convergence.
With this RNG it is not possible to determine pi more exactly than to 1E-2.

## Sand Piles
Main idea from https://www.hiskp.uni-bonn.de/uploads/media/sandpiles.pdf

`ffmpeg -framerate 3 -pattern_type glob -i '*.png' out.mp4` generates a `.mp4` from all `.png` files in the folder.
