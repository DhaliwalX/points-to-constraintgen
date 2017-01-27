# Constraint generation for LLVM IR
<!-- Tell people more about this repo -->

### Dependencies

 - LLVM (>=4.0)
 - g++
 - cmake (>=3.1)

### Building
Follow these steps to build this project,

  1) Clone the repository

  ```bash
    git clone https://github.com/princedhaliwal/points-to-constraintgen
  ```

  2) Generate Makefiles

  ```bash
    mkdir build
    cd build
    cmake ..
  ```

  3) Build

  ```bash
    make
  ```

### Running & Testing
You need opt tool to run this project. The project provides a number of test C files in tests/ directory. When you run cmake to build Makefiles, those C files are converted into .ll (LLVM IR) files into various directories (O0/, O1/, O2/, etc) based on level of optimization in the build/ directory.

To run the generated pass on a particular test file, run this command

```
make test<number-of-test>-O<optmization-level>
```

Examples,

```bash
make test1-O0
make test3-O2
```

You can look into all the options available using following command,

```bash
make help
```
