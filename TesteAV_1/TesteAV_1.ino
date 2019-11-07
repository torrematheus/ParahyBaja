#include <SD.h>
#include <SoftwareSerial.h>

SoftwareSerial HC12(9,8);

#define rpm 2        //Pino RPM                                                     
#define vel 3       //Pino Velocidade
#define quantidadePinosVel 1  
#define INICIO 254
#define FIM 253

volatile int contadorVel=0, contadorRPM=0, contadorDist;
//float dist = 3.14*0.277*float(quantidadePinosVel)/2, 
float dist = 2*3.14*0.277, Velocidade = 0, Distancia = 0;
unsigned long Tempo2Vel=0, TempoAtual=0, Tempo2RPM=0, TempoAntigo = 0; 
int RPM = 0;

void transmissao(float velocidade, int RPM_Funcao, float Distancia){
  
  uint8_t vel1 = (uint8_t) velocidade;
  uint8_t vel2 = (velocidade - vel1) * 100;
  uint8_t rpm1 = RPM_Funcao/100;
  uint8_t rpm2 = RPM_Funcao%100;
  uint8_t dist1= int(Distancia/100);
  uint8_t dist2= (int(Distancia)%100);
  uint8_t dist3= (Distancia-(int(Distancia)))*100;

  HC12.write(INICIO);
  HC12.write(vel1);
  HC12.write(vel2);
  HC12.write(rpm1);
  HC12.write(rpm2);
  HC12.write(dist1);
  HC12.write(dist2);
  HC12.write(dist3);
  HC12.write(FIM);
  
 /*Serial.write(INICIO);
 Serial.write(vel1);
 Serial.write(vel2);
 Serial.write(rpm1);
 Serial.write(rpm2);
 Serial.write(FIM);*/

}

void setup() {
  //Serial.begin(9600);
  HC12.begin(9600);

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
    Velocidade=3600.0*dist/float(TempoAtual-Tempo2Vel);                            //Velocidade = ------------------------------- = 3600* ---------
    Tempo2Vel = TempoAtual;                                                       //             tempo(ms)*1000(km)                      tempo(ms)
    contadorVel=0;
    transmissao(Velocidade, RPM, Distancia);
    interrupts();
  }

  if(contadorRPM>1){
    noInterrupts();
    if(TempoAtual-Tempo2RPM>11)                                                   //if(RPM<4000rpm)
    RPM = int(60000.0/float(TempoAtual-Tempo2RPM));                             //RPM = 1000(s)*60(min)/tempo(ms)
    Tempo2RPM = TempoAtual;
    contadorRPM=0;
    interrupts();
  }

  Distancia = dist*contadorDist;
   
}

void ContaVelocidade(){
  noInterrupts();
  contadorVel++;
  contadorDist++;
  interrupts();
}

void ContaRPM(){
  noInterrupts();
  contadorRPM++;
  interrupts();
  }
