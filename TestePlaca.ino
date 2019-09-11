#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>


#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial HC12(9,8); //RX,TX
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);


#define build 1
#define revision 1

#define rpm 2        //Pino RPM                                                     
#define vel 3       //Pino Velocidade
#define quantidadePinosVel 1  
#define INICIO 254
#define FIM 253
  
volatile int contadorVel=0, contadorRPM=0;
float dist = 3.14*0.277*float(quantidadePinosVel)/2, Velocidade = 0;
unsigned long Tempo2Vel=0, TempoAtual=0, Tempo2RPM=0, TempoAntigo = 0; 
int RPM = 0;
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
// ================================================ DISPLAY ========================================================
// BIG FONT Character Set
// - Each character coded as 1-4 byte sets {top_row(0), top_row(1)... bot_row(0), bot_row(0)..."}
// - number of bytes terminated with 0x00; Character is made from [number_of_bytes/2] wide, 2 high
// - codes are: 0x01-0x08 => custom characters, 0x20 => space, 0xFF => black square
// ================================================ DISPLAY ========================================================

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

//========================================== FUNÇÃO DE FUNDO DO DISPLAY =========================================================================
void preenche_lcd(){

//Velocidade 

  lcd.setCursor(8,3);
  lcd.print("Km/h");
  delay(1);

// RPM
  
  lcd.setCursor(18,0);
  lcd.print("R");
  delay(1);
  lcd.setCursor(18,1);
  lcd.print("P");
  delay(1);
  lcd.setCursor(18,2);
  lcd.print("M");
  delay(1);
  lcd.setCursor(18,3);
  lcd.print("<");
  delay(1);
//barras
  lcd.setCursor(15,0);
  lcd.print("|");
  delay(1);
  lcd.setCursor(15,1);
  lcd.print("|");
  delay(1);    
  lcd.setCursor(15,2);
  lcd.print("|");
  delay(1);
  lcd.setCursor(15,3);
  lcd.print("|");
  delay(1);

// Combustivel

  lcd.setCursor(1,0);
  lcd.print("F");
  delay(1);
  lcd.setCursor(1,1);
  lcd.print("U");
  delay(1);
  lcd.setCursor(1,2);
  lcd.print("E");
  delay(1);
  lcd.setCursor(1,3);
  lcd.print("L");
  delay(1);
 
 //barras
  lcd.setCursor(4,0);
  lcd.print("|");
  delay(1);
  lcd.setCursor(4,1);
  lcd.print("|");
  delay(1);
  lcd.setCursor(4,2);
  lcd.print("|");
  delay(1);
  lcd.setCursor(4,3);
  lcd.print("|");
  delay(1);
}

//=============================================== FUNÇÃO DE IMPRIMIR VALORES NO DISPLAY==================================

void Display(float velocidade, float rpm){
  
  char char_saida;
  lcd.clear();
  preenche_lcd();
  if (velocidade > 99) velocidade = 99;
  int vel = (int)velocidade;
  itoa(vel,&char_saida,10);
  writeBigString(&char_saida, 7, 1);
  
  
  if(rpm >= 1500 && rpm < 3000){
  
     lcd.setCursor(16,0);
     lcd.write(0X20);
     lcd.setCursor(16,1);
     lcd.write(0x20);
     lcd.setCursor(16,2);
     lcd.write(0x20);
     lcd.setCursor(16,3);
     lcd.write(0xFF);
  
}

  else if(rpm >= 3000 && rpm < 4500 )
    {

     lcd.setCursor(16,0);
     lcd.write(0X20);
     lcd.setCursor(16,1);
     lcd.write(0x20);
     lcd.setCursor(16,2);
     lcd.write(0xFF);
     lcd.setCursor(16,3);
     lcd.write(0xFF);

   }
   else if(rpm >= 4500)
    {
    lcd.setCursor(16,0);
     lcd.write(0X20);
     lcd.setCursor(16,1);
     lcd.write(0xFF);
     lcd.setCursor(16,2);
     lcd.write(0xFF);
     lcd.setCursor(16,3);
     lcd.write(0xFF);   

    }

  else if(rpm >= 0 && rpm < 1500 )
        {
    lcd.setCursor(16,0);
     lcd.write(0X20);
     lcd.setCursor(16,1);
     lcd.write(0x20);
     lcd.setCursor(16,2);
     lcd.write(0x20);
     lcd.setCursor(16,3);
     lcd.write(0x20);
}
    /*if(data[9] == 1 && data[10] == 1)
    {
      lcd.setCursor(16,0);
     lcd.write(0X20);
     lcd.setCursor(16,1);
     lcd.write(0xFF);
     lcd.setCursor(16,2);
     lcd.write(0xFF);
     lcd.setCursor(16,3);
     lcd.write(0xFF);
    }
    if(data[9] == 1 && data[10] == 1)
    {
     lcd.setCursor(16,1);
     lcd.write(0x20);
     lcd.setCursor(16,2);
     lcd.write(0xFF);
     lcd.setCursor(16,3);
     lcd.write(0xFF);
    }
    if(data[9] == 0 && data[10] == 0)
    {   
     lcd.setCursor(16,3);
     lcd.write(0xFF);
    }   */

}

// ================================================ DISPLAY ========================================================

void setup() {
  
  Serial.begin(9600);
  HC12.begin(9600);
  lcd.begin(20,4);
  preenche_lcd();
  
  for (nb=0; nb<8; nb++ ) {                     // create 8 custom characters
    for (bc=0; bc<8; bc++) bb[bc]= pgm_read_byte( &custom[nb][bc] );
    lcd.createChar ( nb+1, bb );
  }

  attachInterrupt(digitalPinToInterrupt(vel), ContaVelocidade, RISING);           //Definindo função de interrupção para velocidade
  attachInterrupt(digitalPinToInterrupt(rpm), ContaRPM, RISING);                 //Definindo função de interrupção para rpm

  }

void loop() {

  TempoAtual = millis();  
                                                     
  if(TempoAtual-Tempo2Vel>2000)                                                   //Se não houver alteração na velocidade em 2000 milisegundos, então VELOCIDADE SERÁ 0
    Velocidade=0;
  if(TempoAtual-Tempo2RPM>2000)                                                   //Se não houver alteração na velocidade em 2000 milisegundos, então RPM SERÁ 0
    RPM=0;

  if(contadorVel>quantidadePinosVel-1)
  {
    noInterrupts();
    Velocidade=3600.0*dist/float(TempoAtual-Tempo2Vel);                           //Velocidade = ------------------------------- = 3600* ---------
    Tempo2Vel = TempoAtual;                                                       //             tempo(ms)*1000(km)                      tempo(ms)
    contadorVel=0;
    interrupts();
  }

  if(contadorRPM>1){
    noInterrupts();
    if(TempoAtual-Tempo2RPM>11)                                                   //if(RPM<4000rpm)
    RPM = int(60000.0/float(TempoAtual-Tempo2RPM)); 
    RPM2 = (60000.0/float(TempoAtual-Tempo2RPM));                             //RPM = 1000(s)*60(min)/tempo(ms)
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
  }

  Display(Velocidade, RPM2);
  
}

void ContaVelocidade(){
  noInterrupts();
  contadorVel++;
  interrupts();
}

void ContaRPM(){
  noInterrupts();
  contadorRPM++;
  interrupts();
  }
//========================================================== TRANSMISSÃO ============================================

void transmissao(float velocidade, int RPM_Funcao){
  
  uint8_t vel1 = (uint8_t) velocidade;
  uint8_t vel2 = (velocidade - vel1) * 100;
  uint8_t rpm1 = RPM_Funcao/100;
  uint8_t rpm2 = RPM_Funcao%100;

  HC12.write(INICIO);
  HC12.write(vel1);
  HC12.write(vel2);
  HC12.write(rpm1);
  HC12.write(rpm2);
  HC12.write(FIM);

}

//================================================ DISPLAY ========================================================
// writeBigChar: writes big character 'ch' to column x, row y; returns number of columns used by 'ch'

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

// ================================================ DISPLAY ========================================================


// ================================================ DISPLAY ========================================================
void writeBigString(char *str, byte x, byte y) {
  char c;
  while ((c = *str++))
  x += writeBigChar(c, x, y) + 1;
}
// ================================================ DISPLAY ========================================================
