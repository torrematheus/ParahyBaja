//------------------ BIBLIOTECAS --------------
#include <SPI.h>     
#include <mcp_can.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//------------- DEFINIÇÃO DE PINOS ------------
const int CSPin = 10;
const int RPMPin = 2;
const int COMBPin = 3;

//------------------ VARIÁVEIS -----------------

    //RPM
      volatile int contadorRPM = 0;
      int RPM = 0;

    //TEMPO
      unsigned long  TempoAtual = 0, Tempo2RPM = 0, TempoAntigo = 0;

    //COMBUSTÍVEL
      bool COMB;
      uint8_t combMSG;    

    //VARIAVEIS USADAS NA MESNAGEM DA REDE CAN
      uint32_t Id_msg = 0x66D;                                         //ID DA MENSAGEM ENVIADA 


//--------------- CRIAÇÃO DE OBJETOS -------------

LiquidCrystal_I2C lcd(0x27, 16, 2);                                   //OBJETO DO DISPLAY LCD 16x2
MCP_CAN CAN(CSPin);                                                  // OBJETO CAN INICIALIZADO NO PINO CSPin

//----------- ENVIO DE MENSAGEM PELA REDE CAN ----------

void transmissao(float rpm, float comb) {
  
  uint8_t msg[4];                                                  //MENSAGEM A SER ENVIADA PELA CAN
  memcpy(msg, &rpm, sizeof(float));                               //CONVERTE O DADO DE RPM E ARMAZENA EM msg[0-3] (4 bytes)
  memcpy(msg[4], &comb, 1);                                      //CONVERTE O DADO DE COMBUSTIVEL E ARMAZENA EM msg[4] 
  CAN.sendMsgBuf(Id_msg, 0, 5, msg);                            //FUNÇÃO DA CAN QUE ENVIA A MENSAGEM - parâmetros (Id da msg, inicio da msg, tamanho da msg, msg)
}

//----------------- SETUP ----------------

void setup()
{
  
    lcd.init();                                                              //INICIALIZA O OBJETO LDC
    lcd.backlight();
  
  pinMode(COMBPin, INPUT);                                                  //DEFINE O PINO DO COMBUSTÍVEL COMO ENTRADA
  attachInterrupt(digitalPinToInterrupt(RPMPin), ContaRPM, RISING);        //DEFINE O PINO DO RPM COMO PINO DE INTERRUPÇÃO NA BORDA DE SUBIDA E ATIVA A FUNÇÃO ContaRPM

  while (CAN_OK != CAN.begin(CAN_500KBPS))                                //INICIALIZAÇÃO DA REDE CAN, 
                                                                          //ENQUANTO A REPOSTA DA INICIALIZAÇÃO NÃO FOR 'OK', NÃO SAI DO LOOP WHILE    
  {
    lcd.setCursor(2, 0);
    lcd.println("CAN BUS init Failed");
    delay(100);
  }
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.println("CAN BUS Shield Init OK!");
}

//------------------ LOOP -----------------

void loop()
{
  TempoAtual = millis();

  if (TempoAtual - Tempo2RPM > 2000)                                              //SE NÃO HOUVER ALTERAÇÃO NO RPM EM 2000 MILISSEGUNDOS, O RPM SERÁ 0
    RPM = 0;

  if (contadorRPM > 1) {
    noInterrupts();                                                                //DESABILITA AS INTERRUPÇÕES
    if (TempoAtual - Tempo2RPM > 11)                                              //IF (RPM<4000)
      RPM = int(60000.0 / float(TempoAtual - Tempo2RPM));                        //RPM = 1000(s)*60(min)/tempo(ms)
    Tempo2RPM = TempoAtual;
    contadorRPM = 0;
    interrupts();                                                                //HABILITA AS INTERRUPÇÕES NOVAMENTE     
    lcd.clear();
    lcd.setCursor(3,0);                                                           
    lcd.print("RPM: "); 
    delay(1); 
    lcd.setCursor(7,0);                                                            //EXIBE NO DISPLAY O VALOR DE RPM
    lcd.println(RPM);
    delay(1);
 
  }

  COMB = (bool)digitalRead(COMBPin);                                          //LÊ O VALOR DO PINO DE COMBUSTÍVEL (HIGH OU LOW)

  if (COMB) combMSG = 200;                                                    //DEPENDENDO DO ESTADO DO PINO, É ATRIBUIDO UM VALOR À MSG DE COMBUSTÍVEL
  else combMSG = 0;                                                           // HIGH = 200, LOW = 0
  transmissao(RPM, combMSG);                                                  //PASSA OS PARAMÊTROS PARA A FUNÇÃO DE ENVIO
  lcd.setCursor(3,1);
  lcd.print("COMB: ");
  delay(1);
  lcd.setCursor(8,1);
  lcd.print(combMSG);
  delay(1);

}

//-------------- FUNÇÃO DE CONTAGEM DO NÚMERO DE -----------
//------------- INTERRUPÇÕES PARA CÁLCULO DO RPM -----------
void ContaRPM() {
  noInterrupts();
  contadorRPM++;
  interrupts();
}
