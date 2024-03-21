# Bullet
This is a device that monitors the  performance of sailboats in real time. Measuments of orienation, acceleration and magnetic heading are made from an inertial measurement unit (IMU). All data is stored for later upload to a web based tool. The main display consists of a strip of LEDS in a line spun aroun an axis at one end by a stepper motor to create a polar chart. The LEDs can display a full range of RGB color, at various intesities and display animation effects.
## Build 

Debug

``
/usr/bin/g++ -pthread -fdiagnostics-color=always -g /home/pi/swprojects/bullet/src/*.c* -o /home/pi/swprojects/bullet/build/debug/bullet -L/usr/local/lib -l:libu8g2arm.a -lgpiodcxx -l:libws2811.a "-l:libFusion.a"
``

Release

``
/usr/bin/g++ -pthread /home/pi/swprojects/bullet/src/*.c* -o /home/pi/swprojects/bullet/build/debug/bullet -L/usr/local/lib -l:libu8g2arm.a -lgpiodcxx -l:libws2811.a "-l:libFusion.a"
``

## Run 

Debug

``
sudo ~/swprojects/bullet/build/debug/bullet
``

Release

``
sudo ~/swprojects/bullet/build/release/bullet
``





## Inertial Measurement Unit
Wit Motion WT901B 9-DOF sensor. Uses UART communications.
## Encoder

## Matrix Display


