#include <Servo.h>


int EnPosicion[5];
int IrPosicion[5];
Servo s1,s2,s3,s4,s5,s6;

int LogServo[20];
int LogPosiciones[20];

void setup()
{

s1.attach(3);
s2.attach(5);
s3.attach(6);
s4.attach(9);
s5.attach(10);
s6.attach(11);


}

void loop()
{

  
  
  
  

}



void UpdateServos()
{
  
for (int i=0; i < 5; i++)
{
  if (EnPosicion[i] > IrPosicion[i])
  {
    EnPosicion[i]-=1;
  }
  
  if (EnPosicion[i] < IrPosicion[i])
  {
    EnPosicion[i] += 1;
  }
    
     MoveServo(i,EnPosicion[i]);
}

}


void MoveServo(int ser, int pos)
{
  
  if (FlagRecording==true)
  {
    Resgistrar(ser,pos);
  }
  
  switch(ser)
  {
  case 0:
  s1.write(pos);
          break;
  case 1:
  s2.write(pos);
          break;
  case 2:
  s3.write(pos);
          break;
  case 3:
  s4.write(pos);
          break;
  case 4:
  s5.write(pos);
          break;       
   case 5:
  s6.write(pos); 
          break;
          
  }
  
}


int VP(int pos)
{
  if (pos>180)
  {pos=180;}
  if (pos<0)
  {pos=0;}
  return pos;
}



void Registrar(int ser,int pos)
{
    

}

