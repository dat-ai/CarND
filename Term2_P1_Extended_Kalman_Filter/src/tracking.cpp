#include "Dense"
#include <iostream>
#include "tracking.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

Tracking::Tracking() {
    is_initialized_ = false;
    previous_timestamp_ = 0;

    //create a 4D state vector, we don't know yet the values of the x state
    kf_.x_ = VectorXd(4);

    //state covariance matrix P
    kf_.P_ = MatrixXd(4, 4);
    kf_.P_ << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1000, 0,
            0, 0, 0, 1000;


    //measurement covariance
    kf_.R_ = MatrixXd(2, 2);
    kf_.R_ << 0.0225, 0,
            0, 0.0225;

    //measurement matrix
    kf_.H_ = MatrixXd(2, 4);
    kf_.H_ << 1, 0, 0, 0,
            0, 1, 0, 0;

    //the initial transition matrix F_
    kf_.F_ = MatrixXd(4, 4);
    kf_.F_ << 1, 0, 1, 0,
            0, 1, 0, 1,
            0, 0, 1, 0,
            0, 0, 0, 1;

    //set the acceleration noise components
    noise_ax = 5;
    noise_ay = 5;

}

Tracking::~Tracking() {

}

// Process a single measurement
void Tracking::ProcessMeasurement(const MeasurementPackage &measurement_pack) {
    if (!is_initialized_) {
        cout << "Kalman Filter Initialization " << endl;

        //set the state with the initial location and zero velocity
        //State matrix
        kf_.x_ << measurement_pack.raw_measurements_[0], measurement_pack.raw_measurements_[1], //Position x, y
                  0, 0;                                                                         //Velocity = 0
        previous_timestamp_ = measurement_pack.timestamp_;
        is_initialized_ = true;
        std::cout << "Initializing x_= \n" << kf_.x_ << std::endl;
        std::cout << "Initializing P_= \n" << kf_.P_ << std::endl;
        std::cout << "K_= \n" << kf_.K_ << std::endl;

        return;
    }

    //compute the time elapsed between the current and previous measurements
    float dt = (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0;	//dt - expressed in seconds
    previous_timestamp_ = measurement_pack.timestamp_;

    // Compute values for process covariance matrix
    float dt_2 = dt*dt;
    float dt_3 = dt*dt_2;
    float dt_4 = dt*dt_3;

    // TODO: YOUR CODE HERE
    //1. Modify the F matrix so that the time is integrated
    kf_.F_(0, 2) = dt;
    kf_.F_(1, 3) = dt;
    //2. Set the process covariance matrix Q
    kf_.Q_ = MatrixXd(4, 4);
    kf_.Q_ << noise_ax*(dt_4/4), 0, noise_ax*(dt_3/2), 0,
              0, noise_ay*(dt_4/4), 0, noise_ay*(dt_3/2),
              noise_ax*(dt_3/2), 0, noise_ax*(dt_2), 0,
              0, noise_ay*(dt_3/2), 0, noise_ay*(dt_2);

    //3. Call the Kalman Filter predict() function
    kf_.Predict();
    //4. Call the Kalman Filter update() function
    kf_.Update(measurement_pack.raw_measurements_);
    // with the most recent raw measurements_

    std::cout << "x_= \n" << kf_.x_ << std::endl;
    std::cout << "P_= \n" << kf_.P_ << std::endl;
    std::cout << "K_= \n" << kf_.K_ << std::endl;

}
