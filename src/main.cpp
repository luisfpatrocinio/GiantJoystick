/*
 ==============================================================================
   PROJETO: JOGANDO JUNTO - 6º ANO B (InovaCEV 2026)
   CONTROLE ACESSÍVEL DE 1 BOTÃO GIGANTE (Modo USB / Serial HID)
   
   Orientação: Prof. Luis Felipe Patrocínio (patrodev.com)
   Placa: ESP32 DevKit V1
   Função: Envia comandos de Teclado diretamente pelo CABO USB (Plug-and-Play)
 ==============================================================================
 
 Olá, equipe do 6º Ano B! 👋
 Para garantir 100% de estabilidade e zero atrasos na feira, este código envia
 a batida do botão gigante DIRETAMENTE PELO CABO USB conectado ao computador!
*/

#include <Arduino.h>

// 1. DEFINIÇÃO DOS PINOS DO CIRCUITO
const int PINO_BOTAO = 4;   // GPIO 4 (identificado como D4 na placa)
const int PINO_LED = 2;     // GPIO 2 (LED azul interno do ESP32)

// 2. VARIÁVEIS DE CONTROLE DO DEBOUNCE (FILTRO ANTI-PULA-PULA)
int ultimoEstadoLido = HIGH;             // O que o sensor leu na rodada anterior
int estadoEstavelBotao = HIGH;           // Estado confirmado (HIGH = Solto / LOW = Apertado)
unsigned long tempoUltimaMudanca = 0;    // Relógio que marca a última vibração
const unsigned long TEMPO_DEBOUNCE = 40; // Tempo de espera rápido: 40 milissegundos

void setup() {
  // Inicializa a comunicação USB Serial em alta velocidade (115200 bps)
  Serial.begin(115200);
  delay(500);

  // Mensagem inicial de inicialização USB
  Serial.println("\n==========================================");
  Serial.println("  JOGANDO JUNTO - 6B (MODO USB CABO)     ");
  Serial.println("  Pressione o botao para enviar ESPACO!   ");
  Serial.println("==========================================");

  // Configura o pino do LED azul como Saída
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW); // Começa apagado

  // Configura o botão com o resistor PULL-UP interno do ESP32:
  // Pino D4 fica em 3.3V (HIGH) enquanto o botão estiver solto.
  // Quando encosta no GND, vai para 0V (LOW). Zero resistores externos!
  pinMode(PINO_BOTAO, INPUT_PULLUP);
}

void loop() {
  // Lê o sinal elétrico do botão agora (HIGH = Solto / LOW = Apertado)
  int leituraAtual = digitalRead(PINO_BOTAO);

  // Se houve qualquer alteração ou tremor mecânico no papelão:
  if (leituraAtual != ultimoEstadoLido) {
    tempoUltimaMudanca = millis(); // Reinicia o cronômetro do filtro
  }

  // Se o sinal ficou estável por mais de 40ms:
  if ((millis() - tempoUltimaMudanca) > TEMPO_DEBOUNCE) {
    
    // Se o estado mudou de verdade:
    if (leituraAtual != estadoEstavelBotao) {
      estadoEstavelBotao = leituraAtual;

      // ========================================================
      // CASO A: O BOTÃO FOI APERTADO! (Circuito fechou em LOW)
      // ========================================================
      if (estadoEstavelBotao == LOW) {
        // 1. Acende o LED azul da placa para dar feedback visual
        digitalWrite(PINO_LED, HIGH);

        // 2. Envia os comandos pelo Cabo USB para o computador
        // O formato "SPACE:DOWN" e "PRESS" é reconhecido pela página web e scripts
        Serial.println("SPACE:DOWN");
        Serial.println("PRESS");
      } 
      // ========================================================
      // CASO B: O BOTÃO FOI SOLTO! (Voltou para HIGH)
      // ========================================================
      else {
        // 1. Apaga o LED azul
        digitalWrite(PINO_LED, LOW);

        // 2. Avisa pelo cabo USB que o botão foi liberado
        Serial.println("SPACE:UP");
        Serial.println("RELEASE");
      }
    }
  }

  // Guarda o último estado para comparar na próxima rodada
  ultimoEstadoLido = leituraAtual;

  // Descanso mínimo de 2 milissegundos
  delay(2);
}
