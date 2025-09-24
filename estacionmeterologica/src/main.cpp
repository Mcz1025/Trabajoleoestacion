#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_EnvironmentalSensor.h>   // SEN0501 (tu variante)
#include <DFRobot_AirQualitySensor.h>      // SEN0460

// Pines I2C ESP32
constexpr int I2C_SDA = 21;
constexpr int I2C_SCL = 22;


constexpr uint8_t SEN0501_ADDR = 0x22;  // ambiental
constexpr uint8_t PM25_I2C_ADDR = 0x19; // partículas

// Mantengo tu forma de instanciar 
DFRobot_EnvironmentalSensor sensor(SEN0501_ADDR, &Wire);
DFRobot_AirQualitySensor    pm(&Wire, PM25_I2C_ADDR);

bool senOK = false;
bool pmOK  = false;

void setup() {
  Serial.begin(115200);
  Serial.println(F("\nESP32 + SEN0501 + SEN0460 (lectura simple)"));

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);  // si no es estable pruebo con 100000

  // SEN0501 
  for (int i = 0; i < 5 && !senOK; i++) {
    senOK = sensor.begin();
    if (!senOK) {
      Serial.printf("Error iniciando SEN0501 (intento %d/5)\n", i + 1);
      delay(250);
    }
  }
  Serial.println(senOK ? F("SEN0501 OK") : F("SEN0501 no iniciado"));

  // SEN0460 
  for (int i = 0; i < 5 && !pmOK; i++) {
    pmOK = pm.begin();
    if (!pmOK) {
      Serial.printf("Error iniciando SEN0460 (intento %d/5)\n", i + 1);
      delay(300);
    }
  }
  if (pmOK) {
    Serial.println(F("SEN0460 OK"));
    uint8_t ver = pm.gainVersion();
    Serial.print(F("SEN0460 version: 0x"));
    Serial.println(ver, HEX);
  } else {
    Serial.println(F("SEN0460 no iniciado"));
  }
}

void loop() {
  //SEN0501 
  if (senOK) {
    float temp = sensor.getTemperature(TEMP_C);
    float hum  = sensor.getHumidity();
    float pres = sensor.getAtmospherePressure(HPA);
    float lux  = sensor.getLuminousIntensity();
    float uv   = sensor.getUltravioletIntensity();

    Serial.println(F("=== SEN0501 ==="));
    Serial.printf("Temp: %.2f C\n", temp);
    Serial.printf("Humedad: %.1f %%\n", hum);
    Serial.printf("Presion: %.1f hPa\n", pres);
    Serial.printf("Luz: %.1f lx\n", lux);
    Serial.printf("UV: %.2f\n", uv);
  } else {
    Serial.println(F("[SEN0501] no iniciado"));
  }

  // SEN0460 
  if (pmOK) {
    uint16_t pm1  = pm.gainParticleConcentration_ugm3(PARTICLE_PM1_0_STANDARD);
    uint16_t pm25 = pm.gainParticleConcentration_ugm3(PARTICLE_PM2_5_STANDARD);
    uint16_t pm10 = pm.gainParticleConcentration_ugm3(PARTICLE_PM10_STANDARD);

    Serial.println(F("=== SEN0460 ==="));
    Serial.printf("PM1.0: %u ug/m3\n", pm1);
    Serial.printf("PM2.5: %u ug/m3\n", pm25);
    Serial.printf("PM10 : %u ug/m3\n", pm10);
  } else {
    Serial.println(F("[SEN0460] no iniciado"));
  }

  Serial.println(F("-------------------------------"));
  delay(1000);
}
