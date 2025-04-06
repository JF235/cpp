#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <mkl.h>
#include "jmatrix.h"

std::string format_time(double seconds) {
    // Convert the units to a human-readable format (ns, us, ms, s, min, h)
    std::ostringstream oss;
    if (seconds < 1e-6) {
        oss << std::fixed << std::setprecision(2) << seconds * 1e9 << " ns";
    } else if (seconds < 1e-3) {
        oss << std::fixed << std::setprecision(2) << seconds * 1e6 << " us";
    } else if (seconds < 1) {
        oss << std::fixed << std::setprecision(2) << seconds * 1e3 << " ms";
    } else if (seconds < 60) {
        oss << std::fixed << std::setprecision(2) << seconds << " s";
    } else if (seconds < 3600) {
        oss << std::fixed << std::setprecision(2) << seconds / 60.0 << " min";
    } else {
        oss << std::fixed << std::setprecision(2) << seconds / 3600.0 << " h";
    }
    return oss.str();
}

int main(void) {
    // 1. Calculate the time for multiplying several 2D matrices
    size_t num_matrices = 100;
    size_t rows = 64;
    size_t cols = 64;

    // Create matrices
    auto matrices = jlib::generate_matrices(num_matrices, rows, cols);

    // Set the number of threads for MKL
    mkl_verbose(0);
    mkl_set_num_threads(2);
    mkl_set_dynamic(0); // Disable dynamic adjustment of the number of threads

    // Time standard multiplication
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_matrices - 1; ++i) {
        auto C = matrices[i].matmul(matrices[i+1]);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> std_duration = end - start;
    
    // Time MKL multiplication
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_matrices - 1; ++i) {
        auto C = matrices[i].matmul_mkl(matrices[i+1]);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> mkl_duration = end - start;
    
    // Print results
    std::cout << "Performance comparison for " << num_matrices << " matrix multiplications (" 
              << rows << "x" << cols << " matrices):\n";
    std::cout << "Standard implementation: " << format_time(std_duration.count()) << "\n";
    std::cout << "MKL implementation: " << format_time(mkl_duration.count()) << "\n";
    std::cout << "Speedup: " << std::fixed << std::setprecision(2) 
              << (std_duration.count() / mkl_duration.count()) << "x\n";
    
    return 0;
}