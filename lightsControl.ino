

const bool BT_ENABLED = true; //bluetooth
const bool LD_ENABLED = true; //light sensor

void setup() {
  Serial.begin(9600); //especifica baudrate para la conexion serial

}


void loop() {
 
  int lightLevel = 0;
  char btData = '';
  
  if(LS_ENABLED)
  {
  }
  if(BT_ENABLED)
  {
    btData = readFromBT();
  }
  setLightState(lightState);

  

}

char readFromBT()
{
  if(Serial.available() > 0)      // chequeo si hay data
   {
      data = Serial.read();        
      Serial.print(data);          //imprimo en consola serial
      Serial.print("\n");        
      return data;
   }
}

void setLightState(bool level)
{
  //turn on/off  
}
