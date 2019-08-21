#include <SoftwareSerial.h>

SoftwareSerial hc12(8,9);

#define vel 2                                                                 //Pino Velocidade
#define rpm 3                                                                 //Pino RPM

#define quantidadePinosVel  1                                                 //Faz a leitura do tempo a cada "QuantidadeDePinos" pinos
#define INICIO 254
#define FIM 253

volatile int contadorVel=0, contadorRPM=0;
int RPM=0;
unsigned long Tempo2Vel=0, TempoAtual=0;                                     //Variaveis de tempo pra velocidade
unsigned long Tempo2RPM=0;                                                   //Variaveis de tempo pro RPM
unsigned long Tempo2Display=0, tempoDisplay=0;                                    //Variaveis de tempo pro Display
float dist = 3.14*0.277*float(quantidadePinosVel)/2, Velocidade = 0, distTotal=0; // Como o carro possui 4 pinos, para leituras pino a pino, a distancia é piR/2             //distancia e velocidade


void transmissao(float Vel, int RPM, unsigned long tempo)
{
  int vel1, vel2;
  int rpm1, rpm2;
  int tempohr, tempomin, temposeg, tempoms1, tempoms2;

   vel1=int(Vel);
   vel2=int((Vel-float(vel1))*100);
   rpm1=RPM/100;
   rpm2=RPM%100;
  
   tempoms1=int(tempo%100); 
   tempoms2=int(tempo/100%10);
   temposeg=int((tempo/1000)%60);
   tempomin=int((tempo/60000)%60);
   
   /*hc12.write(INICIO);
   hc12.write(vel1);
   hc12.write(vel2);
   hc12.write(rpm1);
   hc12.write(rpm2);
   hc12.write(tempoms1);
   hc12.write(tempoms2);
   hc12.write(temposeg);
   hc12.write(tempomin);
   
   hc12.write(FIM);
   */
   Serial.write(INICIO);
   Serial.write(vel1);
   Serial.write(vel2);
   Serial.write(rpm1);
   Serial.write(rpm2);
   Serial.write(tempoms1);
   Serial.write(tempoms2);
   Serial.write(temposeg);
   Serial.write(tempomin);
   
   Serial.write(FIM);

   /*'
   Serial.print(INICIO);
   Serial.print(" ");
   Serial.print(vel1);
   Serial.print(" ");
   Serial.print(vel2);
   Serial.print(" ");
   Serial.print(rpm1);
   Serial.print(" ");
   Serial.print(rpm2);
   Serial.print(" ");
   Serial.print(tempoms);
   Serial.print(" ");
   Serial.print(temposeg);
   Serial.print(" ");
   Serial.print(tempomin);
   Serial.print(" ");
   Serial.print(tempohr);
   Serial.print(" ");
   Serial.println(FIM);
*/
}


void setup() {
  Serial.begin(115200);
  hc12.begin(9600);

  //pinMode(vel, INPUT_PULLUP);                                                     //Definindo pino para velocidade
  //pinMode(rpm, INPUT_PULLUP);                                                     //Definindo pino para velocidade
  attachInterrupt(digitalPinToInterrupt(vel), ContaVelocidade, RISING);           //Definindo função de interrupção para velocidade
  attachInterrupt(digitalPinToInterrupt(rpm), ContaRPM, RISING);                 //Definindo função de interrupção para rpm
  
}

void loop() {
  TempoAtual = millis();                                                          //Atribuindo tempo a variável

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
    transmissao(Velocidade,RPM, TempoAtual);
    interrupts();
  }

  if(contadorRPM>1){
    noInterrupts();
    if(TempoAtual-Tempo2RPM>15)                                                   //if(RPM<4000rpm)
      RPM = int(60000.0/float(TempoAtual-Tempo2RPM));                             //RPM = 1000(s)*60(min)/tempo(ms)
    Tempo2RPM = TempoAtual;
    contadorRPM=0;
    interrupts();
  }
  //Serial.print(digitalRead(2));
  //Serial.print(" ");
  //Serial.println(digitalRead(3));
  //Serial.print(" ");
  
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
