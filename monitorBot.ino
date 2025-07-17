#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>

// ======= CONFIGURAÇÕES WiFi e Telegram =======
const char* SSID = "wifi-zone-1";
const char* SENHA = "";
const char* BOT_TOKEN = "";
const int64_t CHAT_ID = ; // sem aspas

WiFiClientSecure cliente;
UniversalTelegramBot bot(BOT_TOKEN, cliente);

// ======= CONFIGURAÇÕES dos SENSORES =======
#define DHT_PIN 5      // Ex: GPIO15
#define DHT_TYPE DHT11
#define MQ2_PIN 7      // Pino analógico
#define MQ9_PIN 6      // Pino analógico
#define FLAMA_PIN 8    // Pino digital do HW-038

DHT dht(DHT_PIN, DHT_TYPE);

unsigned long ultimaVerificacao = 0;
const long intervalo = 1000;

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(MQ2_PIN, INPUT);
  pinMode(MQ9_PIN, INPUT);
  pinMode(FLAMA_PIN, INPUT);

  Serial.println("Conectando ao WiFi...");
  WiFi.begin(SSID, SENHA);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  cliente.setInsecure();
}

void handleComandos(int numMensagens) {
  for (int i = 0; i < numMensagens; i++) {
    String texto = bot.messages[i].text;
    String remetente = bot.messages[i].from_name;
    String chat_id = bot.messages[i].chat_id;

    Serial.println("Comando de: " + remetente);
    Serial.println("Texto: " + texto);

    if (texto == "/start") {
      bot.sendMessage(chat_id, "Bot iniciado com sucesso. Envie /infos para ver os dados dos sensores.", "");
    } 
    else if (texto == "/infos") {
      float temperatura = dht.readTemperature();
      float umidade = dht.readHumidity();
      int mq2 = analogRead(MQ2_PIN);
      int mq9 = analogRead(MQ9_PIN);
      int chama = digitalRead(FLAMA_PIN);

      Serial.println("===== DADOS DOS SENSORES =====");
      Serial.print("Temperatura: ");
      Serial.print(temperatura);
      Serial.println(" °C");

      Serial.print("Umidade: ");
      Serial.print(umidade);
      Serial.println(" %");

      Serial.print("MQ-2: ");
      Serial.println(mq2);

      Serial.print("MQ-9: ");
      Serial.println(mq9);

      Serial.print("Chama detectada (HW-038): ");
      Serial.println(chama == 0 ? "SIM" : "NÃO");

      Serial.println("================================");

      bot.sendMessage(chat_id, "Dados dos sensores enviados via Serial.", "");
    }
    else {
      bot.sendMessage(chat_id, "Comando não reconhecido. Use /start ou /infos.", "");
    }
  }
}

void loop() {
  if (millis() - ultimaVerificacao > intervalo) {
    int numNovas = bot.getUpdates(bot.last_message_received + 1);

    while (numNovas) {
      handleComandos(numNovas);
      numNovas = bot.getUpdates(bot.last_message_received + 1);
    }

    ultimaVerificacao = millis();
  }
}
