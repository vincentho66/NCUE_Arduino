
void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 36;i++)
  {
    if(i < 12){
       tl(11,1);
    }else if(i >= 12 && i <20){
      if(i % 2 == 0){
          tl(11,0);
        }else{
          tl(11,1);
        }
    } else if(i >=20 && i < 24 ){
      tl(12,1);
    }else {
      tl(13,1); 
    } 
    delay(1000/4);
    tl(13,0);
  }
  
}

void tl(int pin,int hl){
  if(hl == 0){
    digitalWrite(13,LOW);
    digitalWrite(12,LOW);
    digitalWrite(11,LOW);
  }else{
    digitalWrite(pin,HIGH);
  }
  
}
