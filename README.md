# 🕹️ Projeto: Jogando Junto - 6º Ano B (InovaCEV)

> **Controle de Videogame Adaptado e Acessível de 1 Botão Gigante**  
> *Inclusão nos eSports e Acessibilidade Motora*  
> **Orientação:** Prof. Luis Felipe Patrocínio ([patrodev.com](https://patrodev.com))  
> **Feira de Robótica:** InovaCEV — 13 de Novembro  
> 🌐 **Apresentação 3D Online (GitHub Pages):** [luisfpatrocinio.github.io/GiantJoystick](https://luisfpatrocinio.github.io/GiantJoystick/)

---

## 📁 Estrutura do Repositório

```text
GiantJoystick/
├── platformio.ini               # Configuração do PlatformIO (VS Code)
├── ponte_teclado_usb.py         # Script Python para transformar USB Serial em tecla real do PC
├── src/
│   └── main.cpp                 # Firmware C++ USB Serial & Plug-and-Play
├── firmware/
│   ├── controle_usb.ino         # Código alternativo USB Serial para Arduino IDE
│   └── controle_bluetooth.ino   # Código alternativo Bluetooth BLE
├── web/
│   └── index.html               # Simulador 3D com Web Serial API (Plug-and-Play)
└── README.md                    # Guia Rápido de Sobrevivência para o Crunch Time
```

---

## ⚡ MODO RÁPIDO: Conexão Direta via Cabo USB (Sem dores de cabeça com Bluetooth!)

Se o Bluetooth do computador ou da placa estiver instável no estande, **use a conexão direta via cabo USB**:

### Opção 1: Direto na Página Web 3D (Recomendado!)
1. Grave o código no ESP32 usando o PlatformIO ou Arduino IDE.
2. Abra a página `web/index.html` no Google Chrome ou Microsoft Edge (ou acesse [luisfpatrocinio.github.io/GiantJoystick](https://luisfpatrocinio.github.io/GiantJoystick/)).
3. Clique no botão verde: **`🔌 Conectar Controle USB (Cabo)`**.
4. Selecione a porta do seu ESP32 (ex: `COM3` ou `COM4`).
5. **Pronto!** A página lerá o botão físico diretamente pelo cabo USB com latência zero e sem precisar parear nada!

### Opção 2: Jogar Qualquer Jogo no PC (ex: Dino do Chrome)
Se quiser jogar jogos fora da nossa página (como o jogo do Dinossauro em `chrome://dino`):
1. Instale as bibliotecas Python (se ainda não tiver):
   ```bash
   pip install pyserial pyautogui
   ```
2. Execute o script ponte:
   ```bash
   python ponte_teclado_usb.py
   ```
3. O script detecta a porta USB do ESP32 automaticamente e pressiona a **Barra de Espaço** real do teclado do Windows/Mac a cada batida no botão!

---

## 🚀 PARTE 1: Compilação com PlatformIO (VS Code)

Usando o **PlatformIO** no VS Code, você **não precisa baixar nenhum arquivo ZIP nem configurar gerenciador de placas manualmente**. O arquivo `platformio.ini` já faz tudo de forma 100% automatizada!

### 1.1 Como Abrir e Gravar no ESP32:
1. Abra o **VS Code** com a extensão **PlatformIO IDE** instalada.
2. Vá em **`File` ➔ `Open Folder...`** e selecione a pasta raiz deste repositório (`GiantJoystick`).
3. O PlatformIO detectará o `platformio.ini` e fará o download automático:
   - Do core estável da Espressif (`espressif32@6.5.0`, baseado no Arduino 2.0.14).
   - Da biblioteca `ESP32-BLE-Keyboard` direto do GitHub oficial.
4. Conecte o cabo USB do ESP32 ao computador.
5. Na barra inferior azul do VS Code, clique no ícone de **seta para a direita (`➔ PlatformIO: Upload`)** ou use o atalho `Ctrl + Alt + U`.
6. Para abrir o monitor serial e ver as batidas no botão em tempo real, clique no ícone de tomada (`PlatformIO: Serial Monitor`).

---

## ⚡ PARTE 1.2: Alternativa via Arduino IDE

A biblioteca `ESP32-BLE-Keyboard` não está disponível no catálogo de pesquisa rápida da Arduino IDE e deve ser instalada manualmente via arquivo ZIP.

### 1.1 Baixando e Instalando a Biblioteca

1. Acesse o repositório oficial da biblioteca:  
   👉 **[GitHub - T-vK/ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)**
2. Clique no botão verde **`Code`** ➔ **`Download ZIP`** (salvará o arquivo `ESP32-BLE-Keyboard-master.zip`).
3. Abra a **Arduino IDE**.
4. No menu superior, vá em:  
   **`Sketch` ➔ `Incluir Biblioteca` ➔ `Adicionar biblioteca .ZIP...`**  
   *(No macOS: `Sketch` ➔ `Include Library` ➔ `Add .ZIP Library...`)*
5. Selecione o arquivo `.zip` baixado e confirme.

---

### 1.2 🚨 ALERTA CRÍTICO DE COMPILAÇÃO: Versão do Pacote ESP32

> [!WARNING]  
> **A versão 3.x.x da Espressif causa erro de compilação com a biblioteca BLE clássica!**  
> Na versão 3.0+ do core do ESP32 para Arduino (que migrou para o ESP-IDF v5), várias funções da biblioteca BLE original do T-vK foram descontinuadas.

#### Como configurar a versão estável e compatível:
1. Na Arduino IDE, abra o **Gerenciador de Placas** (ícone de placa na barra lateral esquerda ou `Ferramentas` ➔ `Placa` ➔ `Gerenciador de Placas`).
2. Pesquise por **`esp32`** (feito pela *Espressif Systems*).
3. Se estiver instalado na versão `3.0.x` ou superior, clique no menu suspenso de versões e selecione a **versão `2.0.14`** (ou `2.0.17`).
4. Clique em **Instalar / Fazer Downgrade**.
5. Aguarde o download terminar. Pronto! O código compilará de primeira sem nenhum erro.

#### Configurações da Placa na Arduino IDE:
* **Placa:** `DOIT ESP32 DEVKIT V1` (ou `ESP32 Dev Module`)
* **Upload Speed:** `921600` (ou `115200`)
* **Flash Frequency:** `80MHz`
* **Porta:** Selecione a porta COM correspondente ao seu cabo USB conectado.

---

## 🔌 PARTE 2: Diagrama de Ligação Física

Você só precisa de **2 Fios com Garra Jacaré**:

```text
 ┌──────────────────────┐                     ┌──────────────────────────┐
 │   ESP32 DevKit V1    │                     │  Botão Mecânico Gigante  │
 │                      │    Fio Amarelo      │                          │
 │             [GPIO 4] ├─────────────────────┤ [Terminal 1]             │
 │                      │   (Garra Jacaré)    │                          │
 │                      │                     │      [ 🔴 PRESS ]        │
 │                      │     Fio Preto       │                          │
 │              [GND]   ├─────────────────────┤ [Terminal 2]             │
 └──────────────────────┘   (Garra Jacaré)    └──────────────────────────┘
```

> **Por que não precisa de resistor externo?**  
> Usamos a instrução `pinMode(PINO_BOTAO, INPUT_PULLUP)`. O ESP32 ativa um resistor interno de 45 kΩ ligado aos 3.3V. Quando as placas do botão de papelão se tocam, o sinal vai para o `GND` (nível `LOW`), acionando o clique com total segurança!

---

## 💻 PARTE 3: Como Parear no Dia da Feira (13/11)

### No Windows 10 / 11:
1. Ligue o ESP32 conectando-o a uma porta USB do notebook ou a um Power Bank.
2. Abra as **Configurações do Windows** (`Win + I`) ➔ **Dispositivos** (ou **Bluetooth e Dispositivos**).
3. Ative o Bluetooth e clique em **"Adicionar dispositivo"** ➔ **"Bluetooth"**.
4. Procure na lista por:  
   👉 **`Jogando Junto - 6B`**
5. Clique nele para conectar. Uma notificação confirmará: *"O teclado está pronto para uso!"*.
6. Abra o Bloco de Notas ou o navegador e aperte o botão gigante: o cursor dará saltos de espaço!

### No macOS (MacBook):
1. Ligue o ESP32.
2. Vá em **Ajustes do Sistema** (System Settings) ➔ **Bluetooth**.
3. Na seção "Dispositivos Próximos", localize **`Jogando Junto - 6B`**.
4. Clique em **Conectar**.
5. Se o assistente de identificação de teclado abrir, basta fechá-lo (pois é um controle de 1 botão só).

---

## 🌐 PARTE 4: Como Rodar a Apresentação 3D no Estande

1. Vá até a pasta `web/` e dê dois cliques no arquivo `index.html`.
2. Ele abrirá instantaneamente em tela cheia no Google Chrome, Edge, Safari ou Firefox.
3. Não requer servidor Node.js ou instalação prévia: todas as bibliotecas (Tailwind CSS e Three.js) são carregadas automaticamente via CDN.
4. Quando os jurados baterem no botão de papelão físico, **o botão 3D na tela afundará com animação de mola, emitirá um som de arcade retrô e disparará o brilho verde de "CLIQUE DETECTADO!"**.

---

## 🎮 PARTE 5: Jogos Acessíveis de 1 Botão Recomendados para o Estande

Deixe uma aba aberta no estande com um desses jogos para os jurados jogarem usando o botão gigante:
1. **Dino do Google Chrome:** Digite `chrome://dino` na barra de endereços (o clássico t-rex que pula com a tecla espaço!).
2. **Canabalt:** O lendário jogo de corrida infinita de 1 único botão de salto ([canabalt.com](https://canabalt.com)).
3. **Flappy Bird Web:** O pássaro que voa a cada toque no espaço.
4. **Cat Bounce ou OneSwitch.org.uk:** Jogos desenvolvidos especificamente para acessibilidade motora e reabilitação.
