#include <QTRSensors.h>

#define rightMaxSpeed 255 
#define leftMaxSpeed 255 
#define rightBaseSpeed 150 
#define leftBaseSpeed 150  
#define IN1  9  
#define IN2  8    
#define IN3  7    
#define IN4  6  
#define ENA  10    
#define ENB  5   

#define NUM_SENSORS  6   
#define IRLeft A5
#define IRRight A4

#define IRPIN 4 
#define CH_UP 0x85E09D61
#define CH_DW 0xD51CCE5D
#define VOL_UP 0xC76EF4E5
#define VOL_DW 0x57E346E1
#define UP 0x6BFD8B01
#define DW 0xB1EFBA9D


double Kp=0.45; 
double Kd=4.41; 
double Ki=0.0025;

double IRSensor[2]={0,0};
QTRSensorsRC qtrrc((unsigned char[]) {A0, A1, A2, A3} , 4); 

unsigned int sensorValues[NUM_SENSORS];

void go_ahead() 
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4,LOW); 
}

void go_stop() 
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4,LOW); 
}

void turn_right()  
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turn_left() 
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void setup()
{
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  for (int i = 0; i < 100; i++) 
    qtrrc.calibrate();   
  delay(20);
  go_stop();
  delay(2000); 
} 


void set_motorspeed(int lspeed,int rspeed) //change motor speed
{
  analogWrite(ENA,lspeed);
  analogWrite(ENB,rspeed);  
}

void readSensors(){
  IRSensor[0]=analogRead(IRLeft);
  IRSensor[1]=analogRead(IRRight);
  unsigned int sensors[4];
  qtrrc.readLine(sensors);
  sensorValues[0]=IRSensor[0];
  sensorValues[1]=sensors[2];
  sensorValues[2]=sensors[3];
  sensorValues[3]=sensors[1];
  sensorValues[4]=sensors[0];
  sensorValues[5]=IRSensor[1];
}

double lastError = 0.0;
double integral=0.0;
double error=0.0;

void update(){
   error=0.0;
   lastError=0.0;
   integral=0.0;
}

int lastTurn=0;
void loop()
{
  readSensors();
  error=-0.3*sensorValues[0]-0.2*sensorValues[1]-0.1*sensorValues[2]+0.1*sensorValues[3]+0.2*sensorValues[4]+0.3*sensorValues[5];
  integral=integral+error;
  if(integral<0){integral=0;}
  if(integral>4000){integral=4000;}
  int motorSpeed = Kp * error + Kd * (error - lastError)+Ki*integral;
  lastError = error;

  int rightMotorSpeed = rightBaseSpeed + motorSpeed;
  int leftMotorSpeed = leftBaseSpeed - motorSpeed;
  
  if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed; 
  if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed; 
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; 
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; 

   {
    int dif=rightMotorSpeed-leftMotorSpeed;
    if(rightMotorSpeed==0){set_motorspeed(leftMotorSpeed,leftMotorSpeed);turn_left();}
    else if(leftMotorSpeed==0){set_motorspeed(rightMotorSpeed,rightMotorSpeed);turn_right();}
    else if(dif>0 && dif<100){set_motorspeed(rightMotorSpeed,rightMotorSpeed);go_ahead();}
    else if(dif<0 && (-1*dif)<100){set_motorspeed(leftMotorSpeed,leftMotorSpeed);go_ahead();}
    else{
      set_motorspeed(leftMotorSpeed,rightMotorSpeed);
      go_ahead();
    }
   }
}
 
