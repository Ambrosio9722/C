#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128   // Largura do display OLED
#define SCREEN_HEIGHT 32   // Altura do display OLED

#define OLED_RESET -1      // Pino de reset (usado por alguns módulos)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define umidadeAnalogica A0 // Atribui o pino A0 à variável umidade - leitura analógica do sensor
#define umidadeDigital 7    // Atribui o pino 7 à variável umidadeDigital - leitura digital do sensor

int IN3 = 6;                // Pino de controle do motor (mini bomba)
int valorUmidade;           // Armazena o valor da umidade lida - saída analógica
int valorUmidadeDigital;    // Armazena a saída digital do sensor de umidade do solo

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial
  pinMode(umidadeAnalogica, INPUT); // Define o pino umidadeAnalogica como entrada
  pinMode(umidadeDigital, INPUT);   // Define o pino umidadeDigital como entrada
  pinMode(IN3, OUTPUT);             // Define o pino do motor como saída

  // Inicializa o display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Endereço padrão do display OLED 128x32
    Serial.println(F("Falha ao inicializar o display OLED"));
    for (;;);
  }
  
  // Limpa o buffer do display
  display.clearDisplay();
  display.setTextSize(1);      // Tamanho do texto
  display.setTextColor(WHITE); // Cor do texto
}

void loop() {
  valorUmidade = analogRead(umidadeAnalogica); // Lê o valor analógico do sensor e armazena em valorUmidade
  valorUmidade = map(valorUmidade, 1023, 315, 0, 100); // Transforma o valor analógico em uma escala de 0 a 100%

  Serial.print("Umidade encontrada: ");
  Serial.print(valorUmidade); 
  Serial.println(" %");

  valorUmidadeDigital = digitalRead(umidadeDigital); // Lê o valor digital do sensor de umidade

  // Atualiza o display OLED com a porcentagem de umidade
  display.clearDisplay();
  display.setCursor(0, 0); // Define a posição do cursor no display
  display.print("Umidade: ");
  display.print(valorUmidade); 
  display.println(" %");

  // Verifica a condição da umidade e exibe no display
  if (valorUmidade <= 30) {
    Serial.println("Status: Solo seco - Ligando a bomba de água.");
    digitalWrite(IN3, HIGH); // Liga a bomba de água (motor)
    display.println("Status: Seco");
  } else {
    Serial.println("Status: Solo úmido - Desligando a bomba de água.");
    digitalWrite(IN3, LOW); // Desliga a bomba de água (motor)
    display.println("Status: Umido");
  }
  
  // Exibe as mudanças no display
  display.display();

  delay(500); // Atraso de 500ms
}
