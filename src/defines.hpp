#ifndef _DEFINES_H_
#define _DEFINES_H_

#define MAIN_LOOP_INTERVAL_US   10000    // 10 ms

// DRV8825 stepper motor driver
#define DRV8825_EN_GPIO         17      // RASPI pin 11
#define DRV8825_M0_GPIO         27      // RASPI pin 13
#define DRV8825_M1_GPIO         22      // RASPI pin 15
#define DRV8825_M2_GPIO         23      // RASPI pin 16
#define DRV8825_RESET_GPIO      24      // RASPI pin 18
#define DRV8825_SLEEP_GPIO      25      // RASPI pin 22
#define DRV8825_STEP_GPIO       5       // RASPI pin 29
#define DRV8825_DIR_GPIO        6       // RASPI pin 31
 
 // Motion 
#define MOTOR_SWEEP_STEPS       280     // 63 degrees  
#define MOTOR_MIN_STEP_INTERVAL_US  800  
#define MOTOR_MAX_STEP_INTERVAL_US  4000  
#define MOTOR_ACCEL 5


// Unipolar stepper driver
#define PHASE_1_GPIO 17     // Pin 11
#define PHASE_2_GPIO 27     // pin 13
#define PHASE_3_GPIO 22     // pin 15
#define PHASE_4_GPIO 23     // pin 16
      
#define UNIPOLAR_STEPS_PER_DEGREE 1.433f            // 516 steps per revolution
#define UNIPOLAR_MIN_STEP_INTERVAL_US  4000         // 4 ms
#define UNIPOLAR_MAX_STEP_INTERVAL_US  10000  
#define UNIPOLAR_ACCEL 5
#define UNIPOLAR_TRAVEL 64.0f



// WS2812 Addressable LED array
#define BLACK   0x00000000
#define WHITE   0xffffffff
#define RED     0x00ff0000
#define GREEN   0x0000ff00
#define BLUE    0x000000ff
#define YELLOW  0x00ffff00
#define GREEN10 0x00001000
#define GREEN80 0x00008000
#define RED80   0x00800000
#define BLUE80  0x00000080
#define LED_STRING_FREQUENCY 800000
#define LED_STRING_PIXELS 12
#define WSS2812_DATA_GPIO 18    // RASPI pin 12 (PCM CLK)

// IMU
#define IMU_SERIAL_PORT (char*)"/dev/ttyS0"
#define ACCEL_PER_G 32768.0f * 16.0f
#define GYRO_PER_DEGREES_PER_SECOND 32768.0f * 2000.0f
#define ANGLE_PER_DEGREE 32768.0f * 180.0f
#define index_to_angle(index)(index - HEADING_0_BUCKET)
#define angle_to_index(angle)(angle + HEADING_0_BUCKET)

// Headings histogram 
#define IMU_SAMPLE_INTERVAL_US 100000
#define IMU_READ_MULTIPLIER 10
#define IMU_LEAK_RATE 20
#define STEPS_PER_DEGREE 8.0 / 1.8       
#define HEADING_WINDOW_TENTHS 300       // 30 degrees
#define HEADING_BUCKETS 360 * 10       

// Polar graph
#define ACCEL_AVERAGE_SAMPLES 10
#define HEADING_AVERAGE_SAMPLES 1000
#define TACKING_ANGLE 30.0f

// Bar graph
#define BARGRAPH_ZERO_PIXEL  ((LED_STRING_PIXELS - 2) / 2)

// Gamma table:
// {
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
// 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
// 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
// 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
// 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
// 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
// 25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
// 37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
// 51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
// 69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
// 90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
// 115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
// 144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
// 177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
// 215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 
//}

#endif