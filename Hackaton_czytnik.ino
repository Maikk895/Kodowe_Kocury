#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <String.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RFrec 8
#define TouchPin 7
#define KroczPin 6

char* ciekawostka[5];

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
    pinMode(RFrec, INPUT);
    pinMode(TouchPin, INPUT);
    pinMode(KroczPin, INPUT);
  }

  attachInterrupt(KroczPin, detectStep, RISING);
  // attachInterrupt(RFrec, detectLight, CHANGE);

  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  ciekawostka[0] = { "Prawa strona mozgu jest bardziej zwiazana z przetwarzaniem emocjonalnym, a lewa strona z jezykiem." };
  ciekawostka[1] = { "Czekolada uwalnia podobne skladniki chemiczne w organizmie ( fenyloetyloamine)  jak odczucie milosci" };
  ciekawostka[2] = { "Mozg jest bardziej kreatywny ,gdy jest zmeczony" };
  ciekawostka[3] = { "Czlowiek jest bardzie logiczny, gdy mysli w innym jezyku ( niz ojczystym)" };
}

int wybor = 0;
int dotyk;
int kroki = 0;
int x;
int punkty = 0;
int stanRF;
long previousMillis = 0;
long previousMillis2 = 0;
uint8_t debounceTime = 500;

void loop() {
  x = digitalRead(KroczPin);
  int rand = random(4);
  dotyk = digitalRead(7);
  if (dotyk == HIGH) {
    wybor = wybor + 1;
    if (wybor == 5) {
      wybor = 0;
    }
  }
  if (wybor == 0) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.print("Kroki: ");
    display.println(kroki);
    display.display();
    delay(1000);
  } else if (wybor == 1) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.print("Punkty: ");
    display.println(punkty);
    display.display();
    delay(1000);
  } else if (wybor == 2) {
    while (wybor == 2) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 10);
      display.println("Czy wiesz ze... ");
      display.print(ciekawostka[rand]);
      display.display();
      delay(1000);
      if (dotyk == HIGH) {
        wybor++;
        delay(300);
      }
    }
  } else if (wybor == 4) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.print("Odbierz punkty i idz do czytnika nr ");
    display.println(rand+1);
    display.display();
    delay(250);
    stanRF = digitalRead(RFrec);
    delay(10);
    if (stanRF == 0) {
      punkty = punkty + 3;
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 10);
      display.println("Gratulacje, na twoje konto wlatują 3 Punkty!");
      display.display();
      delay(3000);
      wybor = 0;
    }
  }
}

void detectStep() {
  long currentMillis = millis();
  if (currentMillis - previousMillis >= debounceTime) {
    previousMillis = currentMillis;
    kroki += 2;
  }
}