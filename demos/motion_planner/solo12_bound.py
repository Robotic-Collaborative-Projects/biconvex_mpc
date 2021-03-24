## This contains a pure IK based bounding motion plan
## Author : Avadesh Meduri
## Date : 26/02/2021

import time
import numpy as np
import pinocchio as pin

from py_biconvex_mpc.motion_planner.biconvex import BiConvexMP
from robot_properties_solo.config import Solo12Config
from py_biconvex_mpc.ik.inverse_kinematics import InverseKinematics
from py_biconvex_mpc.ik_utils.gait_generator import GaitGenerator

from cnt_plan_utils import SoloCntGen
from py_biconvex_mpc.bullet_utils.solo_env import Solo12Env


robot = Solo12Config.buildRobotWrapper()
n_eff = 4
m = pin.computeTotalMass(robot.model)
q0 = np.array(Solo12Config.initial_configuration)
x0 = np.concatenate([q0, pin.utils.zero(robot.model.nv)])
# initial and ter state
X_init = np.zeros(9)
X_init[0:3] = q0[0:3]
X_ter = X_init.copy()
# contact plan
st = 0.2 # step time 
sh = 0.15 # step height
sl = np.array([0.0,0.0,0]) # step length
n_steps = 4 # number of steps
T = st*(n_steps + 2)
dt = 2e-2

X_ter[0:3] += sl*(n_steps)
X_nom = np.zeros((9*int(T/dt)))
X_nom[2::9] = X_init[2]

print(X_ter)
cnt_planner = SoloCntGen(T, dt, gait = 2)
cnt_plan = cnt_planner.create_trot_plan(st, sl, n_steps)

# weights
W_X = np.array([1e-5, 1e-5, 1e-5, 1e-4, 1e-4, 1e-4, 1e3, 1e3, 1e3])

W_X_ter = 10*np.array([1e+5, 1e+5, 1e+5, 1e+5, 1e+5, 1e+5, 1e+5, 1e+5, 1e+5])

W_F = np.array(4*[1e+1, 1e+1, 1e+1])

rho = 1e+5 # penalty on dynamic constraint violation

# constraints 
bx = 0.25
by = 0.25
bz = 0.25
fx_max = 15
fy_max = 15
fz_max = 15

# optimization
mp = BiConvexMP(m, dt, T, n_eff, rho = rho)
mp.create_contact_array(cnt_plan)
mp.create_bound_constraints(bx, by, bz, fx_max, fy_max, fz_max)
mp.create_cost_X(W_X, W_X_ter, X_ter, X_nom)
mp.create_cost_F(W_F)
com_opt, F_opt, mom_opt = mp.optimize(X_init, 30)
# mp.stats()
np.savez("./dat_file/bound", com_opt = com_opt, mom_opt = mom_opt, F_opt = F_opt)

f = np.load("dat_file/bound.npz")
mom_opt, com_opt, F_opt = f["mom_opt"], f["com_opt"], f["F_opt"]
ik_solver = cnt_planner.create_ik_step_costs(cnt_plan, sh, [1e+6, 1e+6])
# assert False
ik_solver.ik.add_com_position_tracking_task(0, T, com_opt, 1e+4, "com_track_cost")
ik_solver.create_centroidal_task(mom_opt, 0, T, "mom_track_cost", 1e+4)
xs = ik_solver.optimize(x0, wt_xreg=1e-2)

# simulation
kp = 4*[400.,400, 400]
kd = 4*[10.0,10.0, 15.0]
kc = [100, 100, 100]
dc = [10,10,10]
kb = [500, 500, 100]
db = [10,10,10]
env = Solo12Env(X_init, T, dt, kp, kd, kc, dc, kb, db)
env.generate_motion_plan(com_opt, mom_opt, F_opt, mp.cnt_arr.copy(), mp.r_arr.copy())
env.generate_end_eff_plan(xs)
env.plot()
# env.sim()