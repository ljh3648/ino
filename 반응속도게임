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
//#define D1 8
//#define D2 9
//#define D3 10
int led_D[3] = { 8,9,10 };

// SW 디지털핀 지정
//#define SW1 5
//#define SW2 6
//#define SW3 7
int sw_D[3] = { 5,6,7 };

#define BUZ 4

// 게임 소리
int game_start[] = { C,D,E,F,G,F,E,D,C };
int game_value[10] = { C,D,E,F,G,A,B,C,D,E };
int game_end[] = { F,F,F,F,F,F,F,F };
int game_clear[] = { G, G, A, A, G, G, E, G, G, E, E, D, G, G, A, A, G, G, E, G, E, D, E, C };
// 단계 난이도
int level_DFCLT[10] = { 2000,1500,1000,800,700,600,500,400,400,400};

void setup() {
    // put your setup code here, to run once:
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
    int r = 0; // 랜덤 배정 확인
    bool game_ruling = true; //게임 승패 확인 승리 true, 패배 false

    GAME_SOUND(1, 0); // 게임시작 소리 내기
    LED_ON_ALL();

    for (int i=1; i<=10; i++) {
        gameloop(i);
    }
    GAME_SOUND(4,0);
}

int LED_ON_R(void) {
    int r = rand() % 3;
    digitalWrite(led_D[r], HIGH);
    return r;
}

bool GAME_READING(int r, int g_time) {
    bool game_ruling = false;
    unsigned int time_previous = millis();
    unsigned int time_current = millis();
    while (time_current - time_previous <= g_time) {
        Serial.println("While문 진입");
        Serial.println(digitalRead(sw_D[0]));
        Serial.println(digitalRead(sw_D[1]));
        Serial.println(digitalRead(sw_D[2]));
        if (digitalRead(sw_D[r]) == LOW) { //LED 켜진 위치와 눌린 스위치가 같다면
            return game_ruling = true; // 성공
        }

        time_current = millis();
    }

    return game_ruling = false; // 패배
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

void gameloop(int level) {
    int r = 0; // 랜덤 배정 확인
    bool game_ruling = true; //게임 승패 확인 승리 true, 패배 false

    GAME_SOUND(2, level); // 1단계 시작 음
    r = LED_ON_R();
    game_ruling = GAME_READING(r, level_DFCLT[level]);// 스위치 입력 받기
    if (game_ruling == false) {
        LED_OFF_ALL();
        GAME_SOUND(3, 0); // 종료 브금
        Serial.println("실패");
        gameover();
    }
    else if (game_ruling == true) {
        Serial.println("성공!");
    }
    LED_OFF_ALL();
    delay(1000);
}

void gameover() {
    digitalWrite(12, LOW);
}
