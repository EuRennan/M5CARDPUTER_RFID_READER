#include <MFRC522_I2C.h>
#include <M5Cardputer.h>
#include <Wire.h>

// Definição dos pinos
#define MFRC522_I2C_ADDRESS 0x28
#define RST_PIN -1

// Instancia o objeto MFRC522
MFRC522_I2C mfrc522(MFRC522_I2C_ADDRESS, RST_PIN, &Wire);

void setup() {
  /*Inicializa o Display*/
  auto cfg = M5.config();
  M5Cardputer.begin(cfg);
  M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.fillScreen(BLACK);
    M5Cardputer.Display.setTextColor(WHITE);
    M5Cardputer.Display.setTextDatum(middle_center);
    M5Cardputer.Display.setTextFont(&fonts::FreeSansBold9pt7b);
    M5Cardputer.Display.setTextSize(1.5);
    M5Cardputer.Display.drawString("Inicializando",
                                   M5Cardputer.Display.width() / 2,
                                   M5Cardputer.Display.height() / 2);
  
  Serial.begin(115200); // Inicializa a comunicação serial
  Wire.begin(G2, G1);   // Inicializa o barramento I2C
  mfrc522.PCD_Init();   // Inicializa o módulo RFID
  Serial.println("Inicializando");
  delay(1000);
  M5Cardputer.Display.clear();
}

void loop() {
  M5Cardputer.update();
  // Verifica se há um novo cartão presente
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("Aproxime o cartão do leitor");
    //M5Cardputer.Display.clear();
    M5Cardputer.Display.setTextFont(&fonts::FreeSansBold9pt7b);
    M5Cardputer.Display.setTextSize(0.9);
    M5Cardputer.Display.drawString("Aproxime o Cartao do Leitor",
                                   M5Cardputer.Display.width() / 2,
                                   M5Cardputer.Display.height() / 2);
    delay(50);
    return;
  }

  // Seleciona um dos cartões
  if (!mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Exibe o UID no Monitor Serial
  Serial.print("UID do cartão: ");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem: ");
  content.toUpperCase();
  Serial.println(content.substring(1));
  M5Cardputer.Speaker.tone(10000, 20);
  M5Cardputer.Display.clear();
  M5Cardputer.Display.fillScreen(GREEN);
    M5Cardputer.Display.setTextColor(WHITE);
    M5Cardputer.Display.setTextDatum(middle_center);
    M5Cardputer.Display.setTextFont(&fonts::FreeSansBold24pt7b);
    M5Cardputer.Display.setTextSize(0.8);
    M5Cardputer.Display.drawString(content.substring(1),
                                   M5Cardputer.Display.width() / 2,
                                   M5Cardputer.Display.height() / 2);
  delay(3000); // Adiciona um atraso de 1 segundo antes de ler outro cartão
  M5Cardputer.Display.clear();
}