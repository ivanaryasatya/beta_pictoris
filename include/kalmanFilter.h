#pragma once
#include <Arduino.h>

class KalmanFilter {

private:

    float dt;

    // state
    float position;
    float velocity;

    // covariance matrix
    float P[2][2];

    // process noise
    float Q[2][2];

    // measurement noise
    float R;

    // kalman gain
    float K[2];

public:

    KalmanFilter(float deltaTime = 0.01) {
        dt = deltaTime;
        position = 0;
        velocity = 0;
        P[0][0] = 1;
        P[0][1] = 0;
        P[1][0] = 0;
        P[1][1] = 1;
        Q[0][0] = 0.01;
        Q[0][1] = 0;
        Q[1][0] = 0;
        Q[1][1] = 0.01;
        R = 0.1;
    }

    void setDeltaTime(float deltaTime) {
        dt = deltaTime;
    }

    void setProcessNoise(float qPos, float qVel) {

        Q[0][0] = qPos;
        Q[1][1] = qVel;

    }

    void setMeasurementNoise(float r) {
        R = r;
    }

    void reset(float pos = 0, float vel = 0) {

        position = pos;
        velocity = vel;

        P[0][0] = 1;
        P[0][1] = 0;
        P[1][0] = 0;
        P[1][1] = 1;
    }

    // prediction step
    void predict(float acceleration = 0) {

        position = position + velocity * dt + 0.5 * acceleration * dt * dt;
        velocity = velocity + acceleration * dt;

        float P00 = P[0][0];
        float P01 = P[0][1];
        float P10 = P[1][0];
        float P11 = P[1][1];

        P[0][0] = P00 + dt * (P10 + P01) + dt * dt * P11 + Q[0][0];
        P[0][1] = P01 + dt * P11;
        P[1][0] = P10 + dt * P11;
        P[1][1] = P11 + Q[1][1];

    }

    // update with measurement
    void update(float measurement) {
        float y = measurement - position;
        float S = P[0][0] + R;
        K[0] = P[0][0] / S;
        K[1] = P[1][0] / S;
        position = position + K[0] * y;
        velocity = velocity + K[1] * y;
        float P00 = P[0][0];
        float P01 = P[0][1];
        P[0][0] = P00 - K[0] * P00;
        P[0][1] = P01 - K[0] * P01;
        P[1][0] = P[1][0] - K[1] * P00;
        P[1][1] = P[1][1] - K[1] * P01;
    }

    float getPosition() {
        return position;
    }

    float getVelocity() {
        return velocity;
    }

};





// #include "kalmanFilter.h"

// #define TRIG_PIN 5
// #define ECHO_PIN 18

// KalmanFilter kalman(0.05);   // dt = 50 ms

// float readUltrasonic() {

//     digitalWrite(TRIG_PIN, LOW);
//     delayMicroseconds(2);

//     digitalWrite(TRIG_PIN, HIGH);
//     delayMicroseconds(10);

//     digitalWrite(TRIG_PIN, LOW);

//     long duration = pulseIn(ECHO_PIN, HIGH);

//     float distance = duration * 0.0343 / 2.0; // cm

//     return distance;
// }

// void setup() {

//     Serial.begin(115200);

//     pinMode(TRIG_PIN, OUTPUT);
//     pinMode(ECHO_PIN, INPUT);

// }

// void loop() {

//     float rawDistance = readUltrasonic();

//     // langkah Kalman
//     kalman.predict();             // prediksi
//     kalman.update(rawDistance);   // update dengan data sensor

//     float filtered = kalman.getPosition();

//     Serial.print("Raw: ");
//     Serial.print(rawDistance);

//     Serial.print(" cm   Filtered: ");
//     Serial.println(filtered);

//     delay(50);

// }