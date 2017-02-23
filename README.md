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
There is a test pass built using this library in the tests/ directory.
You can use the opt tools provided by LLVM to run the pass.

```bash
opt -load <build-dir>/lib/libTestPass.so -test-pass -analyze
```

You can test the pass using `check` target as

```bash
make check
```
