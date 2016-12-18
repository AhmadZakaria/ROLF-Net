#include <vector>
#include <memory>

#include "Eigen/Dense"

#include "rolfneuron.cpp"

enum sigma_method {
    INIT_SIGMA,
    MEAN_SIGMA
};

using namespace Eigen;

template <int X_DIM>
class ROLFNet {
    typedef Matrix<float, X_DIM, 1> centerMat;

  public:
    const int MAX_NEURONS = 10000;
    explicit ROLFNet(sigma_method method): method(method) {

    }

    void train(MatrixXd inputs) {
        assert(inputs.cols() == X_DIM);

        for (int p = 0; p < inputs.rows(); ++p) {
            centerMat p_i = inputs.row(p);
            std::shared_ptr< ROLFNeuron<X_DIM> > exists = neuron_exists(p_i);
            if(exists) {
                //get closest accepting
                //adapt exists (c and sigma) to p_i
            } else {
                create_neuron(p_i);
            }

        }

    }

    ROLFNet(const ROLFNet&) = delete;
    ROLFNet& operator= (const ROLFNet&) = delete;
    ~ROLFNet() {
        for(int i = 0; i < neurons.size(); i++) {
            std::shared_ptr< ROLFNeuron<X_DIM> >n = neurons[i];
            delete n;

        }
        neurons.clear();
    }



  private:
    std::vector<ROLFNeuron<X_DIM>* > neurons;
    sigma_method method = INIT_SIGMA;

    std::shared_ptr< ROLFNeuron<X_DIM> > neuron_exists(centerMat& x) {
        // implement this
    }

    float get_mean_sigma(){
        // implement this
    }

    void create_neuron(centerMat& x) {
        std::shared_ptr< ROLFNeuron<X_DIM> > newNeuron;
        switch (method) {
        case MEAN_SIGMA:
            newNeuron = new ROLFNeuron<X_DIM>(x, get_mean_sigma());
            break;

        case INIT_SIGMA:
        default:
            newNeuron = new ROLFNeuron<X_DIM>(x);
            break;
        }

        newNeuron.setSigma(/*WHAT IS A GOOD SIGMA?*/);// TODO
        neurons.push_back(newNeuron);

    }

};
