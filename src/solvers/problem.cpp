#include "problem.hpp"

#include <iostream>

namespace function
{
    ProblemData::ProblemData() {
        std::cout << "Made empty instance of Problem Data Class" << std::endl;
    }

    ProblemData::ProblemData(Eigen::MatrixXd Q, Eigen::VectorXd q,
                             Eigen::MatrixXd A, Eigen::VectorXd b, 
                             Eigen::VectorXd P_k, int n, double rho)
            : Q_e(Q), q_e(q), A_e(A), b_e(b), Pk_e(P_k), n_(n), rho_(rho){
        std::cout << "Setting up problem data with copied data" << std::endl;
        // Q_sp.resize(Q_e.rows(), Q_e.cols());        
        // for (unsigned i = 0 ; i < Q_sq.rows())

        
        // ATA_ = rho*(A_e).transpose()*(A_e);
        // ATbPk_ = (A_e).transpose()*(-b_e + Pk_e) + q_e;
        bPk_ = -b_e + Pk_e;
    }

    double ProblemData::compute_obj(const Eigen::VectorXd& x) {
        auto obj =  x.transpose()*(Q_e)*x + (q_e).dot(x) + ((rho_)*((A_e)*x - b_e + Pk_e).norm());
    //     auto obj =  x.transpose()*(*Q_)*x + (*q_).dot(x) + ((rho_)*(*A_)*x - *bPk_).norm();
    
        return obj;
    }

    Eigen::VectorXd ProblemData::compute_grad_obj(const Eigen::VectorXd& x) {
        Eigen::VectorXd grad =  2*(*Q_)*x + (*q_) + 2*(rho_)*(*A_).transpose()*((*A_)*x - *b_ + *Pk_);
        return grad;
    }

    Eigen::VectorXd ProblemData::compute_grad_obj_pybind(const Eigen::VectorXd& x) {
        Eigen::VectorXd grad =  2*(Q_e)*x + (q_e) + 2*(rho_)*(A_e).transpose()*((A_e)*x - b_e + *Pk_);
        return grad;
    }

} //namespace fista