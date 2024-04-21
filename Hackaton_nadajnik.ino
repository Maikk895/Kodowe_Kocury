uint8_t emitterPin = 12; //D6
uint8_t sensorPin = 5; //D1

uint8_t motionFlag = 0;
uint8_t infraredState = LOW;
uint16_t blinkPeriod = 500; //ms
uint16_t emitTime = 5000; //ms
uint32_t previousMillis = 0;

void setup() 
{
  pinMode(emitterPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  attachInterrupt(sensorPin, detectMotion, RISING);

  Serial.begin(115200);
}

void loop() 
{
  long currentMillis = millis();

  if (motionFlag == 1)
  {
    infraredState = !infraredState;
    digitalWrite(emitterPin, infraredState);
    Serial.println("blinking ON");
    delay(blinkPeriod);

    if (currentMillis - previousMillis >= emitTime)
    {      
      previousMillis = currentMillis;
      Serial.println("blinking OFF");
      digitalWrite(emitterPin, LOW);
      motionFlag = 0;
    }
  }
}

ICACHE_RAM_ATTR void detectMotion()
{
  if (motionFlag == 0) 
  {
    motionFlag = 1;
    Serial.println("motion");
  }
}
