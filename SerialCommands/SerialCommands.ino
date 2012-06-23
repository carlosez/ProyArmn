

void setup(){
Serial.begin(9600);
// Serial.println("Hello, enter your name!");
}




if (inByte == 10 || inByte == 13){
{
  inByte = 0;

// Respond to a command:
if      (command.equalsIgnoreCase("hey")){opcion=hey;}
else if (command.startsWith("minombree")){opcion=heylo;}
else if (command.indexOf('+') > 0){opcion=suma;}
else if (command.startsWith("mynameis")){opcion=minombre;}
else  {  if (!command.equalsIgnoreCase("")) {Serial.println("Invalid argument.");}
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
