#include "can.h"
int send_index = 0; 
// Variáveis globais para controlar o estado da leitura
int msg_size = 0;
int readIndex = 0;
can_msg receivedMsg;

void canInit(int crx, int ctx, long baud)
{
    // Set the pins
  CAN.setPins (crx, ctx);

  // start the CAN bus at 500 kbps
  if (!CAN.begin (baud)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }
}

void canSend(int id, const uint8_t *buffer, size_t size, int dlc, bool rtr)
{
    // Verifica se todos os bytes já foram enviados
    if (send_index >= size) {
        // Reset do índice para permitir reutilização da função
        send_index = 0;
        return;  // Sai da função se todos os bytes já foram enviados
    }

    // Começa um novo pacote
    CAN.beginPacket(id);
    // Envia o byte atual
    CAN.write(buffer[send_index]);
    // Termina o pacote
    CAN.endPacket();

    // Incrementa o índice para apontar para o próximo byte a ser enviado
    send_index++;
}


can_msg canReceive() {
    // Se não houver mais dados disponíveis, reinicializa o estado da leitura
    if (readIndex >= msg_size) {
        msg_size = CAN.parsePacket();
        readIndex = 0;
        receivedMsg.id = 0;
        receivedMsg.size = 0;
    }

    // Se houver um pacote disponível para leitura
    if (msg_size) {
        // Se for o primeiro byte a ser lido, atualiza o ID e o tamanho da mensagem
        if (readIndex == 0) {
            receivedMsg.id = CAN.packetId();
            receivedMsg.size = msg_size;
        }

        // Lê o próximo byte do pacote
        if (CAN.available()) {
            receivedMsg.rxData[readIndex++] = CAN.read();
        }

        // Se todos os bytes foram lidos, retorna a mensagem
        if (readIndex >= msg_size) {
            return receivedMsg;
        }
    }

    // Se não houver pacote disponível ou se a leitura ainda não estiver completa, retorna uma mensagem vazia
    return receivedMsg;
}