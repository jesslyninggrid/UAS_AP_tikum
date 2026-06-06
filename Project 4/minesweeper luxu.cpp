#include <iostream>
#include <cstdlib>
#include <ctime>

const int MAX_SIZE = 10;

//simbolnya
const char SYM_HIDDEN  = '.';
const char SYM_FLAG    = 'F';
const char SYM_BOMB    = '*';
const char SYM_EXPLODE = 'X';

// ganti int ke string (ganti std::to_string)
std::string intToStr(int n) {
    if (n == 0) return "0";
    bool neg = (n < 0);
    if (neg) n = -n;
    std::string s = "";
    while (n > 0) {
        s = (char)('0' + n % 10) + s;
        n /= 10;
    }
    if (neg) s = "-" + s;
    return s;
}

//class game
class Game {
private:
    //state papannya
    int  size;
    int  totalBombs;

    bool hasBomb  [MAX_SIZE][MAX_SIZE];
    bool isOpen   [MAX_SIZE][MAX_SIZE];
    bool isFlagged[MAX_SIZE][MAX_SIZE];
    int  adjacent [MAX_SIZE][MAX_SIZE];

    // state gamenya
    bool gameOver;
    bool gameWon;
    int  flagsPlaced;
    int  openedSafe;
    int  totalSafe;

    // bom gerak
    bool movingBombsOn;   // fitur aktif atau tidak
    int  moveInterval;    // gerak setiap berapa langkah
    int  stepCount;       // langkah yang sudah dilakukan
    bool bombJustMoved;   // untuk notifikasi ke pemain

	//
    time_t startTime;

    //papan
    void resetBoard() {
        for (int r = 0; r < size; r++)
            for (int c = 0; c < size; c++) {
                hasBomb  [r][c] = false;
                isOpen   [r][c] = false;
                isFlagged[r][c] = false;
                adjacent [r][c] = 0;
            }
        gameOver      = false;
        gameWon       = false;
        flagsPlaced   = 0;
        openedSafe    = 0;
        totalSafe     = size * size - totalBombs;
        stepCount     = 0;
        bombJustMoved = false;
    }

    void placeBombs() {
        int placed = 0;
        while (placed < totalBombs) {
            int r = rand() % size;
            int c = rand() % size;
            if (!hasBomb[r][c]) {
                hasBomb[r][c] = true;
                placed++;
            }
        }
    }

    void calcAdjacent() {
        int dr[] = {-1,-1,-1, 0, 0, 1, 1, 1};
        int dc[] = {-1, 0, 1,-1, 1,-1, 0, 1};
        for (int r = 0; r < size; r++)
            for (int c = 0; c < size; c++) {
                if (hasBomb[r][c]) { adjacent[r][c] = -1; continue; }
                int cnt = 0;
                for (int d = 0; d < 8; d++) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (nr >= 0 && nr < size && nc >= 0 && nc < size && hasBomb[nr][nc])
                        cnt++;
                }
                adjacent[r][c] = cnt;
            }
    }


    /*  bom bergeraj
      setiap bom yang belum terbuka mencoba bergeser 1 kotak
      ke arah acak (8 arah). kotak tujuan harus:
        - dalam batas papan
        - belum dibuka user
        - bukan bom lain (tidak boleh tumpuk)
      lalu jika bom berhasil pindah ke kotak bertanda F:
        - Flag terlepas otomatis (fair warning)
      jika bom pindah ke kotak yang sedang terbuka:
        - game over (bom meledak di bawah kaki user) */

    void moveBombs() {
        // kumpulkan semua posisi bom saat ini
        int bombR[MAX_SIZE * MAX_SIZE];
        int bombC[MAX_SIZE * MAX_SIZE];
        int bombCount = 0;
        for (int r = 0; r < size; r++)
            for (int c = 0; c < size; c++)
                if (hasBomb[r][c]) {
                    bombR[bombCount] = r;
                    bombC[bombCount] = c;
                    bombCount++;
                }

        // acak urutan gerak bom agar tidak saling menghalangi secara berurutan
        for (int i = bombCount - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tr = bombR[i]; bombR[i] = bombR[j]; bombR[j] = tr;
            int tc = bombC[i]; bombC[i] = bombC[j]; bombC[j] = tc;
        }

        int dr[] = {-1,-1,-1, 0, 0, 1, 1, 1};
        int dc[] = {-1, 0, 1,-1, 1,-1, 0, 1};

        bombJustMoved = false;

        for (int i = 0; i < bombCount; i++) {
            int r = bombR[i];
            int c = bombC[i];

            //bom sudah tidak ada di sini (dipindahkan iterasi sebelumnya)
            if (!hasBomb[r][c]) continue;

            //acak 8 arah, coba satu per satu sampai ketemu yang valid
            // buat permutasi acak dari 8 arah
            int order[8] = {0,1,2,3,4,5,6,7};
            for (int k = 7; k > 0; k--) {
                int j = rand() % (k + 1);
                int tmp = order[k]; order[k] = order[j]; order[j] = tmp;
            }

            bool moved = false;
            for (int k = 0; k < 8 && !moved; k++) {
                int d  = order[k];
                int nr = r + dr[d];
                int nc = c + dc[d];

                //cek batas
                if (nr < 0 || nr >= size || nc < 0 || nc >= size) continue;
                //tidak boleh ke kotak bom lain
                if (hasBomb[nr][nc]) continue;

                // kotak tujuan valid maka pindahkan bom
                hasBomb[r][c]   = false;
                hasBomb[nr][nc] = true;
                bombJustMoved   = true;

                //jika tujuan punya flag, flag terlepas
                if (isFlagged[nr][nc]) {
                    isFlagged[nr][nc] = false;
                    flagsPlaced--;
                }

                // jika tujuan sudah terbuka maka meledak dan game over
                if (isOpen[nr][nc]) {
                    gameOver = true;
                }

                moved = true;
            }
            // jika semua 8 arah terblokir, bom diam di tempat
        }

        //hitung ulang semua angka tetangga setelah bom bergerak
        if (bombJustMoved) {
            calcAdjacent();

            /* Perbaiki kotak yang sudah terbuka:
			jika angkanya berubah menjadi 0 setelah bom pergi,
            tidak otomatis flood fill (biarkan pemain lihat perubahan).
            ini desain sengaja biar gak confusing. */
        }
    }

    //flood fill rekursif
    void floodFill(int r, int c) {
        if (r < 0 || r >= size || c < 0 || c >= size) return;
        if (isOpen[r][c] || isFlagged[r][c])           return;
        if (hasBomb[r][c])                              return;

        isOpen[r][c] = true;
        openedSafe++;

        if (adjacent[r][c] == 0) {
            int dr[] = {-1,-1,-1, 0, 0, 1, 1, 1};
            int dc[] = {-1, 0, 1,-1, 1,-1, 0, 1};
            for (int d = 0; d < 8; d++)
                floodFill(r + dr[d], c + dc[d]);
        }
    }

    //cek menang 
    bool checkWin() {
        if (flagsPlaced != totalBombs) return false;
        for (int r = 0; r < size; r++)
            for (int c = 0; c < size; c++)
                if (isFlagged[r][c] && !hasBomb[r][c]) return false;
        return true;
    }

    //bentuk papan
    void printBoard(bool reveal = false) {
        time_t now     = time(NULL);
        int    elapsed = (int)difftime(now, startTime);

        std::cout << "\n";

        //notifikasi bom bergerak
        if (bombJustMoved && !reveal) {
            std::cout << "  !! BOM BERGERAK !! Angka di papan sudah diperbarui.\n";
        }

        // info baris
        std::cout << "  Bom tersisa : " << (totalBombs - flagsPlaced)
                  << "   |   Waktu : " << elapsed << " detik";
        if (movingBombsOn) {
            int langkahBerikut = moveInterval - (stepCount % moveInterval);
            std::cout << "   |   Bom gerak dalam : " << langkahBerikut << " langkah";
        }
        std::cout << "\n\n";

        // nomor kolom
        std::cout << "     ";
        for (int c = 1; c <= size; c++) {
            if (c < 10) std::cout << " " << c << " ";
            else        std::cout << c << " ";
        }
        std::cout << "\n";

        //garis atas
        std::cout << "    +";
        for (int c = 0; c < size; c++) std::cout << "---";
        std::cout << "+\n";

        for (int r = 0; r < size; r++) {
            if (r + 1 < 10) std::cout << "  " << (r+1) << " |";
            else             std::cout << " "  << (r+1) << " |";

            for (int c = 0; c < size; c++) {
                char ch;
                if (reveal && hasBomb[r][c]) {
                    ch = (isOpen[r][c]) ? SYM_EXPLODE : SYM_BOMB;
                } else if (isOpen[r][c]) {
                    // tmpilkan angka terbaru (bisa berubah setelah bom gerak)
                    ch = (adjacent[r][c] == 0) ? ' ' : ('0' + adjacent[r][c]);
                } else if (isFlagged[r][c]) {
                    ch = SYM_FLAG;
                } else {
                    ch = SYM_HIDDEN;
                }
                std::cout << " " << ch << " ";
            }
            std::cout << "|\n";
        }

        // garis bawah
        std::cout << "    +";
        for (int c = 0; c < size; c++) std::cout << "---";
        std::cout << "+\n";

        std::cout << "\n  Legenda : . = tersembunyi | F = ditandai"
                     " | angka = bom sekitar | ' ' = kosong\n";
    }

    //validasi input
    int readInt(const std::string& prompt, int lo, int hi) {
        int val;
        while (true) {
            std::cout << prompt;
            if (std::cin >> val) {
                std::cin.ignore(1000, '\n');
                if (val >= lo && val <= hi) return val;
            } else {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
            }
            std::cout << "  [!] Masukkan angka antara " << lo
                      << " dan " << hi << ".\n";
        }
    }

public:
    //game baru setup
    void setup() {
        std::cout << "\n+============================+\n";
        std::cout <<   "|        MINESWEEPER++       |\n";
        std::cout <<   "|      [ Bom Bergerak! ]     |\n";
        std::cout <<   "+============================+\n\n";

        size = readInt("  Ukuran papan (4-10) : ", 4, MAX_SIZE);
        int maxBombs = size * size - 1;
        totalBombs   = readInt("  Jumlah bom (1-" + intToStr(maxBombs) + ") : ",
                               1, maxBombs);

        // option buat idupin moving bomb
        std::cout << "\n  Aktifkan Moving Bombs? (1=Ya / 2=Tidak) : ";
        int pil = readInt("", 1, 2);
        movingBombsOn = (pil == 1);

        if (movingBombsOn) {
            moveInterval = readInt("  Bom gerak setiap berapa langkah? (1-10) : ",
                                   1, 10);
            std::cout << "  [OK] Bom akan bergerak setiap "
                      << moveInterval << " langkah!\n";
        } else {
            moveInterval = 0;
            std::cout << "  [OK] Mode klasik (bom tidak bergerak).\n";
        }

        srand((unsigned int)time(NULL));
        resetBoard();
        placeBombs();
        calcAdjacent();

        startTime = time(NULL);
    }

    // loop satu ronde
    void play() {
        while (!gameOver && !gameWon) {
            printBoard();

            std::cout << "\n  Aksi : (1) Buka kotak  (2) Tandai/hapus tanda\n";
            int aksi = readInt("  Pilih aksi : ", 1, 2);

            int r = readInt("  Baris (1-" + intToStr(size) + ") : ", 1, size) - 1;
            int c = readInt("  Kolom (1-" + intToStr(size) + ") : ", 1, size) - 1;

            bool validMove = false;

            if (aksi == 1) {
                // buat buka kotak
                if (isOpen[r][c]) {
                    std::cout << "  [!] Kotak sudah terbuka.\n";
                } else if (isFlagged[r][c]) {
                    std::cout << "  [!] Kotak bertanda. Hapus tanda dulu.\n";
                } else {
                    validMove = true;
                    if (hasBomb[r][c]) {
                        isOpen[r][c] = true;
                        gameOver = true;
                    } else {
                        floodFill(r, c);
                        if (checkWin()) gameWon = true;
                    }
                }
            } else {
                // ngasih tanda atau apus tanda
                if (isOpen[r][c]) {
                    std::cout << "  [!] Kotak sudah terbuka, tidak bisa ditandai.\n";
                } else {
                    validMove = true;
                    if (isFlagged[r][c]) {
                        isFlagged[r][c] = false;
                        flagsPlaced--;
                        std::cout << "  Tanda dihapus.\n";
                    } else {
                        isFlagged[r][c] = true;
                        flagsPlaced++;
                        std::cout << "  Kotak ditandai.\n";
                        if (checkWin()) gameWon = true;
                    }
                }
            }

            // trigger vbom gerak
            if (validMove && movingBombsOn && !gameOver && !gameWon) {
                stepCount++;
                if (stepCount % moveInterval == 0) {
                    moveBombs();
                    // Cek game over akibat bom meledak saat bergerak
                    // (sudah di-set di dalam moveBombs)
                }
            }
        }

        // hasil akhirnya
        time_t endTime = time(NULL);
        int elapsed    = (int)difftime(endTime, startTime);

        if (gameOver) {
            bombJustMoved = false;  //jangan tampilkan notif gerak saat game over
            printBoard(true);
            std::cout << "\n  *** BOOM! Game Over! ***\n";
            std::cout << "  Kamu menginjak bom - semua bom terungkap.\n";
            std::cout << "  Total langkah  : " << stepCount << "\n";
            std::cout << "  Waktu bermain  : " << elapsed << " detik.\n\n";
        } else {
            printBoard();
            std::cout << "\n  *** SELAMAT! Kamu Menang! ***\n";
            std::cout << "  Semua bom berhasil ditandai dengan tepat!\n";
            std::cout << "  Total langkah  : " << stepCount << "\n";
            std::cout << "  Waktu bermain  : " << elapsed << " detik.\n\n";
        }
    }
};

//menu utama
int main() {
    srand((unsigned int)time(NULL));

    while (true) {
        std::cout << "\n+==============================+\n";
        std::cout <<   "|         MENU UTAMA           |\n";
        std::cout <<   "|  1. Mulai Permainan Baru     |\n";
        std::cout <<   "|  2. Keluar                   |\n";
        std::cout <<   "+==============================+\n";
        std::cout <<   "  Pilihan (1/2) : ";

        int pilihan;
        if (!(std::cin >> pilihan)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        std::cin.ignore(1000, '\n');

        if (pilihan == 1) {
            Game g;
            g.setup();
            g.play();
        } else if (pilihan == 2) {
            std::cout << "\n  Sampai jumpa!\n\n";
            break;
        } else {
            std::cout << "  [!] Pilihan tidak valid.\n";
        }
    }

    return 0;
}
