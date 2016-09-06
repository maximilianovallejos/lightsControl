
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

const bool BT_ENABLED = true; //bluetooth
const bool LS_ENABLED = true; //light sensor

const int LIGHT_SENSOR_PIN = A7;//analog in
const int LIGHT_PIN_W = 2;//PWM pin
const int LIGHT_PIN_R = 3;//PWM pin
const int LIGHT_PIN_G = 4;//PWM pin
const int LIGHT_PIN_B = 5;//PWM pin
const int LS_LED_PIN = 53;//auto enabled indicator
const int LEVEL_BUTTON_PIN =  52;
const int COLOR_BUTTON_PIN =  51;
const int AUTO_BUTTON_PIN = 50;

//lightSensor
unsigned long lastLSensorCheck = 0;//light sensor timer
const int lightSensorCheckTime = 2000;

//button
unsigned long buttonLastPressed = 0;//buttons timer
const int BUTTON_MIN_TIME = 150;

bool levelChanged;
bool colorChanged;
bool automaticLevel;

int currentLevel = 0;
int currentColor = 0;
int targetLevel = 0;
int targetColor = 0;
const int MAX_LEVEL = 3;
const int MAX_COLOR = 3;

int lightsPin[4] = {LIGHT_PIN_W,
                  LIGHT_PIN_R,
                  LIGHT_PIN_G,
                  LIGHT_PIN_B};

void setup() 
{
  for(int i=0; i<= MAX_COLOR; i++)
  {
    pinMode(lightsPin[i], OUTPUT);
  }

  pinMode(COLOR_BUTTON_PIN, INPUT);
  pinMode(LEVEL_BUTTON_PIN, INPUT);
  pinMode(AUTO_BUTTON_PIN, INPUT);
  pinMode(LS_LED_PIN, OUTPUT);

  Serial.begin(9600);

  testLights();

  automaticLevel = false;
}

void testLights()
{
  for(int i=0; i<=MAX_COLOR; i++)
  {
    for(int j=MAX_LEVEL; j<=0; j--)
    { 
      updateLigts(j,i);
      delay(500);
    }
  }

  //test autoLevel indicator led
  digitalWrite(LS_LED_PIN, HIGH);
  delay(500);
  digitalWrite(LS_LED_PIN, LOW);
  delay(500);
}

void loop() 
{
  digitalWrite(LS_LED_PIN, automaticLevel);
  //read level from button
  readLevelButton();
  //read color fromm button
  readColorButton();

  //read level from light sensor
  readLightSensor();

  //check active auto level
  readAutomaticButton();

  //read mode from bluetooh
  //readBluetooth();

  levelChanged = currentLevel != targetLevel;
  colorChanged = currentColor != targetColor;
  if(levelChanged || colorChanged)
  {
    currentLevel = targetLevel;
    currentColor = targetColor;
    updateLigts(currentLevel, currentColor);
  }
}

int getLightValueByLevel(int level)
{
  if(level == 0)
  {
    return 0;
  }
  else
  {
    return level * 75 + 30;
  }
}

void updateLigts(int currentLevel, int currentColor)
{
  for(int i=0; i<=MAX_COLOR; i++)
  {
    if(i == currentColor)
    {
      analogWrite(lightsPin[i], getLightValueByLevel(currentLevel));
    }
    else
    {
      analogWrite(lightsPin[i], 0);
    }
  }
}

void readLevelButton()
{
  if(digitalRead(LEVEL_BUTTON_PIN) == HIGH)
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

void readColorButton()
{
  if(digitalRead(COLOR_BUTTON_PIN) == HIGH)
  {
    if(millis() - buttonLastPressed >= BUTTON_MIN_TIME)
    {
      Serial.print("Color button pressed");
      
      targetColor = currentColor + 1;
      if(targetColor > MAX_COLOR)
      {
        targetColor = 0;
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
