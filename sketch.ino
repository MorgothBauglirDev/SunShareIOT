#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "DHTesp.h"
#include <PubSubClient.h>

// Configurações de WiFi
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";

// Configurações de MQTT
const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "sunshare_iot";
const char *TOPIC_PUBLISH_SENSOR = "sunshare/iot/clima";

#define PIN_DHT 12
#define PIN_LDR 34
#define PUBLISH_DELAY 2000

// Variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
DHTesp dht;
unsigned long publishUpdate = 0;
TempAndHumidity sensorValues;
const int TAMANHO = 200;

// Funções protótipos
void updateSensorValues();
void initWiFi();
void initMQTT();
void reconnectMQTT();
void reconnectWiFi();
void checkWiFIAndMQTT();
float calcularPredicao(float luminosidade);

// Atualiza os valores dos sensores
void updateSensorValues() {
  Serial.println("Lendo valores do DHT22...");
  sensorValues = dht.getTempAndHumidity();

  // Se a leitura falhar, define valores padrão
  if (isnan(sensorValues.temperature) || isnan(sensorValues.humidity)) {
    Serial.println("Erro ao ler os valores do DHT22. Definindo valores padrão.");
    sensorValues.temperature = 0.0;  // valor padrão de temperatura
    sensorValues.humidity = 0.0;     // valor padrão de umidade
  } else {
    Serial.println("Valores lidos com sucesso:");
    Serial.print("Temperatura: ");
    Serial.println(sensorValues.temperature);
    Serial.print("Umidade: ");
    Serial.println(sensorValues.humidity);
  }
}

// Inicializa o Wi-Fi
void initWiFi() {
  Serial.print("Conectando à rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado ao Wi-Fi: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Inicializa o MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

// Reconecta ao MQTT
void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
    } else {
      Serial.println("Falha na conexão MQTT. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}

// Reconecta ao Wi-Fi
void reconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wi-Fi reconectado: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Verifica conexão Wi-Fi e MQTT
void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}

// Função para calcular previsão solar com base na luminosidade
float calcularPredicao(float luminosidade) {
  return luminosidade * 1.25;  // simulando a conversão de luminosidade à produção em kWh
}

void setup() {
  Serial.begin(9600);

  // Inicializa os sensores
  Serial.println("Configurando sensores...");
  pinMode(PIN_DHT, INPUT);  // Configura o pino como entrada
  dht.setup(PIN_DHT, DHTesp::DHT22);

  // Inicializa o Wi-Fi e MQTT
  initWiFi();
  initMQTT();
}

void loop() {
  checkWiFIAndMQTT();
  MQTT.loop();

  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    Serial.println("Atualizando valores dos sensores...");
    updateSensorValues();

    Serial.println("Lendo luminosidade...");
    float luminosidade = analogRead(PIN_LDR);
    float predicaoOutput = calcularPredicao(luminosidade);

    // Publica os dados se os valores forem válidos
    Serial.println("Criando JSON para envio...");
    StaticJsonDocument<TAMANHO> doc;
    doc["temperatura"] = sensorValues.temperature;
    doc["umidade"] = sensorValues.humidity;
    doc["luminosidade"] = luminosidade;
    doc["previsao_solar"] = predicaoOutput;

    char buffer[TAMANHO];
    serializeJson(doc, buffer);
    if (MQTT.publish(TOPIC_PUBLISH_SENSOR, buffer)) {
      Serial.println("publicacao MQTT");
    } else {
      Serial.println("falha na publicacao");
    }
  }
}
