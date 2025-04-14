//----------------sensores IR digitais-------------
const int sensorIR1 = 6;  
const int sensorIR2 = 5; 

//------------------ULTRASONICO---------------------
const int trigPin = 7;
const int echoPin = 4;

//----------------MOTORES---------------------------
// Pinos de controle do motor A
const int IN1 = 10;  
const int IN2 = 9;
const int velocidadeA = 3; // PWM motor A

// Pinos de controle do motor B
const int IN3 = 12;
const int IN4 = 11;
const int velocidadeB = 8; // PWM motor B

//variável de velocidade geral
int velocidade = 150; // valor de 0 a 255

//----------FUNÇÕES MOVIMENTO MOTORES-----------------
void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  analogWrite(velocidadeA, 0);
  analogWrite(velocidadeB, 0);
}

void tras() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(velocidadeA, velocidade);
  analogWrite(velocidadeB, velocidade);
}

void frente() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(velocidadeA, velocidade);
  analogWrite(velocidadeB, velocidade);
}

void esquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(velocidadeA, velocidade);
  analogWrite(velocidadeB, velocidade);
}

void direita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(velocidadeA, velocidade);
  analogWrite(velocidadeB, velocidade);
}

//-----------ULTRASSONICO MEDIR DISTÂNCIA------------
float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);
  float distanciaCm = duracao * 0.034 / 2;

  return distanciaCm;
}

//----------------------------------------------------
void setup() {
  pinMode(sensorIR1, INPUT);
  pinMode(sensorIR2, INPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(velocidadeA, OUTPUT);
  pinMode(velocidadeB, OUTPUT);

  Serial.begin(9600);
}
void loop() {
  int leituraSensor1 = digitalRead(sensorIR1);
  int leituraSensor2 = digitalRead(sensorIR2);
  float distancia = medirDistancia();

  Serial.print("IR1: ");
  Serial.print(leituraSensor1);
  Serial.print(" IR2: ");
  Serial.print(leituraSensor2);
  Serial.print(" Distância: ");
  Serial.println(distancia);

  // ----- 1º PRIORIDADE: Não cair do ringue -----
  if (leituraSensor1 == HIGH && leituraSensor2 == HIGH) {
    tras();
    delay(500);
    esquerda();
    delay(500);
  }
  else if (leituraSensor1 == HIGH) {
    tras();
    delay(500);
    direita();
    delay(500);
  }
  else if (leituraSensor2 == HIGH) {
    tras();
    delay(500);
    esquerda();
    delay(500);
  }
  else {
    // ----- 2º PRIORIDADE: Atacar o oponente -----
    if (distancia > 0 && distancia <= 20) {
      velocidade = 200; // Aumenta velocidade para atacar
      frente();
    } 
    else {
      // ----- 3º PRIORIDADE: Procurar o oponente -----
      velocidade = 100; // Diminui velocidade para procurar
      direita();
      delay(300); // gira só um pouquinho
      parar();    // depois para
      delay(200); // pausa para estabilidade
    }
  }
}
