# Aduino based RC Tank (PWM, DC motors)

[![Arduino RC Tank](https://img.youtube.com/vi/WQx10WutNS8/hqdefault.jpg)](https://youtu.be/WQx10WutNS8 "Arduino RC Tank")


A simple and basic code for an arduino based RC tank with an RF receiver, two DC motors, and a servo for fpv camera movement (pan/tilt).
  

Hardware:

- Tank Chasis
- 2 DC Motors
- Arduino UNO
- L293D dual-channel H-Bridge motor driver
- Servo for FPV Camera movement (pan/tilt)
- RF Receiver (I use FrSky X8R)
- RF Transmitter (mine is Turnigy 9x with XJT Module)
- FPV setup (camera, VTX, ground station/goggle, dvr)

  

Required libraries:
- [Arduino Servo Library](https://www.arduino.cc/en/reference/servo)
- [Adafruit Motor Shield Library](https://github.com/adafruit/Adafruit-Motor-Shield-library)

  

The main code (_Arduino_RCTank_PWM_DC_motors.ino_) is responsible for steering the vehicle (and controlling the servo). The code simply converts throttle channel (CH 01) and steering channel (CH 02) into two throttle channels (controlling right and left motor speed). Channel 03 is used for controlling servo.

  

For reading and decoding PWM signal, _pwmread_rcfailsafe.ino_, a script written by Kelvin Nelson is used with some adjusments. The original script and a good explanation on how decoding PWM signals using arduino interupt can be found [here](https://create.arduino.cc/projecthub/kelvineyeone/read-pwm-decode-rc-receiver-input-and-apply-fail-safe-6b90eb). Another resourceful reading is [here](http://rcarduino.blogspot.com/2012/05/interfacing-rc-channels-to-l293d-motor.html).
 


