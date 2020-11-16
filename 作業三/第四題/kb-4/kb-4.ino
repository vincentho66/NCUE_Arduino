#define data 6
#define clk 7
#define stb 8
#define g 11
#define y 12
#define r 13
byte l[8]={0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce};
byte a[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
int count[6];
int b=0,j[3],temp[3],p=1;
bool mode=0;
int x[6]={0xc0,0xc2,0xc8,0xca,0xcc,0xce};
int w=p;
byte pre=0x00;
void setup() {
  pinMode(g,OUTPUT);
  pinMode(y,OUTPUT);
  pinMode(r,OUTPUT);
  pinMode(data,OUTPUT);
  pinMode(clk,OUTPUT);
  pinMode(stb,OUTPUT);
  digitalWrite(stb,HIGH);
  digitalWrite(clk,HIGH);
  sendCommand(0x40);
  sendCommand(0x88);
  for(int i=0;i<8;i++)
  {
    setSegNum(l[i],0x00);
  }
  j[0]=temp[0]=3;
  j[1]=temp[1]=1;
  j[2]=temp[2]=5;
}
void loop()
{ 
  byte keys=get_Button();
  if(keys!=pre)
  {
    pre=keys;
    if(keys&0x80&&keys&0x40)
      {
        if(mode==0)
        {
          digitalWrite(g, 0);
          digitalWrite(y, 0);
          digitalWrite(r, 0);
          j[0]=(j[0]<0?0:j[0]);
          j[1]=(j[1]<0?0:j[1]);
          j[2]=(j[2]<0?0:j[2]);
          p=0;
          count[0]=j[0]/10;
          count[1]=j[0]%10;
          count[2]=j[1]/10;
          count[3]=j[1]%10;
          count[4]=j[2]/10;
          count[5]=j[2]%10;
          mode=1;
        }
        else if(mode==1)
        {
          j[0]=temp[0]=count[0]*10+count[1];
          j[1]=temp[1]=count[2]*10+count[3];
          j[2]=temp[2]=count[4]*10+count[5];                    
          mode=0;
        }
      }
  }
  if(mode==0)
  {
    if(keys&0x40)
    {
      j[0]=temp[0];
      j[1]=temp[1];
      j[2]=temp[2];
    }      
    setSegNum(x[0],a[j[0]/10]);
    setSegNum(x[1],a[j[0]%10]);
    setSegNum(x[2],a[j[1]/10]);
    setSegNum(x[3],a[j[1]%10]);
    setSegNum(x[4],a[j[2]/10]);
    setSegNum(x[5],a[j[2]%10]);            
    
    if(j[0]>=1)
    {
      setSegNum(x[2],0x3f);
      setSegNum(x[3],0x3f);
      setSegNum(x[4],0x3f);
      setSegNum(x[5],0x3f);
      digitalWrite(g, 1);
      delay(1000);
      j[0]--;   
    }
    else if(j[1]>=1)
    {
      setSegNum(x[4],0x3f);
      setSegNum(x[5],0x3f);      
      digitalWrite(g, 0);
      if(j[1]<=3)
      {
        digitalWrite(y, 1);
        delay(500);
        digitalWrite(y, 0);
        delay(500);
      }
      else
      {
        digitalWrite(y, 1);
        delay(1000);
      }
      j[1]--;   
    }
    else if(j[2]>=1)
    {
      digitalWrite(y, 0);
      digitalWrite(r, 1);
      delay(1000);
      digitalWrite(r, 0);
      j[2]--;
    }
  }
  if(mode==1)
  {
    if(keys&0x20&&!(keys&0x80))
    {
      if(p==0)
        p=4;
      else
        p-=2;
    }
    if(keys&0x40&&!(keys&0x80))
    {
      if(p==4)
        p=0;
      else
        p+=2;
    }    
    int i;
    b=(keys&0x80?5:0);
    for(i=0;i<8;i++)
    {
      if(keys&0x01)
      {
        if(i==0||i==1||i==2||i==3||i==4)
        {
          w=(w==p?p+1:p);
          if(i==4&&b==5)
            count[w]=0;
          else
            count[w]=i+b+1;
        }
      }
      keys>>=1;
    }
    for(int j=0;j<6;j++)
      setSegNum(x[j],a[count[j]]);
    delay(100);
    for(int j=p;j<p+2;j++)
      setSegNum(x[j],0x00);
    delay(100);
  }
}
byte get_byte()
{
  byte dat=0x00;
  pinMode(data,INPUT);
  for(int i=0;i<8;i++)
  {
    digitalWrite(clk,LOW);
    if(digitalRead(data))
    {
      dat|=(0x01<<i);
    }
    digitalWrite(clk,HIGH);
  }
  pinMode(data,OUTPUT);
  return dat;
}
byte get_Button() //讀取按鍵回傳的函數
{
  byte keys = 0x00; //初設keys為0x00(表示沒有顯示資料)
  digitalWrite(stb,LOW);
  send(0x42); //開始執行從鍵盤讀入內容
  for(int i = 0 ; i < 4 ; i++) //開始分別讀取4個byte (byte0, byte1, byte2, byte3)
  {
    byte temp = get_byte(); 
    keys |= (temp << i); //將temp內部獲得的資料左移 i 格 然後合併到keys內部
  }
  digitalWrite(stb,HIGH);
  return keys; 
}
void send(byte data1) 
{
  for (int i = 0; i < 8; i++) {
    digitalWrite(clk, LOW);
    digitalWrite(data, (data1 & 1 ? HIGH : LOW));
    data1 >>= 1;
    digitalWrite(clk, HIGH);
  }
}
void sendCommand(byte cmd)
{
  digitalWrite(stb, LOW); 
  send(cmd);
  digitalWrite(stb, HIGH);
}
void setSegNum(byte seg_address, byte pattern)
{
  digitalWrite(stb, LOW);//開始進行輸入
  send(seg_address);//指定寫入的七段顯示器位置
  send(pattern);//寫入圖案
  digitalWrite(stb, HIGH);//結束輸入
}
