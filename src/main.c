#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// =================================================================
// 1. STRUKTUR DATA: PRIORITY QUEUE (LINKED LIST) UNTUK ANTREAN
// =================================================================
typedef struct AntreanNode {
    char nama[50];
    bool isMember;
    struct AntreanNode* next;
} AntreanNode;

AntreanNode* headAntrean = NULL;

void tambahAntrean(const char* nama, bool isMember) {
    AntreanNode* newNode = (AntreanNode*)malloc(sizeof(AntreanNode));
    strcpy(newNode->nama, nama);
    newNode->isMember = isMember;
    newNode->next = NULL;
    
    // Jika antrean kosong
    if (headAntrean == NULL) {
        headAntrean = newNode;
        return;
    }
    
    // Jika Member dan yang di depan Regular, potong ke paling depan
    if (isMember && !headAntrean->isMember) {
        newNode->next = headAntrean;
        headAntrean = newNode;
        return;
    }
    
    // Cari posisi (Member ditaruh setelah Member terakhir)
    AntreanNode* curr = headAntrean;
    while (curr->next != NULL) {
        if (isMember && !curr->next->isMember) {
            break; 
        }
        curr = curr->next;
    }
    newNode->next = curr->next;
    curr->next = newNode;
}

AntreanNode* popAntrean() {
    if (headAntrean == NULL) return NULL;
    AntreanNode* temp = headAntrean;
    headAntrean = headAntrean->next;
    return temp;
}

void cetakAntrean() {
    if (headAntrean == NULL) {
        printf("Antrean saat ini kosong.\n");
        return;
    }
    AntreanNode* curr = headAntrean;
    int no = 1;
    while (curr != NULL) {
        printf("%d. %s %s\n", no++, curr->nama, curr->isMember ? "[MEMBER VIP]" : "[Regular]");
        curr = curr->next;
    }
}


// =================================================================
// 2. STRUKTUR DATA: BINARY SEARCH TREE (BST) UNTUK MENU WARKOP
// =================================================================
typedef struct MenuNode {
    char nama[50];
    int harga;
    struct MenuNode* left;
    struct MenuNode* right;
} MenuNode;

MenuNode* buatMenuNode(const char* nama, int harga) {
    MenuNode* newNode = (MenuNode*)malloc(sizeof(MenuNode));
    strcpy(newNode->nama, nama);
    newNode->harga = harga;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

MenuNode* insertBST(MenuNode* root, const char* nama, int harga) {
    if (root == NULL) return buatMenuNode(nama, harga);
    
    int cmp = strcmp(nama, root->nama);
    if (cmp < 0) root->left = insertBST(root->left, nama, harga);
    else if (cmp > 0) root->right = insertBST(root->right, nama, harga);
    
    return root;
}

void inorderBST(MenuNode* root) {
    if (root != NULL) {
        inorderBST(root->left);
        printf("- %s (Rp%d)\n", root->nama, root->harga);
        inorderBST(root->right);
    }
}

// Versi dengan nomor urut
void inorderBSTWithNumber(MenuNode* root, int* no) {
    if (root != NULL) {
        inorderBSTWithNumber(root->left, no);
        printf("%d. %s (Rp%d)\n", (*no)++, root->nama, root->harga);
        inorderBSTWithNumber(root->right, no);
    }
}

void cetakMenuAlpabet(MenuNode* root) {
    if (root == NULL) {
        printf("[INFO] Belum ada menu. Tambahkan menu terlebih dahulu.\n");
        return;
    }
    printf("\n=== DAFTAR MENU (A-Z) ===\n");
    int no = 1;
    inorderBSTWithNumber(root, &no);
}

MenuNode* searchBST(MenuNode* root, const char* nama) {
    if (root == NULL || strcmp(root->nama, nama) == 0) return root;
    if (strcmp(nama, root->nama) < 0) return searchBST(root->left, nama);
    return searchBST(root->right, nama);
}

MenuNode* findMin(MenuNode* root) {
    while (root != NULL && root->left != NULL) {
        root = root->left;
    }
    return root;
}

MenuNode* deleteBST(MenuNode* root, const char* nama) {
    if (root == NULL) return NULL;

    int cmp = strcmp(nama, root->nama);
    if (cmp < 0) {
        root->left = deleteBST(root->left, nama);
    } else if (cmp > 0) {
        root->right = deleteBST(root->right, nama);
    } else {
        if (root->left == NULL) {
            MenuNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            MenuNode* temp = root->left;
            free(root);
            return temp;
        } else {
            MenuNode* temp = findMin(root->right);
            strcpy(root->nama, temp->nama);
            root->harga = temp->harga;
            root->right = deleteBST(root->right, temp->nama);
        }
    }
    return root;
}

void inorderSaveMenuTxt(MenuNode* root, FILE* file) {
    if (root != NULL) {
        inorderSaveMenuTxt(root->left, file);
        fprintf(file, "%s %d\n", root->nama, root->harga);
        inorderSaveMenuTxt(root->right, file);
    }
}

void saveMenuTxt(MenuNode* root) {
    FILE* file = fopen("menu.txt", "w");
    if (file != NULL) {
        inorderSaveMenuTxt(root, file);
        fclose(file);
        printf("[] Menu berhasil disimpan ke menu.txt\n");
    }
}

MenuNode* updateMenuHarga(MenuNode* root, const char* nama, int harga) {
    MenuNode* item = searchBST(root, nama);
    if (item != NULL) {
        item->harga = harga;
    }
    return root;
}

MenuNode* editMenuNama(MenuNode* root, const char* lama, const char* baru) {
    if (searchBST(root, baru) != NULL) return root;
    MenuNode* item = searchBST(root, lama);
    if (item != NULL) {  
        int harga = item->harga;
        root = deleteBST(root, lama);
        root = insertBST(root, baru, harga);
    }
    return root;
}

// Import data menu dari txt
MenuNode* loadMenuTxt(MenuNode* root) {
    FILE* file = fopen("menu.txt", "r");
    if (file == NULL) {
        printf("[INFO] File menu.txt tidak ditemukan. Mulai dengan menu kosong.\n");
        return root;
    }
    char nama[50];
    int harga;
    while (fscanf(file, "%s %d", nama, &harga) == 2) {
        root = insertBST(root, nama, harga);
    }
    fclose(file);
    printf("[] Menu berhasil dimuat dari menu.txt\n");
    return root;
}

int countMenu(MenuNode* root) {
    if (root == NULL) return 0;
    return 1 + countMenu(root->left) + countMenu(root->right);
}


// =================================================================
// 3. STRUKTUR DATA: LINKED LIST UNTUK RIWAYAT TRANSAKSI (DINAMIS)
// =================================================================
typedef struct RiwayatNode {
    char nama_pelanggan[50];
    char pesanan[200];
    int total_harga;
    struct RiwayatNode* next; // Pointer ke node selanjutnya
} RiwayatNode;

RiwayatNode* headRiwayat = NULL; // Head untuk linked list riwayat
int jumlahRiwayat = 0;           // Counter total transaksi dinamis

void swapRiwayat(Riwayat* a, Riwayat* b) {
    Riwayat temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Riwayat arr[], int low, int high, bool ascending) {
    int pivot = arr[high].total_harga;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        bool condition = ascending ? (arr[j].total_harga < pivot) : (arr[j].total_harga > pivot);
        if (condition) {
            swapRiwayat(&arr[i + 1], &arr[j]);
            i++;
        }
    }
    swapRiwayat(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(Riwayat arr[], int low, int high, bool ascending) {
    if (low < high) {
        int pi = partition(arr, low, high, ascending);
        quickSort(arr, low, pi - 1, ascending);
        quickSort(arr, pi + 1, high, ascending);
    }
}

void simpanKeRiwayatTxt(Riwayat r) {
    FILE* file = fopen("riwayat.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s %d\n", r.nama_pelanggan, r.pesanan, r.total_harga);
        fclose(file);
    }
}


// =================================================================
// 4. SUBMENU: KELOLA MENU DENGAN INTERFACE YANG LEBIH BAIK
// =================================================================
void subMenuKelola(MenuNode** rootMenu) {
    int menuPilihan;
    char konfirmasi;
    
    do {
        printf("\n====================================\n");
        printf("   KELOLA MENU WARKOP (A-Z)\n");
        printf("====================================\n");
        printf("1. Tambah Menu                    \n");
        printf("2. Ubah Nama Menu                 \n");
        printf("3. Ubah Harga Menu                \n");
        printf("4. Hapus Menu                     \n");
        printf("5. Lihat Daftar Menu              \n");
        printf("6. Kembali ke Menu Utama          \n");
        printf("====================================\n");
        printf("Pilih (1-6): ");
        scanf("%d", &menuPilihan);
        getchar(); // Clear newline from input buffer

        if (menuPilihan == 1) {
            // ===== TAMBAH MENU =====
            char namaBaru[50];
            int hargaBaru;
            
            printf("\n>>> TAMBAH MENU BARU <<<\n");
            printf("Nama menu (tanpa spasi): ");
            fgets(namaBaru, sizeof(namaBaru), stdin);
            namaBaru[strcspn(namaBaru, "\n")] = '\0'; // Remove newline
            
            if (strlen(namaBaru) == 0) {
                printf("[ERROR] Nama menu tidak boleh kosong!\n");
                continue;
            }
            
            if (searchBST(*rootMenu, namaBaru) != NULL) {
                printf("[ERROR] Menu '%s' sudah ada!\n", namaBaru);
            } else {
                printf("Harga menu (Rp): ");
                scanf("%d", &hargaBaru);
                getchar();
                
                if (hargaBaru <= 0) {
                    printf("[ERROR] Harga harus lebih dari 0!\n");
                    continue;
                }
                
                *rootMenu = insertBST(*rootMenu, namaBaru, hargaBaru);
                saveMenuTxt(*rootMenu);
                printf("[SUKSES] Menu '%s' (Rp%d) berhasil ditambahkan.\n", namaBaru, hargaBaru);
            }

        } else if (menuPilihan == 2) {
            // ===== UBAH NAMA MENU =====
            char namaLama[50], namaBaru[50];
            
            printf("\n>>> UBAH NAMA MENU <<<\n");
            cetakMenuAlpabet(*rootMenu);
            
            printf("\nNama menu yang akan diubah: ");
            fgets(namaLama, sizeof(namaLama), stdin);
            namaLama[strcspn(namaLama, "\n")] = '\0';
            
            MenuNode* menuDitemukan = searchBST(*rootMenu, namaLama);
            if (menuDitemukan == NULL) {
                printf("[ERROR] Menu '%s' tidak ditemukan.\n", namaLama);
            } else {
                printf("Nama baru: ");
                fgets(namaBaru, sizeof(namaBaru), stdin);
                namaBaru[strcspn(namaBaru, "\n")] = '\0';
                
                if (strlen(namaBaru) == 0) {
                    printf("[ERROR] Nama baru tidak boleh kosong!\n");
                    continue;
                }
                
                if (searchBST(*rootMenu, namaBaru) != NULL) {
                    printf("[ERROR] Nama '%s' sudah dipakai.\n", namaBaru);
                } else {
                    printf("\nKonfirmasi ubah '%s' � '%s'? (y/n): ", namaLama, namaBaru);
                    scanf("%c", &konfirmasi);
                    getchar();
                    
                    if (konfirmasi == 'y' || konfirmasi == 'Y') {
                        *rootMenu = editMenuNama(*rootMenu, namaLama, namaBaru);
                        saveMenuTxt(*rootMenu);
                        printf("[SUKSES] Nama menu berhasil diubah menjadi '%s'.\n", namaBaru);
                    } else {
                        printf("[INFO] Pembatalan ubah nama menu.\n");
                    }
                }
            }

        } else if (menuPilihan == 3) {
            // ===== UBAH HARGA MENU =====
            char namaUbah[50];
            int hargaBaru;
            
            printf("\n>>> UBAH HARGA MENU <<<\n");
            cetakMenuAlpabet(*rootMenu);
            
            printf("\nNama menu yang ingin diubah harga: ");
            fgets(namaUbah, sizeof(namaUbah), stdin);
            namaUbah[strcspn(namaUbah, "\n")] = '\0';
            
            MenuNode* itemDitemukan = searchBST(*rootMenu, namaUbah);
            if (itemDitemukan == NULL) {
                printf("[ERROR] Menu '%s' tidak ditemukan.\n", namaUbah);
            } else {
                printf("Harga saat ini: Rp%d\n", itemDitemukan->harga);
                printf("Harga baru (Rp): ");
                scanf("%d", &hargaBaru);
                getchar();
                
                if (hargaBaru <= 0) {
                    printf("[ERROR] Harga harus lebih dari 0!\n");
                    continue;
                }
                
                printf("Konfirmasi ubah harga %s dari Rp%d � Rp%d? (y/n): ", 
                       namaUbah, itemDitemukan->harga, hargaBaru);
                scanf("%c", &konfirmasi);
                getchar();
                
                if (konfirmasi == 'y' || konfirmasi == 'Y') {
                    *rootMenu = updateMenuHarga(*rootMenu, namaUbah, hargaBaru);
                    saveMenuTxt(*rootMenu);
                    printf("[SUKSES] Harga '%s' berhasil diubah menjadi Rp%d.\n", namaUbah, hargaBaru);
                } else {
                    printf("[INFO] Pembatalan ubah harga.\n");
                }
            }

        } else if (menuPilihan == 4) {
            // ===== HAPUS MENU =====
            char namaHapus[50];
            
            printf("\n>>> HAPUS MENU <<<\n");
            cetakMenuAlpabet(*rootMenu);
            
            printf("\nNama menu yang ingin dihapus: ");
            fgets(namaHapus, sizeof(namaHapus), stdin);
            namaHapus[strcspn(namaHapus, "\n")] = '\0';
            
            MenuNode* itemHapus = searchBST(*rootMenu, namaHapus);
            if (itemHapus == NULL) {
                printf("[ERROR] Menu '%s' tidak ditemukan.\n", namaHapus);
            } else {
                printf("\n[PERHATIAN] Anda akan menghapus menu:\n");
                printf("  � '%s' (Rp%d)\n", namaHapus, itemHapus->harga);
                printf("Yakin ingin menghapus? (y/n): ");
                scanf("%c", &konfirmasi);
                getchar();
                
                if (konfirmasi == 'y' || konfirmasi == 'Y') {
                    *rootMenu = deleteBST(*rootMenu, namaHapus);
                    saveMenuTxt(*rootMenu);
                    printf("[SUKSES] Menu '%s' berhasil dihapus.\n", namaHapus);
                } else {
                    printf("[INFO] Pembatalan penghapusan menu.\n");
                }
            }

        } else if (menuPilihan == 5) {
            // ===== LIHAT DAFTAR MENU =====
            cetakMenuAlpabet(*rootMenu);
            int totalMenu = countMenu(*rootMenu);
            printf("\nTotal menu: %d item\n", totalMenu);

        } else if (menuPilihan == 6) {
            printf("[INFO] Kembali ke menu utama...\n");
            break;

        } else {
            printf("[ERROR] Pilihan tidak valid! Silakan masukkan 1-6.\n");
        }
    } while (menuPilihan != 6);
}


// =================================================================
// 5. MAIN & USER INTERFACE (TAMPILAN KASIR)
// =================================================================
int main() {
    MenuNode* rootMenu = NULL;
    rootMenu = loadMenuTxt(rootMenu); 

    int pilihan;
    do {
        printf("\n====================================\n");
        printf("  SISTEM MANAJEMEN WARKOP KOPI    \n");
        printf("====================================\n");
        printf("1. Pendaftaran Antrean Pelanggan  \n");
        printf("2. Layani Pesanan (Antrean Depan) \n");
        printf("3. Lihat Laporan Penjualan (Sort) \n");
        printf("4. Lihat Daftar Menu (A-Z)        \n");
        printf("5. Kelola Menu Warkop             \n");
        printf("6. Keluar                         \n");
        printf("====================================\n");
        printf("Pilih opsi (1-6): ");
        scanf("%d", &pilihan);
        getchar();

        if (pilihan == 1) {
            char nama[50];
            int status;
            printf("\n>>> PENDAFTARAN ANTREAN <<<\n");
            printf("Masukkan Nama Pelanggan (tanpa spasi): ");
            scanf("%s", nama);
            printf("Pilih Status (1 = Member VIP, 2 = Regular): ");
            scanf("%d", &status);
            getchar();
            
            if (strlen(nama) > 0) {
                tambahAntrean(nama, status == 1);
                printf("\n[SUKSES] %s masuk antrean!\n", nama);
                printf("Kondisi antrean saat ini:\n");
                cetakAntrean();
            } else {
                printf("[ERROR] Nama tidak boleh kosong!\n");
            }

        } else if (pilihan == 2) {
            // Pengecekan apakah menu sudah ada atau masih kosong
            if (rootMenu == NULL) {
                printf("\n[ERROR] Tidak dapat melayani pesanan karena menu warkop belum ada!\n");
                printf("[INFO] Silakan isi daftar menu terlebih dahulu di menu Kelola Menu Warkop (Opsi 5).\n");
                continue; // Langsung lompat ke perulangan berikutnya/kembali ke menu utama
            }

            AntreanNode* pelanggan = popAntrean();
            if (pelanggan == NULL) {
                printf("\n[INFO] Antrean kosong! Belum ada pelanggan.\n");
            } else {
            
                printf("\n====================================\n");
                printf(">>> MELAYANI: %s %s\n", pelanggan->nama, 
                       pelanggan->isMember ? "[MEMBER VIP]" : "[Regular]");
                printf("====================================\n");
                
                printf("\n=== DAFTAR MENU WARKOP ===\n");
                int no = 1;
                inorderBSTWithNumber(rootMenu, &no);
                
                char pesanan[50];
                char detailPesanan[200] = "";
                int qty;
                int jenisMenu;
                int total = 0;
                printf("\nBerapa jenis menu yang ingin dipesan? ");
                scanf("%d", &jenisMenu);
                getchar();

                for (int i = 0; i < jenisMenu; i++) {
                    do {
                        printf("\nKetik nama menu ke-%d: ", i + 1);
                        fgets(pesanan, sizeof(pesanan), stdin);
                        pesanan[strcspn(pesanan, "\n")] = '\0';
                        
                        MenuNode* menuDitemukan = searchBST(rootMenu, pesanan);
                        if (menuDitemukan == NULL) {
                            printf("[ERROR] Menu '%s' tidak ditemukan. Coba lagi.\n", pesanan);
                        } else {
                            printf("Jumlah porsi untuk %s: ", pesanan);
                            scanf("%d", &qty);
                            getchar();
                            
                            if (qty > 0) {
                                total += menuDitemukan->harga * qty;
                                char itemDetail[100];
                                sprintf(itemDetail, "%s x%d", pesanan, qty);
                                if (strlen(detailPesanan) > 0) {
                                    strcat(detailPesanan, ", ");
                                }
                                strcat(detailPesanan, itemDetail);
                                break;
                            } else {
                                printf("[ERROR] Jumlah harus lebih dari 0!\n");
                            }
                        }
                    } while (true);
                }

                printf("\n====================================\n");
                printf("PEMBAYARAN\n");
                printf("Total Tagihan: Rp%d\n", total);
                printf("====================================\n");
                
                // Masukkan ke array historis (max 100) & file txt
                if (jumlahRiwayat < 100) {
                    strcpy(listRiwayat[jumlahRiwayat].nama_pelanggan, pelanggan->nama);
                    strcpy(listRiwayat[jumlahRiwayat].pesanan, detailPesanan);
                    listRiwayat[jumlahRiwayat].total_harga = total;
                    
                    simpanKeRiwayatTxt(listRiwayat[jumlahRiwayat]);
                    jumlahRiwayat++;
                    printf("[SUKSES] Pesanan selesai diproses & disimpan!\n");
                } else {
                    printf("[WARNING] Memori riwayat penuh!\n");
                }
                free(pelanggan);
            }

        } else if (pilihan == 3) {
            if (jumlahRiwayat == 0) {
                printf("\n[INFO] Belum ada riwayat penjualan.\n");
            } else {
                int sortOpt;
                printf("\nUrutkan Laporan Berdasarkan:\n");
                printf("1. Pendapatan Terkecil ke Terbesar (Ascending)\n");
                printf("2. Pendapatan Terbesar ke Terkecil (Descending)\n");
                printf("Pilih: ");
                scanf("%d", &sortOpt);
                getchar();

                quickSort(listRiwayat, 0, jumlahRiwayat - 1, sortOpt == 1);

                printf("\n=== LAPORAN PENJUALAN ===\n");
                for (int i = 0; i < jumlahRiwayat; i++) {
                    printf("%d. %s | %s | Total: Rp%d\n", i+1, 
                           listRiwayat[i].nama_pelanggan, 
                           listRiwayat[i].pesanan, 
                           listRiwayat[i].total_harga);
                }
            }

        } else if (pilihan == 4) {
            cetakMenuAlpabet(rootMenu);
            int totalMenu = countMenu(rootMenu);
            printf("\nTotal menu: %d item\n", totalMenu);

        } else if (pilihan == 5) {
            subMenuKelola(&rootMenu);

        } else if (pilihan == 6) {
            printf("\n====================================\n");
            printf("Sistem dimatikan.\n");
            printf("Data riwayat disimpan di riwayat.txt\n");
            printf("Sampai jumpa! \n");
            printf("====================================\n");
        } else {
            printf("\n[ERROR] Pilihan tidak valid! Masukkan 1-6.\n");
        }
    } while (pilihan != 6);

    return 0;
}
