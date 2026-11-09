#include <DHT.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// ================= 引脚设置 =================
#define DHT_PIN 4
#define DHT_TYPE DHT11

#define LED_PIN 18
#define BUZZER_PIN 19

#define SDA_PIN 21
#define SCL_PIN 22

// ============== 舒适温度区间 ==============
// 默认适用于0～1周的小奶猫
const float MIN_TEMP = 27.0;
const float MAX_TEMP = 31.0;

// ================= 创建设备 =================
DHT dht(DHT_PIN, DHT_TYPE);

// 自动寻找LCD的I2C地址
hd44780_I2Cexp lcd;

// 0=正常，1=温度过低，2=温度过高
int alarmStatus = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);

  // 启动DHT11
  dht.begin();

  // 启动I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // 启动16列、2行LCD
  int lcdResult = lcd.begin(16, 2);

  if (lcdResult != 0) {
    Serial.print("LCD启动失败，错误代码：");
    Serial.println(lcdResult);
  } else {
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Kitten Monitor");

    lcd.setCursor(0, 1);
    lcd.print("Starting...");

    delay(2000);
    lcd.clear();
  }

  Serial.println("小奶猫温湿度监测系统启动");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // 检查DHT11是否读取成功
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("DHT11读取失败，请检查接线！");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT11 ERROR");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring");

    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);

    delay(2000);
    return;
  }

  // 判断温度状态
  if (temperature < MIN_TEMP) {
    alarmStatus = 1;
  }
  else if (temperature > MAX_TEMP) {
    alarmStatus = 2;
  }
  else {
    alarmStatus = 0;
  }

  // 串口显示
  Serial.print("温度：");
  Serial.print(temperature, 1);
  Serial.print(" °C    湿度：");
  Serial.print(humidity, 1);
  Serial.print(" %    状态：");

  if (alarmStatus == 1) {
    Serial.println("温度过低");
  }
  else if (alarmStatus == 2) {
    Serial.println("温度过高");
  }
  else {
    Serial.println("正常");
  }

  // LCD第一行显示温度
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.print(" C     ");

  // LCD第二行显示湿度和状态
  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print(humidity, 0);
  lcd.print("% ");

  if (alarmStatus == 1) {
    lcd.print("COLD ");
  }
  else if (alarmStatus == 2) {
    lcd.print("HOT  ");
  }
  else {
    lcd.print("OK   ");
  }

  // 控制LED和蜂鸣器
  if (alarmStatus == 1) {
    // 温度过低：低音慢速报警
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 800);
    delay(500);

    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    delay(500);
  }
  else if (alarmStatus == 2) {
    // 温度过高：高音快速报警
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1500);
    delay(200);

    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    delay(200);
  }
  else {
    // 温度正常
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    delay(2000);
  }
}
