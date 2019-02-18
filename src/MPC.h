#ifndef MPC_H
#define MPC_H

#include <vector>
#include "Eigen-3.3/Eigen/Core"
//bugbug
#include <chrono>
#include <ctime>


using namespace std;

class MPC {
 public:
  MPC();
  std::chrono::time_point<std::chrono::system_clock> now;
  double interval; // the time between each json communication of simulator
  int index_dt; // the iterations of dt

  virtual ~MPC();

  // Solve the model given an initial state and polynomial coefficients.
  // Return the first actuatotions.
  vector<double> Solve(Eigen::VectorXd state, Eigen::VectorXd coeffs);
};

#endif /* MPC_H */
