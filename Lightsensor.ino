#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // endereço MAC disponivel na rede
IPAdress ip(192, 168, 1, 69); // IP disponivel
EthernetServer server(80) // porta 80

#define lightSensor A0
#define portao 7

int curVal = 0;
int ledFlag = 0;
int lightlevel;

String RString;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(portao, OUTPUT);
  digitalWrite(portao, HIGH);
  Ethernet.begin(mac, ip);
  server.begin();
}

/* delay corrigido
void abrirportao(){
    digitalWrite(portao, LOW);
    Serial.println("passou");
    delay(3000);
    digitalWrite(portao, HIGH);
}

void LightDoIt(){
  int lightlevel = analogRead(lightSensor);
  Serial.println(lightlevel);
  if(lightlevel <= 90){
    abrirportao();
  }
  delay(1000);
}
*/
void abrir(){
  curVal = curVal + 1;
  ledFlag = 3000;
  digitalWrite(portao, LOW);
}

void fechar(){
  digitalWrite(portao, HIGH);
}

void lightcontrol(){
  lightlevel = analogRead(lightSensor);
  Serial.println(lightlevel);
}


void loop(){
  char data;
  lightcontrol();
  if(Serial.available()>0){
    data = Serial.read();
    switch(data){
      case '1': Serial.println("Led on"); abrir(); break;
      case '0': Serial.println("Led off"); fechar(); break;
      default: break;
    }
  }
  if(lightlevel <= 90){
    abrir();
  }
  if(ledFlag>0){
    ledFlag = ledFlag-1;
  }
  else{
    fechar();
  }

  // SERVER com o modulo de Shield Ethernet W5100
  EthernetClient client = server.available();
  if(client){
    boolean cLIB = true;
    while (client.connected()){
      if(client.available()){
        char r = client.read();

        if(RString.length() < 100) {
          RString += r;
        }
        if (r == '\n' && cLIB){ // criar pagina HTML
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE html>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Parque de estacionamento</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Numero de ocupantes</h1>");
          client.print("<p>");
          client.print(curVal, DEC);
          client.println("</p>");
          client.println("<a href=/?on2 >ON</a>");
          client.println("<a href=/?off3 >OFF</a>");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (r == '\n'){
          cLIB = true;
        }
        else if (r != '\r'){
          cLIB = false;
        }
      }
    }
  }
  delay(1);
  client.stop();
  // href=...
  if(RString.indexOf('3') >0){
    abrir();
  }
  if(RString.indexOf('4') >0){
    fechar();
  }
  RString="";
}
