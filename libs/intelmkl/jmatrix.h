#include <mkl.h>

namespace jlib
{
    template <typename T>
    class Matrix
    {
    private:
        std::vector<T> data;
        size_t rows_;
        size_t cols_;

    public:
        // Constructor
        Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols)
        {
            data.resize(rows * cols);
        }

        // Fill with random values
        void randomize(T min = 0, T max = 1)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<T> dist(min, max);

            for (size_t i = 0; i < data.size(); ++i)
            {
                data[i] = dist(gen);
            }
        }

        // Access element
        T &operator()(size_t row, size_t col)
        {
            return data[row * cols_ + col];
        }

        const T &operator()(size_t row, size_t col) const
        {
            return data[row * cols_ + col];
        }

        // Get dimensions
        size_t rows() const { return rows_; }
        size_t cols() const { return cols_; }

        // Get raw data pointer (for MKL)
        T *get_data() { return data.data(); }
        const T *get_data() const { return data.data(); }

        // Standard matrix multiplication
        Matrix<T> matmul(const Matrix<T> &B) const
        {
            if (cols_ != B.rows())
            {
                throw std::invalid_argument("Matrix dimensions don't match for multiplication");
            }

            Matrix<T> result(rows_, B.cols());

            for (size_t i = 0; i < rows_; ++i)
            {
                for (size_t j = 0; j < B.cols(); ++j)
                {
                    result(i, j) = 0;
                    for (size_t k = 0; k < cols_; ++k)
                    {
                        result(i, j) += (*this)(i, k) * B(k, j);
                    }
                }
            }

            return result;
        }

        // MKL-based matrix multiplication
        Matrix<T> matmul_mkl(const Matrix<T> &B) const;
    };

    // Specialized implementation for float
    template <>
    Matrix<float> Matrix<float>::matmul_mkl(const Matrix<float> &B) const
    {
        if (cols_ != B.rows())
        {
            throw std::invalid_argument("Matrix dimensions don't match for multiplication");
        }

        Matrix<float> result(rows_, B.cols());

        // Call MKL's SGEMM: C = alpha*A*B + beta*C
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    rows_, B.cols(), cols_,
                    1.0f, get_data(), cols_,
                    B.get_data(), B.cols(),
                    0.0f, result.get_data(), result.cols());

        return result;
    }

    // Specialized implementation for double
    template <>
    Matrix<double> Matrix<double>::matmul_mkl(const Matrix<double> &B) const
    {
        if (cols_ != B.rows())
        {
            throw std::invalid_argument("Matrix dimensions don't match for multiplication");
        }

        Matrix<double> result(rows_, B.cols());

        // Call MKL's DGEMM: C = alpha*A*B + beta*C
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    rows_, B.cols(), cols_,
                    1.0, get_data(), cols_,
                    B.get_data(), B.cols(),
                    0.0, result.get_data(), result.cols());

        return result;
    }

    std::vector<Matrix<float>> generate_matrices(size_t num_matrices, size_t rows, size_t cols)
    {
        std::vector<Matrix<float>> matrices;
        matrices.reserve(num_matrices);
        for (size_t i = 0; i < num_matrices; ++i)
        {
            Matrix<float> mat(rows, cols);
            mat.randomize(-1.0f, 1.0f);
            matrices.push_back(mat);
        }
        return matrices;
    }
}