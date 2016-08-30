

const bool BT_ENABLED = false; //bluetooth
const bool LS_ENABLED = false; //light sensor
const bool COM_ENABLED = true;

int led = 6;
int level = 0;
int button =  31;
const int MAX_LEVEL = 3;


// the setup routine runs once when you press reset:
void setup() {

  pinMode(led, OUTPUT);
  pinMode(button, INPUT);

  testlights();
  Serial.begin(9600);
}

void testlights()
{
  analogWrite(led, 0);  
  
  analogWrite(led, 255);
  delay(1000);
}


void loop() 
{
 
  bool changed  = false;
 
 Serial.print(digitalRead(button));
 if(digitalRead(button) == HIGH)
 {
   changed = true;
 	level++;
   if(level > MAX_LEVEL)
   {
     level = 0;
   }
 }
  
  if(level == 0)
  {
    analogWrite(led, 0);
  }
  else
  {
    analogWrite(led, (level * 75) + 30);
  }
  if(changed)
  {
 	 delay(250);
  }
}
