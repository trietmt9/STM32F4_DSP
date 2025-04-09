/** 
 * @file kalman_filter.h
 * @brief Kalman filter implementation header file
 * @author Stephen Truong
 * @date 2025-03-21 
*/
#ifndef __KALMAN_FILTER_H
#define __KALMAN_FILTER_H

#include "stdint.h"
#include "math.h"

typedef struct
{
    float Q_angle;
    float Q_bias;
    float R_measure;
    float angle;
    float bias;
    float rate;
    float P[2][2];
} kalman_var;

void kalman_init(kalman_var *var, float Q_angle, float Q_bias, float R_measure, float angle, float bias);
float kalman_update(kalman_var *var, float new_angle, float new_rate, float dt);

#endif