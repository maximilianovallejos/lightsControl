

const bool BT_ENABLED = false; //bluetooth
const bool LS_ENABLED = false; //light sensor
const bool COM_ENABLED = true;

int lightPin = 6;
int currentLevel = 0;
int currentColor = 0;
int targetLevel = 0;
int targetColor = 0;
int button =  31;
const int MAX_LEVEL = 3;
bool automaticLevel = false;

int buttonLastPressed = 0;
const int BUTTON_MIN_TIME = 300;

void setup() {

  pinMode(lightPin, OUTPUT);
  pinMode(button, INPUT);

  Serial.begin(9600);
  testlights();
}

void testlights()
{
  analogWrite(lightPin, 0);  
   delay(500);
  analogWrite(lightPin, 255);
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
  analogWrite(lightPin, currentLevel);
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
  if(!LS_ENABLED || !automaticLevel)
  {
    return;
  }
}
