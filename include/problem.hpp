#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <Eigen/Dense>

namespace function
{
/**
 *  Class storing the data to solve the problem (0.5 x*Q*x + 2*qx) s.t. Ax = b
*/
class ProblemData 
{
public:
    ProblemData(std::shared_ptr<Eigen::MatrixXd> Q, std::shared_ptr<Eigen::VectorXd> q, std::shared_ptr<Eigen::MatrixXd> A, std::shared_ptr<Eigen::VectorXd> b);

    //Compute cost function for given x
    double compute_obj(const Eigen::VectorXd& x);

    //Compute gradient of cost function for a given x
    double compute_grad_obj(const Eigen::VectorXd& x);
private:
    std::shared_ptr<Eigen::MatrixXd> Q_;
    std::shared_ptr<Eigen::VectorXd> q_;
    std::shared_ptr<Eigen::MatrixXd> A_;
    std::shared_ptr<Eigen::VectorXd> b_;
    std::shared_ptr<Eigen::VectorXd> Pk_;
    double rho;

    //FISTA related optimization variables
    Eigen::VectorXd y_k;
    Eigen::VectorXd y_k_1;
    Eigen::VectorXd x_k;
    Eigen::VectorXd x_k_1;
    Eigen::VectorXd G_k_norm;
};

} //namespace function

#endif