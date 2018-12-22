#include "LedControl.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER
#define POTENTIOMETRU 9
#define JOYSTICK1 A4
#define JOYSTICK2 A5
#define  TIMP 700

int hi[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 0, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 1, 1, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int emoticonFericit[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}
};

int emoticonTrist[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0}
};

struct paleta {
  int xPaleta, yPaleta;
};

struct Bila {
  int xBila, yBila;
  int directieX, directieY;
};

struct Jucator {
  int scor;
};
int k = 1;
long timpCurent;
long ultimaMiscarePentruBila;
long timpPentruUltimaActualizare = 0;
long timpPentruMiscarePaleta = 0;
long timpPentruMiscarePaleta2 = 0;
int valY1;
int valY2;
int dimensiunePaleta = 3;
int pornireBila = 0;
int vitezaPaleta = 100;
int vitezaBila = 500;
int verifJoc = 0;
int vitezaAdd = 0;
paleta paleta1[3] = { {2 , 0}, {3 , 0}, {4 , 0} };
paleta paleta2[3] = { {2 , 7}, {3 , 7}, {4 , 7} };
Bila bila;
Jucator jucator1, jucator2;
void deseneazaPaleta() {
  for (int i = 0; i < dimensiunePaleta; i++) {
    lc.setLed(0, paleta1[i].xPaleta, paleta1[i].yPaleta, true);
    lc.setLed(0, paleta2[i].xPaleta, paleta2[i].yPaleta, true);
  }
}

void pornesteBila() {
  bila.xBila = random(3, 5);
  bila.yBila = random(3, 5);
  int xAleatoriu = random(0, 2);
  int yAleatoriu = random(0, 2);
  if (xAleatoriu % 2 == 0)
    bila.directieX = 1;
  else
    bila.directieX = -1;
  if (yAleatoriu % 2 == 0)
    bila.directieY = 1;
  else
    bila.directieY = -1;
  pornireBila = 1;
}

void deseneazaBila() {
  lc.setLed(0, bila.xBila, bila.yBila, true);
}

void scadePaleta(paleta p[], int n) {
  if (p[0].xPaleta == 0)
    return;
  for (int i = 0; i < dimensiunePaleta; i++) {
    p[i].xPaleta = p[i].xPaleta - n;
  }
} void verificareScor() {
  //Crestere scoruri pentru playeri si afisare pe lcd
  if (bila.yBila <= 0 || bila.yBila >= 7) {

    if (bila.yBila <= 0)
    {
      lcd.clear();
      jucator2.scor++;
      lcd.setCursor(0, 0);
      lcd.print("WIN jucator2");
      lcd.setCursor(0, 1);
      lcd.print(jucator2.scor);
    }
    else if (bila.yBila >= 7)
    {
      lcd.clear();
      jucator1.scor++;
      lcd.setCursor(0, 0);
      lcd.print("WIN jucator1");
      lcd.setCursor(0, 1);
      lcd.print(jucator1.scor);

    }
    pornireBila = 0;
    verifJoc = 0;
    vitezaAdd = 0;

  }
}


void coliziuneaZid() {
  if (bila.xBila <= 0 || bila.xBila >= 7)
  {
    bila.directieX = bila.directieX * (-1);
  }
}



void coliziuneaPaleta() {
  if (bila.xBila >= paleta1[0].xPaleta && bila.xBila <= paleta1[dimensiunePaleta - 1].xPaleta && bila.yBila == paleta1[0].yPaleta + 1) {  //coliziune paleta1
    bila.directieY =  bila.directieY * (-1);
    //schimba directia pe axa X
    vitezaAdd =  vitezaAdd + 75;
  }

  if (bila.xBila >= paleta2[0].xPaleta && bila.xBila <= paleta2[dimensiunePaleta - 1].xPaleta && bila.yBila == paleta2[0].yPaleta - 1) {   //coliziune paleta2
    bila.directieY = bila.directieY * (-1);
    //schimba directia pe axa X
    vitezaAdd =  vitezaAdd + 75;
  }
}

void miscarePalete() {
  valY1 = analogRead(JOYSTICK1);
  valY2 = analogRead(JOYSTICK2);
  if (millis() - timpPentruMiscarePaleta > vitezaPaleta)
  {
    if (valY1 < 400 )
    {
      crestePaleta(paleta1, 1);
    }
    if (valY1 > 600) 
    {
      scadePaleta(paleta1, 1);
    }
    timpPentruMiscarePaleta = millis();
  }
  if (millis() - timpPentruMiscarePaleta2 > vitezaPaleta) {
    if (valY2 < 400)
    {
      crestePaleta(paleta2, 1);
    }
    if (valY2 > 600)
    {
      scadePaleta(paleta2, 1);
    }
    timpPentruMiscarePaleta2 = millis();
  }
}
void crestePaleta(paleta p[], int n) {
  if (p[dimensiunePaleta - 1].xPaleta == 7)
    return;
  for (int i = 0; i < dimensiunePaleta; i++) {
    p[i].xPaleta = p[i].xPaleta + n;
  }
}


void miscaBila() {
  bila.xBila = bila.xBila + bila.directieX;
  bila.yBila = bila.yBila + bila.directieY;
  ultimaMiscarePentruBila = millis();
  coliziuneaPaleta();
  coliziuneaZid();
  verificareScor();
}

void deseneazaJoc() {
  if (millis() - timpPentruUltimaActualizare > 100) {
    lc.clearDisplay(0);
    deseneazaPaleta();
    deseneazaBila();
    miscarePalete();
    if (millis() - ultimaMiscarePentruBila > vitezaBila - vitezaAdd)
      miscaBila();
    timpCurent = millis();
    timpPentruUltimaActualizare = millis();
  }
}

void joc() {
  deseneazaJoc();
}

void matrice(int m[8][8]) {
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      lc.setLed(0, i, j, m[i][j]);
}

void conometru() {
  timpCurent = millis();
  matrice(hi);
  while (millis() < timpCurent + 3 * TIMP) {}
  matrice(emoticonFericit);
  while (millis() < timpCurent + 4 * TIMP) {}

}

void paletaInitializare() {
  for (int i = 0; i < 3; i++) {
    paleta1[i].xPaleta = i + 2;
    paleta1[i].yPaleta = 0;
    paleta2[i].xPaleta = i + 2;
    paleta2[i].yPaleta = 7;
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(POTENTIOMETRU, OUTPUT);
  analogWrite(POTENTIOMETRU, 90);
  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);
  Serial.begin(9600);
  randomSeed(analogRead(2));
  jucator1.scor = 0;
  jucator2.scor = 0;

}

void loop() {

  if (!verifJoc) {
    if (k == 1) {
      matrice(emoticonTrist);
      k == 2;
    }
    while (millis() < timpCurent + 1000) {}
    conometru();
    paletaInitializare();
    verifJoc = 1;
  } else {
    if (pornireBila == 0) pornesteBila();
    joc();

  }

}
