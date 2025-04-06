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

# Apêndice: Introdução ao CBLAS

O CBLAS (C Bindings for Basic Linear Algebra Subprograms) é uma interface em linguagem C para as funções definidas na especificação BLAS (*Basic Linear Algebra Subprograms*), um padrão amplamente utilizado para operações básicas de álgebra linear, como multiplicação de matrizes, produtos vetoriais e transformações.  

Enquanto o BLAS original foi projetado para Fortran, o CBLAS adapta essas rotinas para uso em ambientes C/C++, oferecendo uma sintaxe mais natural para programadores dessas linguagens. **A MKL da Intel implementa o CBLAS** de forma altamente otimizada, aproveitando instruções específicas da arquitetura (como AVX-512) e paralelismo multicore para acelerar cálculos numéricos.  

## Principais Características:  
1. Portabilidade: Segue o padrão BLAS, garantindo compatibilidade entre sistemas.  
2. Desempenho: A MKL acelera as funções CBLAS usando otimizações específicas da Intel.  
3. Funcionalidades:  
   - Operações nível 1 (vetor-vetor): `cblas_ddot`, `cblas_daxpy`.  
   - Operações nível 2 (matriz-vetor): `cblas_dgemv`.  
   - Operações nível 3 (matriz-matriz): `cblas_dgemm`.  

## Exemplo de Uso:  
No estudo anterior, utilizamos `cblas_dgemm` para multiplicar matrizes. Essa função abstrai a complexidade de loops manuais e garante eficiência próxima ao limite teórico do hardware.  

```cpp  
#include <mkl.h> // Header do CBLAS na MKL

// Multiplicação de matrizes: C = alpha*A*B + beta*C  
cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
            m, n, k, alpha, A, k, B, n, beta, C, n);  
```  

## Por Que Usar CBLAS?  
- Produtividade: Elimina a necessidade de reimplementar algoritmos manuais.  
- Confiabilidade: Funções testadas e otimizadas por décadas.  
- Desempenho: Aproveita paralelismo e vetorização da MKL.  

Para explorar todas as funções, consulte a [documentação oficial do CBLAS](https://www.intel.com/content/www/us/en/develop/documentation/onemkl-developer-reference-c/top/blas-and-sparse-blas-routines.html).

# Apêndice: Introdução ao SIMD  
O SIMD (*Single Instruction, Multiple Data*) é um paradigma de computação paralela que permite executar uma única operação em múltiplos dados simultaneamente. Essa técnica é essencial para acelerar tarefas intensivas em cálculos, como processamento de sinais, renderização gráfica, machine learning e simulações numéricas, ao explorar o paralelismo no nível do processador.

## Como Funciona?  
Em arquiteturas SIMD, registradores vetoriais de largura fixa ou escalável armazenam blocos de dados (ex: 4 floats, 8 inteiros). Uma única instrução (ex: adição, multiplicação) é aplicada a todos os elementos do registrador ao mesmo tempo. Por exemplo:  
- Sem SIMD: `a1 + b1`, `a2 + b2`, `a3 + b3`, `a4 + b4` (4 instruções).  
- Com SIMD: `[a1, a2, a3, a4] + [b1, b2, b3, b4]` (1 instrução).  

---

## Principais Conjuntos de Instruções SIMD  
1. Intel x86/x64:  
   - SSE (*Streaming SIMD Extensions*):  
     - Registradores de 128 bits (ex: `__m128`).  
     - Operações básicas (soma, multiplicação) para floats e inteiros.  
   - AVX (*Advanced Vector Extensions*):  
     - Registradores de 256 bits (`__m256`) e 512 bits no AVX-512.  
     - Suporte a operações mais complexas (FMA: *Fused Multiply-Add*).  

2. ARM (Mobile/Embedded):  
   - NEON:  
     - Registradores de 128 bits, comum em smartphones e Raspberry Pi.  
     - Usado em operações de áudio, vídeo e machine learning (ex: TensorFlow Lite).  
   - SVE (*Scalable Vector Extension*):  
     - Vetores escaláveis (tamanho variável por hardware), adotado em supercomputadores como o Fugaku.  

3. Outros:  
   - AltiVec (PowerPC): Usado em sistemas embarcados e consoles antigos (ex: PlayStation 3).  
   - RISC-V V Extension: Padrão aberto para vetores escaláveis.  

---

## Exemplo de Uso com Intrinsics (AVX2 em C++)  
```cpp  
#include <immintrin.h> // Headers para AVX

void add_vectors(float* A, float* B, float* C, int n) {  
    for (int i = 0; i < n; i += 8) {  
        // Carrega 8 floats de A e B em registradores AVX  
        __m256 vecA = _mm256_loadu_ps(&A[i]);  
        __m256 vecB = _mm256_loadu_ps(&B[i]);  
        
        // Soma vetorial (8 floats simultaneamente)  
        __m256 vecC = _mm256_add_ps(vecA, vecB);  
        
        // Armazena o resultado em C  
        _mm256_storeu_ps(&C[i], vecC);  
    }  
}  
```  

---

## SIMD na MKL  
A Intel MKL utiliza instruções SIMD de forma transparente para acelerar funções BLAS e LAPACK. Por exemplo:  
- A função `cblas_dgemm` (multiplicação de matrizes) emprega AVX-512 para processar blocos de matrizes em paralelo.  
- A escolha da instrução (SSE, AVX, AVX-512) é automática, baseada na CPU do sistema.  

---

## Vantagens e Desafios  
- Vantagens:  
  - Aceleração de 2x a 16x em operações vetoriais.  
  - Redução de consumo de energia (menos ciclos de clock).  
- Desafios:  
  - Código dependente de arquitetura (ex: AVX não funciona em CPUs antigas).  
  - Alinhamento de memória exigido para máxima eficiência.  

---

## Quando Usar SIMD?  
- Cenários Ideais:  
  - Processamento de grandes vetores/matrizes.  
  - Algoritmos com operações repetitivas (ex: filtros digitais, transformadas de Fourier).  
- Alternativas:  
  - Bibliotecas otimizadas (como MKL) já encapsulam SIMD – prefira usá-las em vez de "reinventar a roda".  

Para detalhes técnicos, consulte:  
[Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html).

# Apêndice: Introdução ao OpenMP  
O OpenMP (*Open Multi-Processing*) é uma API multiplataforma para programação paralela em sistemas de memória compartilhada (ex: CPUs multicore). Ele simplifica a paralelização de loops, tarefas e regiões de código por meio de diretivas de compilador (*pragmas*), permitindo que desenvolvedores explorem o poder de processadores modernos sem gerenciar threads manualmente.

---

## Principais Conceitos  
1. Modelo Fork-Join:  
   - O programa inicia com uma *thread mestre*.  
   - Regiões paralelas criam *threads worker* (`fork`).  
   - Ao final da região paralela, as threads sincronizam e se unem (`join`).  

2. Diretivas:  
   - Compiladas apenas se o compilador suportar OpenMP (ex: `g++ -fopenmp`).  
   - Exemplo: `#pragma omp parallel for` paraleliza um loop.  

3. Escopo de Variáveis:  
   - `shared`: Variáveis compartilhadas entre threads (cuidado com *data races*!).  
   - `private`: Cópia independente para cada thread.  
   - `reduction`: Combina resultados parciais (ex: soma, máximo).  

---

## Exemplo Básico (Soma Paralela)  
```cpp  
#include <iostream>  
#include <omp.h>  

int main() {  
    const int N = 1000;  
    double sum = 0.0;  
    double arr[N];  

    // Preenche o array  
    for (int i = 0; i < N; i++) {  
        arr[i] = i * 0.1;  
    }  

    // Soma paralela com OpenMP  
    #pragma omp parallel for reduction(+:sum)  
    for (int i = 0; i < N; i++) {  
        sum += arr[i];  
    }  

    std::cout << "Soma total: " << sum << std::endl;  
    return 0;  
}  
```  

Compilação:  
```bash  
g++ -fopenmp exemplo_openmp.cpp -o exemplo_openmp  
```  

---

## OpenMP e Intel MKL  
A MKL utiliza OpenMP internamente para paralelizar operações (ex: multiplicação de matrizes). Para controlar o número de threads:  
```bash  
export OMP_NUM_THREADS=4  # Define 4 threads para OpenMP  
export MKL_NUM_THREADS=4  # Específico para MKL (se diferente de OMP)  
```  

Notas:  
- Evite conflitos configurando `OMP_NUM_THREADS` e `MKL_NUM_THREADS` com o mesmo valor.  
- Desative o paralelismo da MKL se OpenMP já estiver gerenciando threads:  
  ```cpp  
  mkl_set_num_threads(1);  
  ```  

---

## Vantagens  
- Simplicidade: Paralelização com poucas alterações no código.  
- Escalabilidade: Ajuste dinâmico do número de threads.  
- Portabilidade: Funciona em GCC, Clang, e compiladores da Intel.  

---

## Desafios e Boas Práticas  
- Data Races: Use `critical`, `atomic`, ou reduções para evitar corrupção de dados.  
- Balanceamento de Carga: Distribua tarefas uniformemente (ex: `schedule(dynamic)`).  
- Overhead: Evite paralelizar loops pequenos (o custo de criar threads pode superar os benefícios).  

---

## Exemplo Avançado (Seções Paralelas)  
```cpp  
#include <omp.h>  
#include <iostream>  

int main() {  
    #pragma omp parallel sections  
    {  
        #pragma omp section  
        {  
            std::cout << "Seção 1 executada pela thread " << omp_get_thread_num() << std::endl;  
        }  

        #pragma omp section  
        {  
            std::cout << "Seção 2 executada pela thread " << omp_get_thread_num() << std::endl;  
        }  
    }  
    return 0;  
}  
```  

Saída:  
```  
Seção 1 executada pela thread 0  
Seção 2 executada pela thread 1  
```  

---

## Quando Usar OpenMP?  
- Cenários Ideais:  
  - Loops com iterações independentes.  
  - Tarefas heterogêneas que podem ser divididas em seções.  
- Alternativas:  
  - *Threads nativas* (mais controle, mas complexas).  
  - *MPI* para sistemas de memória distribuída.  

Para detalhes, consulte a [documentação oficial](https://www.openmp.org/).