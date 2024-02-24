* Build debug
``
/usr/bin/g++ -pthread -fdiagnostics-color=always -g /home/pi/swprojects/bullet/src/*.c* -o /home/pi/swprojects/bullet/build/debug/bullet -L/usr/local/lib -l:libu8g2arm.a -lgpiodcxx
``

* gpiodetect - list all gpiochips present on the system, their names, labels
               and number of GPIO lines

* gpioinfo   - list all lines of specified gpiochips, their names, consumers,
               direction, active state and additional flags

* gpioget    - read values of specified GPIO lines

* gpioset    - set values of specified GPIO lines, potentially keep the lines
               exported and wait until timeout, user input or signal

* gpiofind   - find the gpiochip name and line offset given the line name

* gpiomon    - wait for events on GPIO lines, specify which events to watch,
               how many events to process before exiting or if the events
               should be reported to the console
               