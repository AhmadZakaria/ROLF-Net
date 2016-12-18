#include "Eigen/Dense"

using namespace Eigen;

template <int X_DIM>
class ROLFNeuron {
    typedef Matrix<float, X_DIM, 1> centerMat;

  public:

    explicit ROLFNeuron();

    // sigma_init ctor
    explicit ROLFNeuron(centerMat& x) {
        c = x;
        sigma = sigma_init;
    }

    // sigma_mean ctor
    explicit ROLFNeuron(centerMat& x, float sigma_mean) {
        c = x;
        sigma = sigma_mean;
    }

    ROLFNeuron(const ROLFNeuron&) = delete;
    ROLFNeuron& operator= (const ROLFNeuron&) = delete;
    ~ROLFNeuron() = default;



    centerMat c ; //center
    float ro = 2.0f;
    float sigma_init = 0.4f;
    float sigma;

    /**
     * @brief a get distance from neuron's center
     * @param x new pattern
     * @return float Euclidean distance between x and c
     */
    float a(centerMat& x) {
        centerMat diff = c - x;
        return diff.squaredNorm();
    }

    void setSigma(float s) {
        sigma = s;
    }

};
