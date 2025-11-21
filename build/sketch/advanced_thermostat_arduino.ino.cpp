#include <Arduino.h>
#line 1 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
#include <BLEDevice.h>
#include <SimpleTimer.h>

#define LYWSD03MMC_ADDR "a4:c1:38:D3:71:A1"
//D371A1
//"a4:c1:38:D3:71:A1"
//"a4:c1:38:8C:DA:85"


BLEClient* pClient;

static BLEAddress htSensorAddress(LYWSD03MMC_ADDR);

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

#line 35 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
static void notifyCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
#line 50 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void registerNotification();
#line 72 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void setup();
#line 84 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void loop();
#line 92 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void createBleClientWithCallbacks();
#line 97 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
void connectSensor();
#line 35 "C:\\Users\\ctgcontrols8601\\Documents\\GitHub\\LckidsLabs\\advanced_thermostat_arduino\\advanced_thermostat_arduino.ino"
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  float temp;
  float humi;
  Serial.print("Notify callback for characteristic ");
  Serial.println(pBLERemoteCharacteristic->getUUID().toString().c_str());
  temp = (pData[0] | (pData[1] << 8)) * 0.01; //little endian 
  humi = pData[2];
  Serial.printf("temp = %.1f : humidity = %.1f \n", temp, humi);
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

void setup() {
  Serial.begin(115200);
  Serial.println("Starting MJ client...");
  delay(500);

  BLEDevice::init("ESP32");
  //createBleClientWithCallbacks();
  //delay(500);
  //connectSensor();
  //registerNotification();
}

void loop() {
  // do nothing
  createBleClientWithCallbacks();
  delay(500);
  connectSensor();
  registerNotification();
}

void createBleClientWithCallbacks() {
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
}

void connectSensor() {
  pClient->connect(htSensorAddress);
  connectionSuccessful = true;
}
