/*
 ==============================================================================
   PROJETO: JOGANDO JUNTO - 6º ANO B (InovaCEV 2026)
   CONTROLE ACESSÍVEL DE 1 BOTÃO GIGANTE (Modo USB / Serial HID)
   
   Orientação: Prof. Luis Felipe Patrocínio (patrodev.com)
   Placa: ESP32 DevKit V1
   Função: Envia comandos de Teclado diretamente pelo CABO USB (Plug-and-Play)
 ==============================================================================
*/

const int PINO_BOTAO = 4;   // GPIO 4 (D4)
const int PINO_LED = 2;     // GPIO 2 (LED azul)

int ultimoEstadoLido = HIGH;
int estadoEstavelBotao = HIGH;
unsigned long tempoUltimaMudanca = 0;
const unsigned long TEMPO_DEBOUNCE = 40;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n==========================================");
  Serial.println("  JOGANDO JUNTO - 6B (MODO USB CABO)     ");
  Serial.println("==========================================");

  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);

  pinMode(PINO_BOTAO, INPUT_PULLUP);
}

void loop() {
  int leituraAtual = digitalRead(PINO_BOTAO);

  if (leituraAtual != ultimoEstadoLido) {
    tempoUltimaMudanca = millis();
  }

  if ((millis() - tempoUltimaMudanca) > TEMPO_DEBOUNCE) {
    if (leituraAtual != estadoEstavelBotao) {
      estadoEstavelBotao = leituraAtual;

      if (estadoEstavelBotao == LOW) {
        digitalWrite(PINO_LED, HIGH);
        Serial.println("SPACE:DOWN");
        Serial.println("PRESS");
      } else {
        digitalWrite(PINO_LED, LOW);
        Serial.println("SPACE:UP");
        Serial.println("RELEASE");
      }
    }
  }

  ultimoEstadoLido = leituraAtual;
  delay(2);
}
