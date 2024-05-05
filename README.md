# Marching Cubes Implementation

This is a C program that implements the Marching Cubes algorithm for extracting isosurfaces from 3D discrete scalar fields. It takes input in the form of a `.ply` or `.raw` file and outputs a polygonal mesh in `.obj` format representing an isosurface. The program is designed to run on a UNIX-like system and requires a C compiler to build.

## Table of Contents
- [Marching Cubes Implementation](#marching-cubes-implementation)
  - [Table of Contents](#table-of-contents)
  - [Installation](#installation)
  - [Usage](#usage)
    - [Arguments](#arguments)
  - [Examples](#examples)
  - [File Structure](#file-structure)
  - [Notes](#notes)

## Installation

To compile the program, you will need a C compiler such as `gcc` and `make` installed on your system.

1. Clone the repository or download the source code.
2. Navigate to the root of the project directory.
3. Run `make` to compile the source code. This will produce an executable called `marching_cubes` in the `bin` directory.

```bash
git clone https://github.com/israelMateos/marching-cubes/
cd marching-cubes
make
```

## Usage

Once the program is compiled, you can run it from the `bin` directory. The general command-line usage is:

```bash
./bin/marching_cubes [-x <x_size> -y <y_size> -z <z_size>] <input_filename> <isovalue> <output_filename>
```

- If the input file is in `.raw` format, you **must** specify the dimensions (x, y, z).
- If the input file is in `.ply` format, the dimensions are automatically determined.

### Arguments

- `-x <x_size>`: The size in the x-dimension (required for `.raw`).
- `-y <y_size>`: The size in the y-dimension (required for `.raw`).
- `-z <z_size>`: The size in the z-dimension (required for `.raw`).
- `<input_filename>`: The path to the input file (can be `.ply` or `.raw`).
- `<isovalue>`: The value at which to extract the isosurface.
- `<output_filename>`: The path for the output `.obj` file.

## Examples

Here are some examples of how to use the program:

- For a `.ply` file with an isovalue of 0.5, and output to `output.obj`:

  ```bash
  ./bin/marching_cubes input.ply 0.5 output.obj
  ```

- For a `.raw` file with specified dimensions, isovalue of 0.5, and output to `output.obj`:

  ```bash
  ./bin/marching_cubes -x 100 -y 100 -z 100 input.raw 0.5 output.obj
  ```

## File Structure

- `bin/`: Contains the compiled executable `marching_cubes`.
- `include/`: Contains header files for the program (`geometry.h`, `marching_cubes.h`, `utilities.h`).
- `src/`: Contains source files for the program (`main.c`, `marching_cubes.c`, `utilities.c`).
- `Makefile`: Build script to compile the program.

## Notes

- This program has been designed as a final project for the Computer Graphics course at the University of Castilla-La Mancha, Spain.
- The original Marching Cubes algorithm was developed by Lorensen and Cline in 1987:

```
@article{lorensen1987marching,
  title={Marching cubes: A high resolution 3D surface construction algorithm},
  author={Lorensen, William E and Cline, Harvey E},
  journal={ACM siggraph computer graphics},
  volume={21},
  number={4},
  pages={163--169},
  year={1987},
  publisher={ACM}
}
```
- The lookup tables used in this implementation were generated by Paul Bourke and are available at [paulbourke.net/geometry/polygonise](http://paulbourke.net/geometry/polygonise/).
