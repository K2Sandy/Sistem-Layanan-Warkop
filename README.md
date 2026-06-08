# Sistem-Layanan-Warkop

sistem ini kami buat karena permasalahan pada antrian warkop yang kami rasa masih kurang efisien, sehingga kami terpikir suatu solusi untuk membuat sebuah sistem yang bernama sistem layanan warkop yang diharap dapat membuat layanan operasional pada warkop menjadi lebih efisien dan dapat mengurangi human error.

Fitur Utama : 
Sistem ini menyediakan 5 fitur utama beserta sub-fitur pengelolaan yang dinamis:

Pendaftaran Antrean Pelanggan: Menggunakan sistem prioritas (Priority Queue). Pelanggan berstatus Member VIP akan otomatis bergeser ke barisan depan setelah member VIP lainnya, mendahului pelanggan Regular.

Pelayanan Pesanan: Memproses antrean paling depan (Pop Queue), memilih item menu, menentukan jumlah porsi, menghitung total tagihan, serta menyimpan transaksi ke dalam riwayat.

Laporan Penjualan Dinamis: Menampilkan riwayat pendapatan yang dapat diurutkan berdasarkan nominal terkecil (Ascending) maupun terbesar (Descending).

Daftar Menu Otomatis Terurut: Menampilkan seluruh menu yang tersedia secara alfabetis (A-Z).

Kelola Menu Warkop (CRUD): Sub-menu khusus untuk Menambah Menu baru, Mengubah Nama, Mengubah Harga, serta Menghapus Menu dengan sistem validasi.

Program manajemen warkop ini menerapkan beberapa struktur data dinamis untuk mengelola operasionalnya secara efisien. Pada sistem pendaftaran antrean pelanggan, program menggunakan struktur data antrean berprioritas (priority queue) yang diimplementasikan melalui singly linked list. Alih-alih menerapkan prinsip first-in, first-out (FIFO) secara kaku, penyisipan data diatur berdasarkan status keanggotaan pelanggan. Ketika pelanggan berstatus member VIP mendaftar, algoritma penyisipan akan memindai barisan dan menempatkan node baru tersebut di depan pelanggan reguler, namun tetap berada di belakang member VIP lain yang sudah mengantre lebih dulu. Proses penghapusan antrean (pop) tetap dilakukan dari barisan paling depan (head) untuk melayani pelanggan yang memiliki prioritas tertinggi saat itu.

Untuk pengelolaan daftar menu warkop, program memanfaatkan struktur data pohon pencarian biner atau binary search tree (BST). Setiap item menu disimpan dalam bentuk node pohon yang posisi penempatannya ditentukan berdasarkan perbandingan alfabetis nama menu menggunakan komparasi string. Penggunaan BST ini memungkinkan proses pencarian, pembaruan harga, dan penghapusan menu berjalan secara efisien tanpa harus memeriksa seluruh data satu per satu. Selain itu, program menggunakan algoritma penelusuran in-order traversal secara rekursif saat mencetak daftar menu atau menyimpannya ke dalam file eksternal, sehingga seluruh hidangan otomatis tersusun secara berurutan dan rapi dari huruf A sampai Z.

Komponen terakhir dari sistem ini adalah pencatatan riwayat transaksi dan penyusunan laporan penjualan yang menggabungkan struktur data penampung dengan algoritma pengurutan cepat (quick sort). Setiap transaksi yang selesai dilayani akan langsung dimasukkan ke dalam singly linked list khusus riwayat menggunakan metode penyisipan di akhir (insert last). Ketika pengguna ingin melihat laporan penjualan yang terurut berdasarkan pendapatan, program menerapkan strategi dengan mengalokasikan sebuah array of pointers dinamis secara sementara untuk menyalin alamat memori dari setiap node riwayat. Algoritma quick sort yang berbasis divide and conquer kemudian dijalankan untuk mengurutkan pointer transaksi tersebut berdasarkan nominal total harga, baik secara menaik (ascending) maupun menurun (descending), sebelum akhirnya menampilkan data yang sudah terurut ke layar dan membebaskan kembali memori array pembantu tersebut.

Persistensi Data (File Handling)
Aplikasi ini mendukung penyimpanan data lokal secara permanen menggunakan file eksternal:

menu.txt: Menyimpan struktur menu yang ada di dalam BST. File ini akan dibaca otomatis (auto-load) sesaat setelah program dijalankan.

riwayat.txt: Menyimpan rekaman transaksi final setiap kali pesanan selesai dilayani (menggunakan mode append).

 Cara Kompilasi dan Menjalankan Program
Pastikan kamu sudah menginstal compiler GCC di perangkatmu.

Klon atau simpan kode program ke dalam file bernama main.c.

Buka Terminal/Command Prompt di direktori yang sama dengan file tersebut.

Kompilasi kode menggunakan perintah berikut: gcc main.c -o  main kemudian ./main



