#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>

//Configuração do Display
#define build 1
#define revision 1

const int CSPin = 10;
const int VELPin1 = 2;
const int VELPin2 = 3;
float velocidade = 0, velocidade1 =0, velocidade2 = 0, RPM = 0;
uint8_t comb_buf;

const int quantidadePinosVel = 1;
const int INICIO = 254;
const int FIM = 253;

volatile int contadorVel1=0, contadorVel2=0;
float dist = 3.14*0.277*float(quantidadePinosVel)/2;
unsigned long TempoVel1=0, TempoAtual=0, TempoVel2=0, TempoAntigo = 0; 

MCP_CAN CAN(CSPin);
SoftwareSerial HC12(8,9 ); //RX,TX
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);

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


void Display(float velocidade, float rpm, uint8_t comb){
  
  char char_saida;
  lcd.clear();
  preenche_lcd();
  if (velocidade > 99) velocidade = 99;
  int vel = (int) velocidade;
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

if(comb == 0){

     lcd.setCursor(3,0);
     lcd.write(0xFF);
     lcd.setCursor(3,1);
     lcd.write(0xFF);
     lcd.setCursor(3,2);
     lcd.write(0xFF);
     lcd.setCursor(3,3);
     lcd.write(0xFF);
}

else if(comb == 1){

     lcd.setCursor(3,0);
     lcd.write(0x20);
     lcd.setCursor(3,1);
     lcd.write(0x20);
     lcd.setCursor(3,2);
     lcd.write(0xFF);
     lcd.setCursor(3,3);
     lcd.write(0xFF);
}


}


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

void setup()
{
    Serial.begin(9600);
    HC12.begin(9600);
    lcd.begin(20,4);
    preenche_lcd();

    attachInterrupt(digitalPinToInterrupt(VELPin1), ContaVelocidade1, RISING);           //Definindo função de interrupção para velocidade
    attachInterrupt(digitalPinToInterrupt(VELPin2), ContaVelocidade2, RISING);                 //Definindo função de interrupção para rpm
    
   //Cria 8 caracteres personalizados 
    for (nb=0; nb<8; nb++ ) {   
      for (bc=0; bc<8; bc++) bb[bc]= pgm_read_byte( &custom[nb][bc] );
      lcd.createChar ( nb+1, bb );
    }

    while (CAN_OK != CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS Init Failed");
        delay(100);
    }
    Serial.println("CAN BUS  Init OK!");
}


void loop()
{
    unsigned char len = 0;  // len vai armazenar o tamanho da mensagem recebida
    uint8_t buf[8];         //buf vai armazenar a mensagem recebida 

    if(CAN_MSGAVAIL == CAN.checkReceive())  // Verifica se chegou alguma mensagem
    {
        CAN.readMsgBuf(&len, buf);          // Lê a mensagem

        unsigned long canId = CAN.getCanId(); //Lê o Id da mensagem e armazena na variavel canID

        Serial.println("-----------------------------");
        Serial.print("Data from ID: ");
        Serial.println(canId, DEC);

        if(canId == 0x66D){               //Verifica se a mensagmem recebida é de rpm
            
            uint8_t rpm_buf[4];  

           // comb_buf = buf[0];          
            for(int i = 0; i<len-1; i++)
            {    
                rpm_buf[i] = buf[i];        //Armazena os dados no array aux_rpm                       
            }   

            comb_buf = buf[len-1];

            Serial.print(comb_buf);
            uint8_to_float(&RPM, rpm_buf);  // Converte o array em float e armazena na variavel RPM
        }

    }

    TempoAtual = millis();  
                                                     
  if(TempoAtual-TempoVel1>2000)                                                   //Se não houver alteração na velocidade em 2000 milisegundos, então VELOCIDADE SERÁ 0
    velocidade1=0;
  if(TempoAtual-TempoVel2>2000)                                                   //Se não houver alteração na velocidade em 2000 milisegundos, então RPM SERÁ 0
    velocidade2=0;

  if(contadorVel1>quantidadePinosVel-1)
  {
    noInterrupts();
    velocidade1=3600.0*dist/float(TempoAtual-TempoVel1);                           //Velocidade = ------------------------------- = 3600* ---------
    TempoVel1 = TempoAtual;                                                       //             tempo(ms)*1000(km)                      tempo(ms)
    contadorVel1=0;
    interrupts();
  }

   if(contadorVel2>quantidadePinosVel-1)
  {
    noInterrupts();
    velocidade2=3600.0*dist/float(TempoAtual-TempoVel2);                           //Velocidade = ------------------------------- = 3600* ---------
    TempoVel2 = TempoAtual;                                                       //             tempo(ms)*1000(km)                      tempo(ms)
    contadorVel2=0;
    interrupts();
  }

  velocidade = (velocidade1 + velocidade2)/2;
  
  if(TempoAtual-TempoAntigo>200)
  {
  noInterrupts();
  transmissaoHC12(velocidade, RPM); 
  interrupts();
  TempoAntigo = TempoAtual;
  }
  
    /*Serial.print("Combustivel: \n");
    Serial.println(comb_buf);
    Serial.print("RPM: \n");
    Serial.println(RPM);*/
    Display(velocidade, RPM, comb_buf);
}

void ContaVelocidade1(){
  noInterrupts();
  contadorVel1++;
  interrupts();
}

void ContaVelocidade2(){
  noInterrupts();
  contadorVel2;
  interrupts();
 }


void uint8_to_float(float *valor, uint8_t data[])
{
  unsigned char receiveds[sizeof(float)];
  int i;
  int j = 0;
  for(i = 4; i < sizeof(float); i++)
  {
        receiveds[j] = data[i];
        j++;
  }

  memcpy(receiveds, data, sizeof(float));
  float received;
  memcpy(&received, receiveds, sizeof received);
  *valor = received;
}


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
