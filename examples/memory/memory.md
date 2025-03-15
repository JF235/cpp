# Funções memset e memcpy

As funções `memset` e `memcpy` são funções da biblioteca padrão C (também disponíveis em C++) que manipulam blocos de memória. Ambas estão declaradas no cabeçalho `<cstring>` em C++ (ou `<string.h>` em C).

## memset

`memset` preenche um bloco de memória com um valor específico.

### Assinatura
```cpp
void* memset(void* dest, int value, size_t count);
```

### Parâmetros
- `dest`: Ponteiro para o bloco de memória a ser preenchido
- `value`: Valor a ser definido (convertido para unsigned char)
- `count`: Número de bytes a serem preenchidos

### Retorno
- Retorna o ponteiro `dest`

### Uso comum
```cpp
int array[100];
memset(array, 0, sizeof(array));  // Preenche todo o array com zeros
```

### Observações importantes
- Útil para inicializar arrays e estruturas com valores simples (geralmente 0)
- Cuidado: só funciona corretamente para preencher bytes com valores como 0 ou -1
- Não é recomendado para valores complexos como floats ou inteiros não-zero

## memcpy

`memcpy` copia um bloco de memória de uma localização para outra.

### Assinatura
```cpp
void* memcpy(void* dest, const void* src, size_t count);
```

### Parâmetros
- `dest`: Ponteiro para o destino onde os dados serão copiados
- `src`: Ponteiro para a origem dos dados a serem copiados
- `count`: Número de bytes a serem copiados

### Retorno
- Retorna o ponteiro `dest`

### Uso comum
```cpp
int source[100] = {1, 2, 3, /* ... */};
int destination[100];
memcpy(destination, source, sizeof(source));
```

### Observações importantes
- Mais rápido que cópias elemento por elemento em loops
- As áreas de memória não devem se sobrepor (para sobreposição, use `memmove`)
- Útil para cópias binárias e eficientes de qualquer tipo de dados
- Simplesmente copia bytes, não tem conhecimento de construtores/destrutores

## Comparação

```cpp
#include <cstring>
#include <iostream>

int main() {
    int arr1[5] = {10, 20, 30, 40, 50};
    int arr2[5];
    
    // Copiar arr1 para arr2
    memcpy(arr2, arr1, sizeof(arr1));
    
    // Zerar arr1
    memset(arr1, 0, sizeof(arr1));
    
    // Mostrar resultados
    std::cout << "arr1 após memset: ";
    for(int i = 0; i < 5; i++) {
        std::cout << arr1[i] << " ";  // 0 0 0 0 0
    }
    
    std::cout << "\narr2 após memcpy: ";
    for(int i = 0; i < 5; i++) {
        std::cout << arr2[i] << " ";  // 10 20 30 40 50
    }
    
    return 0;
}
```