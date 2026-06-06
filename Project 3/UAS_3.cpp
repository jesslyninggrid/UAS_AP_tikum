#include <iostream>
#include <vector>

using namespace std;

struct Barang {
    string kodeBarang;
    string namaBarang;
    double harga;
    int stok;
};

vector<Barang> daftarBarang;

// Tambah Barang
void tambahBarang() {
    system("cls");
    Barang barang;

    cout << "\n=== TAMBAH BARANG ===\n";

    cout << "Kode Barang : ";
    cin >> barang.kodeBarang;

    cin.ignore();

    cout << "Nama Barang : ";
    getline(cin, barang.namaBarang);

    do {
        cout << "Harga Barang : ";
        cin >> barang.harga;

        if (barang.harga < 0) {
            cout << "Harga tidak boleh negatif!\n";
        }

    } while (barang.harga < 0);

    do {
        cout << "Stok Barang : ";
        cin >> barang.stok;

        if (barang.stok < 0) {
            cout << "Stok tidak boleh negatif!\n";
        }

    } while (barang.stok < 0);

    daftarBarang.push_back(barang);

    cout << "Barang berhasil ditambahkan.\n";
}

// Tampilkan Semua Barang
void tampilkanBarang() {

    if (daftarBarang.empty()) {
        cout << "\nBelum ada data barang.\n";
        return;
    }

    cout << "\n=== DAFTAR BARANG ===\n";

    for (int i = 0; i < daftarBarang.size(); i++) {

        cout << "Kode Barang : "
             << daftarBarang[i].kodeBarang << endl;

        cout << "Nama Barang : "
             << daftarBarang[i].namaBarang << endl;

        cout << "Harga       : "
             << daftarBarang[i].harga << endl;

        cout << "Stok        : "
             << daftarBarang[i].stok << endl;

        cout << "--------------------------\n";
    }
}

// Cari Barang Termahal
void cariBarangTermahal() {

    if (daftarBarang.empty()) {
        cout << "\nBelum ada data barang.\n";
        return;
    }

    int indeksTermahal = 0;

    for (int i = 1; i < daftarBarang.size(); i++) {

        if (daftarBarang[i].harga >
            daftarBarang[indeksTermahal].harga) {

            indeksTermahal = i;
        }
    }

    cout << "\n=== BARANG TERMAHAL ===\n";

    cout << "Kode Barang : "
         << daftarBarang[indeksTermahal].kodeBarang << endl;

    cout << "Nama Barang : "
         << daftarBarang[indeksTermahal].namaBarang << endl;

    cout << "Harga       : "
         << daftarBarang[indeksTermahal].harga << endl;

    cout << "Stok        : "
         << daftarBarang[indeksTermahal].stok << endl;
}

// Hitung Total Nilai Inventaris
void hitungTotalInventaris() {

    double total = 0;

    for (int i = 0; i < daftarBarang.size(); i++) {

        total += daftarBarang[i].harga *
                 daftarBarang[i].stok;
    }

    cout << "\nTotal Nilai Inventaris = Rp "
         << total << endl;
}

// Cek Restock
void cekRestock() {

    bool ditemukan = false;

    cout << "\n=== BARANG YANG PERLU RESTOCK ===\n";

    for (int i = 0; i < daftarBarang.size(); i++) {

        if (daftarBarang[i].stok < 5) {

            ditemukan = true;

            cout << "Kode Barang : "
                 << daftarBarang[i].kodeBarang << endl;

            cout << "Nama Barang : "
                 << daftarBarang[i].namaBarang << endl;

            cout << "Stok        : "
                 << daftarBarang[i].stok << endl;

            cout << "--------------------------\n";
        }
    }

    if (!ditemukan) {
        cout << "Tidak ada barang yang perlu restock.\n";
    }
}

// Update Stok Menggunakan Pointer
void updateStok() {

    string kode;
    bool ditemukan = false;

    cout << "\nMasukkan Kode Barang : ";
    cin >> kode;

    for (int i = 0; i < daftarBarang.size(); i++) {

        if (daftarBarang[i].kodeBarang == kode) {

            ditemukan = true;

            int *ptrStok = &daftarBarang[i].stok;

            int perubahan;

            cout << "Stok Saat Ini : "
                 << *ptrStok << endl;

            cout << "Masukkan jumlah perubahan stok : ";
            cin >> perubahan;

            if ((*ptrStok + perubahan) < 0) {

                cout << "Stok tidak boleh kurang dari 0.\n";
            }
            else {

                *ptrStok += perubahan;

                cout << "Stok berhasil diperbarui.\n";
                cout << "Stok Sekarang : "
                     << *ptrStok << endl;
            }

            break;
        }
    }

    if (!ditemukan) {
        cout << "Barang tidak ditemukan.\n";
    }
}

int main() {

    int pilihan;

    do {

        cout << "\n===== SISTEM INVENTARIS TOKO =====\n";
        cout << "1. Tambah Barang\n";
        cout << "2. Tampilkan Semua Barang\n";
        cout << "3. Cari Barang Termahal\n";
        cout << "4. Hitung Total Nilai Inventaris\n";
        cout << "5. Cek Restock\n";
        cout << "6. Update Stok\n";
        cout << "7. Keluar\n";
        cout << "Pilih Menu : ";
        cin >> pilihan;

        switch (pilihan) {

            case 1:
                tambahBarang();
                break;

            case 2:
                tampilkanBarang();
                break;

            case 3:
                cariBarangTermahal();
                break;

            case 4:
                hitungTotalInventaris();
                break;

            case 5:
                cekRestock();
                break;

            case 6:
                updateStok();
                break;

            case 7:
                cout << "\nProgram selesai.\n";
                break;

            default:
                cout << "\nPilihan tidak valid.\n";
        }

    } while (pilihan != 7);

    return 0;
}