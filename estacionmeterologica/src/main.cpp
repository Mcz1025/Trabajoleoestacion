#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_EnvironmentalSensor.h>   // trae la clase I2C también
#include <DFRobot_AirQualitySensor.h>      // SEN0460

// Pines I2C ESP32
#define I2C_SDA 21
#define I2C_SCL 22

// Ajusta con lo que te dio el escaneo
#define SEN0501_ADDR  0x28   // ambiental
#define PM25_I2C_ADDR 0x19   // partículas

// OJO: usar la clase I2C, no la base
//DFRobot_EnvironmentalSensor_I2C env(&Wire, SEN0501_ADDR);

// Para DFRobot_AirQualitySensor 1.0.0 el ctor típico es (&Wire, addr)
DFRobot_AirQualitySensor pm(&Wire, PM25_I2C_ADDR);

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\nESP32 + SEN0501 + SEN0460 (lectura simple)");

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  // SEN0501
  //bool envOK = env.begin();  // esta variante devuelve bool
  //Serial.println(envOK ? "SEN0501 OK" : "ERROR: SEN0501 no detectado");

  // SEN0460
  bool pmOK = pm.begin();    // en 1.0.0 begin() va sin args
  Serial.println(pmOK ? "SEN0460 OK" : "ERROR: SEN0460 no detectado");
}
  
void loop() {

  // --- SEN0501 ---
  //float temp = env.getTemperature(TEMP_C);
  //float hum  = env.getHumidity();
  //float pres = env.getAtmospherePressure(HPA);
  //float lux  = env.getLuminousIntensity();
  //float uv   = env.getUltravioletIntensity();

  //Serial.println("=== SEN0501 ===");
  //Serial.printf("Temp: %.2f C\n", temp);
 // Serial.printf("Humedad: %.1f %%\n", hum);
 // Serial.printf("Presion: %.1f hPa\n", pres);
 // Serial.printf("Luz: %.1f lx\n", lux);
 // Serial.printf("UV: %.2f\n", uv);

  // SEN0460 
  
  uint16_t pm1  = pm.gainParticleConcentration_ugm3(PARTICLE_PM1_0_STANDARD);
  uint16_t pm25 = pm.gainParticleConcentration_ugm3(PARTICLE_PM2_5_STANDARD);
  uint16_t pm10 = pm.gainParticleConcentration_ugm3(PARTICLE_PM10_STANDARD);

  Serial.println("=== SEN0460 ===");
  Serial.printf("PM1.0: %u ug/m3\n", pm1);
  Serial.printf("PM2.5: %u ug/m3\n", pm25);
  Serial.printf("PM10 : %u ug/m3\n", pm10);

  Serial.println("-------------------------------");
  delay(2000);
}
