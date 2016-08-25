

const bool BT_ENABLED = false; //bluetooth
const bool LS_ENABLED = false; //light sensor
const bool COM_ENABLED = true;

const int PIN_LED=9;
int lightLevel = 0;
  
void setup() {
  Serial.begin(9600); //especifica baudrate para la conexion serial
  pinMode(PIN_LED,OUTPUT);
}


void loop() { 


  if(LS_ENABLED)
  {
  }
  if(BT_ENABLED)
  {
      char btData;
    btData = readFromBT();
  }
  if(COM_ENABLED)
  {
    int data = readFromCom();
    if(data >= 0)
    {
      lightLevel = data;
    }
  }
  Serial.print("\n");
  Serial.print(lightLevel);
  setLightLevel(lightLevel);

 
   delay(1000);
}

int readFromCom()
{
    if(Serial.available() > 0)      // chequeo si hay data
   {

      int data = Serial.parseInt();        
      Serial.print("\n com ");
      Serial.print(data);          //imprimo en consola serial
      Serial.print("\n");        
      
    //  Serial.write("In byte ");
  //    int inBytes = unitToByte(data);
//       Serial.write(parseInt(inBytes));
       
      return data;
   }
   else
   {
     return -1;
   }
}

char readFromBT()
{
  if(Serial.available() > 0)      // chequeo si hay data
   {
      char data = Serial.read();        
      Serial.print("\n bt ");
      Serial.print(data);          //imprimo en consola serial
      Serial.print("\n");        
      return data;
   }
   else
   {
     return ' ';
   }
}

void setLightLevel(int level)
{
  if(lightLevel != level)
  {
    Serial.write("\n Level");
    Serial.write(level);
    Serial.write("\n");
  //turn on/off  
   analogWrite(PIN_LED, level);
   lightLevel = level;
   
       
   
   
  }
  
}

int unitToByte(int value)
{
  return 25 * value;
}
