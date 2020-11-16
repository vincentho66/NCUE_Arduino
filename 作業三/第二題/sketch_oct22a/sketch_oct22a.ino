#define data 6
#define clk 7
#define stb 8
byte l[8]={0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce};
byte a[10]={0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x3f};
int t[8]={-1,-1,-1,-1,-1,-1,-1,-1};
int b=0;
byte pre=0x00;
void setup() {
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
}
void loop() {
  byte keys=get_Button();
  if(keys!=pre)
  {
   pre=keys;
    if(keys&0x80)
    {
      b=5;
    }
    else
    {
      b=0;
    }
   for(int i=0;i<8;i++)
   {
    if(keys&0x01)
    {
      if(i==0||i==1||i==2||i==3||i==4)
      {
        for(int k=7;k>0;k--)
        {
          t[k]=t[k-1];
        }
        t[0]=i+b;
      }
      
      
      for(int j=0;j<8;j++)
      {
        if(t[j]==-1)
          setSegNum(l[j],0x00);
        else
        {
          setSegNum(l[j],a[t[j]]);
        }
          
      }
    }
    keys>>=1;
   }
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
