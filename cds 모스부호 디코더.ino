#include <LiquidCrystal_I2C.h>


//파일이 일부 손상되어 일부 코드만 올렸습니다.

#define bright 60 // 입력 밝기조절
#define MORSE_SIZE 4
#define MORSE_EA 26 //알파벳 갯수

int DOT = 100;
int DASH = DOT * 4;
int OVER = DOT / 2;
int interval = DOT * 5;
int space = DOT * 7;
int dot_min = DOT - OVER;
int dot_max = DOT + OVER;
int dash_min = DASH - OVER;
int dash_max = DASH + OVER;

#define BUZ 3
#define SW 2 // 인트럽트
LiquidCrystal_I2C lcd(0x27,16,2);

unsigned long end_time = 0;
bool re = false; // 스바이스바 역할
int lcd_c = 0 ; // \n 관련

int morse[MORSE_EA][MORSE_SIZE+1]= {
  {1,2,0,0,65},{2,1,1,1,66},{2,1,2,1,67},{2,1,1,0,68},{1,0,0,0,69},{1,1,2,1,70},
  {2,2,1,0,71},{1,1,1,1,72},{1,1,0,0,73},{1,2,2,2,74},{2,1,2,0,75},{1,2,1,1,76},
  {2,2,0,0,77},{2,1,0,0,78},{2,2,2,0,79},{1,2,2,1,80},{2,2,1,2,81},{1,2,1,0,82},
  {1,1,1,0,83},{2,0,0,0,84},{1,1,2,0,85},{1,1,1,2,86},{1,2,2,0,87},{2,1,1,2,88},
  {2,1,2,2,89},{2,2,1,1,90}
};// 알파벳 26개

void setup() {
  pinMode(BUZ, OUTPUT);  
  pinMode(SW, INPUT_PULLUP); //
  attachInterrupt(digitalPinToInterrupt(SW),morse_wpm,FALLING); 
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
}

void loop() {
  char s_ch = 64;
  int w_morse[MORSE_SIZE] = {};

  while(true){
    for(int i=0; i<MORSE_SIZE; i++) w_morse[i] = 0;
    
    write_morse(w_morse);
    s_ch = translation_morse(w_morse);
    newline_lcd();
    lcd.print(s_ch);
    lcd_c++;
    for(int i=0; i<MORSE_SIZE; i++){
    Serial.print(w_morse[i]);
  }
  Serial.println();
  }

}

void write_morse(int* r_morse){
   int i = 0;
   bool start_1 = false;
   while(i<MORSE_SIZE){
      if ((millis() - end_time) > space - (OVER*2) && re == true){
        re = false;
        if(lcd_c == 16) break;
        lcd.print(" ");
        lcd_c++;
      }   
      Serial.println(analogRead(A0));
      if(analogRead(A0) <= bright){// a[i] 기록시작
        start_1 = true;
        unsigned long t_millis = millis();  // 0초인 상황
        digitalWrite(BUZ,HIGH);
        while(analogRead(A0) <= bright){}  // 불빛들어오면 계속멈춤
        digitalWrite(BUZ,LOW);
        unsigned long tw_millis = millis() - t_millis; // 불빛들어오는거 끝나면 시간기록
        r_morse[i] = dot_dash_morse(tw_millis);
        i++;
        end_time = millis();
        re = true;
      } else if(millis() - end_time > interval - (OVER*3) && start_1 == true) break;
   }
}

int dot_dash_morse(unsigned long tw_millis){
   if(tw_millis > dot_min && tw_millis < dot_max) return 1; // dot
   else if( tw_millis > dash_min && tw_millis < dash_max) return 2; // dash
   return 0; // error
}

// 모스부호 4개를 보내면 해석해서 단어로 리턴해줌 만약 결과가 도출되지 않으면 @으로 리턴
char translation_morse(int* read_morse){
  for(int i=0; i<MORSE_EA; i++){
    bool a = false;
    for(int j=0; j<MORSE_SIZE; j++){
      if(read_morse[j] == morse[i][j]) a = true;
      else {
        a = false;
        break;
      }
    }
    if(a == true) return morse[i][MORSE_SIZE];
  }
  return 64;
}
 
void newline_lcd(){
  if(lcd_c == 16)lcd.setCursor(0,1);
  else if(lcd_c >= 32){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd_c=0;
  }
}

///////////////////////////////////////////
//////////////WPM 조정/////////////////////
////////////2022-12-12////////////////////

void morse_wpm(){
  interrupts();
  Serial.println("WPM");
  delayMicroseconds(2000);
  if(digitalRead(SW) != LOW) return;
  morse_input_dot();
  morse_input_dash();
  morse_dot_dash_check();
  OVER = DOT / 2;
  interval = DOT * 5;
  space = DOT * 7;
  dot_min = DOT - OVER;
  dot_max = DOT + OVER;
  dash_min = DASH - OVER;
  dash_max = DASH + OVER;
}

void morse_input_check(){
  while(!(analogRead(A0) <= bright));
}

void morse_uninput_check(){
  while(analogRead(A0) <= bright);
}


void morse_input_dot(){
  unsigned long t_now = 0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Input dot.....");

  morse_input_check(); //빛 들어옴
  t_now = millis();
  morse_uninput_check(); //빛 안들어옴
  DOT = millis() - t_now;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("dot entered");
  lcd.setCursor(0,1);
  lcd.print("...");
  lcd.print(DOT);

  delay(1000);
}

void morse_input_dash(){
  unsi
