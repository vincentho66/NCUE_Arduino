#define data 6
#define clk 7
#define stb 8
byte l[8] = {0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce};
byte a[8] = {0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f};
byte l_it[3][3], now_a[8];
int t[8] = { -1, -1, -1, -1, -1, -1, -1, -1};
int it[3] = {0, 0, 1}, b = 0;
bool mode = 0;
byte pre = 0x00;
void setup()
{
  pinMode(data, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(stb, OUTPUT);
  digitalWrite(stb, HIGH);
  digitalWrite(clk, HIGH);
  sendCommand(0x40);
  sendCommand(0x88);
  for (int i = 0; i < 8; i++)
  {
    setSegNum(l[i], 0x00);
  }
  l_it[0][0] = 0x20; l_it[0][1] = 0x01; l_it[0][2] = 0x02;
  l_it[1][0] = 0x10; l_it[1][1] = 0x40; l_it[1][2] = 0x04; l_it[2][1] = 0x08;
}
void loop()
{
  byte keys = get_Button();
  if (keys != pre)
  {
    pre = keys;
    if (mode == false)
    {
      for (int i = 0; i < 8; i++)
      {
        if (keys & 0x01)
        {
          for (int k = 7; k > 0; k--)
          {
            t[k] = t[k - 1];
          }
          t[0] = i;
          setSegNumadd(t);
        }
        keys >>= 1;
      }
      if (t[7] != -1)
      {
        mode = true;
        for (int i = 0; i < 8; i++)
          now_a[i] = a[t[i]];
      }
    }
    else
    {
      if (keys & 0x80)
        b = 2;
      else if (keys & 0x40)
        b = 1;
      else
        b = 0;
      for (int i = 0; i < 8; i++)
      {
        if (keys & 0x01)
        {
          if (i == 0 && it[1] > 0)
          {
            if (b == 0)
              now_a[it[0]] = now_a[it[0]] ^ l_it[it[1]][it[2]];
            else if (b == 1)
              now_a[it[0]] = now_a[it[0]] & (~l_it[it[1]][it[2]]);
            else
              now_a[it[0]] = now_a[it[0]] | l_it[it[1]][it[2]];
            it[1] -= 1;
          }
          else if (i == 1 && it[1] < 1 || i == 1 && it[1] < 2 && it[2] == 1)
          {
            if (b == 0)
              now_a[it[0]] = now_a[it[0]] ^ l_it[it[1]][it[2]];
            else if (b == 1)
              now_a[it[0]] = now_a[it[0]] & (~l_it[it[1]][it[2]]);
            else
              now_a[it[0]] = now_a[it[0]] | l_it[it[1]][it[2]];
            it[1] += 1;
          }
          else if (i == 2)
          {
            if (it[0] == 0 && it[2] == 0 || it[0] == 0 && it[1] == 2 && it[2] == 1);
            else if (it[2] == 0)
            {
              if (b == 0)
                now_a[it[0]] = now_a[it[0]] ^ l_it[it[1]][it[2]];
              else if (b == 1)
                now_a[it[0]] = now_a[it[0]] & (~l_it[it[1]][it[2]]);
              else
                now_a[it[0]] = now_a[it[0]] | l_it[it[1]][it[2]];
              it[0] -= 1;
              it[2] = 2;
            }
            else if (it[1] == 2 && it[2] == 1)
            {
              if (b == 0)
                now_a[it[0]] = now_a[it[0]] ^ l_it[it[1]][it[2]];
              else if (b == 1)
                now_a[it[0]] = now_a[it[0]] & (~l_it[it[1]][it[2]]);
              else
                now_a[it[0]] = now_a[it[0]] | l_it[it[1]][it[2]];
              it[0] -= 1;
            }
            else
            {
              if (b == 0)
                now_a[it[0]] = now_a[it[0]] ^ l_it[it[1]][it[2]];
              else if (b == 1)
                now_a[it[0]] = now_a[it[0]] & (~l_it[it[1]][it[2]]);
              else
                now_a[it[0]] = now_a[it[0]] | l_it[it[1]][it[2]];
              it[2] -= 1;
            }
          }
          else if (i == 3)
          {
            if (it[0] == 7 && it[2] == 2 || it[0] == 7 && it[1] == 2 && it[2] == 1);
            else if (it[2] == 2)
            {
              if (b == 0)
                now_a[it[0]] = now_a[it[0]] ^ l_it[it[1]][it[2]];
              else if (b == 1)
                now_a[it[0]] = now_a[it[0]] & (~l_it[it[1]][it[2]]);
              else
                now_a[it[0]] = now_a[it[0]] | l_it[it[1]][it[2]];
              it[0] += 1;
              it[2] = 0;
            }
            else if (it[1] == 2 && it[2] == 1)
            {
              if (b == 0)
                now_a[it[0]] = now_a[it[0]] ^ l_it[it[1]][it[2]];
              else if (b == 1)
                now_a[it[0]] = now_a[it[0]] & (~l_it[it[1]][it[2]]);
              else
                now_a[it[0]] = now_a[it[0]] | l_it[it[1]][it[2]];
              it[0] += 1;
            }
            else
            {
              if (b == 0)
                now_a[it[0]] = now_a[it[0]] ^ l_it[it[1]][it[2]];
              else if (b == 1)
                now_a[it[0]] = now_a[it[0]] & (~l_it[it[1]][it[2]]);
              else
                now_a[it[0]] = now_a[it[0]] | l_it[it[1]][it[2]];
              it[2] += 1;
            }
          }
        }
        keys >>= 1;
      }

    }
  }
  if (mode == true)
  {
    for (int i = 0; i < 8; i++)
      setSegNum(l[i], now_a[i]);
    delay(100);
    for (int i = 0; i < 8; i++)
    {
      if (i == it[0])
      {
        setSegNum(l[i], now_a[it[0]]^l_it[it[1]][it[2]]);
      }
      /*else
        setSegNum(l[i],now_a[i]);*/
    }
    delay(100);
  }
}
byte get_byte()
{
  byte dat = 0x00;
  pinMode(data, INPUT);
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(clk, LOW);
    if (digitalRead(data))
    {
      dat |= (0x01 << i);
    }
    digitalWrite(clk, HIGH);
  }
  pinMode(data, OUTPUT);
  return dat;
}
byte get_Button() //讀取按鍵回傳的函數
{
  byte keys = 0x00; //初設keys為0x00(表示沒有顯示資料)
  digitalWrite(stb, LOW);
  send(0x42); //開始執行從鍵盤讀入內容
  for (int i = 0 ; i < 4 ; i++) //開始分別讀取4個byte (byte0, byte1, byte2, byte3)
  {
    byte temp = get_byte();
    keys |= (temp << i); //將temp內部獲得的資料左移 i 格 然後合併到keys內部
  }
  digitalWrite(stb, HIGH);
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
void setSegNumadd(int pattern[])
{
  digitalWrite(stb, LOW);
  send(0xc0);
  for (int i = 0; i < 8; i++) {
    if (pattern[i] == -1) {
      send(0x00);
    } else {
      send(a[pattern[i]]);
    }
    send(0x00);
  }
  digitalWrite(stb, HIGH);
}
