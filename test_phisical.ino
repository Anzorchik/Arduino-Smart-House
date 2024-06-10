#define tempPin  A0 //пин датчика температуры 
#define motionSigPin  13 //пин датчика движения для сигнализации
#define motionPin  3 //пин датчика движения для света
#define gasPin  A3 //пин датчика газа
#define lightPin  A4 //пин датчика света
#define powerPin  A5 //пин датчика удара
#define buttonPin  A6 //пин датчика кнопки

#define ledPin1  2 //пин светодиода для сигнализации
#define ledPin2  4 //пин светодиода для датчика газа
#define ledPin3  5 //пин светодиода для света
#define buttonPinOut  6 //пин для эмулятора кнопки
#define powerPinOut  7 //пин для эмулятора датчика разбития стекла
#define lightPinOut  8 //пин для эмулятора датчика света
#define gasPinOut  9 //пин для эмулятора датчика газа
#define motionPinOut  10 //пин для эмулятора датчика движения для света
#define motionSigPinOut  11 //пин для эмулятора датчика движения для сигнализации
#define tempPinOut  12 //пин для эмулятора датчика температуры

int temp = 15; //устанавливаем температуру
int light = 2; //устанавливаем уровень света от 1 до 10
int gas = 8; //устанавливаем уровень утечки газа от 1 до 10
int power = 8; //устанавливаем силу удара стекла от 1 до 10

//устанвливаем выводимые датчиками результаты
int setPower = 1023/power; 
int setTemp = 1023/temp;
int setLight = 1023/light;
int setGas = 1023/gas;
int button = 2; //первый режим: под охраной, второй режим: ночь
//третий режим: отключить сигнализацию
int setButton = 1023/button;


void setup() {
  Serial.begin(9600);
  //пины на вход
  pinMode(tempPin, INPUT);
  pinMode(motionSigPin, INPUT);
  pinMode(motionPin, INPUT);
  pinMode(gasPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(powerPin, INPUT);
  pinMode(buttonPin, INPUT);
  //пины на выход
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buttonPinOut, OUTPUT);
  pinMode(powerPinOut, OUTPUT);
  pinMode(lightPinOut, OUTPUT);
  pinMode(gasPinOut, OUTPUT);
  pinMode(motionPinOut, OUTPUT);
  pinMode(motionSigPinOut, OUTPUT);
  pinMode(tempPinOut, OUTPUT);
}

void loop() {
  //включаем эмуляторы датчиков
  digitalWrite(tempPinOut, HIGH);
  digitalWrite(motionSigPinOut, HIGH);
  digitalWrite(motionPinOut, LOW);
  digitalWrite(gasPinOut, HIGH);
  digitalWrite(lightPinOut, HIGH);
  digitalWrite(powerPinOut, HIGH);
  digitalWrite(buttonPinOut, HIGH);
  //считываем значения с эмуляторов
  int buttonRead = analogRead(buttonPin);
  int tempRead = analogRead(tempPin);
  int gasRead = analogRead(gasPin);
  int motionRead = digitalRead(motionPin);
  int motionSigRead = digitalRead(motionSigPin);
  int lightRead = analogRead(lightPin);
  int powerRead = analogRead(powerPin);
  //настраиваем вывод температуры на экран
  int TEMP = tempRead / setTemp;
  Serial.print(TEMP);
  Serial.println(" градусов");
  delay(1000);
  //устнавливаем значения результатов с датчиков
  int LIGHT = lightRead / setLight;
  int GAS = gasRead / setGas;
  int POWER = powerRead / setPower;
  //настраиваем режим работы кнопки
  //если стоит первый режим
  if (button == 1 )
  {
    if (POWER > 6 || motionSigRead == HIGH)
    {
      digitalWrite(ledPin1, HIGH);
    }
    else
    {
      digitalWrite(ledPin1, LOW);
    }
  }
  //если стоит второй режим
  else if(button == 2)
  {
    if(POWER > 6)
    {
      digitalWrite(ledPin1, HIGH);
    }
    else
    {
      digitalWrite(ledPin1, LOW);
    }
  }
  //если стоит третий режим
  else
  {
    digitalWrite(ledPin1, LOW);
  }
  //обработка датчика света
  if(LIGHT < 4 && motionRead == HIGH)
  {
    digitalWrite(ledPin3, HIGH);
  }
  else
  {
    digitalWrite(ledPin3, LOW);
  }
  //обработка датчика газа
  if( GAS > 6)
  {
    digitalWrite(ledPin2, HIGH);
  }
  else
  {
    digitalWrite(ledPin2, LOW);
  }
}