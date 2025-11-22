#include <Arduino.h>
#line 1 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
#include <BLEDevice.h>
#include <SimpleTimer.h>

#define LYWSD03MMC_ADDR_HOUSE "a4:c1:38:D3:71:A1"
#define LYWSD03MMC_ADDR_BARN "a4:c1:38:8C:DA:85"
//D371A1
//"a4:c1:38:D3:71:A1"
//"a4:c1:38:8C:DA:85"

int led = LED_BUILTIN;
int loops = 0;


BLEClient* pClient;

static BLEAddress htSensorAddress01(LYWSD03MMC_ADDR_HOUSE);
static BLEAddress htSensorAddress02(LYWSD03MMC_ADDR_BARN);

bool connectionSuccessful = false;

// The remote service we wish to connect to.
static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
      Serial.println("Connected");
    }

    void onDisconnect(BLEClient* pclient) {
      Serial.println("Disconnected");
      if (!connectionSuccessful) {
        Serial.println("RESTART");
        ESP.restart();
      }
    }
};

#line 40 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
static void notifyCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
#line 59 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void registerNotification();
#line 91 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void setup();
#line 107 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void loop();
#line 148 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void createBleClientWithCallbacks();
#line 153 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void connectSensor(int loops);
#line 40 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  float temp;
  float humi;
  float mV;
  Serial.print("Notify callback for characteristic ");
  Serial.println(pBLERemoteCharacteristic->getUUID().toString().c_str());
  temp = (pData[0] | (pData[1] << 8)) * 0.01; //little endian 
  humi = pData[2];
  mV = (pData[3] | (pData[4] << 8));
  Serial.printf("temp = %.1f : humidity = %.1f  : mV = %.1f \n", temp, humi, mV);
  size_t xlength = sizeof(pData) / sizeof(pData[0]);
  Serial.println(xlength );
  pClient->disconnect();
}

void registerNotification() {

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
  }
  Serial.println(" - Found our service");
  try
  {
    // Obtain a reference to the characteristic in the service of the remote BLE server.
    BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
    }
    Serial.println(" - Found our characteristic");
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }
  catch(const std::exception& e)
  {
    Serial.println(e.what());
  }
  
  
  

}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting MJ client...");

  // set LED to be an output pin
  pinMode(led, OUTPUT);
  
  delay(3000);

  BLEDevice::init("ESP32");
  //createBleClientWithCallbacks();
  //delay(1000);
  //connectSensor();
  //registerNotification();
}

void loop() {
  // do nothing
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("");
  Serial.print("Loop: ");
  Serial.println(loops);
  try
  {
    createBleClientWithCallbacks();
  }
  catch(const std::exception& e)
  {
    Serial.println(e.what());
  }
  delay(500);
  try
  {
    connectSensor(loops);
  }
  catch(const std::exception& e)
  {
    Serial.println(e.what());
  }
    try
  {
    registerNotification();
  }
  catch(const std::exception& e)
  {
    Serial.println(e.what());
  }
  
  
  
  
  // wait for a half second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(45000);                // wait for a half second
  loops++;
}

void createBleClientWithCallbacks() {
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
}

void connectSensor(int loops) {
  if (loops % 2 == 0) {
        pClient->connect(htSensorAddress01);
        Serial.println("Connecting to the House");
  } else {
        pClient->connect(htSensorAddress02);
        Serial.println("Connecting to the Barn");
  }
  connectionSuccessful = true;
}
