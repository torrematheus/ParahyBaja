import processing.serial.*;                                                                                    // Importa a biblioteca Serial
Serial myPort;                                                                                                 // Cria uma variavel do tipo Serial

int[] serialInArray = new int[20];                                                                             // Dados recebidos um a um
int serialCount = 0;                                                                                           // Conta quantos dados ja foram recebidos
boolean firstContact = false;       

int rpm1 = 0, rpm2=0,rpm1Movida = 0, rpm2Movida=0, vel1=0, vel2=0, dist1=0, dist2=0, dist3=0;
int RPM=0, RPMMovida=0;
float vel=0, distancia=0;
int tempoMin=0, tempoSeg=0, tempoMiliSeg=0;

long tAn=0, tAt=0;

PImage turma, Limc, Parahybaja, Logo;
PrintWriter Velocidadet, RPMt, Tempot, Distanciat;
PFont font;

int[][] a= new int[31][2];

void setup(){
  size(800,600);
  font=createFont("Stencil",1);
  
  Limc = loadImage("limc.png");
  Limc.resize(6*width/10,Limc.height*6*width/(10*Limc.width));
  Parahybaja = loadImage("parahybaja2.jpeg");
  Logo = loadImage("parahybaja.png");
  Logo.resize(2*width/10,Logo.height*2*width/(10*Logo.width));
  //Tempot = createWriter("Tempo.txt");
  Velocidadet = createWriter("Velocidade.txt");                                                                 ////////////////////////////////////////
  RPMt = createWriter("RPM.txt");       
  Distanciat = createWriter("Distancia.txt"); //                                    //
  
  for(int i=0; i<20;i++){
    serialInArray[i]=0;
  }
  
  
  println(Serial.list());                                                                                      // Printa na tela as Portas seriais em uso pelo computador
  String portName = Serial.list()[0];                                                                          // Define portName como sendo a primeira Serial printada na tela
  myPort = new Serial(this, portName,9600);                                                               // Define a primeira serial como à ser usada(provavelmente o arduino)
}

void draw(){
  background(104,0,0);
  fill(50,50,50);
  image(Parahybaja, 0,0);
  rect(2 *width/10, 4 *height/8, 1 *width/10, -3*(float(RPM)/4500) *height/8);  //Barra RPM
  rect(5 *width/10, 4 *height/8, 1 *width/10, -3 *(vel/60)*height/8);  //Barra Velocidade
  
  rect(2 *width/10, 5 *height/8, 1 *width/10, 1 *height/8);  //Retangulo RPM
  rect(5 *width/10, 5 *height/8, 1 *width/10, 1 *height/8);  //Retangulo Velocidade
  rect(2 *width/10, 7 *height/8, 3 *width/10, 1 *height/8);  //Retangulo Distancia
  
  rect(3 *width/20, 1 *height/30, 2 *width/10, 1 *height/16);  //Retangulo Nome RPM
  rect(9 *width/20, 1 *height/30, 2 *width/10, 1 *height/16);  //Retangulo Nome Velocidade
  
  fill(255,255,255);
  textFont(font,width/30);
  textAlign(CENTER,CENTER);
  text("RPM",5 *width/20, 1 *height/16);
  text("Velocidade",11 *width/20, 1 *height/16);
  
  text(RPM,5 *width/20, 11 *height/16);
  text(vel,11 *width/20, 11 *height/16);
  text("Distância" ,6 *width/20, 15 *height/16);
  text(distancia ,9 *width/20, 15 *height/16);
  //text(tempoMin+" : "+ tempoSeg+ " : " + tempoMiliSeg ,11 *width/20, 13 *height/16);
  //text(RPMMovida ,11 *width/20, 15 *height/16);
  
  image(Logo, 8 *width/10, 13 *height/16);
  
  //Tempot.println(tempoMin+ " : " + tempoSeg + " : " + tempoMiliSeg);
  RPMt.println(RPM);                                                                                    // Escreve no .txt RPM seus valores
  Velocidadet.println(vel);   
  Distanciat.println(distancia);  // Escreve no .txt Velocidade seus valores
  
  if(mousePressed && mouseX> width-140 && mouseX< width-40 && mouseY> 0 && mouseY< 100 ){     ///// Se pressionar em desligar(fechar programa)//////
       //Tempot.close();
       RPMt.close();                                                                                            //     Encerra o arquivo .txt do RPM e o salva      //
       Velocidadet.close();        
       Distanciat.close();       //  Encerra o arquivo .txt da Velocidade e o salva  //
       exit();                                                                                                 //////////////  Fecha o Programa  ////////////////////
   }  
    
  Grafico(int(vel), 7 *width/10, 3 *height/8, 2 *width/10, 60.0/256,0);
  Grafico(RPM, 7 *width/10, 6 *height/8, 2 *width/10, 4500/256.0,1);
  
}


void serialEvent(Serial p) {                                                                              // Função do processing que trabalha com a Serial
  try{
    int inByte = p.read();                                                                                  // Lê o que tem na porta serial e escreve em inByte
    
    if (inByte == 254) 
    {                                                                                       // E se a variavel lida for "A" ou "255"
      serialInArray[0] = inByte;
      serialCount++; 
    }
    if(inByte!=254 && serialInArray[0] == 254)
      {                                                                                                       // Após o primeiro contato:
      serialInArray[serialCount] = inByte;                                                                       // Salva no Array cada valor novo lido
      
      if(serialInArray[0]==254 )                                                                                  // Se o primeiro valor lido for 255
        serialCount++;                                                                                           // ComeÇa a contar para salvar os próximos valores
  
      if (serialInArray[serialCount-1] == 253 || serialCount>9 ) { 
        vel1 = serialInArray[1];
        vel2 = serialInArray[2];
        rpm1 = serialInArray[3];
        rpm2 = serialInArray[4];
        //rpm1Movida = serialInArray[5];
        //rpm2Movida = serialInArray[6];
        /*t1 = serialInArray[7];
        t2 = serialInArray[8];
        t3 = serialInArray[9];
        t4 = serialInArray[10]; */           //AB CD EF GH     ABCDE.FGH
        dist1 = serialInArray[5];
        dist2 = serialInArray[6];
        dist3 = serialInArray[7];
        
        RPM = rpm1*100+rpm2;
        RPMMovida = rpm1Movida*100+rpm2Movida;
        vel = float(vel1)+float(vel2)*0.01;
        //tempoMin = t4;
        //tempoSeg = t3;
        //tempoMiliSeg = t2*100+t1;
        distancia = float(dist1*100+dist2)+float(dist3)*0.01;
        serialInArray[0] = 0;
        serialCount = 0;
      }
    }
  }
  catch(RuntimeException e) {
    e.printStackTrace();
  }

}


void Grafico(int variavel, int x1, int y1, int tamanho,float fatorM, int numG) {             // Função que desenha um gráfico a partir de uma variavel dada
 
  stroke(255);                                                                                                 // Bordas terão cor branca
  line(x1,y1,x1,y1-tamanho);                                                                                   // Desenha a linha do eixoY
  line(x1,y1,x1+tamanho,y1);                                                                                   // Desenha a linha do eixoX
  
  for(int pq=0; pq<=16;pq++){                                                                                  ///////////////////////////////////////////////////////////
    
      fill(255,255);                                                                                           //    Define a cor como branco sem transparencia(255)    //
      textSize(10);                                                                                            //              Tamanho do texto será 10                 //
      text(int(float(pq)*16*fatorM), x1-30, y1-pq*tamanho/16);                                                 //           Escreve os valores de cada passo            //
      strokeWeight(1);                                                                                         //               Largura da borda será 1                 //
      stroke(200,50);                                                                                          // Define a cor da borda como cinza com transparencia 50 //
      line(x1,y1-pq*tamanho/16,x1+tamanho,y1-pq*tamanho/16);                                                   //       Desenha uma linha na altura de cada passo       //
      
  }                                                                                                            ///////////////////////////////////////////////////////////
  tAt = millis();
  if(tAt-tAn>50){
    for(int i=30;i>0;i--){                                                                                       //   A cada passo de tempo o valor será colocado para a  //
    
      a[i][numG]=a[i-1][numG];                                                                                   //    linha de tras e o novo valor para a linha atual    //
      if(a[i][numG]<y1-tamanho)
        a[i][numG]= y1-tamanho;
    }                                                                                                            ///////////////////////////////////////////////////////////
  
    a[0][numG]=y1-int(float(variavel)*float(tamanho)/(256.0*fatorM));                                                                          // Linha atual que será mudada de acordo com a variavel
    tAn = tAt;
  }
  
  fill(255,100);
  beginShape();
  vertex(x1, y1);
  for(int i=0;i<30 ;i++)
  {                                                                                 
    vertex(x1+(i*tamanho)/30, a[i][numG]);
  }
  vertex(x1+tamanho, y1);
  endShape();
  smooth();
}
