// Command line variables
String command; // String input from command prompt
String temp1,temp2; // temporary strings
char inByte; // Byte input from command prompt
char carray[6]; // character array for string to int // manipulation
int a,b,c; // temporary numbers
enum comando {na,hey,hola,minombre,suma,invalid,heylo};
comando opcion=na;

void setup(){
Serial.begin(9600);
// Serial.println("Hello, enter your name!");
}

void loop(){
// Input serial information:
if (Serial.available() > 0){
   inByte = Serial.read();
    // only input if a letter, number, =,?,+ are typed! 
    if ((inByte >= 65 && inByte <= 90) || (inByte >=97 && inByte <=122) || (inByte >= 48 && 
    inByte <=57) || inByte == 43 || inByte == 61 || inByte == 63) {
    command.concat(inByte);
}
}// end serial.available
// Process command when NL/CR are entered:
if (inByte == 10 || inByte == 13){
{
  inByte = 0;

// Respond to a command:
if      (command.equalsIgnoreCase("hey")){opcion=hey;}
else if (command.equalsIgnoreCase("heylo")){opcion=heylo;}
else if (command.indexOf('+') > 0){opcion=suma;}
else if (command.startsWith("mynameis")){opcion=minombre;}
else  {
  if (!command.equalsIgnoreCase(""))
    {Serial.println("Invalid argument.");}
}



switch(opcion)
{
case hey:
Serial.println("hello there!");break;
case heylo:
Serial.println("Halo, no heylo!!!");break;


}
command="";
opcion=na;

}
}// fin de recepcoio


}
