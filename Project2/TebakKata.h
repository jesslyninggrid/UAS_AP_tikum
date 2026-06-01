#ifndef TEBAKKATA_H
#define TEBAKKATA_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

/*

Project     : Project2
Game        : Mystery Word Quest
File        : TebakKata.h
Deskripsi   : Deklarasi struct dan fungsi
=========================================

*/

struct KataGame {
string kataAsli;
string statusTebakan;
int nyawa;
char tebakanSalah[26];
int jumlahSalah;
};

struct Leaderboard {
string nama;
int skor;
};

extern Leaderboard top5[5];

void pilihKata(KataGame *game);
void setKesulitan(KataGame *game, int level);
void tampilkanGame(KataGame *game);
bool sudahDitebak(KataGame *game, char huruf);
void prosesTebakan(KataGame *game, char huruf);
bool cekMenang(KataGame *game);
void gunakanHint(KataGame *game);
int hitungSkor(KataGame *game, int level);
void updateLeaderboard(string nama, int skor);
void tampilkanLeaderboard();
void bersihkanLayar();

#endif
