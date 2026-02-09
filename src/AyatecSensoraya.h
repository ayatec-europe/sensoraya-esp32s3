#pragma once

// ================================
// Sensoraya board revision
// ================================
// Use this line if using older hardware:
//
// #define SENSORAYA_V20

#if !defined(SENSORAYA_V20) && !defined(SENSORAYA_V21)
  #define SENSORAYA_V21
#endif

// ================================
// Common pins
// ================================
#define PIN_SYS        0

// Digital
#define PIN_D1         1
#define PIN_D2         2
#define PIN_D3         4
#define PIN_D4         5
#define PIN_D5         6

// Optocoupler
#define PIN_O1         9
#define PIN_O2         10
#define PIN_O3         11
#define PIN_O4         12
#define PIN_O5         13

// PWM
#define PIN_P1         15
#define PIN_P2         16
#define PIN_P3         17
#define PIN_P4         18
#define PIN_P5         19
#define PIN_P6         20
#define PIN_P7         3
#define PIN_P8         46

// W5500 Ethernet
#define PIN_SCK        35
#define PIN_MOSI       36
#define PIN_MISO       37
#define PIN_SS         38

// I2C
#define PIN_SCL        39
#define PIN_SDA        40

// DFPlayer
#define PIN_DFPlayer_RX   41
#define PIN_DFPlayer_TX   42

// 595 Shift register
#define PIN_SR_SRCLK   21
#define PIN_SR_RCLK    45
#define PIN_SR_OE      47
#define PIN_SR_SER     48

// Analog multiplexer indices (595 positions)
#define AM_A           4
#define AM_B           5
#define AM_C           6

// ================================
// Revision-specific pins
// ================================
#if defined(SENSORAYA_V21)
  #pragma message("Sensoraya: compiling for HW v2.1 (default)")
  #define PIN_BUZZER   14
  #define PIN_ANALOG   8

#elif defined(SENSORAYA_V20)

  #pragma message("Sensoraya: compiling for HW v2.0")
  #define PIN_BUZZER   8
  #define PIN_ANALOG   14

#endif
