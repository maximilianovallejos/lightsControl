

const bool BT_ENABLED = false; //bluetooth
const bool LS_ENABLED = false; //light sensor
const bool COM_ENABLED = true;

const int LIGHT_SENSOR_PIN = A7;
const int LIGHT_PIN = 6;

//lightSensor
unsigned long lastLSensorCheck = 0;
const int lightSensorCheckTime = 2000;

//button
unsigned long buttonLastPressed = 0;
const int BUTTON_MIN_TIME = 300;


int currentLevel = 0;
int currentColor = 0;
int targetLevel = 0;
int targetColor = 0;
int button =  31;
const int MAX_LEVEL = 3;
bool automaticLevel = false;



void setup() 
{
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(button, INPUT);

  Serial.begin(9600);
  testlights();
}

void testlights()
{
  analogWrite(LIGHT_PIN, 0);  
   delay(500);
  analogWrite(LIGHT_PIN, 255);
  delay(500);
}

void loop() 
{
  readButton();
  readBluetooth();
  readLightSensor();

  bool levelChanged = currentLevel != targetLevel;
  bool colorChanged = currentColor != targetColor;
  if(levelChanged)
  {
    applyLightLevel(targetLevel);
  }
  if(colorChanged)
  {
    applyLightcolor(targetColor);
  }
}

void applyLightLevel(int level)
{
  if(level == 0)
  {
    currentLevel = 0;
  }
  else
  {
    currentLevel = currentLevel * 75 + 30;
  }
  analogWrite(LIGHT_PIN, currentLevel);
}

void applyLightcolor(int color)
{
  currentColor = color;
}

void readButton()
{
  if(digitalRead(button) == HIGH)
  {
    if(millis() - buttonLastPressed >= BUTTON_MIN_TIME)
    {
      targetLevel = currentLevel + 1;
      if(targetLevel > MAX_LEVEL)
      {
        targetLevel = 0;
      }
    }
    buttonLastPressed = millis();
  }
}

void readBluetooth()
{
  if(!BT_ENABLED)
  {
    return;
  }
}

void readLightSensor()
{
  if(LS_ENABLED && automaticLevel)
  {
    if(millis() -lastLSensorCheck > lightSensorCheckTime)
    {
      int data =  analogRead(LIGHT_SENSOR_PIN);
      //Serial.print("Read from light sensor: " + data + " of 1024");
      Serial.print("\n");
      
      if(data >= 300)
      {
        //level 0
        targetLevel = 0;
      }
      else if(data >= 40)
      {
        //level 1
        targetLevel = 1;
      }
      else if(data >= 25)
      {
        //level 2
        targetLevel = 2;
      }
      else
      {
        //level 3
        targetLevel = 3;
      }
      
      //update light sensor timer
      lastLSensorCheck = millis();
    }
  }
}
