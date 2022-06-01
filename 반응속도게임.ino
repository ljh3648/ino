#include <stdlib.h>

// 프로게이머를 위한 반응속도 게임!
#define C 262 // 도 
#define D 294 // 레 
#define E 330 // 미 
#define F 349 // 파 
#define G 392 // 솔 
#define A 440 // 라 
#define B 494 // 시 

// LED 디지털핀 지정
int led_D[3] = { 8,9,10 };

// SW 디지털핀 지정
int sw_D[3] = { 5,6,7 };

// 피에조 디지털핀 지정
#define BUZ 4

// 게임 소리
int game_start[] = { C,D,E,F,G,F,E,D,C }; // 게임 시작 시 소리
int game_value[10] = { C,D,E,F,G,A,B,C,D,E }; // 게임 난이도 표시 소리
int game_end[] = { F,F,F,F,F,F,F,F }; // 게임 패배 소리
int game_clear[] = { G, G, A, A, G, G, E, G, G, E, E, D, G, G, A, A, G, G, E, G, E, D, E, C }; // 게임 클리어 소리 "학교종일땡땡땡"

// 단계 난이도
int level_DFCLT[10] = { 2000,1500,1000,800,700,600,500,400,300,150};

void setup() {
    digitalWrite(12,HIGH);
    Serial.begin(9600);
    
    for (int i = 0; i < 3; i++) {
        pinMode(led_D[i], OUTPUT);
        pinMode(sw_D[i], INPUT_PULLUP);
    }
    pinMode(BUZ, OUTPUT);

    //리셋
    delay(1000);
    pinMode(12, OUTPUT);
}

void loop() {
    bool game_ruling = true; //게임 승패 확인 승리 true, 패배 false

    GAME_SOUND(1, 0); // 게임시작 소리
    LED_ON_ALL();     // LED 3초간 점등후 소등
    

    for (int level=1; level<=10; level++) { //i는 난이도 1단계부터 10단계 까지.
        GAMELOOP(level);
    }
    
    GAME_SOUND(4,0); // 게임클리어 소리
}

// 게임진행되는 곳
void GAMELOOP(int level) {
    int r = GAME_Q_RAND(); // 랜덤배정 받는 변수
    bool game_ruling = true; //게임 승패 확인 승리 true, 패배 false

    GAME_SOUND(2, level); // 1단계 시작 음
    LED_ON_R(r);
    game_ruling = GAME_READING(r, level_DFCLT[level]);// 게임 판정 보내기
    if (game_ruling == false) {
        LED_OFF_ALL();
        GAME_SOUND(3, 0); // 종료 브금
        GAMEOVER();
    }
    LED_OFF_ALL();
    delay(1000);
}

// 게임 승패판정
bool GAME_READING(int r, int g_time) {
    bool game_ruling = false;
    unsigned int time_previous = millis();
    unsigned int time_current = millis();
    while (time_current - time_previous <= g_time) {
        if (digitalRead(sw_D[r]) == LOW) { //LED 켜진 위치와 눌린 스위치가 같다면
            return game_ruling = true; // 성공
        }

        time_current = millis();
    }

    return game_ruling = false; // 패배
}

int GAME_Q_RAND(void){ // 게임 문제 랜덤 생성
  int r = rand()%3;
  return r;
}

// LED 제어 함수
void LED_ON_R(int r) { // 랜덤배정 받은 LED를 켜는곳
    digitalWrite(led_D[r], HIGH);
}

void LED_ON_ALL() { // LED 3초간 점등후 소등
    for (int i = 0; i < 3; i++) {
        digitalWrite(led_D[i], HIGH);
    }
    delay(3000);
    LED_OFF_ALL();
}

void LED_OFF_ALL() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(led_D[i], LOW);
    }
}

// 게임의 사운드 관리 함수
// song은 음악 트랙 선택, cnt는 게임 단계에 맞는 소리의 갯수를 가져옴.
void GAME_SOUND(int song, int cnt) {
    switch (song) {
    case 1:
        for (int i = 0; i < sizeof(game_start) / 2; i++) {
            tone(BUZ, game_start[i], 200);
            delay(300);
        }
        break;
    case 2:
        for (int i = 0; i < cnt; i++) {
            tone(BUZ, game_value[i], 200);
            delay(300);
        }
        break;
    case 3:
        for (int i = 0; i < sizeof(game_end) / 2; i++) {
            tone(BUZ, game_end[i], 200);
            delay(300);
        }
        break;
    case 4:
        for (int i = 0; i < sizeof(game_clear) / 2; i++) {
            tone(BUZ, game_clear[i], 200);
            delay(300);
        }
        break;
    }
}

// 게임 리셋 초기화
void GAMEOVER() {
    digitalWrite(12, LOW);
}
