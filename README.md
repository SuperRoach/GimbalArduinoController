# GimbalArduinoController
Controls a Feiyutech Mini3D 3 Axis Gimbal.

Check the code out, but you can use an Arduino Pro Mini, or Arduino Nano for this project. If there is demand I can make a schematic. But you are basicly using PWM to talk to the Gimbal controller, to program it's Tilt/Yaw/Pan, as well as the Mode it's operating in. 

You will need one Potentiometer (Pot) dial, and one push button. Depending on the mode, the Pot will Control the Yaw or your selected axis. 

Hold the button for a few seconds to change the mode, and double click to change what axis you are controlling with the Pot. The onboard Led will blink indicating what you have done (check source code but longer beeps indicate a mode change, shorter ones are for axis change)

Url for printing out the Gimbal holder and base: 
http://www.thingiverse.com/thing:1839607

Picture of a printed and designed Gimbal.
<img src="http://i.imgur.com/5DdT22j.jpg">
