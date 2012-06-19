/*
Copyright 2011 Jonathan Castro
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 Author: Jonathan Castro.
 Co-Author and Shield PCB disigner: Engineer Joksan Alvarado.
 Project Name: PlasticWarrior
 Description: A Robotic arm developed in Arduino
 */

#include <Servo.h>


Servo MotorWarrior[7];

int valx = 0;
int valy = 0;

int valorx,valory,intervalox,intervaloy;

boolean flagx = false;
boolean flagy = false;

String s1,s2;
int servo;
int angulo;
String command;
char inByte;
char carray[6];

int intervalo;
int valor;

long previousMillisx,previousMillisy = 0;

int Motor[7] = {
  7, 8, 9, 10, 11, 12, 13};
//int posAngular[7]={426, 472, 472,341, 568, 425, 570};
int posAngular[7] = {
  75, 75, 72, 64, 121, 75, 0 }; //posicion Inici

int velDiv=0;

enum Estados {
  base,  muneca,  codo, secuencia, descansando, serial};
enum Estados estado_actual = base;

//definicion de estados para secuencia1

enum estados_secuencia {
  subirIzquierda, desplazarDerecha, bajarDerecha, subirDerecha, desplazarIzquierda, bajarIzquierda, paro, inicial};
enum estados_secuencia estado_secuencia1 = inicial;

boolean botones[6]={
  HIGH, HIGH, HIGH, HIGH, HIGH,HIGH};
boolean botonAnterior[6]={
  HIGH, HIGH, HIGH, HIGH, HIGH,HIGH};
boolean botonPresionado[6];




void setup(){

  for(int PinCounter = 2; PinCounter<7; PinCounter++){
    pinMode(PinCounter, INPUT);
    digitalWrite(PinCounter, HIGH);
  }
  Serial.begin(9600);

  //valores iniciales

  for(int i = 0; i<7; i++){
    MotorWarrior[i].attach(Motor[i]);
    MotorWarrior[i].write(posAngular[i]);
  }

}

void loop(){
  inData();
  MEF1();
  outData();
}

void inData(){

  if (Serial.available() > 0){
    inByte = Serial.read();
    // only input if a letter, number, =,?,+ are typed! 
    if ((inByte >= 65 && inByte <= 94) || (inByte >=97 && inByte <=122) ||
      (inByte >= 48 &&  inByte <=57) || inByte == 43 || inByte == 44   || 
      inByte == 45   || inByte == 61 || inByte == 63) {
      command.concat(inByte);
      
      
      
    }
  }// end serial.available

    if (inByte == 10 || inByte == 13)
  {
    inByte = 0;
  

  if (command.indexOf('[')>0 && estado_actual == serial)
  {

    s1 = command.substring(1,1);
    s2 = command.substring(command.indexOf(']')+1);

    s1.toCharArray(carray,6);
    servo = atoi(carray);

    s2.toCharArray(carray,6);
    angulo = atoi(carray);
    
    Serial.print("Comando [");
    Serial.print(servo,DEC);
    Serial.print("] ");
    Serial.println(angulo,DEC);
    posAngular[servo]=angulo;///Abra que arreglar esto para que no sea directo



  }
  else if((estado_actual == codo  ||  estado_actual==muneca || estado_actual==base) || 
    (command.indexOf('x') ||  command.indexOf('y')))
  {
    s1.toCharArray(carray,6);


    if ( command.indexOf('x')== 0 )
    {
      s1 = command.substring(1,1);
      s2 = command.substring(command.indexOf(',')+1);
      flagx=true;

      s1.toCharArray(carray,6);
      intervalox= atoi(carray);

      s2.toCharArray(carray,6);
      valorx= atoi(carray)*1000;
      Serial.print("Comando  x");
      Serial.print(intervalox,DEC);
      Serial.print(" , ");
      Serial.println(valorx,DEC);
      previousMillisx= millis();

    }
  }
  else if (command.indexOf('y')==0)
  {
    s1 = command.substring(1,1);
    s2 = command.substring(command.indexOf(',')+1);
    flagy=true;

    s1.toCharArray(carray,6);
    intervaloy= atoi(carray)*1000;

    s2.toCharArray(carray,6);
    valory= atoi(carray);
    Serial.print("Comando  y");
    Serial.print(intervaloy,DEC);
    Serial.print(" , ");
    Serial.println(valory,DEC);
    previousMillisy= millis();
  }

  else 
  { 
    if (!command.equalsIgnoreCase(""))
    {
      Serial.println("Invalid argument.");
    }
  }

  command="";
  }
  if(millis() - previousMillisx < intervalox)
  {
    if (flagx== true)
    {
      valx=valorx;
    }
    if (flagy == true)
    {
      valy=valorx;
    }
  } 
  else //una vez sea mayor
  {
    if (flagx== true)
    {
      valx=0;
      flagx=false; 
    }


  }



  if(millis() - previousMillisy < intervaloy)
  {
    if (flagy== true)
    {
      valy=valory;
    }
    if (flagy == true)
    {
      valy=valory;
    }
  } 
  else //una vez sea mayor
  {
    if (flagx== true)
    {
      valx=0;
      flagx=false; 
    }


  }




  for(int i = 0; i<6; i++)
  {
    botonAnterior[i]=botones[i];
    botones[i]=digitalRead(i+2);
    botonPresionado[i]=(!(botonAnterior[i]) && botones[i]);
  }
}



void MEF1(){
  switch(estado_actual){

  case base:
    posAngular[0] = posAngular[0] - valx;   //integrador para Motor0
    posAngular[1] = posAngular[1] + valy;   //integrador para Motor1

    if(botonPresionado[0]) estado_actual=descansando;      
    if(botonPresionado[1]) estado_actual=codo;
    if(botonPresionado[2]) estado_actual=muneca;
    if(botonPresionado[4]){ 
      estado_actual=secuencia;
      estado_secuencia1=inicial;
    }
    if(botonPresionado[5])

    break;

  case codo:
    posAngular[3]=posAngular[3] - valy;
    posAngular[6]=posAngular[6] + valx;

    if(botonPresionado[0]) estado_actual=descansando;
    if(botonPresionado[2]) estado_actual=muneca;
    if(botonPresionado[3]) estado_actual=base;
    if(botonPresionado[4]){ 
      estado_actual=secuencia;
      estado_secuencia1=inicial;
    }          
    break;

  case muneca:
    posAngular[5]=posAngular[5] + valx;
    posAngular[4]=posAngular[4] + valy;

    if(botonPresionado[0]) estado_actual=descansando;      
    if(botonPresionado[1]) estado_actual=codo;
    if(botonPresionado[3]) estado_actual=base;
    if(botonPresionado[4]){ 
      estado_actual=secuencia;
      estado_secuencia1=inicial;
    }

    break;

  case serial:

    


    break;

  case descansando:
    posInicial();

    if(botonPresionado[1]) estado_actual=codo;
    if(botonPresionado[2]) estado_actual=muneca;
    if(botonPresionado[3]) estado_actual=base;
    if(botonPresionado[4]){ 
      estado_actual=secuencia;
      estado_secuencia1=inicial;
    }
    break;

  case secuencia:
    switch(estado_secuencia1){
    case inicial:
      posInicial();

      if((posAngular[0]==75) && (posAngular[1]==75) && (posAngular[2]==72) && 
        (posAngular[3]==64) && (posAngular[4]==121) && (posAngular[5]==75) && (posAngular[6]==12))
        estado_secuencia1 = desplazarIzquierda;
      break;

    case desplazarIzquierda:
      if(posAngular[0]>170) posAngular[0]--;
      else if(posAngular[0]<170) posAngular[0]++;
      else if(posAngular[0] == 170) estado_secuencia1 = subirIzquierda;

      break;


    case subirIzquierda:
      if(posAngular[4]>20) posAngular[4]--;
      else if(posAngular[4]<20) posAngular[4]++;
      if(posAngular[4]==20){
        if(posAngular[6]>50) posAngular[6]--;
        else if(posAngular[6]<50) posAngular[6]++;
        else estado_secuencia1=bajarIzquierda;
      }

      break;

    case subirDerecha:
      if(posAngular[4]>20) posAngular[4]--;
      else if(posAngular[4]<20) posAngular[4]++;
      if(posAngular[4]==20){
        if(posAngular[6]>50) posAngular[6]--;
        else if(posAngular[6]<50) posAngular[6]++;
        else estado_secuencia1=bajarDerecha;
      }          

      break;

    case bajarIzquierda:
      if(posAngular[6]>12) posAngular[6]--;
      else if(posAngular[6]<12) posAngular[6]++;
      if(posAngular[6]==12){
        if(posAngular[4]>121) posAngular[4]--;
        else if(posAngular[4]<121) posAngular[4]++;
        else estado_secuencia1=desplazarDerecha;
      }

      break;

    case bajarDerecha:
      if(posAngular[6]>12) posAngular[6]--;
      else if(posAngular[6]<12) posAngular[6]++;
      if(posAngular[6]==12){
        if(posAngular[4]>121) posAngular[4]--;
        else if(posAngular[4]<121) posAngular[4]++;
        else estado_secuencia1=desplazarIzquierda;
      }

      break;

    case desplazarDerecha:
      if(posAngular[0]>75) posAngular[0]--;
      else if(posAngular[0]<75) posAngular[0]++;
      else if(posAngular[0] == 75) estado_secuencia1 = subirDerecha;

      break;


    case paro:



      break;

    }


    if(botonPresionado[0]) estado_actual=descansando;
    if(botonPresionado[1]) estado_actual=codo;
    if(botonPresionado[2]) estado_actual=muneca;
    if(botonPresionado[3]) estado_actual=base;

    break;
    
    

  }

}

void outData(){
  int desfase = -3;


  for(int i=0; i<7; i++){

    if(posAngular[i]>180) posAngular[i]=180;
    else if(posAngular[i]<=3) posAngular[i]=3;

    posAngular[2]= posAngular[1] + desfase;

    if(posAngular[6]>90) posAngular[6]=90;
    else  if(posAngular[6] < 12) posAngular[6]=12;

    MotorWarrior[i].write(posAngular[i]);
    // Serial.print(posAngular[i],DEC);
    // Serial.print(" ");
  }
  // Serial.print(valx,DEC);
  // Serial.print(" ");


}

void posInicial(){



  if(posAngular[3] > 64) posAngular[3]--;
  else if(posAngular[3] < 64) posAngular[3]++;

  if(posAngular[6] > 12) posAngular[6]--;


  if(posAngular[0] > 75) posAngular[0]=posAngular[0]-1;
  else if(posAngular[0] < 75) posAngular[0]=posAngular[0]+1;

  if(posAngular[1] > 75) posAngular[1]=posAngular[1]-1;
  else if(posAngular[1] < 75) posAngular[1]=posAngular[1]+1;

  if(posAngular[2] > 72) posAngular[2]=posAngular[2]-1;
  else if(posAngular[2] < 72) posAngular[2]=posAngular[2]+1;


  if(posAngular[4] > 121) posAngular[4]=posAngular[4]-1;
  else if(posAngular[4] < 121) posAngular[4]=posAngular[4]+1;

  if(posAngular[5] > 75) posAngular[5]=posAngular[5]-1;
  else if(posAngular[5] < 75) posAngular[5]=posAngular[5]+1;

}




