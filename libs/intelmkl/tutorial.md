# Instalando MKL

1. Página de Download: [Get Intel® oneAPI Math Kernel Library (oneMKL)](https://www.intel.com/content/www/us/en/developer/tools/oneapi/onemkl-download.html?operatingsystem=linux&linux-install=offline)

    ```
    Step 1: From the console, locate the downloaded install file.

    Step 2: Use $ sudo sh ./<installer>.sh to launch the GUI Installer as the root.

    Optionally, use $ sh ./<installer>.sh to launch the GUI Installer as the current user.

    Step 3: Follow the instructions in the installer.

    Step 4: Explore the Get Started Guide.
    ```

    No final, tem instalado em

    ```bash
    ls $HOME/intel/oneapi/mkl/2025.1/
    ```

# Getting Started

1. Página de Get Started: [Get Started with Intel® oneAPI Math Kernel Library, 3/31/2025](https://www.intel.com/content/www/us/en/docs/onemkl/get-started-guide/2025-1/overview.html)
2. Página de Link: [Intel® oneAPI Math Kernel Library Link Line Advisor ]

    ```
    The Intel® oneAPI Math Kernel Library (oneMKL) is designed to run on multiple processors and operating systems. It is also compatible with several compilers and third-party libraries, and provides different interfaces to the functionality. To support these different environments, tools, and interfaces, oneMKL provides multiple libraries to choose from.

    To see which libraries are recommended for a particular use case, specify the parameters in the drop-down lists below.
    ```

## Link Options

OS: Linux; Programming Language: C/C++; Compiler: GNU C/C++; Architecture: Intel(R) 64; Dynamica or static linking: static; Inteface Layer: C API with 32-bit integer; Select threading layer: Sequential.

```bash
# link line
-m64  -Wl,--start-group ${MKLROOT}/lib/libmkl_intel_lp64.a ${MKLROOT}/lib/libmkl_sequential.a ${MKLROOT}/lib/libmkl_core.a -Wl,--end-group -lpthread -lm -ldl

# compiler options
-m64  -I"${MKLROOT}/include"
```

For Dynamic or Static: SDL (SIngle Dynamic Library):

```bash

-m64  -L${MKLROOT}/lib -lmkl_rt -Wl,--no-as-needed -lpthread -lm -ldl

-m64  -I"${MKLROOT}/include"
```

```
Notes:
- Set INCLUDE, MKLROOT, TBBROOT, LD_LIBRARY_PATH, LIBRARY_PATH, and CPATH environment variables in the command shell using the Intel(R) oneAPI setvars script in Intel(R) oneAPI root directory. Please also see the Intel(R) oneMKL Developer Guide.
- Some domains provide special 64-bit integer API with _64 suffix that can be mixed with 32-bit integer API. Check Using the ILP64 Interface vs. LP64 Interface for more details. 
```

## No VSCode

Update c_cpp_properties.json

```
"includePath": [
    "${workspaceFolder}/**",
    "/home/joao/intel/oneapi/mkl/2025.1/include"
],
```

## Configurações do MKL

```
mkl_verbose(1);
mkl_set_num_threads(1);
mkl_set_dynamic(0); // Disable dynamic adjustment of the number of threads
```

# Using the ILP64 Interface vs. LP64 Interface

ILP64: Usa integer de 8 bytes (64 bits). Importante para aplicações onde array são maiores que $2^{31} - 1$.

LP64: Usa integer de 4 bytes (32 bits).

```
Choose the ILP64 interface if your application uses Intel® oneAPI Math Kernel Library for calculations with large data arrays or the library may be used so in the future.
```

# Exemplo

Programa [example1.cpp](example1.cpp).

1. Setvars
    ```bash
    $ echo $MKLROOT
    # Se vazio, tem que setar
    $ source ~/intel/oneapi/mkl/2025.1/env/vars.sh
    $ echo $MKLROOT
    # /home/joao/intel/oneapi/mkl/2025.1
    ```
2. Compilar com SDL:

    Compile with -fopenmp to enable OpenMP.
    ```
    g++ example1.cpp -o example1 -m64 -I"${MKLROOT}/include" -L${MKLROOT}/lib -lmkl_rt -Wl,--no-as-needed -lpthread -lm -ldl -fopenmp -O3
    ```

# Otimização de Hardware

```
lscpu | grep avx
```

To see which is activated:

```
To change the verbose mode, do one of the following:
- set the environment variable MKL_VERBOSE
- call     mkl_verbose(1);
```

O meu só funcionou chamando a função `mkl_verbose()`.

```
MKL_VERBOSE oneMKL 2025 Update 1 Product build 20250306 for Intel(R) 64 architecture Intel(R) Advanced Vector Extensions 2 (Intel(R) AVX2) enabled processors, Lnx 2.40GHz lp64 gnu_thread
MKL_VERBOSE SGEMM(N,N,64,64,64,0x7ffd8d7f9520,0x583a584af2a0,64,0x583a584ab290,64,0x7ffd8d7f951c,0x583a584b32b0,64) 2.91ms CNR:OFF Dyn:1 FastMM:1 TID:0  NThr:2
```

Mostrando que está usando AVX2.