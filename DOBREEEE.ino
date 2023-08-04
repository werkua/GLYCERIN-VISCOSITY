// zmienic warunki na przeciwne 
/* WYŚWIETLACZ
*  LCM1602 & Arduino Uno
*  VCC - > 5 V
*  GND - GND
*  SCL -> A5
*  SDA -> A4
*/
/* CZUJNIKI ŚWiATŁA
* Arduino Uno
*  VCC - > 5 V
*  GND - GND
*  AO_1 -> A0 // GÓRA
*  AO_2 -> A1 // DÓŁ
*/
/* CZUJNIK TEMPERATURY
* Arduino Uno
*  VCC - > 5 V
*  GND - GND
*  AO_1 -> D4
*/
/* PRZYCISK DO RESETU 
* kabel D 12 ---> RESET
* przycisk AO --> D8
*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>   // standardowa biblioteka Arduino
#include <LiquidCrystal_I2C.h> // dolaczenie pobranej biblioteki I2C dla LCD
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Ustawienie adresu ukladu na 0x27
DallasTemperature sensors(&oneWire);


#define gora A0
#define dol A1

int warunek_swiatla_gora = 42;
int warunek_swiatla_dol = 42;

double czas[2]{};
int nr_czujnika = 0; 
#define PRZYCISK_TAK_PIN 9
#define PRZYCISK_NIE_PIN 8

double czas_przelotu_1 {};
int ilosc_wykrytych {};

int nr_pomiaru{};
      bool przycisk_tak_wcisniety = false;
      bool przycisk_nie_wcisniety = false;
      bool pytanie_wyswietlone = false;
      bool start_wyswietlony = false;
int numer_pytania = 0;

void setup() {
  Serial.begin(9600);
   lcd.begin();   // Inicjalizacja LCD 
   lcd.backlight(); // zalaczenie podwietlenia
  sensors.begin();

      pinMode(gora, INPUT);
      pinMode(dol, INPUT);
  pinMode(PRZYCISK_TAK_PIN, INPUT_PULLUP); // Przycisk "Tak" z rezystorem pull-up wewnętrznie
  pinMode(PRZYCISK_NIE_PIN, INPUT_PULLUP); // Przycisk "Nie" z rezystorem pull-up wewnętrznie

}

void loop() {

    int czujnik_gora = analogRead(gora);
    int czujnik_srodek = analogRead(dol);


if(numer_pytania == 0){
   if (!pytanie_wyswietlone) {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Shall we begin?");
        lcd.setCursor(0, 0);
        lcd.print("1-Yes, 2-No");
        pytanie_wyswietlone = true;
    }

    if (!przycisk_tak_wcisniety && !przycisk_nie_wcisniety) {
        przycisk_tak_wcisniety = digitalRead(PRZYCISK_TAK_PIN) == LOW;
        przycisk_nie_wcisniety = digitalRead(PRZYCISK_NIE_PIN) == LOW;
        delay(40); // Krótka pauza, aby nie wykrywać wielokrotnego naciśnięcia
    }


 if (przycisk_nie_wcisniety) {
   lcd.clear();
  nr_czujnika = 0;
  czas[0] = 0;
  czas[1] = 0;
  lcd.print("RESET");
  delay(2000);
  lcd.clear();
       przycisk_tak_wcisniety = false;
       przycisk_nie_wcisniety = false;
        pytanie_wyswietlone = false;

}



 if (przycisk_tak_wcisniety) {

       if (!start_wyswietlony) {
        lcd.clear();
        lcd.print("START");
        delay(2000);
       lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("1-   , 2-Reset");
        start_wyswietlony = true;
    }

if(nr_czujnika == 0){
    if(czujnik_gora > warunek_swiatla_gora){
        czas[0] = millis();
        nr_czujnika = 1;
     //  lcd.clear();
       lcd.setCursor(15, 0);
       lcd.print("X");
    }
  }

  else if(nr_czujnika == 1){
    if(czujnik_srodek > warunek_swiatla_dol){
        czas[1] = millis();
        nr_czujnika = 0;
        czas_przelotu_1 = czas[1] - czas[0];
   
        lcd.clear();
        lcd.setCursor(15, 1);
        lcd.print("X");
        lcd.setCursor(15, 0);
        lcd.print("X");
        lcd.setCursor(0, 0);
        lcd.print("t: ");
        lcd.print(czas_przelotu_1,0);
        lcd.print(" ms");

              for(int i = 0; i < 3; i++){ // miganie ekranu po obliczeniu i wyświetleniu czasu
                lcd.noDisplay(); //Wyłącz i poczekaj
                delay(500);
                lcd.display(); //Włącz i poczekaj
                delay(500);
                }
        lcd.setCursor(15, 1);
        lcd.print(" ");
        lcd.setCursor(15, 0);
        lcd.print(" ");
       przycisk_tak_wcisniety = false;
       przycisk_nie_wcisniety = false;
       pytanie_wyswietlone = false;
        start_wyswietlony = false;
        numer_pytania = 1;

    }
  }
  przycisk_nie_wcisniety = digitalRead(PRZYCISK_NIE_PIN) == LOW;
  if (przycisk_nie_wcisniety) {
   lcd.clear();
  nr_czujnika = 0;
  czas[0] = 0;
  czas[1] = 0;
  lcd.print("RESET");
       przycisk_tak_wcisniety = false;
       przycisk_nie_wcisniety = false;
       pytanie_wyswietlone = false;
        start_wyswietlony = false;
        numer_pytania = 0;
    delay(2000);
  lcd.clear();

}

}
}

if(numer_pytania == 1){
   if (!pytanie_wyswietlone) {
        lcd.setCursor(0, 1);
        lcd.print("1-Save, 2-Lose");
        pytanie_wyswietlone = true;
    }

    if (!przycisk_tak_wcisniety && !przycisk_nie_wcisniety) {
        przycisk_tak_wcisniety = digitalRead(PRZYCISK_TAK_PIN) == LOW;
        przycisk_nie_wcisniety = digitalRead(PRZYCISK_NIE_PIN) == LOW;
        delay(40); // Krótka pauza, aby nie wykrywać wielokrotnego naciśnięcia
    }
if (przycisk_tak_wcisniety) {
        //    Serial.print("Czas w ms: ");
            Serial.println(czas_przelotu_1);
        lcd.clear();
        lcd.setCursor(0, 0);
            lcd.print("Saving");
                    for(int i = 0; i < 3; i++){ // miganie ekranu po obliczeniu i wyświetleniu czasu
                lcd.print(".");
                lcd.noDisplay(); //Wyłącz i poczekaj
                delay(500);
                lcd.display(); //Włącz i poczekaj
                delay(500);
                }
       przycisk_tak_wcisniety = false;
       przycisk_nie_wcisniety = false;
       pytanie_wyswietlone = false;
        start_wyswietlony = false;
        numer_pytania = 0;
        }

        if (przycisk_nie_wcisniety) {
            lcd.clear();
       przycisk_tak_wcisniety = false;
       przycisk_nie_wcisniety = false;
       pytanie_wyswietlone = false;
        start_wyswietlony = false;
        numer_pytania = 0;
        }


}


delay(2);
}
