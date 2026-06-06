#include <iostream>
#include <cstdlib>
#include <ctime>

const int MAX_SIZE = 10;

// simvol tampilan
const char SYM_HIDDEN  = '.';
const char SYM_FLAG    = 'F';
const char SYM_BOMB    = '*';
const char SYM_EXPLODE = 'X';

//buat cetak int ke string (ganti std::to_string)
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


class Game {
private:
    //state papan
    int  size;
    int  totalBombs;

    bool hasBomb  [MAX_SIZE][MAX_SIZE];
    bool isOpen   [MAX_SIZE][MAX_SIZE];
    bool isFlagged[MAX_SIZE][MAX_SIZE];
    int  adjacent [MAX_SIZE][MAX_SIZE];

    // state permainannya
    bool gameOver;
    bool gameWon;
    int  flagsPlaced;
    int  openedSafe;
    int  totalSafe;

    //timer
    time_t startTime;

    //inisialisasi papannya
    void resetBoard() {
        for (int r = 0; r < size; r++)
            for (int c = 0; c < size; c++) {
                hasBomb  [r][c] = false;
                isOpen   [r][c] = false;
                isFlagged[r][c] = false;
                adjacent [r][c] = 0;
            }
        gameOver    = false;
        gameWon     = false;
        flagsPlaced = 0;
        openedSafe  = 0;
        totalSafe   = size * size - totalBombs;
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

    //flood fill nya rekursif
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

    //cek kl menang
    bool checkWin() {
        if (flagsPlaced != totalBombs) return false;
        for (int r = 0; r < size; r++)
            for (int c = 0; c < size; c++)
                if (isFlagged[r][c] && !hasBomb[r][c]) return false;
        return true;
    }

    //board nya
    void printBoard(bool reveal = false) {
        time_t now     = time(NULL);
        int    elapsed = (int)difftime(now, startTime);

        std::cout << "\n";
        std::cout << "  Bom tersisa : " << (totalBombs - flagsPlaced)
                  << "   |   Waktu : " << elapsed << " detik\n\n";

        //nomor kolom
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

        //garis bawah
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
    //game baru
    void setup() {
        std::cout << "\n==============================\n";
        std::cout <<   "|       MINESWEEPER++        |\n";
        std::cout << "\n==============================\n\n";

        size = readInt("  Ukuran papan (4-10) : ", 4, MAX_SIZE);
        int maxBombs = size * size - 1;
        totalBombs   = readInt("  Jumlah bom (1-" + intToStr(maxBombs) + ") : ",
                               1, maxBombs);

        srand((unsigned int)time(NULL));
        resetBoard();
        placeBombs();
        calcAdjacent();

        startTime = time(NULL);
    }

    //loop 1 game
    void play() {
        while (!gameOver && !gameWon) {
            printBoard();

            std::cout << "\n  Aksi : (1) Buka kotak  (2) Tandai/hapus tanda\n";
            int aksi = readInt("  Pilih aksi : ", 1, 2);

            int r = readInt("  Baris (1-" + intToStr(size) + ") : ", 1, size) - 1;
            int c = readInt("  Kolom (1-" + intToStr(size) + ") : ", 1, size) - 1;

            if (aksi == 1) {
                // -- Buka kotak --------------------------------
                if (isOpen[r][c]) {
                    std::cout << "  [!] Kotak sudah terbuka.\n";
                    continue;
                }
                if (isFlagged[r][c]) {
                    std::cout << "  [!] Kotak bertanda. Hapus tanda dulu.\n";
                    continue;
                }
                if (hasBomb[r][c]) {
                    isOpen[r][c] = true;
                    gameOver = true;
                } else {
                    floodFill(r, c);
                    if (checkWin()) gameWon = true;
                }
            } else {
                // -- Tandai / hapus tanda ----------------------
                if (isOpen[r][c]) {
                    std::cout << "  [!] Kotak sudah terbuka, tidak bisa ditandai.\n";
                    continue;
                }
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

        // hasl akhir
        time_t endTime = time(NULL);
        int elapsed    = (int)difftime(endTime, startTime);

        if (gameOver) {
            printBoard(true);
            std::cout << "\n  *** BOOM! Game Over! ***\n";
            std::cout << "  Kamu menginjak bom - semua bom terungkap.\n";
            std::cout << "  Waktu bermain : " << elapsed << " detik.\n\n";
        } else {
            printBoard();
            std::cout << "\n  *** SELAMAT! Kamu Menang! ***\n";
            std::cout << "  Semua bom berhasil ditandai dengan tepat!\n";
            std::cout << "  Waktu bermain : " << elapsed << " detik.\n\n";
        }
    }
};

//ini menu utamanya
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
