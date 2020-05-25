#include <Wire.h> //Inclusão da biblioteca de conexão I2C
#include "RTClib.h" //Inclusão da biblioteca do modulo Real Time Clock RTC DS3231
#include <TM1637Display.h> //Inclusão da biblioteca de controle do modulo de 7 segmentos e 4 digitos
 
RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231

//Pinos de controle do modulo RTC_DS3231
 #define CLK 4
 #define DIO 5
 
//Inicializa o display
  TM1637Display display(CLK, DIO);

//define display
  uint8_t position[] = { 0, 0, 0, 0 };
  
//Pinos do Buzzer e do led interno
  const int buzzerPin = 10;
  const int ledPin1 = 13;
 
 
//DECLARAÇÃO DOS DIAS DA SEMANA -- para impressão no console
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

//Define as notas musicais:
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
 
void setup(){
  pinMode(buzzerPin, OUTPUT);//define pino do buzzer
  pinMode(ledPin1, OUTPUT);//define led interno
  Serial.begin(9600); //INICIALIZA ACOMUNICAÇÃO SERIAL 
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
  }
  if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
    Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
    //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    //rtc.adjust(DateTime(2018, 9, 29, 15, 00, 45)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
  display.clear();//LIMPA DISPLAY
  
}

void loop () {
  
  //------------------Mostra Data e Hora no console------------------
  DateTime now = rtc.now(); //CAPITURA A DATA ATUAL ATRAVES DO TIPO DE OBJETO DATATIME
  imprimirDataConsole(now); // Chama metodo de impressao no console

  //------------------Mostra Data------------------
  //Se o if for verdadeiro mostra a a data por um segundo
  if((now.second()%15) == 0){
    display.setBrightness(1); //Define o brilho do display  
    geraData(now); // Chama metodo para impressão da data no display
    delay(1000); // Aguarda um segundo
  }else{ //Se o if não for verdadeiro, mostra a hora
  //------------------Mostra Hora------------------  
    display.setBrightness(6); //Define o brilho do display  
    geraHora(now); // Chama metodo para impressão da hora no display
  }
  
  //------------------Despertador------------------
  if(now.hour() == 6){ //Para definir a hora do despertador tocar
    if(now.minute() == 0){ // Para definir o minuto do despertador tocar
      tocaMusica(); // Metodo para tocar a musica do despertador
    }
  }
  
  delay(500); //INTERVALO DE MEIO SEGUNDO
}


//<metodo>Imprimir data e hora no console!</metodo>
void imprimirDataConsole(DateTime now){
    Serial.print("Data: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.year(), DEC); //IMPRIME NO MONITOR SERIAL O ANO
    Serial.print(" / Dia: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO MONITOR SERIAL O DIA
    Serial.print(" / Horas: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
    Serial.println(); //QUEBRA DE LINHA NA SERIAL
}

//<metodo>Imprimir no display a hora recebida pelo paramentro 'now'</metodo>
void geraHora(DateTime now){
  position[0]=display.encodeDigit(now.hour()/10);
  position[1]=display.encodeDigit(now.hour()%10);
  position[2]=display.encodeDigit(now.minute()/10);
  position[3]=display.encodeDigit(now.minute()%10);
  display.setSegments(position);
  delay(200);
  position[1] = position[1] + 128;
  delay(200);
  display.setSegments(position);
}

//<metodo>Imprimir no display a Data recebida pelo paramentro 'now'</metodo>
void geraData(DateTime now){
  display.clear();
  delay(10);
  position[0]=display.encodeDigit(now.day()/10);
  position[1]=display.encodeDigit(now.day()%10);
  position[2]=display.encodeDigit(now.month()/10);
  position[3]=display.encodeDigit(now.month()%10);
  position[1] = position[1] + 128;
  display.setSegments(position);
  delay(1000);
}


//----------Definição de metodos para configuração da musica do dispertador---------------
//<metodo>Metodo para tocar a musica do alarme</metodo>
void tocaMusica(){
  
//Toca a primeira variação
firstSection();

//Toca a segunda variação
secondSection();

//Variação 1:
beep(f, 250); 
beep(gS, 500); 
beep(f, 350); 
beep(a, 125);
beep(cH, 500);
beep(a, 375); 
beep(cH, 125);
beep(eH, 650);

delay(500);

secondSection();

//Variação 2:
beep(f, 250); 
beep(gS, 500); 
beep(f, 375); 
beep(cH, 125);
beep(a, 500); 
beep(f, 375); 
beep(cH, 125);
beep(a, 650);

delay(650);
}

void beep(int note, int duration)
{
//Toca um som no buzzer:
tone(buzzerPin, note, duration);

digitalWrite(ledPin1, HIGH);
delay(duration);
digitalWrite(ledPin1, LOW);

//Para o som no buzzer
noTone(buzzerPin);

delay(50);
}

void firstSection()
{
beep(a, 500);
beep(a, 500); 
beep(a, 500);
beep(f, 350);
beep(cH, 150); 
beep(a, 500);
beep(f, 350);
beep(cH, 150);
beep(a, 650);

delay(500);

beep(eH, 500);
beep(eH, 500);
beep(eH, 500); 
beep(fH, 350);
beep(cH, 150);
beep(gS, 500);
beep(f, 350);
beep(cH, 150);
beep(a, 650);

delay(500);
}

void secondSection()
{
beep(aH, 500);
beep(a, 300);
beep(a, 150);
beep(aH, 500);
beep(gSH, 325);
beep(gH, 175);
beep(fSH, 125);
beep(fH, 125); 
beep(fSH, 250);

delay(325);

beep(aS, 250);
beep(dSH, 500);
beep(dH, 325); 
beep(cSH, 175); 
beep(cH, 125); 
beep(b, 125); 
beep(cH, 250); 

delay(350);
}
