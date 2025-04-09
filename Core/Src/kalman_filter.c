/** 
 * @file kalman_filter.c
 * @brief Kalman filter implementation
 * @author Stephen Truong
 * @date 2025-03-21 
*/

#include "kalman_filter.h"

/**
 * @brief Initialize Kalman filter
 * @param var: Kalman filter variable
 * @param Q_angle: Process noise covariance
 * @param Q_bias: Gyroscope noise covariance
 * @param R_measure: Measurement noise covariance
 * @param angle: Initial angle
 * @param bias: Initial bias
 * @retval None
 */
void kalman_init(kalman_var *var, float Q_angle, float Q_bias, float R_measure, float angle, float bias)
{
    var->Q_angle = Q_angle;
    var->Q_bias = Q_bias;
    var->R_measure = R_measure;
    var->angle = angle;
    var->bias = bias;
    var->P[0][0] = 0.0f;
    var->P[0][1] = 0.0f;
    var->P[1][0] = 0.0f;
    var->P[1][1] = 0.0f;
}

/**
 * @brief Update Kalman filter
 * @param var: Kalman filter variable
 * @param new_angle: New angle
 * @param new_rate: New rate
 * @param dt: Time interval
 * @retval Filtered angle
 */
float kalman_update(kalman_var *var, float new_angle, float new_rate, float dt)
{
    float S, K[2], y;
    var->rate = new_rate - var->bias;
    var->angle += dt * var->rate;

    var->P[0][0] += dt * (dt * var->P[1][1] - var->P[0][1] - var->P[1][0] + var->Q_angle);
    var->P[0][1] -= dt * var->P[1][1];
    var->P[1][0] -= dt * var->P[1][1];
    var->P[1][1] += var->Q_bias * dt;

    S = var->P[0][0] + var->R_measure;
    K[0] = var->P[0][0] / S;
    K[1] = var->P[1][0] / S;

    y = new_angle - var->angle;
    var->angle += K[0] * y;
    var->bias += K[1] * y;

    var->P[0][0] -= K[0] * var->P[0][0];
    var->P[0][1] -= K[0] * var->P[0][1];
    var->P[1][0] -= K[1] * var->P[0][0];
    var->P[1][1] -= K[1] * var->P[0][1];

    return var->angle;
}