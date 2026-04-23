#pragma once

#include <Arduino.h>
#include <Servo.h>

/**
 * @brief Menghubungkan objek Servo ke pin tertentu pada mikrokontroler.
 * 
 * @param s Objek Servo yang akan diinisialisasi.
 * @param pin Nomor pin fisik yang terhubung ke kabel sinyal Servo.
 */
inline void servoAttach(Servo &s, byte pin) {
  s.attach(pin);
}

/**
 * @brief Pilihan kurva transisi kecepatan (easing) untuk memperhalus gerakan servo.
 */
enum EasingType {
  EASE_IN_OUT = 0, ///< Lambat di awal dan lambat di akhir (sangat halus).
  EASE_IN = 1,     ///< Lambat di awal, lalu berakselerasi hingga akhir.
  EASE_OUT = 2,    ///< Cepat di awal, lalu melambat saat mendekati titik akhir.
  EASE_LINEAR = 3  ///< Kecepatan konstan tanpa efek percepatan (kasar).
};

/**
 * @brief Menggerakkan servo standar (180 derajat) ke sudut tujuan.
 *        Dapat menggunakan fitur transisi halus (easing) jika diberikan durasi waktu.
 * 
 * @param s Objek Servo 180 derajat.
 * @param angle Sudut tujuan dari 0 hingga 180 derajat.
 * @param durationMs Total durasi pergerakan dalam milidetik. Jika 0, pergerakan instan.
 * @param easingIntensity Tingkat kelengkungan/intensitas kehalusan transisi (default 2.0). 
 *                        Semakin besar nilai, efek transisi semakin kuat terasa.
 * @param easeType Jenis kurva easing yang akan digunakan (default EASE_IN_OUT).
 */
inline void servo180(Servo &s, byte angle, unsigned int durationMs = 0, float easingIntensity = 2.0, EasingType easeType = EASE_IN_OUT) {
  if (angle > 180) angle = 180;
  
  if (durationMs <= 0) {
    s.write(angle);
    return;
  }

  int startAngle = s.read();
  if (startAngle == angle) return;

  unsigned long startTime = millis();
  unsigned long endTime = startTime + durationMs;
  
  while (millis() < endTime) {
    float progress = (float)(millis() - startTime) / durationMs;
    
    float p = 1.0 + easingIntensity;
    float easedT = progress;

    switch (easeType) {
      case EASE_IN_OUT:
        easedT = pow(progress, p) / (pow(progress, p) + pow(1.0 - progress, p));
        break;
      case EASE_IN:
        easedT = pow(progress, p);
        break;
      case EASE_OUT:
        easedT = 1.0 - pow(1.0 - progress, p);
        break;
      case EASE_LINEAR:
      default:
        easedT = progress;
        break;
    }
    
    int currentAngle = startAngle + (angle - startAngle) * easedT;
    s.write(currentAngle);
    
    delay(15); // Update position ~66Hz
  }
  
  s.write(angle); // Memastikan akurasi titik akhir
}

/**
 * @brief Menghentikan putaran motor servo continuous (360 derajat) sepenuhnya.
 * 
 * @param s Objek Servo 360 derajat.
 */
inline void servo360Stop(Servo &s) {
  s.write(90);
}

/**
 * @brief Memutar motor servo continuous (360 derajat) ke satu arah (kiri/mundur).
 * 
 * @param s Objek Servo 360 derajat.
 * @param speed Tingkat kecepatan putaran (0 hingga 90, default 30).
 */
inline void servo360Left(Servo &s, byte speed = 30) {
  s.write(90 - speed);
}

/**
 * @brief Memutar motor servo continuous (360 derajat) ke arah sebaliknya (kanan/maju).
 * 
 * @param s Objek Servo 360 derajat.
 * @param speed Tingkat kecepatan putaran (0 hingga 90, default 30).
 */
inline void servo360Right(Servo &s, byte speed = 30) {
  s.write(90 + speed);
}