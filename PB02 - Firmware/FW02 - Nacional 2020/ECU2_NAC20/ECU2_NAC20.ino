
//--------------------------- Bibliotecas -------------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mcp_can.h>
#include <SoftwareSerial.h>

//----------------------- CONFIGURAÇÃO DO DISPLAY ---------------------

//CRIA OS CARACTERES DO DISPLAY

const char custom[][8] PROGMEM = {                        // Custom character definitions
      { 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00 }, // char 1 
      { 0x18, 0x1C, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }, // char 2 
      { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x07, 0x03 }, // char 3 
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F }, // char 4 
      { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1E, 0x1C, 0x18 }, // char 5 
      { 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F }, // char 6 
      { 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F }, // char 7 
      { 0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }  // char 8 
};

// BIG FONT Character Set
// - Each character coded as 1-4 byte sets {top_row(0), top_row(1)... bot_row(0), bot_row(0)..."}
// - number of bytes terminated with 0x00; Character is made from [number_of_bytes/2] wide, 2 high
// - codes are: 0x01-0x08 => custom characters, 0x20 => space, 0xFF => black square

const char bigChars[][8] PROGMEM = {
      { 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Space
      { 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // !
      { 0x05, 0x05, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 }, // "
      { 0x04, 0xFF, 0x04, 0xFF, 0x04, 0x01, 0xFF, 0x01 }, // #
      { 0x08, 0xFF, 0x06, 0x07, 0xFF, 0x05, 0x00, 0x00 }, // $
      { 0x01, 0x20, 0x04, 0x01, 0x04, 0x01, 0x20, 0x04 }, // %
      { 0x08, 0x06, 0x02, 0x20, 0x03, 0x07, 0x02, 0x04 }, // &
      { 0x05, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '
      { 0x08, 0x01, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00 }, // (
      { 0x01, 0x02, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00 }, // )
      { 0x01, 0x04, 0x04, 0x01, 0x04, 0x01, 0x01, 0x04 }, // *
      { 0x04, 0xFF, 0x04, 0x01, 0xFF, 0x01, 0x00, 0x00 }, // +
      { 0x20, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 
      { 0x04, 0x04, 0x04, 0x20, 0x20, 0x20, 0x00, 0x00 }, // -
      { 0x20, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // .
      { 0x20, 0x20, 0x04, 0x01, 0x04, 0x01, 0x20, 0x20 }, // /
      { 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00 }, // 0
      { 0x01, 0x02, 0x20, 0x04, 0xFF, 0x04, 0x00, 0x00 }, // 1
      { 0x06, 0x06, 0x02, 0xFF, 0x07, 0x07, 0x00, 0x00 }, // 2
      { 0x01, 0x06, 0x02, 0x04, 0x07, 0x05, 0x00, 0x00 }, // 3
      { 0x03, 0x04, 0xFF, 0x20, 0x20, 0xFF, 0x00, 0x00 }, // 4
      { 0xFF, 0x06, 0x06, 0x07, 0x07, 0x05, 0x00, 0x00 }, // 5
      { 0x08, 0x06, 0x06, 0x03, 0x07, 0x05, 0x00, 0x00 }, // 6
      { 0x01, 0x01, 0x02, 0x20, 0x08, 0x20, 0x00, 0x00 }, // 7
      { 0x08, 0x06, 0x02, 0x03, 0x07, 0x05, 0x00, 0x00 }, // 8
      { 0x08, 0x06, 0x02, 0x07, 0x07, 0x05, 0x00, 0x00 }, // 9
      { 0xA5, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // :
      { 0x04, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ;
      { 0x20, 0x04, 0x01, 0x01, 0x01, 0x04, 0x00, 0x00 }, // <
      { 0x04, 0x04, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00 }, // =
      { 0x01, 0x04, 0x20, 0x04, 0x01, 0x01, 0x00, 0x00 }, // >
      { 0x01, 0x06, 0x02, 0x20, 0x07, 0x20, 0x00, 0x00 }, // ?
      { 0x08, 0x06, 0x02, 0x03, 0x04, 0x04, 0x00, 0x00 }, // @
      { 0x08, 0x06, 0x02, 0xFF, 0x20, 0xFF, 0x00, 0x00 }, // A
      { 0xFF, 0x06, 0x05, 0xFF, 0x07, 0x02, 0x00, 0x00 }, // B
      { 0x08, 0x01, 0x01, 0x03, 0x04, 0x04, 0x00, 0x00 }, // C
      { 0xFF, 0x01, 0x02, 0xFF, 0x04, 0x05, 0x00, 0x00 }, // D
      { 0xFF, 0x06, 0x06, 0xFF, 0x07, 0x07, 0x00, 0x00 }, // E
      { 0xFF, 0x06, 0x06, 0xFF, 0x20, 0x20, 0x00, 0x00 }, // F
      { 0x08, 0x01, 0x01, 0x03, 0x04, 0x02, 0x00, 0x00 }, // G
      { 0xFF, 0x04, 0xFF, 0xFF, 0x20, 0xFF, 0x00, 0x00 }, // H
      { 0x01, 0xFF, 0x01, 0x04, 0xFF, 0x04, 0x00, 0x00 }, // I
      { 0x20, 0x20, 0xFF, 0x04, 0x04, 0x05, 0x00, 0x00 }, // J
      { 0xFF, 0x04, 0x05, 0xFF, 0x20, 0x02, 0x00, 0x00 }, // K
      { 0xFF, 0x20, 0x20, 0xFF, 0x04, 0x04, 0x00, 0x00 }, // L
      { 0x08, 0x03, 0x05, 0x02, 0xFF, 0x20, 0x20, 0xFF }, // M
      { 0xFF, 0x02, 0x20, 0xFF, 0xFF, 0x20, 0x03, 0xFF }, // N
      { 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00 }, // 0
      { 0x08, 0x06, 0x02, 0xFF, 0x20, 0x20, 0x00, 0x00 }, // P
      { 0x08, 0x01, 0x02, 0x20, 0x03, 0x04, 0xFF, 0x04 }, // Q
      { 0xFF, 0x06, 0x02, 0xFF, 0x20, 0x02, 0x00, 0x00 }, // R
      { 0x08, 0x06, 0x06, 0x07, 0x07, 0x05, 0x00, 0x00 }, // S
      { 0x01, 0xFF, 0x01, 0x20, 0xFF, 0x20, 0x00, 0x00 }, // T
      { 0xFF, 0x20, 0xFF, 0x03, 0x04, 0x05, 0x00, 0x00 }, // U
      { 0x03, 0x20, 0x20, 0x05, 0x20, 0x02, 0x08, 0x20 }, // V
      { 0xFF, 0x20, 0x20, 0xFF, 0x03, 0x08, 0x02, 0x05 }, // W
      { 0x03, 0x04, 0x05, 0x08, 0x20, 0x02, 0x00, 0x00 }, // X
      { 0x03, 0x04, 0x05, 0x20, 0xFF, 0x20, 0x00, 0x00 }, // Y
      { 0x01, 0x06, 0x05, 0x08, 0x07, 0x04, 0x00, 0x00 }, // Z
      { 0xFF, 0x01, 0xFF, 0x04, 0x00, 0x00, 0x00, 0x00 }, // [
      { 0x01, 0x04, 0x20, 0x20, 0x20, 0x20, 0x01, 0x04 }, // Backslash
      { 0x01, 0xFF, 0x04, 0xFF, 0x00, 0x00, 0x00, 0x00 }, // ]
      { 0x08, 0x02, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 }, // ^
      { 0x20, 0x20, 0x20, 0x04, 0x04, 0x04, 0x00, 0x00 }  // _
};
byte col,row,nb=0,bc=0;                                   // general
byte bb[8];                                               // byte buffer for reading from PROGMEM


//----------------------- VARIAVEIS ----------------------------

//          PINOS
      const int LEDPin = 9;
      const int CSPin = 10;
      const int VELPin1 = 2;
      const int VELPin2 = 3;
      const int qntPinVel = 1;
      const int buttonPin = 4;

//          RPM
      float RPM = 0;
      int indiceRPM = 0;
      
//          COMB
      int COMB = 4;
      uint8_t comb;

//          REDE CAN
      uint8_t buf[8];
      unsigned char len = 0;
          
//          TEMPO
   unsigned long TempoVel1 = 0, TempoVel2 = 0;
   unsigned long TempoAtual = 0, TempoAntigo = 0, TempoCont = 0;
   uint8_t hora = 0, minu = 0, seg = 0;

//        VELOCIDADE
    volatile int contadorVel1 = 0, contadorVel2 = 0;
    float dist = 3.14 * 0.277 * float(qntPinVel) / 2;
    float velocidade = 0, velocidade1 = 0, velocidade2 = 0;

//        HC12
   const int INICIO = 254;
   const int FIM = 253;

bool select = 0;

//------------------------ OBJETOS ---------------------------

LiquidCrystal_I2C lcd(0x27, 16, 2);
MCP_CAN CAN(CSPin);
SoftwareSerial HC12(9,8);     //RT, TX

//---------------------- FUNÇÕES DO DISPLAY -----------------

void preenche_RPM(){
  lcd.setCursor(0,1);
  lcd.print("RPM");
  delay(1);
}

void preenche_LCD(int comb){
  lcd.setCursor(8,0);
  lcd.print("TIME");
  delay(1);
  lcd.setCursor(15,0);
  lcd.print(":");
  delay(10);  
  lcd.setCursor(18,0);
  lcd.print(":");
  delay(10); 
  
  lcd.setCursor(8,1);
  lcd.print("COMB");
  delay(1);
  lcd.setCursor(13,1);
  lcd.print(comb);
  delay(1);
  lcd.setCursor(14,1);
  lcd.print("/");
  delay(1);
  lcd.setCursor(16,1);
  lcd.print("4");
  delay(1);
}

void display_RPM(float rpm){
  char char_saida;
  lcd.clear();
  preenche_RPM();
  if (rpm > 9999) rpm = 9999;
  int Rpm = (int) rpm;
  itoa(Rpm, &char_saida, 10);
  writeBigString(&char_saida, 5, 0);
  delay(10); 
}

void display_LCD(float velocidade,  int comb){
  char char_saida;
  lcd.clear();
  preenche_LCD(comb);
  if (velocidade > 99) velocidade = 99;
  int vel = (int) velocidade;
  int Rpm = (int) rpm;
  itoa(vel, &char_saida, 10);
  writeBigString(&char_saida, 1, 0);
  lcd.setCursor(13,0);
  lcd.print(hora,2);
  delay(10);
  lcd.setCursor(16,0);
  lcd.print(minu,2);
  delay(10); 
  lcd.setCursor(19,0);
  lcd.print(seg,2);
  delay(10);     
}

// ------------------ TRANSMISSÃO VIA HC12 -----------------

void transmissaoHC12(float velocidade, float RPM_Funcao){
  
  uint8_t vel1 = (uint8_t) velocidade;
  uint8_t vel2 = (velocidade - vel1) * 100;
  uint8_t rpm1 = RPM_Funcao/100;
  uint8_t rpm2 = (int)RPM_Funcao%100;

  HC12.write(INICIO);
  HC12.write(vel1);
  HC12.write(vel2);
  HC12.write(rpm1);
  HC12.write(rpm2);
  HC12.write(FIM);
 
}

//-------------------------- SETUP ---------------------------

void setup() {
  
  Serial.begin(9600);
  
  lcd.init();                                                     //INICIALIZA O LCD
  lcd.backlight();
  pinMode(buttonPin, INPUT);                                      //CONFIGURA OS PINOS Do BOTÃO COMO ENTRADA
  pinMode(LEDPin, OUTPUT);                                        //E DO LED COMO SAÍDA (PWM)
  preenche_LCD();                                                 // FUNÇÃO DE PREENCIMENTO DO LCD

                                                                   //CRIA 8 CARACTERES PERSONALIZADOS
  for (nb=0; nb<8; nb++ ) {   
      for (bc=0; bc<8; bc++) bb[bc]= pgm_read_byte( &custom[nb][bc] );
      lcd.createChar ( nb+1, bb );
    }
  
  attachInterrupt(digitalPinToInterrupt(VELPin1), ContaVelocidade1, RISING);           //DEFINE OS PINOS DE VELOCIDADE COMO INTERRPÇÕES
  attachInterrupt(digitalPinToInterrupt(VELPin2), ContaVelocidade2, RISING);           //E HABILITA AS FUNÇÕES DE CONTAGEM

  while (CAN_OK != CAN.begin(CAN_500KBPS))                           //INICIALIZAÇÃO DA REDE CAN 
  {                                                                 //ENQUANTO A REPOSTA DA INICIALIZAÇÃO NÃO FOR 'OK', NÃO SAI DO LOOP WHILE    
    Serial.println("CAN BUS Init Failed");
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Init Failed");
    delay(100);
  }
  Serial.println("CAN BUS  Init OK!");
}


//------------------------------ LOOP ---------------------------

void loop() {

  TempoCont = millis();
  
  TempoAtual = millis();

  if (CAN_MSGAVAIL == CAN.checkReceive())                            //CHECA SE HÁ MENSAGEM RECEBIDA NA REDE CAN
  {
    CAN.readMsgBuf(&len, buf);                                      //LÊ A MENSAGEM (buf) E O TAMANHO DA MESNAGEM (len)

    unsigned long canId = CAN.getCanId();                           //LÊ O ID DA MENSAGEM

    Serial.println("-----------------------------");                //IMPRIME NA TELA AS INFORMAÇÕES DA MENSAGEM RECEBIDA
    Serial.print("Data from ID: 0x");
    Serial.println(canId, HEX);
    Serial.println(len);

    if (canId == 0x66D) {                                           //VERIFICA SE O ID RECEBIDO CORRESPONDE AO ID ESPERADO

      uint8_t rpm_buf[4];                                           //CRIA UM BUFFER PARA OS DADOS DE RPM
      for (int i = 0; i < len - 1; i++)
      {
        rpm_buf[i] = buf[i];                                          //ARMAZENA OS DADOS DE RPM EM rpm_buf
      }
      uint8_to_float(&RPM, rpm_buf);                                  //CONVERTE O ARRAY EM UM VALOR FLOAT E ARMAZENA EM 'RPM'

      comb = buf[len - 1];                                            //A INFORMÇÃO DO COMBUSTÍVEL É A ÚLTIMA DO PACOTE, POR ISSO (len-1)
        
      Serial.println("RPM : ");
      Serial.println(RPM);

      if (comb == 200) COMB = 1;                                      //O VALOR DE comb DEFINE SE O TANQUE ESTÁ CHEIO (4/4) OU NA RESERVA (1/4) 
      else if (comb == 0) COMB = 4;                                   
    }
  }

  if (TempoAtual - TempoVel1 > 2000)                                   //SE NÃO HOUVER VELOVIDADE EM 200 MILISSEGUNDOS, VELOCIDADE SERÁ 0
    velocidade1 = 0;
  if (TempoAtual - TempoVel2 > 2000)                                             
    velocidade2 = 0;

  if (contadorVel1 > qntPinVel - 1)
  {
    noInterrupts();
    velocidade1 = 3600.0 * dist / float(TempoAtual - TempoVel1);          //Velocidade = ------------------------------- = 3600* ---------
    TempoVel1 = TempoAtual;                                               //             tempo(ms)*1000(km)                      tempo(ms)
    contadorVel1 = 0;
    interrupts();
  }

  if (contadorVel2 > qntPinVel - 1)
  {
    noInterrupts();
    velocidade2 = 3600.0 * dist / float(TempoAtual - TempoVel2);        //Velocidade = ------------------------------- = 3600* ---------
    TempoVel2 = TempoAtual;                                             //             tempo(ms)*1000(km)                      tempo(ms)
    contadorVel2 = 0;
    interrupts();
  }

  velocidade = (velocidade1 + velocidade2) / 2;                        //FAZ A MÉDIA ENTRE OS DOIS VALORES DE VELOCIDADE
  Serial.println("Velocidade : ");
  Serial.println(velocidade);

  if(RPM <= 1800)                                                       //VERIFICA O VALOR DE RPM E ATRIBUI UM VALOR PARA indice
   indiceRPM = 10;                                                     
  else if(RPM > 1800 && RPM <= 2000)
    indiceRPM = 20;
  else if(RPM > 2000 && RPM <= 2200)
    indiceRPM = 30;
  else if(RPM > 2200 && RPM <= 2400)
    indiceRPM = 40;
  else if(RPM > 2400 && RPM <= 2600)
    indiceRPM = 50;
  else if(RPM > 2600 && RPM <= 2800)
    indiceRPM = 60;
  else if(RPM > 2800 && RPM <= 3000)
    indiceRPM = 70;
  else if(RPM > 3000 && RPM <= 3300)
    indiceRPM = 80;
  else if(RPM > 3300 && RPM <= 3600)
    indiceRPM = 90;
  else if(RPM > 3600 && RPM < 3800)
    indiceRPM = 100;
  else if(RPM > 3800)
    indiceRPM = 200;                                              //VALOR MÁXIMO DE indice = 255

   analogWrite(LEDPin, indiceRPM);                              //indice É USADA COMO VALOR DE SAÍDA DE PWM PATA O PINO LEDPin QUE ACENDE
                                                                //UMA SEQUÊNCIA DE LEDS REFERENTES AO VALOR DE RPM

  tempo();

   select = digitalRead(buttonPin);                                      //LÊ O VALOR DO PINO REFERENTE AO BOTÃO DE SELECIONAR A TELA DO DISPLAY QUE IRÁ APARECER
  
  if (!select) display_LCD(velocidade, RPM, 0);                          //PREENCHE O DISPLAY COM A TELA SELECIONADA
  else display_RPM(RPM);

   transmissaoHC12(velocidade, RPM);                          //PASSA OS VALORES DE VELOCIDADE E RPM PARA A FUNÇÃO QUE TRANSMITE OS DADOS PELO HC12
}


//------------------------------ TEMPO --------------------------------------

//CONVERTE O TEMPO DE MILISSEGUNDOS PARA HORAS, MINUTOS E SEGUNDOS

void tempo()
{
  seg = (uint8_t)TempoCont/1000;
  
  if(seg <= 59) minu++;
  else if(seg == 60) seg = 0;
  
  if(minu <= 59) hora++;
  else if(minu == 60) minu = 0;
   
  
}

//------------------------------- FUNÇÕES BIGCHAR --------------------------

int writeBigChar(char ch, byte x, byte y) {
  if (ch < ' ' || ch > '_') return 0;               // If outside table range, do nothing
  nb=0;                                             // character byte counter 
  for (bc=0; bc<8; bc++) {                        
    bb[bc] = pgm_read_byte( &bigChars[ch-' '][bc] );  // read 8 bytes from PROGMEM
    if(bb[bc] != 0) nb++;
  }  
 
  bc=0;
  for (row = y; row < y+2; row++) {
    for (col = x; col < x+nb/2; col++ ) {
      lcd.setCursor(col, row);                      // move to position
      lcd.write(bb[bc++]);                          // write byte and increment to next
    }
//    lcd.setCursor(col, row);
//    lcd.write(' ');                                 // Write ' ' between letters
  }
  return nb/2-1;                                      // returns number of columns used by char
}

void writeBigString(char *str, byte x, byte y) {
  char c;
  while ((c = *str++))
  x += writeBigChar(c, x, y) + 1;
}

//--------------------- CONVERSÃO PARA FLOAT ------------------------------

void uint8_to_float(float *valor, uint8_t data[])
{
  unsigned char receiveds[sizeof(float)];
  int i;
  int j = 0;
  for (i = 4; i < sizeof(float); i++)
  {
    receiveds[j] = data[i];
    j++;
  }

  memcpy(receiveds, data, sizeof(float));
  float received;
  memcpy(&received, receiveds, sizeof received);
  *valor = received;
}

//------------------- FUNÇÕES DE CONTAGEM DAS INTERRUPÇÕES ------------------

void ContaVelocidade1() {
  noInterrupts();
  contadorVel1++;
  interrupts();
}

void ContaVelocidade2() {
  noInterrupts();
  contadorVel2++;
  interrupts();
}
