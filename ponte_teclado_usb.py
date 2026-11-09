"""
==============================================================================
  PROJETO: JOGANDO JUNTO - 6º ANO B (InovaCEV 2026)
  PONTE USB -> TECLADO FÍSICO DO WINDOWS / MAC / LINUX
  
  Este script le a porta USB do ESP32 e converte as mensagens recebidas
  em toques REAIS da tecla ESPACO no computador para qualquer jogo (ex: Dino do Chrome)!
==============================================================================

Como usar:
1. Instale as bibliotecas (se nao tiver):
   pip install pyserial pyautogui

2. Execute o script:
   python ponte_teclado_usb.py
"""

import sys
import time
import serial
import serial.tools.list_ports
import pyautogui

# Desativa o delay padrao do pyautogui para maxima velocidade nos games
pyautogui.PAUSE = 0.0

def encontrar_porta_esp32():
    portas = list(serial.tools.list_ports.comports())
    for p in portas:
        descricao = p.description.lower()
        if "cp210" in descricao or "ch340" in descricao or "usb" in descricao or "serial" in descricao:
            return p.device
    if portas:
        return portas[0].device
    return None

def main():
    print("=" * 60)
    print("  JOGANDO JUNTO (6B) - PONTE USB -> TECLADO FISICO")
    print("=" * 60)

    porta = encontrar_porta_esp32()
    if not porta:
        print("[ERRO] Nenhuma porta COM do ESP32 encontrada. Conecte o cabo USB!")
        input("\nPressione ENTER para sair...")
        sys.exit(1)

    print(f"[OK] Conectando ao ESP32 na porta: {porta} (115200 bps)...")
    
    try:
        ser = serial.Serial(porta, 115200, timeout=0.1)
        time.sleep(1.5) # Aguarda inicializacao da placa
        print("[SUCESSO] Conectado com sucesso!")
        print(">> Abra o jogo (ex: chrome://dino) e bata no botao gigante! <<\n")
    except Exception as e:
        print(f"[ERRO] Nao foi possivel abrir a porta {porta}: {e}")
        input("\nPressione ENTER para sair...")
        sys.exit(1)

    botao_pressionado = False

    try:
        while True:
            if ser.in_waiting > 0:
                linha = ser.readline().decode('utf-8', errors='ignore').strip()
                
                # Detecta aperto
                if ("SPACE:DOWN" in linha or "PRESS" in linha) and not botao_pressionado:
                    botao_pressionado = True
                    pyautogui.keyDown('space')
                    print(">> [BOTAO] APERTADO! -> Tecla ESPACO pressionada no PC!")

                # Detecta soltura
                elif ("SPACE:UP" in linha or "RELEASE" in linha) and botao_pressionado:
                    botao_pressionado = False
                    pyautogui.keyUp('space')
                    print(">> [BOTAO] SOLTO -> Tecla ESPACO liberada!")

            time.sleep(0.002)

    except KeyboardInterrupt:
        print("\n[ENCERRADO] Ponte finalizada pelo usuario.")
        if botao_pressionado:
            pyautogui.keyUp('space')
        ser.close()

if __name__ == "__main__":
    main()
