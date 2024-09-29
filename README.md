# Simplex Algortithm

This repository is a implementation of the Simplex Algorithm.

## How to compile

Go to the root dir of this repo and run:

```shell
make
```

## How to run a instance

Go to the root dir of this repo and run:

```shell
./bin/simplex <path/to/your/instance>
```

You can find some instances examples in /instances_examples. You can see the format of a instance in <a href="/instances_examples/format.txt">/instances_examples/format.txt</a>.

The simplex is a algorthm to solve linear programming problems in the format:

$
\begin{align}
    & \min cx \\
    & Ax = b \\
    & x >= 0
\end{align}
$

The algorthm uses the idea of extreme points to jump from one solution to another util it finds the best solution. If you want to learn more about, here are some useful links:

- [Simplex Algortithm](https://optimization.cbe.cornell.edu/index.php?title=Simplex_algorithm)
- [Maximization By The Simplex Method](https://math.libretexts.org/Bookshelves/Applied_Mathematics/Applied_Finite_Mathematics_(Sekhon_and_Bloom)/04%3A_Linear_Programming_The_Simplex_Method/4.02%3A_Maximization_By_The_Simplex_Method)
- [Simplex Method](https://www.britannica.com/science/optimization)