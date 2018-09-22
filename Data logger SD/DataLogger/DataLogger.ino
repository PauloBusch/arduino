#include <Thermistor3.h>
#include <SD.h>                        //Biblioteca SD inclusa
Thermistor temp(0);
double resistor = 9850;
File myFile;                           //Cria um ponteiro para arquivo


// --- Mapeamento de Hardware ---
#define CS_pin      10               //Comunicação SPI, CS_pin no digital 10
#define temp_pin    A0               //Sensor de temperatura no pino analógico 0
#define light_pin   A1               //Sensor de luminosidade no pino analógico 1
#define Led         2

// --- Protótipo das Funções Auxiliares ---
//int AVERAGE_TEMP();                  //Função para calcular a média de temperatura


// --- Variáveis Globais ---
const int voltage = 5;              //Alimentação do Arduino
//const int celsius_unity = 100;      //Variação em Celsius
const int ad_resolution = 1024;     //Resolução do conversor AD utilizada (10 bits)
/*
unsigned int store01;               //Variável de armazenamento
unsigned int resto01;               //Recebe o primeiro resto de divisão
unsigned int resto02;               //Recebe o segundo resto da divisão
unsigned int dec01;                 //Referente à primeira divisão, primeiro valor após ponto decimal
unsigned int dec02;                 //Referente à primeira divisão, primeiro valor após ponto decimal
*/
unsigned long minuto = 1;           //Armazena os minutos transcorridos


// --- Configurações Iniciais ---
void setup()
{
 // -- Inicia comunicação serial e aguarda sua abertura --
  Serial.begin(9600);
   //while (!Serial) {  ;   }


  Serial.print("Inicializando SD card.....");     //Escreve frase de inicialização
 
  pinMode(CS_pin, OUTPUT);                            //Configura saída para Chip Select
   
  if (!SD.begin(CS_pin))                              //Testa se o cartão inicializa com sucesso
  {
    Serial.println("Falha no cartao!");           //Não, falha no cartão
    return;                                       //retorna
  }
  
  Serial.println("Sucesso na inicializacao!");    //Sim, inicialização ok
  digitalWrite(Led,HIGH);
  delay(5000);
  myFile = SD.open("logger.csv", FILE_WRITE);     //Abre arquivo para escrita
  
  
  if (myFile)                                     //Arquivo aberto com sucesso?
  {                                               //Sim...
    myFile.println(", , ,"); // 
    String header = "Minuto, T_Celsius, T_Fahrenheit, Luminosidade";
    myFile.println(header);
    myFile.close();
    Serial.println(header);
  } //end if
  else                                            //Não...
  {
    
    Serial.println("Erro ao abrir arquivo");      //Informa que há algum erro
  }
  
   
  
} //end setup


// --- Loop Infinito
void loop()
{
 //Verifica nível de luminosidade
  int  lum_nivel = 0;
  double temp_c = 0;
  int m = 10; // minutos
    digitalWrite(Led,LOW);
   for(int i = 1; i <= m;i ++){
  lum_nivel += analogRead(light_pin);
  temp_c += temp.getTemp(resistor);
  delay(1000);
  }
  digitalWrite(Led,HIGH);
  lum_nivel = lum_nivel / m;
  temp_c = temp_c / m;
  double temp_f = temp_c*1.8+32;
  String dataString = String(minuto) + ", " + String(temp_c) + ", "+ String(temp_f) + ", " + String(lum_nivel-500);
  
  //Abre o arquivo para escrita
  //Apenas um arquivo pode ser aberto de cada vez
  File myFile = SD.open("logger.csv", FILE_WRITE);
  if(myFile)
  {
    myFile.println(dataString);
    myFile.close();
    Serial.println(dataString);
  } //end if logFile
  else
  {
    Serial.println("Erro ao abrir arquivo para escrita final");
  }
  
  //Incrementa os minutos
  minuto++;
} //end loop
