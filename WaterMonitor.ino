// Arduino sketch to monitor water level.
//Original source: http://www.electroschematics.com/9964/arduino-water-level-sensor/
byte sensorPin[] = {9,10,11};
byte ledPin[] = {3,5,6}; // number of leds = numbers of sensors
const byte sensors = 3;
const byte bright = 20;
int level = 0; 

void setup()
{
  for(int i = 0; i<sensors; i++) 
  {
     pinMode(sensorPin[i], INPUT); 
     pinMode(ledPin[i], OUTPUT);
  } 
}
 
void loop() 
{
  level = 0; 
  for(int i = 0; i<sensors; i++) 
  {
     if(digitalRead(sensorPin[i]) == LOW) 
     {
        level = sensors - i;
     } 
     analogWrite(ledPin[i], 0);           
  }
  switch(level)
  {
    case 1:
      analogWrite(ledPin[2], bright);
      break;
      
    case 2:
      analogWrite(ledPin[1], bright);
      break;
      
    case 3:
      analogWrite(ledPin[0], bright);
      break;

    default:
      analogWrite(ledPin[0], bright);
      break;
  }
  delay(50);  
 }
