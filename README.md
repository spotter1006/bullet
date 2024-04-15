# Bullet
This is a device that monitors the  performance of sailboats in real time. Measuments of orienation, acceleration and magnetic heading are made from an inertial measurement unit (IMU). All data is stored for later upload to a web based tool. The main display consists of a strip of LEDS in a line spun aroun an axis at one end by a stepper motor to create a polar chart. The LEDs can display a full range of RGB color, at various intesities and display animation effects.
## Build 
Libraries
Fusion - build from source: https://github.com/xioTechnologies/Fusion
```
cd ~/swprojects
git clone https://github.com/xioTechnologies/Fusion.git
cd Fusion
sudo apt install python-pip
pip install numpy
/usr/bin/cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S/home/pi/swprojects/Fusion -B/home/pi/swprojects/Fusion/build -G "Unix Makefiles"
sudo cp build/Fusion/libFusion.a /usr/local/lib
sudo cp Fusion/*.h /usr/include
```
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


## Debugging with codelldb:
The debugger has to run as root because the led library asccesses protected memory. The strategy is to use lldb in platfrorm mode with the following command on the target:

`sudo ~/.vscode-server/extensions/vadimcn.vscode-lldb-1.10.0/lldb/bin/lldb-server platform --server --listen *:2001`

This server stays running after the first launch so you have to launch it only once until a reboot.


## Inertial Measurement Unit
Wit Motion WT901B 9-DOF sensor. Uses UART communications.
## Encoder

## Matrix Display


