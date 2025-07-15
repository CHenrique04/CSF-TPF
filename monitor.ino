#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT11
#define MQ9PIN 6
#define MQ2PIN 7

DHT dht(DHTPIN, DHTTYPE);

void setup(){
	Serial.begin(115200);
	Serial.println("DHT funcionando!");
	dht.begin();
}

void loop(){
	//sensor DHT
	float humid = dht.readHumidity();
	float temp = dht.readTemperature();

	//sensor de fumaça MQ-2
	int valorMQ2 = analogRead(MQ2PIN);

	//sensor de CO MQ-9
	int valorMQ9 = analogRead(MQ9PIN); 

	//saida DHT11 temperatura e umidade
	if (isnan(temp) || isnan(humid)){
	Serial.println("DHT Não está coletando dados!");
	} else {
	Serial.print("Umidade: ");
	Serial.print(humid);
	Serial.print(" ");
	Serial.print("Temperatura: ");
	Serial.print(temp);
	Serial.println(" *C");
	//delay(2000);
	}

	//saida MQ-2 sensor de fumaça
	Serial.print("Fumaça: ");
	Serial.println(valorMQ2);

	//saida MQ-9 sensor de monoxido carbono CO
	Serial.print("Nível CO: ");
	Serial.println(valorMQ9);

	delay(2000);
}