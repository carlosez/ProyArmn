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
int Motor[7] = {7, 8, 9, 10, 11, 12, 13};
//int posAngular[7]={426, 472, 472,341, 568, 425, 570};
int posAngular[7] = {75, 75, 72, 64, 121, 75, 0 }; //posicion Inici

int velDiv=0;


// Command line variables
String command; // String input from command prompt
String temp1,temp2; // temporary strings
char inByte; // Byte input from command prompt
char carray[6]; // character array for string to int // manipulation
int a,b,c,d,e; // temporary numbers



enum Estados {base,  muneca,  codo, secuencia, descansando};
enum Estados estado_actual = base;

//definicion de estados para secuencia1

enum estados_secuencia {subirIzquierda, desplazarDerecha, bajarDerecha, subirDerecha, desplazarIzquierda, bajarIzquierda, paro, inicial};
enum estados_secuencia estado_secuencia1 = inicial;
enum comando {na,hey,minombre,invalid,nombre,servopos,des,cod,mun,bas,sec,btn};
comando opcion=na;

boolean botones[5]={HIGH, HIGH, HIGH, HIGH, HIGH};
boolean botonAnterior[5]={HIGH, HIGH, HIGH, HIGH, HIGH};
boolean botonPresionado[5];




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
   
   // if ((inByte >= 65 && inByte <= 90) || (inByte >=97 && inByte <=122) || (inByte >= 48 && 
    //inByte <=57) || inByte == 43 || inByte == 61 || inByte == 63) 
    {
    command.concat(inByte);
}
}


if (inByte == 10 || inByte == 13){

  inByte = 0;

if      (command.equalsIgnoreCase("hey")){opcion=hey;}
else if (command.startsWith("minombrees")){opcion=nombre;}
else if (command.startsWith("mynameis")){opcion=minombre;}
else if (command.indexOf('.') == 1){opcion=servopos;}
else if (command.equalsIgnoreCase("des")){opcion=bas;}
else if (command.equalsIgnoreCase("cod")){opcion=cod;}
else if (command.equalsIgnoreCase("mun")){opcion=mun;}
else if (command.equalsIgnoreCase("bas")){opcion=bas;}
else if (command.equalsIgnoreCase("sec")){opcion=sec;}
else if (command.equalsIgnoreCase("bas")){opcion=bas;}
else if (command.equalsIgnoreCase("btn")){opcion=btn;}
else  {  if (!command.equalsIgnoreCase("")) {opcion=invalid;}
}



switch(opcion)
{
case hey:
Serial.println("hello there!");
break;

case nombre:
temp1 = command.substring(10);
Serial.print("Hola, ");
Serial.println(temp1);
break;

case servopos:
temp1 = command.substring(0,command.indexOf('.'));//substring(indice,indice) dif de C#
temp2 = command.substring(command.indexOf('.')+1);
temp1.toCharArray(carray,6);
a = atoi(carray);
temp2.toCharArray(carray,6);
b = atoi(carray);
MotorWarrior[a].write(posAngular[b]);
Serial.print("Motor ");Serial.print(a,DEC);Serial.print(" angulo ");Serial.println(b,DEC);
break;

case des: botonPresionado[0]=true; break;
case cod: botonPresionado[1]=true; break;
case mun: botonPresionado[2]=true; break;
case bas: botonPresionado[3]=true; break;
case sec: botonPresionado[4]=true; break;

case btn: 
      if(botonPresionado[0]) Serial.println("Estado: descansando");
      if(botonPresionado[1]) Serial.println("Estado: codo");
      if(botonPresionado[2]) Serial.println("Estado: muneca");
      if(botonPresionado[3]) Serial.println("Estado: base");
      if(botonPresionado[3]) Serial.println("Estado: secuencia");
      break;

case invalid:
Serial.println("Invalid argument.");
break;

}
command="";
opcion=na;

}// fin de recepcion
  
   
    for(int i = 0; i<5; i++){
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
      if(botonPresionado[4]){ estado_actual=secuencia;
                              estado_secuencia1=inicial;}
      
    break;
    
    case codo:
      posAngular[3]=posAngular[3] - valy;
      posAngular[6]=posAngular[6] + valx;

      if(botonPresionado[0]) estado_actual=descansando;
      if(botonPresionado[2]) estado_actual=muneca;
      if(botonPresionado[3]) estado_actual=base;
      if(botonPresionado[4]){ estado_actual=secuencia;
                              estado_secuencia1=inicial;}          
    break;
    
    case muneca:
      posAngular[5]=posAngular[5] + valx;
      posAngular[4]=posAngular[4] + valy;
      
      if(botonPresionado[0]) estado_actual=descansando;      
      if(botonPresionado[1]) estado_actual=codo;
      if(botonPresionado[3]) estado_actual=base;
      if(botonPresionado[4]){ estado_actual=secuencia;
                              estado_secuencia1=inicial;}
    
    break;
   
    case descansando:
         posInicial();
  
         if(botonPresionado[1]) estado_actual=codo;
         if(botonPresionado[2]) estado_actual=muneca;
         if(botonPresionado[3]) estado_actual=base;
         if(botonPresionado[4]){ estado_actual=secuencia;
                              estado_secuencia1=inicial;}
    break;
    
    case secuencia:
      switch(estado_secuencia1){
        case inicial:
          posInicial();
         
         if((posAngular[0]==75) && (posAngular[1]==75) && (posAngular[2]==72) && (posAngular[3]==64) && (posAngular[4]==121) && (posAngular[5]==75) && (posAngular[6]==12))
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
        
        
   
        
      }
      
    
      if(botonPresionado[0]) estado_actual=descansando;
      if(botonPresionado[1]) estado_actual=codo;
      if(botonPresionado[2]) estado_actual=muneca;
      if(botonPresionado[3]) estado_actual=base;
      
    break;

  }
  
  // ver el estado de los botones y del sistema
  
  
  
  

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
   
    }

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



