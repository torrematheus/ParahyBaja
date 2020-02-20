#include <SPI.h>          //      Bibliotecas de comunicação        //
#include <mcp_can.h>      //               da CAN                   //

const int CSPin = 10;
const int RPMPin = 2;
const int COMBPin = 3;

volatile int contadorRPM = 0;
unsigned long  TempoAtual = 0, Tempo2RPM = 0, TempoAntigo = 0;
int RPM = 0;
int indice, cont = 0;
bool COMB;
uint8_t combMSG;

uint32_t Id_msg = 0x66D;

uint8_t msg[4];

MCP_CAN CAN(CSPin);      // Objeto CAN com parâmetro CSPin

void transmissao(float rpm, float comb) { //função de construção da msg - parâmetros (dados da msg)

  //  uint8_t msg[5];
  memcpy(msg, &rpm, sizeof(float));     //função para converter valor da variável em char
  memcpy(msg[4], &comb, 1);
  CAN.sendMsgBuf(Id_msg, 0, 5, msg);    //função de envio da msg na CAN - parâmetros (Id da msg, inicio da msg, tamanho da msg, msg)

}

void setup()
{
  Serial.begin(9600);

  pinMode(COMBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(RPMPin), ContaRPM, RISING);

  while (CAN_OK != CAN.begin(CAN_500KBPS))
  {
    Serial.println("CAN BUS init Failed");
    delay(100);
  }
  Serial.println("CAN BUS Shield Init OK!");
}
void loop()
{

  TempoAtual = millis();


  if (TempoAtual - Tempo2RPM > 2000)                                              //Se não houver alteração no RPM em 2000 milisegundos, então RPM SERÁ 0
    RPM = 0;

  if (contadorRPM > 1) {
    noInterrupts();
    if (TempoAtual - Tempo2RPM > 11)                                              //if(RPM<4000rpm)
      RPM = int(60000.0 / float(TempoAtual - Tempo2RPM));                         //RPM = 1000(s)*60(min)/tempo(ms)
    Tempo2RPM = TempoAtual;
    contadorRPM = 0;
    interrupts();
    Serial.println("RPM: ");
    Serial.println(RPM);
 
  }

  COMB = (bool)digitalRead(COMBPin);

  if (COMB) combMSG = 200;
  else combMSG = 0;
  transmissao(RPM, combMSG);
  //Serial.println("comb");
  //Serial.println(COMB);
}

void ContaRPM() {
  noInterrupts();
  contadorRPM++;
  interrupts();
}
