# Projects for the lecture Numerical Methods of Statistical Pyhsics

The lecture was held by Markus Quandt and Giuseppe Burgio in summersemester 2020 at University of Tuebingen.

The used language for simulations is C++20. The IDE is CLion.

## Installation requirements

1. CMake https://cmake.org/install/
2. Armadillo http://arma.sourceforge.net/download.html
3. eventually in further releases: Matplotlib-cpp from https://github.com/lava/matplotlib-cpp

## How to compile?

## Tests

Unit-Tests are performed with CTest. Therefore every projekt has its own cpp-File with the
nomenclature: `test{projectName}.cpp` in folder `test/ctest`

## Testing different random-number-generators (RNGs)

![Diagram of Convergence](./Projects/RngTests/Genauigkeit_vs_n_1E10.png)

I did a pi-Test with 2 different random number generators (RNGs). The program can run in a fully parallelized mode.

The RNGs were used from standard C++ header `Random.h`.

We can see the convergence of the **Mersenne-Twister MT19937** algorithm with
![O(sqrt(n))](https://latex.codecogs.com/gif.latex?%5Cmathcal%7BO%7D%28%5Csqrt%7Bn%7D%29)
up to 1E10 random numbers per calculated point. Convergence doesn't seem to slow down until 1E10 so i assume we can
calculate pi more exactly with more time. The maximum period of random numbers of MT19937 is pow(2,19937)-1, so this
would be a thinkable limit.

The **linear congruential RNG** (LCG) is limited in randomness and periodicity wich results in limited convergence. With
this RNG it is not possible to determine pi more exactly than to 1E-2.

## Sand Piles

Main idea from https://www.hiskp.uni-bonn.de/uploads/media/sandpiles.pdf.

**SandPiles** is a simple cellular-automaton where one can observe the collective behavior of falling sand on a field. The
boundary conditions are open in this implementations. You can start with a random distribution of piles of sand or
define one on your own.

The collaps condition of a sandpile is, when the max difference in height to a *von Neumann neighbour* exceeds the
specified `criticalSlope`. So different systems can be simulated.

The program outputs figures generated by https://github.com/lava/matplotlib-cpp.
`matplotlib-cpp` depends on a python-pipe which generates plots by the `matplotlib`. I think this is the cause of the
slow generation of the images. Maybe one can accelerate the process with a c++ only plotting library or simply
generate `bmp`-files.

The final animation is produced by commandline `ffmpeg -framerate 25 -pattern_type glob -i './sandPiles/*.png' out.mp4` which
generates a `.mp4`or`.gif` from all `.png` files in the folder.

A small impression as gif with a random initial setting and always add one sand in the middle of the field.
There are more simulations, also in better quality in `./Projects/SandPiles`.

![Example Image](./Projects/SandPiles/Renderings/ImageTopDown.png)

![a SandGlass with a contour-plot-animation](./Projects/SandPiles/Renderings/TopDown.mp4)

![a SandGlass as 3D animation](./Projects/SandPiles/Renderings/SandGlass.mp4)