int redLEDPin = 11;
int greenLEDPin = 10;
int blueLEDPin = 9;

int SwitchPin = 7;
 
int red = 0;
int blue = 0;
int green = 0;
 
void setup()
{
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);  
  pinMode(SwitchPin, INPUT_PULLUP);
}
 
void loop()
{
  if(digitalRead(SwitchPin) == LOW){
      red ++;
      if (red > 255) red = 0;
  
      green ++;
      if (green > 255) green = 0;
  
      blue ++;
      if (blue > 255) blue = 0;
      
    analogWrite(redLEDPin, red);
    analogWrite(greenLEDPin, green);
    analogWrite(blueLEDPin, blue);  
  }
  delay(10);
}
