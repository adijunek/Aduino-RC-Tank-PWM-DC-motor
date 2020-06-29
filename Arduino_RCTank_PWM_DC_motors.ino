/* Juni Adi 06/29/2020
 *  A basic and simple code for an arduino based RC tank with an RF receiver, two DC motors, and a servo for fpv camera movement (pan/tilt) 
 *  This main code is resposible for steering and rotating servo
 *  For reading and decoding PWM signal I use a script written by Kelvin Nelson with a little tweak to meet my need
 *  The original pwmread_rcfailsafe.ino and a good explanation on how decoding PWM signals using arduino interupt can be found at 
 *  https://create.arduino.cc/projecthub/kelvineyeone/read-pwm-decode-rc-receiver-input-and-apply-fail-safe-6b90eb
 */ 
 
/* Motors */
#include <AFMotor.h>

AF_DCMotor lMotor(4);
AF_DCMotor rMotor(3);

/*
 *  Servo for Camera Pan
 */
#include <Servo.h> 
Servo servo;     
const int servo_pin = 10;  

unsigned long now;                                        
unsigned long rc_update;

/* Receiver variables 
 * CH 1 from RC receiver is for throttle, CH 2 for steering, CH3 for servo control, CH4 not used yet
 */
const int channels = 4;                   // specify the number of receiver channels
float RC_in[channels];                    // an array to store the calibrated input from receiver 


void setup() {
  
  // turn on motor
    rMotor.setSpeed(0);
    lMotor.setSpeed(0);
 
    rMotor.run(RELEASE);
    lMotor.run(RELEASE);
    
    servo.attach(servo_pin, 1000, 2000); // attach the servo library to each servo pin, and define min and max uS values
    servo.writeMicroseconds(1500);
        
    setup_pwmRead();                      
    
    Serial.begin(9600);
    delay(1000);
}

void loop() {  
    
    now = millis();
    
    if(RC_avail() || now - rc_update > 18){   // frame rate FrSky X8R, 18 mS

      /* read PWM from receiver and decode */
      rc_update = now;                           
      
      //print_RCpwm();                        // to print raw PWM
      
      for (int i = 0; i<channels; i++){       
        int CH = i+1;
        
        RC_in[i] = RC_decode(CH);             // decode, calibration, apply deadBand,  and apply failsafe 
        Serial.print(RC_in[i]);
        Serial.print(" ");
       
      }
      Serial.println();                       


      /* Motor controller */
      int rcThrottle = RC_in[0];
      int rcSteer   = RC_in[1];

      int lThrottle;
      int rThrottle;

      // failsafe
      if(rcThrottle == 0 & rcSteer  == 0 ){

        Serial.print(" RC SIGNAL LOST ");
        Serial.println();        
                      
        rMotor.setSpeed(0);
        lMotor.setSpeed(0);
        
        rMotor.run(RELEASE);
        lMotor.run(RELEASE);
      }
      

      // move forward
      else if(rcThrottle > 1500 ){
          rcThrottle = map(rcThrottle, 1500,2000,0,255);

          if(rcSteer > 1500){           // forward turn right


            rThrottle= map(rcSteer, 1500,2000,rcThrottle,0);

            rMotor.setSpeed(rThrottle);
            lMotor.setSpeed(rcThrottle);
  
            rMotor.run(FORWARD);
            lMotor.run(FORWARD);
            
          }
          else if(rcSteer < 1500 ){   // FORWARD TURN LEFT

            lThrottle = map(rcSteer, 1500,1000, rcThrottle,0);

            lMotor.setSpeed(lThrottle);
            rMotor.setSpeed(rcThrottle);

            rMotor.run(FORWARD);
            lMotor.run(FORWARD);
          }
          else{                       // forward turn left

            rMotor.setSpeed(rcThrottle);
            lMotor.setSpeed(rcThrottle);
  
            rMotor.run(FORWARD);
            lMotor.run(FORWARD);
          }
      }
       // BACKWARD
      else if(rcThrottle < 1500 && rcThrottle > 0){
        
          rcThrottle = map(rcThrottle, 1500,1000,0,255);

          if(rcSteer > 1500){           // backward turn right


            lThrottle= map(rcSteer, 1500,2000,rcThrottle,0);

            lMotor.setSpeed(lThrottle);
            rMotor.setSpeed(rcThrottle);
  
            rMotor.run(BACKWARD);
            lMotor.run(BACKWARD);
            
          }
          else if(rcSteer < 1500 ){   // backward turn left

            rThrottle = map(rcSteer, 1500,1000, rcThrottle,0);

            rMotor.setSpeed(rThrottle);
            lMotor.setSpeed(rcThrottle);

            rMotor.run(BACKWARD);
            lMotor.run(BACKWARD);
          }
                    
          else{                       // backward straight

            rMotor.setSpeed(rcThrottle);
            lMotor.setSpeed(rcThrottle);
  
            rMotor.run(BACKWARD);
            lMotor.run(BACKWARD);
          }          

      }
       
       // IDDLE
       else{
          if(rcSteer > 1500){           // rotate right


            rcThrottle= map(rcSteer, 1500,2000,0,255);

            rMotor.setSpeed(rcThrottle);
            lMotor.setSpeed(rcThrottle);
  
            rMotor.run(BACKWARD);
            lMotor.run(FORWARD);
            
          }
          else if(rcSteer < 1500){       // rotate left


            rcThrottle= map(rcSteer, 1500,1000,0,255);

            rMotor.setSpeed(rcThrottle);
            lMotor.setSpeed(rcThrottle);
  
            rMotor.run(FORWARD);
            lMotor.run(BACKWARD);
            
          }
          else{
            rMotor.run(RELEASE);
            lMotor.run(RELEASE);
            
          }

       }


      /* Servo controller */
      
      if(RC_in[2] == 1){ // rc signal lost
          servo.writeMicroseconds(1500);
       }else{
          int servo_uS; 
          servo_uS = map(RC_in[2], 1000,2000, 2000,1000);

          servo.writeMicroseconds(servo_uS);   
        
       }
      
    }
}
