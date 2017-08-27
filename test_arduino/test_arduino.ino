/*Prueba de las funciones del arduino 101
 * Double tap detector
 * Arturo Rosete, Montemorelos University
 * arturorosete0@gmail.com
*/

#include <CurieBLE.h>
#include <BMI160.h>
#include <CurieIMU.h>

long lastStepCount = 0;              // step count on previous polling check


BLEPeripheral blePeripheral;
BLEService heartRateService("180D"); //uuid, standar

BLECharacteristic heartRateChar("2A37", BLERead | BLENotify, 2);

int oldHeartRate = 0;
long previousMillis = 0;

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while(!Serial);
  
  //Configuración del BLE
  blePeripheral.setLocalName("UM");
  blePeripheral.setAdvertisedServiceUuid(heartRateService.uuid());
  blePeripheral.addAttribute(heartRateService);   // Add the BLE Heart Rate service
  blePeripheral.addAttribute(heartRateChar); // add HeartRateMeasurement characteristic

  blePeripheral.begin(); //ACtiva el bluetooth
  Serial.println("Bluetooth active, waiting for connections...");
  
  /* Initialise the IMU */
  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);

  /* Enable Shock Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 16000); // 16g = 16000mg
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 75);   // 50ms
  CurieIMU.interrupts(CURIE_IMU_SHOCK);

  /*Enable Step detection*/
  CurieIMU.setStepDetectionMode(CURIE_IMU_STEP_MODE_NORMAL); 
  CurieIMU.setStepCountEnabled(true);  

  Serial.println("IMU initialisation complete, waiting for events...");
}

void loop() {
  
  BLECentral central = blePeripheral.central();

  //Si se ha conectado algun dispositivo
  if(central){
    Serial.println("Dispositivo conectado");
    Serial.println(central.address());
    digitalWrite(13,HIGH);    
    
    while (central.connected()) {
      updateStepCount();
      long currentMillis = millis();
      // if 200ms have passed, check the heart rate measurement:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        updateHeartRate(); //Actualiza el ritmo cardiaco
      }
    }

    digitalWrite(13,LOW);
    Serial.println("Disconnected from device...");
    Serial.println(central.address());
  }
}

void updateHeartRate(){
  //Calcular RPM
}

static void updateStepCount() {
  // get the step count:
  int stepCount = CurieIMU.getStepCount();

  // if the step count has changed, print it:
    if (stepCount != lastStepCount) {
    Serial.print("Step count: ");
    Serial.println(stepCount);
    // save the current count for comparison next check:
    lastStepCount = stepCount;
  }
}

static void eventCallback(void){
  if (CurieIMU.getInterruptStatus(CURIE_IMU_SHOCK)) {
    //Escribir en BLE "Caída"
    Serial.println("Impacto");
  }
}

