#include <iostream>
#include <cstdint>

using distFunc = float(*)(const float *, const float *, const uint8_t);

int returnInt() {
    return 235;
}

float euclideanDistance2(const float *a, const float *b, const uint8_t size) {
    float sum2 = 0; // Square of the sum
    for (uint8_t i = 0; i < size; i++) {
        sum2 += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sum2;
}

int main() {
    uint8_t dim = 3;
    float a[dim] = {1, 2, 3};
    float b[dim] = {4, 5, 6};

    distFunc dist = euclideanDistance2;
    std::cout << dist(a, b, 3) << std::endl;

    // int(*)(void) foo = returnInt;
    int(*foo)(void) = returnInt;
    std::cout << foo() << std::endl;

    return 0;
}