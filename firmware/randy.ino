/* Hi=Techno Barrio  
by:
Christopher Coballes
*/

// Reserve parameters
#define BACKWARD 0
#define STOP 64
#define FORWARD 127
#define RANGE 63

#define LEFT 50
#define CENTER 90
#define RIGHT 130+10 // Extra 10 degrees compensation because steering is slightly off center

// LED Pins
#define RED_LED_PIN 8
#define GREEN_LED_PIN 9
#define BLUE_LED_PIN 10

// Steering Servo Pin
#define STEERING_PIN 7

#include <Servo.h>

int servo_pos = 90;
Servo steering;

// Drive power for robot. Low by default.
int drivePower = 14;

#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1
#define MOTOR_A1_PIN 2 //7

//MOTOR 2
#define MOTOR_A2_PIN  6// 4

#define PWM_MOTOR_1 3 // 5
#define PWM_MOTOR_2 4 // 6


#define MOTOR_1 0
#define MOTOR_2 1

short usSpeed = 150;  //default motor speed
//unsigned short usMotor_Status = BRAKE;

void setup()
{
// Serial is used for communication between node.js server and arduino
 Serial.begin(9600);
 pinMode(MOTOR_A1_PIN, OUTPUT);
 pinMode(MOTOR_A2_PIN, OUTPUT);
 pinMode(PWM_MOTOR_1, OUTPUT);
 pinMode(PWM_MOTOR_2, OUTPUT);
}
  
void loop()
{
    // Read in the 4-byte serial packet
  char buff[4];
    if( Serial.available() >= 4 )
    { 
        Serial.readBytes( buff, 4 );
    }
  
     switch( buff[1] ) {
		 case 'r' 
    
        drivePower = buff[2];
        
        switch( buff[1] ) 
		{
          case 'f':
             forward(drivePower);
            break;
          case 'l':
            //steering.write( LEFT );
            left(drivePower);
            break;
          case 'r':
            //steering.write( RIGHT );
            right(drivePower);
            break;
          case 'b':
            reverse(drivePower);
            break;
          case 's':
            stop (drivePower);
            break;
         }
      break ;
	  // other command possible 
	  case 'l' :
    
        analogWrite(RED_LED_PIN, buff[1]);
        analogWrite(GREEN_LED_PIN, buff[2]);
        analogWrite(BLUE_LED_PIN, buff[3]);
		break;
	 default :
         stop (drivePower);
	 }
}  // loop

void Stop(short usSpeed)
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, usSPeed);
  motorGo(MOTOR_2, usMotor_Status, int usSpeed);
}

void Forward(short usSpeed)
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void Reverse(short usSpeed )
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void Left(short usSpeed)
{
  Serial.println("Left");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed*0.25 );
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void Right(short usSpeed)
{
  Serial.println("Right");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status,usSpeed*0.25);
}


//Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         
{
  if(motor == MOTOR_1)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW); 
    
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
    
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
          
    }
    
    analogWrite(PWM_MOTOR_1, pwm); 
  }
  else if(motor == MOTOR_2)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
    
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A2_PIN, HIGH);
      
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
           
    }
    
    analogWrite(PWM_MOTOR_2, pwm);
  }
}

