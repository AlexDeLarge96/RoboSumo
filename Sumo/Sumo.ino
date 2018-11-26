#include <IRremote.h>

#define IN1  9  
#define IN2  8    
#define IN3  7    
#define IN4  6  
#define ENA  10    
#define ENB  5 

#define IRPIN 4 
#define CH_UP 0x85E09D61
#define CH_DW 0xD51CCE5D
#define VOL_UP 0xC76EF4E5
#define VOL_DW 0x57E346E1
#define UP 0x6BFD8B01
#define DW 0xB1EFBA9D
#define OK 0x25802501

IRrecv IR(IRPIN);  
decode_results IRresults; 
int state=0;

void go_ahead()  
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4,LOW);
}

void go_back() 
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4,HIGH); 
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


void set_motorspeed(int lspeed,int rspeed) 
{
  analogWrite(ENA,lspeed);
  analogWrite(ENB,rspeed);   
}

void readIR(){
  if(IR.decode(&IRresults))
  {
    if(IRresults.value==CH_UP){state=1;go_ahead();}
    else if(IRresults.value==CH_DW){state=1;go_back();}
    else if(IRresults.value==VOL_UP){state=0;turn_right();}
    else if(IRresults.value==VOL_DW){state=0;turn_left();}
    else if(IRresults.value==UP){
      state=0;
      for(int i=0;i<2;i++){      
        turn_left();
        delay(250);
        go_back();
        delay(250);
      }
    }
    else if(IRresults.value==DW){
      state=0;
      for(int i=0;i<2;i++){      
        turn_right();
        delay(250);
        go_back();
        delay(250);
      }
    }
    else if(IRresults.value==OK || state==0){go_stop();}
    delay(500); 
    
    IRresults.value = 0;
    IR.resume();
  }
}

void setup() {
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);
  pinMode(IRPIN, INPUT); 
  digitalWrite(IRPIN, HIGH);  
  IR.enableIRIn();
  Serial.begin(9600);
}

void loop(){
  set_motorspeed(255,255); 
  readIR();
}
