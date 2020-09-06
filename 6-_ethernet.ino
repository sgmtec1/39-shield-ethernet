// Open Source
// TESTE COM SHIELD ETHERNET

#include <SPI.h> //INCLUSÃO DE BIBLIOTECA
#include <Ethernet.h> //INCLUSÃO DE BIBLIOTECA
#include <LiquidCrystal_I2C.h> //INCLUSÃO DE BIBLIOTECA
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //ATRIBUIÇÃO DE ENDEREÇO MAC AO ETHERNET SHIELD W5100
byte ip[] = { 192, 168, 0, 208 }; //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110  **** ISSO VARIA, NO MEU CASO É: 192.168.0.175
byte gateway[] = {192, 168, 0, 1}; //GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
byte subnet[] = {255, 255, 255, 0}; //MASCARA DE REDE (ALTERE PARA A SUA MÁSCARA DE REDE)
EthernetServer server(80); //PORTA EM QUE A CONEXÃO SERÁ FEITA

const int ledPin = 9; //PINO DIGITAL UTILIZADO PELO LED
String readString = String(30); //VARIÁVEL PARA BUSCAR DADOS NO ENDEREÇO (URL)
int status = 0; //DECLARAÇÃO DE VARIÁVEL DO TIPO INTEIRA(SERÁ RESPONSÁVEL POR VERIFICAR O STATUS ATUAL DO LED)

int Pinbuzzer = 8; //PINO UTILIZADO PELO BUZZER

void setup(){
  Ethernet.begin(mac, ip, gateway, subnet); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE
  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA 80
  pinMode(ledPin, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(ledPin, LOW); //LED INICIA DESLIGADO
  lcd.begin (16,2); //SETA A QUANTIDADE DE COLUNAS(16) E O NÚMERO DE LINHAS(2) DO DISPLAY
  lcd.setBacklight(HIGH); //LIGA O BACKLIGHT (LUZ DE FUNDO)
  pinMode(Pinbuzzer, OUTPUT); //DEFINE O PINO COMO SAÍDA
  Serial.begin(9600);//INICIALIZA A SERIAL
  }
void loop(){
  lcd.setCursor(0,0); //SETA A POSIÇÃO DO CURSOR
  lcd.print("CONTROLE REMOTO"); //IMPRIME O TEXTO NO DISPLAY LCD
  Serial.print("CONTROLE REMOTO: "); //EXIBE O TEXTO NO MONITOR SERIAL
  
  
EthernetClient client = server.available(); //CRIA UMA CONEXÃO COM O CLIENTE
  if (client) { // SE EXISTE CLIENTE, FAZ
    while (client.connected()) {//ENQUANTO EXISTIR CLIENTE CONECTADO, FAZ
   if (client.available()) { //SE O CLIENTE ESTÁ HABILITADO, FAZ
    char c = client.read(); //LÊ CARACTERE A CARACTERE DA REQUISIÇÃO HTTP
    if (readString.length() < 100) //SE O ARRAY FOR MENOR QUE 100, FAZ
      {
        readString += c; // "readstring" VAI RECEBER OS CARACTERES LIDO
      }  
        if (c == '\n') { //SE ENCONTRAR "\n" É O FINAL DO CABEÇALHO DA REQUISIÇÃO HTTP
          if (readString.indexOf("?") <0){ //SE ENCONTRAR O CARACTER "?", FAZ
          }
          else //SENÃO,FAZ
        if(readString.indexOf("ledParam=1") >0) //SE ENCONTRAR O PARÂMETRO "ledParam=1", FAZ
           {
             digitalWrite(ledPin, HIGH); //LIGA O LED
             digitalWrite(Pinbuzzer, HIGH); // LIGA BUZZER
             lcd.setCursor(0,1); //SETA A POSIÇÃO DO CURSOR
             lcd.print("LAMPADA LIGADO    "); //IMPRIME O TEXTO NO DISPLAY LCD
             status = 1; //VARIÁVEL RECEBE VALOR 1(SIGNIFICA QUE O LED ESTÁ LIGADO)
           }else{ //SENÃO, FAZ
             digitalWrite(ledPin, LOW); //DESLIGA O LED
             digitalWrite(Pinbuzzer, LOW); // DESLIGA BUZZER
             lcd.setCursor(0,1); //SETA A POSIÇÃO DO CURSOR
             lcd.print("LAMPADA DESLIGADO   "); //IMPRIME O TEXTO NO DISPLAY LCD
             status = 0; //VARIÁVEL RECEBE VALOR 0(SIGNIFICA QUE O LED ESTÁ DESLIGADO)             
           }
          client.println("HTTP/1.1 200 OK"); //ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
          client.println("Content-Type: text/html"); //ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
          client.println("");
          client.println("<!DOCTYPE HTML>"); //INFORMA AO NAVEGADOR A ESPECIFICAÇÃO DO HTML
          client.println("<html>"); //ABRE A TAG "html"
          client.println("<head>"); //ABRE A TAG "head"
          client.println("<link rel='icon' type='image/png' href='https://cdn.pixabay.com/photo/2014/04/03/00/35/footprint-308794_960_720.png'/>"); //DEFINIÇÃO DO ICONE DA PÁGINA
          client.println("<title>Tigre Digital</title>"); //ESCREVE O TEXTO NA PÁGINA
          client.println("</head>"); //FECHA A TAG "head"
          client.println("<body style=background-color:#E8EBFF>"); //DEFINE A COR DE FUNDO DA PÁGINA
          client.println("<center><font color=#B22222><h1>SGMTEC1</font></center></h1>"); //ESCREVE TITULO DA PAGINA                 
          client.println("<h1><font color=#4B0082><center>ACENDIMENTO REMOTO DO LED OU LAMPADA</center></h1>"); //ESCREVE "CONTROLE DE LED" NA PÁGINA
          client.println("<h1><font color=#8B008B><center> VIA REDE ETHERNET</center></h1>"); //ESCREVE "CONTROLE DE LED" NA PÁGINA
          if (status == 1){ //SE VARIÁVEL FOR IGUAL A 1, FAZ
          //A LINHA ABAIXO CRIA UM FORMULÁRIO CONTENDO UMA ENTRADA INVISÍVEL(hidden) COM O PARÂMETRO DA URL E CRIA UM BOTÃO APAGAR (CASO O LED ESTEJA LIGADO)
          client.println("<center><form method=get name=LED><input type=hidden name=ledParam value=0 /><input type=submit value=APAGAR></form></center>");
          }else{ //SENÃO, FAZ
          //A LINHA ABAIXO CRIA UM FORMULÁRIO CONTENDO UMA ENTRADA INVISÍVEL(hidden) COM O PARÂMETRO DA URL E CRIA UM BOTÃO ACENDER (CASO O LED ESTEJA DESLIGADO)
          client.println("<center><form method=get name=LED><input type=hidden name=ledParam value=1 /><input type=submit value=LIGAR></form></center>");
          }
          client.println("<center><font color=#4B0082><font size='5'>Status atual do LED: </center>"); //ESCREVE "Status atual do LED:" NA PÁGINA
          if (status == 1){ //SE VARIÁVEL FOR IGUAL A 1, FAZ
              client.println("<center><font color='green' size='20'>LIGADO</center>"); //ESCREVE "LIGADO" EM COR VERDE NA PÁGINA 
          }else{ //SENÃO, FAZ
              client.println("<center><font color='red' size='20'>DESLIGADO</center>"); //ESCREVE "DESLIGADO" EM COR VERMELHA NA PÁGINA
               }     
          client.println("<hr/>"); //TAG HTML QUE CRIA UMA LINHA HORIZONTAL NA PÁGINA
          client.println("<hr/>"); //TAG HTML QUE CRIA UMA LINHA HORIZONTAL NA PÁGINA
          client.println("</body>"); //FECHA A TAG "body"
          client.println("</html>"); //FECHA A TAG "html"
          readString=""; //A VARIÁVEL É REINICIALIZADA
          client.stop(); //FINALIZA A REQUISIÇÃO HTTP E DESCONECTA O CLIENTE
            }
          }
        }
      }
 }
