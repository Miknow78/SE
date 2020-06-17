#define lightSensor A0
#define portao 7

int ledFlag = 0;
int lightlevel;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(portao, OUTPUT);
  digitalWrite(portao, HIGH);
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
  ledFlag = 3;
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
  delay(1000);
}
