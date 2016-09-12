
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
const int LIGHT_PIN_R = 5;//PWM pin
const int LIGHT_PIN_G = 3;//PWM pin
const int LIGHT_PIN_B = 4;//PWM pin
const int LS_LED_PIN = 53;//auto enabled indicator
const int LEVEL_BUTTON_PIN =  52;
const int COLOR_BUTTON_PIN =  51;
const int AUTO_BUTTON_PIN = 50;

//lightSensor
unsigned long lastLSensorCheck = 0;//light sensor timer
const int lightSensorCheckTime = 1000;

//button
unsigned long buttonLastPressed = 0;//buttons timer
const int BUTTON_MIN_TIME = 150;

bool levelChanged;
bool colorChanged;
bool automaticLevel;

int currentLevel = 1;
int currentColor = 0;
int targetLevel = 1;
int targetColor = 0;
const int MAX_LEVEL = 3;
const int MAX_COLOR = 3;

int lightsPin[4] = {LIGHT_PIN_W,
                  LIGHT_PIN_R,
                  LIGHT_PIN_G,
                  LIGHT_PIN_B};
const int WHITE = 0;
const int RED = 1;
const int GREEN = 2;
const int BLUE = 3;

void setup() 
{
  for(int i=0; i<= MAX_COLOR; i++)
  {
    pinMode(lightsPin[i], OUTPUT);
  }

  pinMode(COLOR_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LEVEL_BUTTON_PIN, INPUT_PULLUP);
  pinMode(AUTO_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LS_LED_PIN, OUTPUT);
    

  Serial.begin(9600);
  

  //testLights();
  
  for(int i=0; i<= MAX_COLOR; i++)
  {
    for(int j=0; j<=MAX_LEVEL; j++)
    {
      updateLights(j, i);
      delay(150);
    }
  }
  updateLights(currentLevel, currentColor);
  
  
  

  automaticLevel = false;
  
  
  
  if(BT_ENABLED)
  {
    //Serial1.begin(9600);
  }
  
     pinMode(32, OUTPUT);        // Al poner en HIGH forzaremos el modo AT
     pinMode(33, OUTPUT);        // cuando se alimente de aqui
     digitalWrite(33, HIGH);
     delay (500) ;              // Espera antes de encender el modulo

     Serial.println("Levantando el modulo HC-05");
     digitalWrite (32, HIGH);    //Enciende el modulo
     Serial.println("Esperando comandos AT:");
     Serial1.begin(9600); 
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
  readBluetooth();

  levelChanged = currentLevel != targetLevel;
  colorChanged = currentColor != targetColor;
  if(levelChanged || colorChanged)
  {
    currentLevel = targetLevel;
    currentColor = targetColor;
    updateLights(currentLevel, currentColor);
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
    return (level * 75 + 30);
  }
}

void updateLights(int currentLevel, int currentColor)
{
  for(int i=0; i<=MAX_COLOR; i++)
  {
    int value = 255;
    if(i == currentColor)
    {
      value = 255 - getLightValueByLevel(currentLevel);
    }
    else
    {
      value = 255;
    }
    analogWrite(lightsPin[i], value);
  }
}

void readLevelButton()
{
  if(digitalRead(LEVEL_BUTTON_PIN) == LOW)
  {
    if(millis() - buttonLastPressed >= BUTTON_MIN_TIME)
    {
      Serial.print("Level button pressed");
      Serial.print("\n");
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
  if(digitalRead(COLOR_BUTTON_PIN) == LOW)
  {
    if(millis() - buttonLastPressed >= BUTTON_MIN_TIME)
    {
      Serial.print("Color button pressed");
      Serial.print("\n");
      
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
  if(digitalRead(AUTO_BUTTON_PIN) == LOW)
  {
    if(millis() - buttonLastPressed >= BUTTON_MIN_TIME)
    {
      Serial.print("AutoLevel button pressed");
      Serial.print("\n");
      setAutomaticEnable(!automaticLevel);
    }
    buttonLastPressed = millis();
  }
}

void readBluetooth()
{
  if(BT_ENABLED && Serial1.available() > 0)
  {
    char data = Serial1.read();
    Serial.print("Bluetooth read: ");
    Serial.print(data);
    Serial.print("\n");
    switch(data)
    {
      case '0':
        targetLevel = 0;
        setAutomaticEnable(false);
        break;
      case '1':
        targetLevel = 1;
        setAutomaticEnable(false);
        break;
      case '2':
        targetLevel = 2;           
        setAutomaticEnable(false);
        break;
      case '3':
        targetLevel = 3;
        setAutomaticEnable(false);
        break;
      case 'a':
        automaticLevel = !automaticLevel;
        break;
      case 'w':
        targetColor = WHITE;
        break;
      case 'r':
        targetColor = RED;
        break;
      case 'g':
        targetColor = GREEN;
        break;
      case 'b':
        targetColor = BLUE;
        break;
      default:
        break;
    }
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
      Serial.print("Read from light sensor: ");
      Serial.print(data);
      Serial.print("\n");
      
      if(data >= 500)
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
