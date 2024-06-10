#include <LiquidCrystal.h>
#include <DHT.h>
#include <Servo.h>

// Определение пинов для подключения LCD дисплея
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define motionSensorPin1 6 // Пин датчика движения
#define motionSensorPin2 10
#define ledPin1 13 // светодиод (свет)
#define ledPin2 8 // светодиод (сигнализация)
#define DHTPIN A3 // Пин, к которому подключен DATA вывод DHT22
#define DHTTYPE DHT22 // Тип датчика DHT22
#define potPin A0 // Пин потенциометра
#define servoPin 9 // Пин сервопривода
#define photoresPin A1 //Пин фоторезистора
#define switchPin A2 // Пин переключателя (вкл/выкл сигнализация)
#define signalPin 7 // Пин сигнализации (звук)

DHT dht(DHTPIN, DHTTYPE);
// Создание экземпляра объекта сервопривода
Servo myServo;

float h = 0;
float t = 0;
int lx = 390; //значение зависит от используемого фоторезистора (390 = 200 lx)
unsigned long previousMillis = 0;
const long interval = 300;
int ledState = HIGH;

void setup() {
  // Настройка пинов для входа
  pinMode(motionSensorPin1, INPUT);
  pinMode(motionSensorPin2, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(potPin, INPUT);
  pinMode(photoresPin, INPUT);
  pinMode(switchPin, INPUT_PULLUP);

  // Настройка пинов для выхода
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(signalPin, OUTPUT);
  pinMode(servoPin, OUTPUT);

  // Инициализация DHT22
  dht.begin();

  // Настройка сервопривода
  myServo.attach(servoPin);

  Serial.begin(9600);

  // Инициализация LCD дисплея
  lcd.begin(20, 4);
}

void loop() {
  // Считывание температуры и влажности
  h = dht.readHumidity();
  t = dht.readTemperature();

  char temp[8];
  char hum[8];
  dtostrf(t, 4, 1, temp);
  dtostrf(h, 4, 1, hum);
  
  // Вывод температуры и влажности
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C");
  lcd.print(" H:");
  lcd.print(hum);
  lcd.print("%");
  
  // Считывание значения с потенциометра
  int potValue = analogRead(potPin);

  // Преобразование значения от 0 до 1023 в угол от 0 до 90
  int angle = map(potValue, 0, 1023, 0, 90);

  // Поворот сервопривода на заданный угол
  myServo.write(angle);

  // Чтение значения с фоторезистора
  int valuePhotores = analogRead(photoresPin);
  int resultPhotores = (valuePhotores > lx) ? 1 : 0;

  // Чтение состояния датчика движения 1
  int pirState1 = digitalRead(motionSensorPin1);

  digitalWrite(ledPin1, resultPhotores*pirState1);

  //считываение состояния переключателя
  int switchState = digitalRead(switchPin);
  unsigned long currentMillis = millis();

  if (switchState == HIGH) {
    // Чтение состояния PIR датчика
    int pirState2 = digitalRead(motionSensorPin2);
    if (pirState2 == HIGH) {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          if (ledState == LOW)
            ledState = HIGH;
          else
            ledState = LOW;
          digitalWrite(ledPin2, ledState);
          tone(signalPin, 500 + ledState*500);
        }
    } else {
      digitalWrite(ledPin2, LOW);
      noTone(signalPin);
      lcd.setCursor(0, 1);
      lcd.print("The alarm is ON ");}
  } else {
    digitalWrite(ledPin2, LOW);
    noTone(signalPin);
    lcd.setCursor(0, 1);
    lcd.print("The alarm is OFF");
  }

}
