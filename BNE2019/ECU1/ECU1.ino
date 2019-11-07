#include <SPI.h>
#include <mcp_can.h>

const int CSPin = 10;
const int RPMPin = 2;                                                      
const int COMBPin = 3; 

volatile int contadorRPM=0;
unsigned long  TempoAtual=0, Tempo2RPM=0, TempoAntigo = 0; 
float RPM = 0;
int indice;
bool COMB;

uint32_t Id_msg = 0x66D;

MCP_CAN CAN(CSPin);

void transmissao(float rpm, bool comb){

  uint8_t msg[5]; 
  memcpy(msg, &rpm, sizeof(float));
  memcpy(msg, &comb, 1);
  CAN.sendMsgBuf(Id_msg, 0, 5, msg);
  
}

void setup()
{
    Serial.begin(115200);

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
  
 if((digitalRead(COMBPin))==HIGH)
  {
    COMB = true;
  }
  if((digitalRead(COMBPin))==LOW)
  {
    COMB = false;  
  }
                                                     
  if(TempoAtual-Tempo2RPM>2000)                                                   //Se não houver alteração no RPM em 2000 milisegundos, então RPM SERÁ 0
    RPM=0;

  if(contadorRPM>1){
    noInterrupts();
    if(TempoAtual-Tempo2RPM>11)                                                   //if(RPM<4000rpm)
    RPM = int(60000.0/float(TempoAtual-Tempo2RPM));                             //RPM = 1000(s)*60(min)/tempo(ms)
    Tempo2RPM = TempoAtual;
    contadorRPM=0;
    interrupts();
  }

transmissao(2000, 0);

}

void ContaRPM(){
  noInterrupts();
  contadorRPM++;
  interrupts();
 }
