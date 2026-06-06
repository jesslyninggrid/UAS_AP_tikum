PROJECT 4 oleh Gabrielle Christin

Minesweeper++

Program Minesweeper++ ini dibuat menggunakan satu kelas utama bernama Game yang bertugas
mengelola seluruh logika permainan, mulai dari pembuatan papan, penempatan bom secara acak,
perhitungan jumlah bom di sekitar setiap kotak, hingga pengecekan kondisi menang dan kalah. Saat
permainan dimulai, pemain dapat menentukan ukuran papan (4–10) dan jumlah bom yang akan digunakan,
kemudian program akan membuat papan permainan menggunakan array 2 dimensi statis berukuran
maksimum 10×10 sesuai ketentuan tugas. Pemain dapat memilih aksi untuk membuka kotak atau memberi
tanda pada kotak yang dicurigai berisi bom. Jika kotak yang dibuka mengandung bom, permainan langsung
berakhir dan seluruh bom ditampilkan, sedangkan jika kotak tersebut memiliki nilai 0 maka program akan
secara otomatis membuka area kosong di sekitarnya menggunakan metode rekursif (flood fill). Selama
permainan berlangsung, program menampilkan papan secara rapi lengkap dengan jumlah bom yang belum
ditandai dan waktu bermain yang dihitung sejak permainan dimulai. Kondisi kemenangan dicapai apabila
seluruh bom berhasil ditandai dengan benar tanpa ada tanda pada kotak aman. Selain itu, program juga
menyediakan menu utama yang memungkinkan pemain memulai permainan baru atau keluar dari aplikasi
setelah satu ronde permainan selesai.
