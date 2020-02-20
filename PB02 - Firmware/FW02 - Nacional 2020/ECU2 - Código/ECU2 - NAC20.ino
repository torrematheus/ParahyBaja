
//===================================================================================
//----- Bibliotecas -----
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mcp_can.h>

// ================================================ DISPLAY ========================================================

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

//===========================================================================
//----- Mapeamento de Hardware -----

//       trasmissão de dados
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool select = 0;
const int CSPin = 10;
MCP_CAN CAN(CSPin);
float RPM = 0;
float COMB = 0;
const int LEDPin = 9;
int indiceRPM = 0;
uint8_t comb;
uint8_t buf[8];
unsigned char len = 0;
void uint8_to_float(float *valor, uint8_t data[]);

//             tempo
unsigned long TempoVel1 = 0, TempoVel2 = 0;
unsigned long TempoAtual = 0, TempoAntigo = 0;

//           velocidade
const int VELPin1 = 2;
const int VELPin2 = 3;
const int qntPinVel = 1;
volatile int contadorVel1 = 0, contadorVel2 = 0;
float dist = 3.14 * 0.277 * float(qntPinVel) / 2;
float velocidade = 0, velocidade1 = 0, velocidade2 = 0;

//=============================================================================
//----- Funções para o display -----
void preenche_RPM(){
  lcd.setCursor(0,1);
  lcd.print("RPM");
  delay(1);
}
void preenche_LCD(){
  lcd.setCursor(8,0);
  lcd.print("RPM");
  delay(1);
  lcd.setCursor(8,1);
  lcd.print("COMB");
  delay(1);
  lcd.setCursor(14,1);
  lcd.print("/");
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
void display_LCD(float velocidade, float rpm, float comb){
  char char_saida;
  lcd.clear();
  preenche_LCD();
  if (velocidade > 99) velocidade = 99;
  int vel = (int) velocidade;
  int Rpm = (int) rpm;
  itoa(vel, &char_saida, 10);
  writeBigString(&char_saida, 1, 0);
  lcd.setCursor(12,0);
  lcd.print(Rpm);
  delay(10);
   
}
//----- Função Principal -----
void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  pinMode(4, INPUT);
  pinMode(LEDPin, OUTPUT);
  preenche_LCD();
  //lcd.setCursor(3, 0);
  //lcd.print("PARAHYBAJA");

  //Cria 8 caracteres personalizados
  for (nb=0; nb<8; nb++ ) {   
      for (bc=0; bc<8; bc++) bb[bc]= pgm_read_byte( &custom[nb][bc] );
      lcd.createChar ( nb+1, bb );
    }
  
  attachInterrupt(digitalPinToInterrupt(VELPin1), ContaVelocidade1, RISING);           //Definindo função de interrupção para vel1
  attachInterrupt(digitalPinToInterrupt(VELPin2), ContaVelocidade2, RISING);           //Definindo função de interrupção para vel2

  while (CAN_OK != CAN.begin(CAN_500KBPS))
  {
    Serial.println("CAN BUS Init Failed");
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Init Failed");
    delay(100);
  }
  Serial.println("CAN BUS  Init OK!");
}

void loop() {
  TempoAtual = millis();

  if (CAN_MSGAVAIL == CAN.checkReceive())
  {
    CAN.readMsgBuf(&len, buf);

    unsigned long canId = CAN.getCanId();

    Serial.println("-----------------------------");
    Serial.print("Data from ID: 0x");
    Serial.println(canId, HEX);
    Serial.println(len);

    if (canId == 0x66D) {            //Verifica se a mensagmem recebida é de rpm

      uint8_t rpm_buf[4];
      for (int i = 0; i < len - 1; i++)
      {
        rpm_buf[i] = buf[i];         //Armazena os dados no array aux_rpm
      }
      uint8_to_float(&RPM, rpm_buf); //Converte o array em float e armazena na variavel RPM

      comb = buf[len - 1];

      Serial.println("RPM : ");
      Serial.println(RPM);

      if (comb == 200)Serial.println("COMB: Reserva");
      else if (comb == 0) Serial.println("COMB: Cheio");

    }
    /*lcd.setCursor(2, 1);
    lcd.print("RPM : ");
    lcd.setCursor(9, 1);
    lcd.print(RPM);*/
  }

  if (TempoAtual - TempoVel1 > 2000)                                              //Se não houver alteração na velocidade em 2000 milisegundos, então VELOCIDADE SERÁ 0
    velocidade1 = 0;
  if (TempoAtual - TempoVel2 > 2000)                                              //Se não houver alteração na velocidade em 2000 milisegundos, então RPM SERÁ 0
    velocidade2 = 0;

  if (contadorVel1 > qntPinVel - 1)
  {
    noInterrupts();
    velocidade1 = 3600.0 * dist / float(TempoAtual - TempoVel1);                   //Velocidade = ------------------------------- = 3600* ---------
    TempoVel1 = TempoAtual;                                                       //             tempo(ms)*1000(km)                      tempo(ms)
    contadorVel1 = 0;
    interrupts();
  }

  if (contadorVel2 > qntPinVel - 1)
  {
    noInterrupts();
    velocidade2 = 3600.0 * dist / float(TempoAtual - TempoVel2);                   //Velocidade = ------------------------------- = 3600* ---------
    TempoVel2 = TempoAtual;                                                       //             tempo(ms)*1000(km)                      tempo(ms)
    contadorVel2 = 0;
    interrupts();
  }

  velocidade = (velocidade1 + velocidade2) / 2;
  Serial.println("Velocidade : ");
  Serial.println(velocidade);

  //display_LCD(velocidade, RPM, 0);

  select = digitalRead(4);
  if (!select) display_LCD(velocidade, RPM, 0);
  else display_RPM(RPM);

  if(RPM <= 1800)
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
    indiceRPM = 200;

   analogWrite(LEDPin, indiceRPM);
  
}

//===============================================================================
//----- Funções -----

//      display
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

//    transmissão
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

//     velocidade
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
