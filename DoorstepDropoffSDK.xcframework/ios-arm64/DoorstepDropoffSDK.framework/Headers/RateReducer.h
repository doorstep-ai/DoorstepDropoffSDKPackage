/**
 * @file RateReducer.h
 *
 * Pure-C IMU rate reducer — port of the strapdown coning/sculling algorithm
 * in the legacy C++ RateReducer.  Designed for integration into the
 * DoorstepDropoffSDK sensor pipeline via SensorRateReducer.swift.
 *
 * Channel treatment:
 *   - rotationRate / rawGyro    → coning-corrected integration (delta-angle)
 *   - accel / userAccel         → sculling-corrected integration (delta-velocity)
 *   - gravity / roll,pitch,yaw  → simple per-sample average over the epoch
 *   - mag / heading / proximity → latest-value pass-through
 *
 * NOT compiled into the Xcode target by default; add RateReducer.c to the
 * framework target if you want to call these functions from Objective-C.
 * The active Swift implementation in SensorRateReducer.swift mirrors this
 * code exactly and requires no project-file changes.
 */

#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { double x, y, z; } rr_vec3_t;

/** Flat sample type — mirrors the fields of InternalSensorData. */
typedef struct {
    /* Inertial channels (coning/sculling-integrated, then rate-recovered) */
    double rotation_rate_x, rotation_rate_y, rotation_rate_z;
    double accel_x,         accel_y,         accel_z;
    double raw_gyro_x,      raw_gyro_y,      raw_gyro_z;
    double user_accel_x,    user_accel_y,    user_accel_z;

    /* Averaged over the epoch */
    double gravity_x, gravity_y, gravity_z;
    double roll, pitch, yaw;

    /* Latest-value pass-through */
    double mag_x,     mag_y,     mag_z;
    int    mag_accuracy;
    double raw_mag_x, raw_mag_y, raw_mag_z;
    double heading;
    int    proximity_state;  /* 0 = false, 1 = true */

    /* Timing */
    double timestamp;    /* wall-clock timestamp of the epoch's last sample */
    double delta_time;   /* total epoch duration in seconds */
} rr_sample_t;

/** Per-pair integration state for one (gyro, accel) channel pair. */
typedef struct {
    rr_vec3_t sum_delta_theta;   /* accumulated angular increments */
    rr_vec3_t prev_delta_theta;  /* last sample's delta — NOT reset across epochs */
    rr_vec3_t coning;            /* coning correction accumulator */

    rr_vec3_t sum_delta_vel;     /* accumulated velocity increments */
    rr_vec3_t prev_delta_vel;    /* last sample's delta — NOT reset across epochs */
    rr_vec3_t sculling;          /* sculling correction accumulator */
} rr_pair_state_t;

/** Top-level rate-reducer state. Zero-initialise before calling rr_init(). */
typedef struct {
    rr_pair_state_t primary;  /* fusion gyro (rotationRate) + fusion accel */
    rr_pair_state_t raw;      /* raw gyro + raw accel (includes gravity) */

    rr_vec3_t sum_gravity;
    double    sum_roll, sum_pitch, sum_yaw;

    rr_sample_t latest;          /* latest-value fields, refreshed every sample */

    double sum_delta_time;
    double prev_timestamp;       /* 0.0 = not yet seen a sample */
    double nominal_input_dt;     /* 1/inputHz — fallback when timestamps are missing */
    int    count;                /* samples accumulated in the current epoch */
    int    n;                    /* target samples per output epoch */

    bool        output_ready;
    rr_sample_t output;
} rr_state_t;

/**
 * Initialise the rate reducer.
 *
 * @param rr        Pointer to a zero-initialised rr_state_t.
 * @param input_hz  Sensor input rate (e.g. 100.0).
 * @param output_hz Desired output rate (e.g. 50.0).
 *                  n = round(input_hz / output_hz), clamped to [2, 100].
 */
void rr_init(rr_state_t *rr, double input_hz, double output_hz);

/**
 * Feed one input sample.
 *
 * @return true when an output sample is ready; retrieve it with rr_get_output().
 */
bool rr_process(rr_state_t *rr, const rr_sample_t *sample);

/**
 * Retrieve the pending output sample.
 *
 * @return true if a sample was available; false if rr_process() has not yet
 *         returned true since the last retrieval.
 */
bool rr_get_output(rr_state_t *rr, rr_sample_t *out);

/**
 * Change the decimation factor at runtime.
 * Epoch-in-progress is discarded; prev_delta_* state is preserved for continuity.
 */
void rr_set_n(rr_state_t *rr, int n);

#ifdef __cplusplus
}
#endif
