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