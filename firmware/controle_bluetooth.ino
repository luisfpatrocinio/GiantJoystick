/*
 ==============================================================================
   PROJETO: JOGANDO JUNTO - 6º ANO B (InovaCEV 2026)
   CONTROLE ACESSÍVEL DE VIDEOGAME DE 1 BOTÃO GIGANTE
   
   Orientação: Prof. Luis Felipe Patrocínio (patrodev.com)
   Placa: ESP32 DevKit V1
   Função: Teclado Bluetooth HID (Human Interface Device)
 ==============================================================================
 
 Olá, equipe do 6º Ano B! 👋
 Este código transforma o nosso microcontrolador ESP32 em um "teclado sem fio".
 Quando qualquer pessoa aperta o nosso botão gigante de papelão, o ESP32 manda
 o sinal da "BARRA DE ESPAÇO" para o computador ou tablet através do Bluetooth!
*/

// 1. IMPORTAÇÃO DAS BIBLIOTECAS
// A biblioteca BleKeyboard permite que o ESP32 finja ser um teclado Bluetooth real!
#include <BleKeyboard.h>

// 2. DEFINIÇÃO DOS PINOS DO CIRCUITO
// O pino onde ligamos a garra de jacaré que vai até o botão gigante:
const int PINO_BOTAO = 4;   // GPIO 4 (identificado como D4 na placa)

// O pino do LED azul embutido na própria placa do ESP32 (ótimo para testes!):
const int PINO_LED = 2;     // GPIO 2 (LED interno)

// 3. CONFIGURAÇÃO DO DISPOSITIVO BLUETOOTH
// Criamos o nosso teclado virtual com o nome que os jurados e jogadores verão no celular/PC:
// Nome visível: "Jogando Junto - 6B"
// Criador: "6o Ano B - InovaCEV"
// Bateria inicial simulada: 100%
BleKeyboard bleKeyboard("Jogando Junto - 6B", "6o Ano B - InovaCEV", 100);

// 4. VARIÁVEIS DE CONTROLE DO DEBOUNCE (FILTRO ANTI-TREPIDAÇÃO)
// Quando batemos com força no botão de papelão, os contatos metálicos vibram
// como um pula-pula por alguns milissegundos antes de encostarem firmemente.
// Se não filtrarmos isso, 1 único clique pareceria centenas de cliques seguidos!
int ultimoEstadoLido = HIGH;      // Guarda o que o sensor leu na última rodada
int estadoEstavelBotao = HIGH;    // Guarda o estado verdadeiro confirmado do botão
unsigned long tempoUltimaMudanca = 0;  // Marca o relógio da última vibração detectada
const unsigned long TEMPO_DEBOUNCE = 50; // Tempo de espera em milissegundos (50 ms)

// Guarda se o botão estava pressionado na última checagem para sabermos quando soltou
bool botaoEstavaPressionado = false;

void setup() {
  // Inicializa a comunicação serial com o computador para vermos mensagens na tela preta (Monitor Serial)
  Serial.begin(115200);
  Serial.println("\n==========================================");
  Serial.println("  INICIANDO: PROJETO JOGANDO JUNTO (6B)   ");
  Serial.println("==========================================");

  // CONFIGURAÇÃO DO PINO DO LED:
  // Definimos como OUTPUT (Saída) pois o ESP32 enviará energia para acender o LED.
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW); // Começa desligado

  // CONFIGURAÇÃO DO PINO DO BOTÃO COM PULL-UP INTERNO:
  // O modo INPUT_PULLUP liga um resistor interno do ESP32 aos 3.3 Volts.
  // Isso significa que, enquanto o botão estiver SOLTO, a leitura será HIGH (1).
  // Quando a pessoa APERTA o botão, ele conecta com o pino GND (0V) e a leitura vira LOW (0).
  // Mágico: Não precisamos de nenhum resistor físico extra no circuito!
  pinMode(PINO_BOTAO, INPUT_PULLUP);

  // Inicializa o módulo Bluetooth do ESP32 e começa a anunciar o sinal pelo ar:
  Serial.println("[BLUETOOTH] Ligando sinal sem fio...");
  bleKeyboard.begin();
  Serial.println("[PRONTO] Procure por 'Jogando Junto - 6B' no Bluetooth do computador!");
}

void loop() {
  // Lê o sinal elétrico do botão agora (HIGH = Solto / LOW = Apertado)
  int leituraAtual = digitalRead(PINO_BOTAO);

  // SE HOUVE UMA MUDANÇA (alguém encostou ou soltou o botão):
  // Reiniciamos o cronômetro do filtro de debounce
  if (leituraAtual != ultimoEstadoLido) {
    tempoUltimaMudanca = millis(); // Guarda o momento atual em milissegundos
  }

  // Se o sinal ficou estável por mais tempo do que a nossa margem de segurança (50 ms):
  if ((millis() - tempoUltimaMudanca) > TEMPO_DEBOUNCE) {
    
    // Se o estado que confirmamos agora é diferente do estado que tínhamos antes:
    if (leituraAtual != estadoEstavelBotao) {
      estadoEstavelBotao = leituraAtual;

      // CASO 1: O BOTÃO FOI APERTADO (Conectou ao GND -> Estado LOW)
      if (estadoEstavelBotao == LOW) {
        Serial.println(">> [BOTAO] APERTADO! <<");

        // 1. Acende o LED azul do ESP32 para dar feedback visual imediato
        digitalWrite(PINO_LED, HIGH);

        // 2. Se o Bluetooth estiver emparelhado com o computador:
        if (bleKeyboard.isConnected()) {
          // Pressiona e segura a Barra de Espaço
          // Usamos ' ' (caractere de espaço ASCII) para acionar o salto ou ação no jogo
          bleKeyboard.press(' ');
          Serial.println("[BLE] Tecla ESPAÇO enviada ao jogo!");
        } else {
          Serial.println("[AVISO] Bluetooth ainda nao conectado ao computador.");
        }
      } 
      // CASO 2: O BOTÃO FOI SOLTO (Desconectou do GND -> Volta para HIGH pelo Pull-Up)
      else {
        Serial.println(">> [BOTAO] SOLTO <<");

        // 1. Apaga o LED azul
        digitalWrite(PINO_LED, LOW);

        // 2. Solta a tecla no computador caso estivesse conectado
        if (bleKeyboard.isConnected()) {
          bleKeyboard.release(' '); // Libera a barra de espaço
          Serial.println("[BLE] Tecla ESPAÇO liberada.");
        }
      }
    }
  }

  // Guarda o último estado lido para comparar na próxima rodada do loop
  ultimoEstadoLido = leituraAtual;

  // Um pequeno intervalo de 5 milissegundos para manter o processador descansado
  delay(5);
}
