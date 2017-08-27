/*Prueba de las funciones del arduino 101
 * Double tap detector
 * Arturo Rosete, Montemorelos University
 * arturorosete0@gmail.com
*/

#include <BMI160.h>
#include <CurieIMU.h>

boolean blinkState = false;          // state of the LED

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while(!Serial) ;    // wait for serial port to connect..
  /* Initialise the IMU */
  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);

  /* Enable Shock Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 16000); // 1.5g = 1500 mg
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 75);   // 50ms
  CurieIMU.interrupts(CURIE_IMU_SHOCK);

  Serial.println("IMU initialisation complete, waiting for events...");
}

void loop() {
  // blink the LED in the main loop:
  digitalWrite(13, blinkState);
  blinkState = !blinkState;
  delay(200);
}

static void eventCallback(void)
{
  if (CurieIMU.getInterruptStatus(CURIE_IMU_SHOCK)) {
    if (CurieIMU.shockDetected(X_AXIS, POSITIVE))
      Serial.println("Negative shock detected on X-axis");
    if (CurieIMU.shockDetected(X_AXIS, NEGATIVE))
      Serial.println("Positive shock detected on X-axis");
    if (CurieIMU.shockDetected(Y_AXIS, POSITIVE))
      Serial.println("Negative shock detected on Y-axis");
    if (CurieIMU.shockDetected(Y_AXIS, NEGATIVE))
      Serial.println("Positive shock detected on Y-axis");
    if (CurieIMU.shockDetected(Z_AXIS, POSITIVE))
      Serial.println("Negative shock detected on Z-axis");
    if (CurieIMU.shockDetected(Z_AXIS, NEGATIVE))
      Serial.println("Positive shock detected on Z-axis");
  }
}

