// This file contains com tracking tasks

#include "ik/inverse_kinematics.hpp"


namespace ik{

    void InverseKinematics::add_com_position_tracking_task(double st, double et, Eigen::MatrixXd traj, 
                                                double wt, std::string cost_name, bool isTerminal)
        {
            // wonder if this is slow?
            sn = std::ceil(st/dt_*100)/100; 
            en = std::ceil(et/dt_*100)/100;

            if (!isTerminal){
                for (unsigned i = sn; i < en; ++i){
                    boost::shared_ptr<crocoddyl::CostModelAbstract> com_track = 
                            boost::make_shared<crocoddyl::CostModelResidual>(
                                    state_, 
                                    boost::make_shared<crocoddyl::ResidualModelCoMPosition>(state_, traj.row(i - sn)));
                    rcost_arr_[i].get()->addCost(cost_name, com_track, wt);
                }
            }
            else{
                boost::shared_ptr<crocoddyl::CostModelAbstract> com_track = 
                            boost::make_shared<crocoddyl::CostModelResidual>(
                                    state_, 
                                    boost::make_shared<crocoddyl::ResidualModelCoMPosition>(state_, traj));
                tcost_model_->addCost(cost_name, com_track, wt);
            }
        };

        void InverseKinematics::add_centroidal_momentum_tracking_task(double st, double et, Eigen::MatrixXd traj, 
                                                double wt, std::string cost_name, bool isTerminal)
        {
            sn = std::ceil(st/dt_*100)/100;
            en = std::ceil(et/dt_*100)/100;

            if (!isTerminal){
                for (unsigned i = sn; i < en; ++i){
                    boost::shared_ptr<crocoddyl::CostModelAbstract> mom_track =
                        boost::make_shared<crocoddyl::CostModelResidual>(
                            state_, 
                            boost::make_shared<crocoddyl::ResidualModelCentroidalMomentum>(state_, traj.row(i - sn)));
                    rcost_arr_[i].get()->addCost(cost_name, mom_track, wt);
                }
            }
            else{
                boost::shared_ptr<crocoddyl::CostModelAbstract> mom_track =
                        boost::make_shared<crocoddyl::CostModelResidual>(
                            state_, 
                            boost::make_shared<crocoddyl::ResidualModelCentroidalMomentum>(state_, traj));
                tcost_model_->addCost(cost_name, mom_track, wt);
            }
        };

    



}