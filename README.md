# Build 

``
/usr/bin/g++ -pthread -fdiagnostics-color=always -g /home/pi/swprojects/bullet/src/*.c* -o /home/pi/swprojects/bullet/build/debug/bullet -L/usr/local/lib -l:libu8g2arm.a -lgpiodcxx -l:libws2811.a
``

``
/usr/bin/g++ -pthread /home/pi/swprojects/bullet/src/*.c* -o /home/pi/swprojects/bullet/build/debug/bullet -L/usr/local/lib -l:libu8g2arm.a -lgpiodcxx -l:libws2811.a
``

# Run 

``
sudo ~/swprojects/bullet/build/debug/bullet
``

``
sudo ~/swprojects/bullet/build/release/bullet
``

# Design
This is a device that monitors the  performance of sailboats in real time. Measuments of orienation, acceleration and magnetic heading are made from an inertial measurement unit (IMU).  All data is stored for later upload to a web based tool.

## Graphics 
The main display consists of a strip of LEDS in a line spun aroun an axis at one end by a stepper motor to create a polar chart. The LEDs can display a full range of RGB color, at various intesities and display animation effects.

- GraphicElement class - reperesents graphics promatives that can be added to frames for rendering in animations. Sub classes:
    - Ray - Represents a line extending from the motor axis to the outer edge of the display
    - Arc - Represents a  segment of a circle around the periphery of the display
    - Wedge - Represents the area enclosed by two rays at different step angle and the arc that connects their outer ends


- Polar class - Represents the polar chart display 
    - Stepper class - interfaces to a DRV8825 stepper motor driver to spin the polar axis
    - Frame class collection - Video buffer frames to show on the polar chart in sequence for animation effects
        - Bar class collection - Represents the WS2811 addressable LED string at a sequence of angles to create the animation
            - LED collection - Represents the individual LEDs on the bar


## Inertial Measurement Unit

## Encoder

## Matrix Display

