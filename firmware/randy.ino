/*
 * 
 * Christopher Coballes
 * Hi-Techno Barrio
 */

// Drive power for robot. Low by default.
int drivePower = 14;
int led = 13 ;

#define BRAKE 0
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
unsigned short usMotor_Status = BRAKE;

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
}  // loop

void serialEvent()
{
// Read in the 4-byte serial packet
  char buff[4];
    if( Serial.available() >= 4 )
    { 
        Serial.readBytes( buff, 4 );
    }
     drivePower = byteToInt(buff[2]);
     switch( buff[0] ) { //case 'r'          
        case 'r' :
            switch( buff[1] ) 
                   {
                  case 'f':
                      Forward(drivePower);
                      analogWrite(led,drivePower*4); 
                      Serial.println(drivePower );
                    break;
                    
                  case 'l':
                      Left(drivePower);
                      analogWrite(led,drivePower*2);
                      Serial.println(drivePower );
                    break;
                    
                  case 'c':
                      Center (drivePower);
                      analogWrite(led,drivePower*3);
                      Serial.println(drivePower );
                    break;
                    
                  case 'r':
                      Right(drivePower);
                      analogWrite(led,drivePower*1.5);
                      Serial.println(drivePower );
                    break;
                    
                  case 'b':
                       Reverse(drivePower);
                       analogWrite(led,drivePower*2.5);
                       Serial.println(drivePower );
                    break;
                    
                  case 's':
                       Stop(drivePower);
                       analogWrite(led,drivePower*0);
                       Serial.println(drivePower );
                    break;
                 } // dHead
      break ;
    // other command possible 
     case 'l' :
          analogWrite(led,drivePower*0);
      break;
   }
  
}
//  127 /-128 = 255 brute force to integer
int  byteToInt( char drive)
{ 
   int tempInt, finalInt ;
   tempInt = drive;
   tempInt  = (int) tempInt;
   if  (tempInt < 0)
   {
    finalInt = 256 + tempInt ;
   }
   else
   {
    finalInt = tempInt;
   }
 return finalInt; 
}


void Stop(short usSpeed)
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}
void Center(short usSpeed)
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status,  usSpeed);
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
     switch (direct)
     { 
     case  CW :
      digitalWrite(MOTOR_A1_PIN, LOW);
      break;     
     
     case  CCW:
      digitalWrite(MOTOR_A1_PIN, HIGH);    
      break;
      
     case BRAKE :
       digitalWrite(MOTOR_A1_PIN, LOW);
       break;
      } // direction
       analogWrite(PWM_MOTOR_1, pwm); 
  }  
  else if(motor == MOTOR_2)
  {
     switch(direct)
     { 
     case  CW :
      digitalWrite(MOTOR_A2_PIN, LOW);
      break;     
     
     case  CCW:
      digitalWrite(MOTOR_A2_PIN, HIGH);    
      break;
      
     case BRAKE :
       digitalWrite(MOTOR_A2_PIN, LOW);
       break;
        } // direction
     
       analogWrite(PWM_MOTOR_2, pwm);     
       }
   
} // MOTOR GO!

