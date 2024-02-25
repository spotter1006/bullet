* Build debug

``
/usr/bin/g++ -pthread -fdiagnostics-color=always -g /home/pi/swprojects/bullet/src/*.c* -o /home/pi/swprojects/bullet/build/debug/bullet -L/usr/local/lib -l:libu8g2arm.a -lgpiodcxx -l:libws2811.a
``

* Run debug

``
sudo ~/swprojects/bullet/build/debug/bullet
``