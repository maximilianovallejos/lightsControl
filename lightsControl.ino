
//+ to "button" level
//BUTTON_PIN to "button"
//"resistor" to BUTTON_PIN
//"resistor" to -

//- to "resistor"
// "resistor" to light
//LIGHT_PIN to light

//"lightSensor" to +
//"lightSensor" to -
//"lightSensor" to LIGHT_SENSOR_PIN


const bool BT_ENABLED = false; //bluetooth
const bool LS_ENABLED = true; //light sensor
const bool COM_ENABLED = true;

const int LIGHT_SENSOR_PIN = A7;
const int LIGHT_PIN = 2;//PWM pin
const int BUTTON_PIN =  52;
const int AUTO_BUTTON_PIN = 50;
const int LS_LED_PIN = 53;

//lightSensor
unsigned long lastLSensorCheck = 0;
const int lightSensorCheckTime = 2000;

//button
unsigned long buttonLastPressed = 0;
const int BUTTON_MIN_TIME = 150;


bool levelChanged;
bool colorChanged;
bool automaticLevel;

int currentLevel = 0;
int currentColor = 0;
int targetLevel = 0;
int targetColor = 0;
const int MAX_LEVEL = 3;

void setup() 
{
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(AUTO_BUTTON_PIN, INPUT);
  pinMode(LS_LED_PIN, OUTPUT);

  Serial.begin(9600);
  testLights();

  automaticLevel = false;
}

void testLights()
{
  analogWrite(LIGHT_PIN, 255);  
  digitalWrite(LS_LED_PIN, HIGH);
  delay(500);
  
  analogWrite(LIGHT_PIN, 0);
   digitalWrite(LS_LED_PIN, LOW);
  delay(500);
}

void loop() 
{
  digitalWrite(LS_LED_PIN, automaticLevel);
  //read level from button
  readLevelButton();

  //check active auto level
  readAutomaticButton();

  //read mode from bluetooh
  //readBluetooth();

  //read level from light sensor
  readLightSensor();

  levelChanged = currentLevel != targetLevel;
  colorChanged = currentColor != targetColor;
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
  int lightValue = 0;
  if(level == 0)
  {
    currentLevel = 0;
    lightValue = 0;
  }
  else
  {
    lightValue = level * 75 + 30;
    currentLevel = level;
  }
  Serial.print(currentLevel);

  analogWrite(LIGHT_PIN, lightValue);
}

void applyLightcolor(int color)
{
  currentColor = color;
  Serial.print("");
}

void readLevelButton()
{
  if(digitalRead(BUTTON_PIN) == HIGH)
  {
    if(millis() - buttonLastPressed >= BUTTON_MIN_TIME)
    {
      Serial.print("Level button pressed");
      setAutomaticEnable(false);
      targetLevel = currentLevel + 1;
      if(targetLevel > MAX_LEVEL)
      {
        targetLevel = 0;
      }
    }
    buttonLastPressed = millis();
  }
}

void readAutomaticButton()
{
  if(digitalRead(AUTO_BUTTON_PIN) == HIGH)
  {
    Serial.print("AutoLevel button pressed");
    if(millis() - buttonLastPressed >= BUTTON_MIN_TIME)
    {
      setAutomaticEnable(!automaticLevel);
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

void setAutomaticEnable(bool value)
{
  automaticLevel = value;
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
