#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

string konversiHuruf(string teks) {
    for (int i = 0; i < teks.length(); i++) {
        teks[i] = tolower(teks[i]); 
    }
    return teks;
}

string sensorNegara(string negara_asli) {
    string teks_sensor = negara_asli;
    int n = negara_asli.length(); 
    
    int jml_hilang = rand() % (n / 2) + 1; 

    int terhapus = 0;
    while (terhapus < jml_hilang) {
        int idx_acak = rand() % n; 
        
        if (teks_sensor[idx_acak] != '_') {
            teks_sensor[idx_acak] = '_';
            terhapus++;
        }
    }
    return teks_sensor;
}

int main() {
    srand(time(0)); 

    vector<string> list_negara = {
        "indonesia", "malaysia", "brunei", "thailand", "vietnam", 
        "philippines", "singapore", "laos", "germany", "brazil",
        "china", "korea", "egypt", "france", "italy", 
        "india", "russia", "canada", "mexico", "spain", 
        "norway", "turkey", "qatar", "swiss", "england", 
        "oman", "portugal", "iran", "australia", "argentina" 
    };

    int skor = 0; 
    char opsi = 'y'; 

    while (opsi == 'y' || opsi == 'Y') {
        int idx = rand() % list_negara.size();
        string kunci_jawaban = list_negara[idx];

        string soal = sensorNegara(kunci_jawaban);
   
        cout << "Tebak negara: ";
        for (int i = 0; i < soal.length(); i++) {
            cout << soal[i] << " "; 
        }
        cout << endl;
        
        cout << "Negara apakah yang dimaksud? ";
        string tebakan; 
        cin >> tebakan;

        if (konversiHuruf(tebakan) == konversiHuruf(kunci_jawaban)) {
            cout << "Selamat! Anda benar." << endl;
            skor++;
        } else {
            cout << "Maaf, jawaban Anda salah. Coba lagi." << endl;
        }

        cout << "Skor Anda: " << skor << endl;
        cout << "Ingin bermain lagi? (y/n): ";
        cin >> opsi;
        cout << endl;
    }

    cout << "Game Selesai :D" << endl;

    return 0;
}