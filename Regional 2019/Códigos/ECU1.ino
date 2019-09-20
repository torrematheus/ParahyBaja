#include <SoftwareSerial.h>
#include <SPI.h>
#include <mcp_can.h>

const int RPMPin = 2;        //Pino RPM                                                     
const int COMBPin = 4; 
const int CSPin = 10;     //Pino CS CAN

const int INICIO = 254;
const int FIM = 253;

volatile int contadorRPM=0;
unsigned long  TempoAtual=0, Tempo2RPM=0, TempoAntigo = 0; 
int RPM = 0;

// ID's usados na transmissão da CAN
uint32_t Id_msg = 0x66D;


MCP_CAN CAN(CSPin);

void transmissao(uint8_t comb, float RPM_Funcao){
// Função de transmissão pela CAN 
  uint8_t msg[5]; 
  
  memcpy(msg, &RPM_Funcao, sizeof(float));            //A função memcpy copia os dados do RPM e do Combustivel em msg
  memcpy(msg+4, &comb, 1);                          
  CAN.sendMsgBuf(Id_msg, 0, 5, msg);                //Envia o ID da mensagem, o inicio da mensagem no array, o tamanho da mensagem e a mensagem, respectivamente

}


void setup()
{
    Serial.begin(9600);       //Inicializa a serial

    pinMode(COMBPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(RPMPin), ContaRPM, RISING);                 //Definindo função de interrupção para rpm

    //Configura a Rede CAN
    while (CAN_OK != CAN.begin(CAN_500KBPS))  //Enquanto não receber uma mensagem de configuração 'OK' permanece chamando a função de configuração
    {
        Serial.println("CAN BUS init Failed");
    }
    Serial.println("CAN BUS Shield Init OK!");
}

    
void loop()
{   

 transmissao(0, 5000);
/* if(digitalRead(RPMPin)){
    COMB = 1;
}
  
   TempoAtual = millis();  
                                                     
  if(TempoAtual-Tempo2RPM>2000)                                                   //Se não houver alteração na velocidade em 2000 milisegundos, então RPM SERÁ 0
    RPM=0;


  if(contadorRPM>1){
    noInterrupts();
    if(TempoAtual-Tempo2RPM>11)                                                   //if(RPM<4000rpm)
    RPM = int(60000.0/float(TempoAtual-Tempo2RPM));                             //RPM = 1000(s)*60(min)/tempo(ms)
    Tempo2RPM = TempoAtual;
    contadorRPM=0;
    interrupts();
  }

  
  if(TempoAtual-TempoAntigo>200)
  {
  noInterrupts();
  transmissao(Velocidade, RPM);
  interrupts();
  TempoAntigo = TempoAtual;
  }*/


}

void ContaRPM(){
  noInterrupts();
  contadorRPM++;
  interrupts();
 }
