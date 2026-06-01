#include "TebakKata.h"

Leaderboard top5[5];

string daftarKata[15] = {
    "komputer",
    "program",
    "algoritma",
    "database",
    "internet",
    "compiler",
    "monitor",
    "keyboard",
    "pointer",
    "struktur",
    "variabel",
    "function",
    "operator",
    "deklarasi",
    "rekursif"
};

void pilihKata(KataGame *game) {

    int index = rand() % 15;

    game->kataAsli = daftarKata[index];
    game->statusTebakan = "";

    for(int i = 0; i < game->kataAsli.length(); i++) {
        game->statusTebakan += '_';
    }

    game->jumlahSalah = 0;

    for(int i = 0; i < 26; i++) {
        game->tebakanSalah[i] = '\0';
    }
}

void setKesulitan(KataGame *game, int level) {

    if(level == 1)
        game->nyawa = 8;
    else if(level == 2)
        game->nyawa = 6;
    else
        game->nyawa = 4;
}

void tampilkanGame(KataGame *game) {

    cout << "\n====================================\n";
    cout << "KATA : ";

    for(int i = 0; i < game->statusTebakan.length(); i++) {
        cout << game->statusTebakan[i] << " ";
    }

    cout << "\nNYAWA : " << game->nyawa;

    cout << "\nHURUF SALAH : ";

    for(int i = 0; i < game->jumlahSalah; i++) {
        cout << game->tebakanSalah[i] << " ";
    }

    cout << "\n====================================\n";
}

bool sudahDitebak(KataGame *game, char huruf) {

    for(int i = 0; i < game->statusTebakan.length(); i++) {
        if(game->statusTebakan[i] == huruf)
            return true;
    }

    for(int i = 0; i < game->jumlahSalah; i++) {
        if(game->tebakanSalah[i] == huruf)
            return true;
    }

    return false;
}

void prosesTebakan(KataGame *game, char huruf) {

    bool ditemukan = false;

    for(int i = 0; i < game->kataAsli.length(); i++) {

        if(game->kataAsli[i] == huruf) {

            game->statusTebakan[i] = huruf;
            ditemukan = true;
        }
    }

    if(!ditemukan) {

        game->tebakanSalah[game->jumlahSalah] = huruf;
        game->jumlahSalah++;

        game->nyawa--;
    }
}

bool cekMenang(KataGame *game) {

    return game->kataAsli == game->statusTebakan;
}

void gunakanHint(KataGame *game) {

    if(game->nyawa <= 1) {

        cout << "\nNyawa tidak cukup untuk menggunakan hint!\n";
        return;
    }

    game->statusTebakan[0] = game->kataAsli[0];
    game->nyawa--;

    cout << "\nHint digunakan! Huruf pertama dibuka.\n";
}

int hitungSkor(KataGame *game, int level) {

    int pengali;

    if(level == 1)
        pengali = 10;
    else if(level == 2)
        pengali = 20;
    else
        pengali = 30;

    return game->nyawa * pengali;
}

void updateLeaderboard(string nama, int skor) {

    for(int i = 0; i < 5; i++) {

        if(skor > top5[i].skor) {

            for(int j = 4; j > i; j--) {
                top5[j] = top5[j - 1];
            }

            top5[i].nama = nama;
            top5[i].skor = skor;

            break;
        }
    }
}

void tampilkanLeaderboard() {

    cout << "\n========== TOP 5 SCORE ==========\n";

    for(int i = 0; i < 5; i++) {

        if(top5[i].skor > 0) {

            cout << i + 1 << ". "
                 << top5[i].nama
                 << " - "
                 << top5[i].skor
                 << endl;
        }
    }
}

void bersihkanLayar() {

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

}

int main() {

    srand(time(0));

    char mainLagi;

    do {

        KataGame game;
        int level;

        cout << "====================================\n";
        cout << "      PROJECT 2 : GAME TEBAK KATA\n";
        cout << "         MYSTERY WORD QUEST\n";
        cout << "====================================\n";

        cout << "Pilih Tingkat Kesulitan\n";
        cout << "1. Easy (8 Nyawa)\n";
        cout << "2. Medium (6 Nyawa)\n";
        cout << "3. Hard (4 Nyawa)\n";
        cout << "Pilihan : ";
        cin >> level;

        while (level < 1 || level > 3) {
            cout << "Pilihan tidak valid. Masukkan 1-3 : ";
            cin >> level;
        }

        pilihKata(&game);
        setKesulitan(&game, level);

        while (game.nyawa > 0 && !cekMenang(&game)) {

            bersihkanLayar();
            tampilkanGame(&game);

            char huruf;

            cout << "\nMasukkan huruf tebakan\n";
            cout << "(ketik ? untuk hint): ";
            cin >> huruf;

            huruf = tolower(huruf);

            if (huruf == '?') {

                gunakanHint(&game);

                cout << "\nTekan Enter untuk lanjut...";
                cin.ignore();
                cin.get();

                continue;
            }

            if (sudahDitebak(&game, huruf)) {

                cout << "\nHuruf sudah pernah ditebak!\n";

                cout << "Tekan Enter untuk lanjut...";
                cin.ignore();
                cin.get();

                continue;
            }

            prosesTebakan(&game, huruf);
        }

        bersihkanLayar();

        if (cekMenang(&game)) {

            cout << "\n====================================\n";
            cout << "ANDA MENANG!\n";
            cout << "Kata yang ditebak : "
                 << game.kataAsli
                 << endl;

            string nama;

            cout << "Masukkan Nama : ";
            cin >> nama;

            int skor = hitungSkor(&game, level);

            cout << "Skor Anda : "
                 << skor
                 << endl;

            updateLeaderboard(nama, skor);
        }
        else {

            cout << "\n====================================\n";
            cout << "ANDA KALAH!\n";
            cout << "Kata Rahasia : "
                 << game.kataAsli
                 << endl;
        }

        tampilkanLeaderboard();

        cout << "\nMain lagi? (Y/N) : ";
        cin >> mainLagi;

    } while (toupper(mainLagi) == 'Y');

    cout << "\nTerima kasih telah bermain.\n";

    return 0;
}