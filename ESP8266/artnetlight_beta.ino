#include <ArtnetWiFi.h>
#include <LittleFS.h>
#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// LED pins
const int redPin = 14;
const int greenPin = 12;
const int bluePin = 13;
 // DC fan pin
const int fan_pin = 15;
int fanSpeed; // variable to store fan speed
 //config button pin
#define BUTTON_PIN D1
//define DHT Sensor Pin
const int DHT_pin = 2; // DHT11 sensor pin
DHT dht(DHT_pin, DHT11);
//define de status led pin
int STATUSLED = 16;  // wifi status LED is connected to pin 4
//define screen settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//define wifi config file path
#define CONFIG_FILE "/config.txt"

// define the buffer size for the config elements
char ssid[64], pwd[64], ip[32], gateway[32], subnet[32];
// Artnet stuff
ArtnetWiFiReceiver artnet;
uint8_t universe = 1;  // 0 - 15

#define LOGO_HEIGHT 32
#define LOGO_WIDTH 128

// 'gerialkot arduino LCD', 128x32px
const unsigned char bitmap_logo [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x38, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3c, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1e, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x06, 0x3f, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x02, 0x7f, 0xe4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x01, 0xef, 0x32, 0x22, 0x12, 0x67, 0xc0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0xff, 0xf8, 0x01, 0x08, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0xff, 0xf8, 0x01, 0x4f, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x7f, 0xec, 0x01, 0x28, 0x72, 0x52, 0x1c, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0xbf, 0xd8, 0x01, 0x28, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0xbf, 0xd8, 0x01, 0x28, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x01, 0x28, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x01, 0x28, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x01, 0x28, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x01, 0x28, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x01, 0x28, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x01, 0x28, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x46, 0x20, 0x01, 0x28, 0x4a, 0x72, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x01, 0x28, 0x4a, 0x52, 0x12, 0x91, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0xcf, 0x4a, 0x53, 0xd2, 0x61, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(redPin, map(r, 0, 255, 0, 255));
  analogWrite(greenPin, map(g, 0, 255, 0, 114));
  analogWrite(bluePin, map(b, 0, 255, 0, 20));
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("System OK.");
  display.println(WiFi.localIP());
  display.println("Temperature: " + String(dht.readTemperature()));
  display.println("R:" + String(r) + " G:" + String(g) + " B:" + String(b));
  display.display();
}

void callback(const uint8_t* data, const uint16_t size) {
  if (size < 3) return;
  uint8_t r = data[0];
  uint8_t g = data[1];
  uint8_t b = data[2];
  setRGB(r, g, b);
}

void ConnectToWifi()
{

  File configFile = LittleFS.open(CONFIG_FILE, "r");
  if (!configFile) {
    Serial.println("Failed to open config file for reading.");
    return;
  }

  String ssid_str = configFile.readStringUntil('\n');
  ssid_str.trim(); // Remove whitespace and newline characters from beginning and end of string
  char ssid[ssid_str.length() + 1];
  ssid_str.toCharArray(ssid, ssid_str.length() + 1);
  const char* ssid_ptr = ssid;

  String pwd_str = configFile.readStringUntil('\n');
  pwd_str.trim(); // Remove whitespace and newline characters from beginning and end of string
  char pwd[pwd_str.length() + 1];
  pwd_str.toCharArray(pwd, pwd_str.length() + 1);
  const char* pwd_ptr = pwd;

  String ip_str = configFile.readStringUntil('\n');
  String gateway_str = configFile.readStringUntil('\n');
  String subnet_str = configFile.readStringUntil('\n');

    byte ip_arr[4];
  int pos = 0;
  for (int i = 0; i < 4; i++) {
    int end_pos = ip_str.indexOf('.', pos);
    if (end_pos == -1) end_pos = ip_str.length();
    ip_arr[i] = ip_str.substring(pos, end_pos).toInt();
    pos = end_pos + 1;
  }
  const IPAddress ip(ip_arr);
  byte gateway_arr[4];
  pos = 0;
  for (int i = 0; i < 4; i++) {
    int end_pos = gateway_str.indexOf('.', pos);
    if (end_pos == -1) end_pos = gateway_str.length();
    gateway_arr[i] = gateway_str.substring(pos, end_pos).toInt();
    pos = end_pos + 1;
  }
  const IPAddress gateway(gateway_arr);
  byte subnet_arr[4];
  pos = 0;
  for (int i = 0; i < 4; i++) {
    int end_pos = subnet_str.indexOf('.', pos);
    if (end_pos == -1) end_pos = subnet_str.length();
    subnet_arr[i] = subnet_str.substring(pos, end_pos).toInt();
    pos = end_pos + 1;
  }
  const IPAddress subnet(subnet_arr);

  // WiFi stuff
  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);
  Serial.println("Attempting to connect to");
  Serial.println(ssid);
  Serial.println("using password:");
  Serial.println(pwd);
  Serial.println( );
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);
  display.println("Connecting to WiFi...");
  display.display();

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    attempts++;
    if (attempts > 10) {
      // Failed to connect after 10 attempts, call the config function
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);        // Draw white text
      display.setCursor(0,0);
      display.println("Failed to connect to Wifi! Please Connect Via USB.");
      display.display();
      Serial.print("Failed to connect to WiFi! Entering Config mode.");
      Serial.println();
      configuration();
      return;
    }
    Serial.println(".");
    pinMode(STATUSLED, OUTPUT);
    digitalWrite(STATUSLED, LOW);
    delay(250);
    digitalWrite(STATUSLED, HIGH);
    delay(250);
  }
  Serial.println();
  Serial.println("WiFi connected, IP = ");
  Serial.println(WiFi.localIP());
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Connected to WiFi");
  display.println("IP address: ");
  display.println(WiFi.localIP());
  display.println("Waiting for DMX...");
  display.display();
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  delay(5000);

  // Artnet stuff
  artnet.begin();

  artnet.subscribe(universe, callback);

}

void configuration() {
  Serial.println("Enter WiFi SSID:");
  while (!Serial.available()) {
    // wait for input from serial monitor
  }
  String ssidInput = Serial.readStringUntil('\n');
  ssidInput.trim();

  Serial.println("Enter WiFi password:");
  while (!Serial.available()) {
    // wait for input from serial monitor
  }
  String pwdInput = Serial.readStringUntil('\n');
  pwdInput.trim();

  Serial.println("Enter IP address:");
  while (!Serial.available()) {
    // wait for input from serial monitor
  }
  String ipInput = Serial.readStringUntil('\n');
  ipInput.trim();

  Serial.println("Enter gateway:");
  while (!Serial.available()) {
    // wait for input from serial monitor
  }
  String gatewayInput = Serial.readStringUntil('\n');
  gatewayInput.trim();

  Serial.println("Enter subnet:");
  while (!Serial.available()) {
    // wait for input from serial monitor
  }
  String subnetInput = Serial.readStringUntil('\n');
  subnetInput.trim();

  if (ssidInput.length() > 63 || pwdInput.length() > 63 ||
      ipInput.length() > 31 || gatewayInput.length() > 31 || subnetInput.length() > 31) {
    Serial.println("Input too long!");
  } else {
    ssidInput.toCharArray(ssid, ssidInput.length() + 1);
    pwdInput.toCharArray(pwd, pwdInput.length() + 1);
    ipInput.toCharArray(ip, ipInput.length() + 1);
    gatewayInput.toCharArray(gateway, gatewayInput.length() + 1);
    subnetInput.toCharArray(subnet, subnetInput.length() + 1);

    File configFile = LittleFS.open(CONFIG_FILE, "w");
    if (!configFile) {
      Serial.println("Failed to open config file for writing.");
      return;
    }
    configFile.println(ssid);
    configFile.println(pwd);
    configFile.println(ip);
    configFile.println(gateway);
    configFile.println(subnet);
    configFile.close();
    Serial.println("Settings saved to file!");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(STATUSLED, OUTPUT);
  Wire.begin();
  delay(5000);   // Add a delay of 5 second
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  delay(1000);   // Add a delay of 1 second

  display.drawBitmap(
  (display.width()  - LOGO_WIDTH ) / 2,
  (display.height() - LOGO_HEIGHT) / 2,
  bitmap_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(5000);
  Serial.println("Welcome to the DonkeyHill Artnet open source LED light project!");
  Serial.println("System startup. Performing Tests...");
  //start the fan at full speed for a second for test
  analogWrite(fan_pin, 255);
  delay(1000);
  analogWrite(fan_pin, 0);
    // LED stuff
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  setRGB(255, 0, 0);
  delay(200);
  setRGB(0, 255, 0);
  delay(200);
  setRGB(0, 0, 255);
  delay(200);
  setRGB(0, 0, 0);
  Serial.println("Tests Complete. Proceeding to WifiConnect");
  delay(1000);
  if (LittleFS.begin()) {
    Serial.println("LittleFS initialized.");
    ConnectToWifi();
  } else {
    Serial.println("Failed to initialize LittleFS.");
  }
}

void loop() {
  pinMode(STATUSLED, OUTPUT);
  digitalWrite(STATUSLED, HIGH);
    // Read temperature and humidity from DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (temperature > 60) { // Cut off power to LEDs when temperature is too high
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Overheat Protection");
  } else {
    artnet.parse();
    }
  fanSpeed = map(temperature, 25, 60, 0, 255); // Control fan speed based on temperature
  analogWrite(fan_pin, fanSpeed);
}