# Sisop-2-2024-MH-IT26

## ***KELOMPOK IT 26***
  | Nama      | NRP         |
  |-----------|-------------|
  | Rafael Jonathan Arnoldus   | 5027231006  |
  | Muhammad Abhinaya Al-Faruqi  | 5027231011  |  
  | Danendra Fidel Khansa  | 5027231063  |

## Bagian Pengerjaan Soal 
+ Soal 1 = Rafael Jonathan Arnoldus
+ Soal 2 = Muhammad Abhinaya Al-Faruqi
+ Soal 3 = Danendra Fidel Khansa
+ Soal 4 = Danendra Fidel Khansa

## ***MODUL 1***
  Berikut untuk struktur repository pengumpulan dari hasil praktikum sistem operasi modul 2 :
  ```
—soal_1:
        — virus.c
—soal_2:
	      — management.c
—soal_3:
	      — admin.c
—soal_4:
	      — setup.c
```
## ***SOAL 1 (Rafael)***
Gavriel adalah seorang cyber security enthusiast. Suatu hari, ia dikontrak oleh sebuah perusahaan ternama untuk membuat sebuah program yang cukup rumit dengan bayaran jutaan rupiah. Karena tergoda dengan nominal bayaran tersebut, Gavriel langsung menerima tawaran tersebut. Setelah mencoba membuat dan mengembangkan program tersebut selama seminggu, laptop yang digunakan Gavriel mengalami overheat dan mati total sehingga harus dilarikan ke tukang servis terdekat. Karena keterbatasan waktu dalam pembuatan program dan tidak ingin mengecewakan perusahaan, Gavriel meminta bantuan kalian untuk membuat program tersebut dengan ketentuan sebagai berikut:

a. Program dapat menerima input path berupa ‘argv’ untuk mengatur folder dimana file akan dieksekusi

b. Program tersebut berfungsi untuk mereplace string dengan ketentuan sebagai berikut:
```   
   i. String m4LwAr3 direplace dengan string [MALWARE]
   ii. String 5pYw4R3 direplace dengan string [SPYWARE]
   iii. String R4nS0mWaR3 direplace dengan string [RANSOMWARE]
```
c. Program harus berjalan secara daemon, dan **tidak diperbolehkan menggunakan command system()** dalam pembuatan program

d. Program akan secara terus menerus berjalan di background dengan jeda **15 detik**

e. Catat setiap penghapusan string yang dilakukan oleh program pada sebuah file bernama **virus.log** dengan format: [dd-mm-YYYY][HH:MM:SS] Suspicious string at <file_name> successfully replaced!

Contoh penggunaan: ./virus /home/user/virus

Contoh isi file **sebelum** program dijalankan:
```
pU=-JWQ$5$)D-[??%AVh]$cB6bm4LwAr3jEQC2p3R{HV]=-AUaxj:Qe+h
!aNX,i:!z3W=2;.tHc3&S+}6F)CFf%tfZLP1*w5m1PAzZJUux(
Pd&f8$F5=E?@#[6jd{TJKj]5pYw4R3{KK1?hz384$ge@iba5GAj$gqB41
#C&&a}M9C#f64Eb.?%c)dGbCvJXtU[?SE4h]BY4e1PR4nS0mWaR3{]S/{w?*
```
Contoh isi file **setelah** setelah program dijalankan:
```
pU=-JWQ$5$)D-[??%AVh]$cB6b[MALWARE]jEQC2p3R{HV]=-AUaxj:Qe+h
!aNX,i:!z3W=2;.tHc3&S+}6F)CFf%tfZLP1*w5m1PAzZJUux(
Pd&f8$F5=E?@#[6jd{TJKj][SPYWARE]{KK1?hz384$ge@iba5GAj$gqB41
#C&&a}M9C#f64Eb.?%c)dGbCvJXtU[?SE4h]BY4e1P[RANSOMWARE]{]S/{w?*
```
## ***PENGERJAAN***
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void mengubah_karakter(char *filename) {
    FILE *file;
    char line[256]; // Array untuk menyimpan setiap baris
    char temp_file[256]; // File sementara untuk menyimpan hasil perubahan
    int masuk_log = 0; // Tanda untuk menandakan apakah ada string yang berhasil diganti di file tersebut

    // Membuat file sementara
    sprintf(temp_file, "%s.tmp", filename);
    file = fopen(temp_file, "w");
    if (file == NULL) {
        return;
    }

    // Membuka file yang akan diubah
    FILE *original_file = fopen(filename, "r");
    if (original_file == NULL) {
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), original_file) != NULL) {
        char *kata = line;
        char *new_kata;

        while ((new_kata = strstr(kata, "m4LwAr3")) != NULL) {
            strncpy(new_kata, "[MALWARE]", strlen("[MALWARE]"));
            kata = new_kata + strlen("[MALWARE]");
            masuk_log = 1; // Set string ke 1 karena string berhasil diganti
        }

        kata = line;
        while ((new_kata = strstr(kata, "5pYw4R3")) != NULL) {
            strncpy(new_kata, "[SPYWARE]", strlen("[SPYWARE]"));
            kata = new_kata + strlen("[SPYWARE]");
            masuk_log = 1;
        }

        kata = line;
        while ((new_kata = strstr(kata, "R4nS0mWaR3")) != NULL) {
            strncpy(new_kata, "[RANSOMWARE]", strlen("[RANSOMWARE]"));
            kata = new_kata + strlen("[RANSOMWARE]");
            masuk_log = 1;
        }

        fputs(line, file); // Menulis baris yang telah diubah ke file sementara
    }

    fclose(original_file);
    fclose(file);

    // Mengganti file asli dengan file sementara
    remove(filename);
    rename(temp_file, filename);

    if (masuk_log) {
        FILE *log_file = fopen("home/ubuntu/SISOP/Modul2/soal_1/virus.log", "a");
        if (log_file != NULL) {
            time_t current_time = time(NULL);
            struct tm tm = *localtime(&current_time);
            fprintf(log_file, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, filename);
            fclose(log_file);
        }
    }

    // Reset nilai string ke 0 untuk penggunaan berikutnya
    masuk_log = 0;
}
void pencarian_direktori(char *directory_path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(directory_path);
    if (dir == NULL) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip directory 
        }

        // Skip file selain .txt
        size_t len = strlen(entry->d_name);
        if (len < 4 || strcmp(entry->d_name + len - 4, ".txt") != 0) {
            continue;
        }

        char full_path[strlen(directory_path) + strlen(entry->d_name) + 2];
        snprintf(full_path, sizeof(full_path), "%s/%s", directory_path, entry->d_name);

        FILE *file = fopen(full_path, "r");
        if (file != NULL) {
            mengubah_karakter(full_path); // memanggil funngsi ubah karakter 
            fclose(file);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    pid_t pid, sid; // Variabel untuk menyimpan PID

    pid = fork(); // Menyimpan PID dari child Psoses 

    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
    * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
   
    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        
        if (argc < 2) {
            exit(EXIT_FAILURE);
        }

        pencarian_direktori(argv[1]);

        sleep(15); // Jeda setiap 15 detik
    }

    exit(EXIT_SUCCESS);
}
```
## ***PENJELASAN PENGERJAAN***

## ***Dokumentasi***

## ***SOAL 2 (Abhinaya)***
Paul adalah seorang mahasiswa semester 4 yang diterima magang di perusahaan XYZ. Pada hari pertama magang, ia diberi tugas oleh atasannya untuk membuat program manajemen file sederhana. Karena kurang terbiasa dengan bahasa C dan environment Linux, ia meminta bantuan kalian untuk mengembangkan program tersebut.

a. Atasannya meminta agar program tersebut dapat berjalan secara daemon dan dapat mengunduh serta melakukan unzip terhadap file berikut. Atasannya juga meminta program ini dibuat **tanpa menggunakan command system()**

b. Setelah ditelusuri, ternyata hanya 6 file teratas yang nama filenya tidak dienkripsi. Oleh karena itu, bantulah Paul untuk melakukan dekripsi terhadap nama file ke-7 hingga terakhir menggunakan algoritma ROT19

c. Setelah dekripsi selesai, akan terlihat bahwa setiap file memuat salah satu dari kode berikut: **r3N4mE, d3Let3, dan m0V3**. Untuk setiap file dengan nama yang memuat kode **d3Let3**, hapus file tersebut. Sementara itu, untuk setiap file dengan nama yang memuat kode **r3N4mE**, lakukan hal berikut:
```
  i. Jika ekstensi file adalah “.ts”, rename filenya menjadi “helper.ts”
  ii. Jika ekstensi file adalah “.py”, rename filenya menjadi “calculator.py”
  iii. Jika ekstensi file adalah “.go”, rename filenya menjadi “server.go”
  iv. Jika file tidak memuat salah satu dari ekstensi diatas, rename filenya menjadi “renamed.file”
```
d. Atasan Paul juga meminta agar program ini dapat membackup dan merestore file. Oleh karena itu, bantulah Paul untuk membuat program ini menjadi 3 mode, yaitu:
```
  i. default: program berjalan seperti biasa untuk me-rename dan menghapus file. Mode ini dieksekusi ketika program dijalankan tanpa argumen tambahan, yaitu dengan command ./management saja
  ii. backup: program memindahkan file dengan kode m0V3 ke sebuah folder bernama “backup”
  iii. restore: program mengembalikan file dengan kode m0V3 ke folder sebelum file tersebut dipindahkan
  iv. Contoh penggunaan: ./management -m backup
```
e. Terkadang, Paul perlu mengganti mode dari program ini tanpa menghentikannya terlebih dahulu. Oleh karena itu, bantulan Paul untuk mengintegrasikan kemampuan untuk mengganti mode ini dengan mengirim sinyal ke daemon, dengan ketentuan:
```
  i. SIGRTMIN untuk mode default
  ii. SIGUSR1 untuk mode backup
  iii. SIGUSR2 untuk mode restore
  iv. Contoh penggunaan: kill -SIGUSR2 <pid_program>
```
f. Program yang telah dibuat ini tidak mungkin akan dijalankan secara terus-menerus karena akan membebani sistem. Maka dari itu, bantulah Paul untuk membuat program ini dapat dimatikan dengan aman dan efisien

g. Terakhir, program ini harus berjalan setiap detik dan mampu mencatat setiap peristiwa yang terjadi ke dalam file .log yang bernama “history.log” dengan ketentuan:
```
  i. Format: [nama_user][HH:MM:SS] - <nama_file> - <action>
  ii. nama_user adalah username yang melakukan action terhadap file
  iii. Format action untuk setiap kode:
      1. kode r3N4mE: Successfully renamed.
      2. kode d3Let3: Successfully deleted.
      3. mode backup: Successfully moved to backup.
      4. mode restore: Successfully restored from backup.
  iv. Contoh pesan log:
      1. [paul][00:00:00] - r3N4mE.ts - Successfully renamed.
      2. [paul][00:00:00] - m0V3.xk1 - Successfully restored from backup.
```
h. Berikut adalah struktur folder untuk pengerjaan nomor 2:
```
    soal_2/
    ├── history.log
    ├── management.c
    └── library/
        └── backup/
```

## ***PENGERJAAN***

## ***PENJELASAN PENGERJAAN***

## ***Dokumentasi***

## ***SOAL 3 (Fidel)***
Pak Heze adalah seorang admin yang baik. Beliau ingin membuat sebuah program admin yang dapat memantau para pengguna sistemnya. Bantulah Pak Heze untuk membuat program  tersebut!

a. Namai program tersebut dengan nama **admin.c**

b. Program tersebut memiliki fitur menampilkan seluruh proses yang dilakukan oleh seorang user dengan menggunakan command:
```
admin <user>
```
c. Program dapat memantau proses apa saja yang dilakukan oleh user. Fitur ini membuat program berjalan secara daemon dan berjalan terus menerus. Untuk menjalankan fitur ini menggunakan command: 
```
admin -m <user>
```
Dan untuk mematikan fitur tersebut menggunakan: 
```
admin -s <user>
```
Program akan mencatat seluruh proses yang dijalankan oleh user di file <user>.log dengan format:
```
[dd:mm:yyyy]-[hh:mm:ss]-pid_process-nama_process_GAGAL/JALAN
```
D. Program dapat menggagalkan proses yang dijalankan user setiap detik dan berjalan terus menerus dengan menjalankan: 
```
admin -c user
```
sehingga user tidak bisa menjalankan proses yang dia inginkan dengan baik. Fitur ini dapat dimatikan dengan command:
```
admin -a user
```
E. Ketika proses yang dijalankan user digagalkan, program juga akan melog dan menset log tersebut sebagai **GAGAL**. Dan jika di log menggunakan fitur poin c, log akan ditulis dengan **JALAN**

## ***PENGERJAAN***

## ***PENJELASAN PENGERJAAN***

## ***Dokumentasi***


## ***SOAL 4 (Fidel)***
Salomo memiliki passion yang sangat dalam di bidang sistem operasi. Saat ini, dia ingin mengotomasi kegiatan-kegiatan yang ia lakukan agar dapat bekerja secara efisien. Bantulah Salomo untuk membuat program yang dapat mengotomasi kegiatan dia!

a. Salomo ingin dapat membuka berbagai macam aplikasi dengan banyak jendela aplikasi dalam satu command. Namai file program tersebut **setup.c**

b. Program dapat membuka berbagai macam aplikasi dan banyak jendela aplikasi sekaligus (bisa membuka 1 atau lebih aplikasi dengan 1 atau lebih window (kecuali aplikasi yang tidak bisa dibuka dengan banyak window seperti discord)) dengan menjalankan:
```
setup -o <app1> <num1> <app2> <num2>.....<appN> <numN>
```
Contoh penggunaannya adalah sebagai berikut: 
```
setup -o firefox 2 wireshark 2
```
Program akan membuka 2 jendela aplikasi firefox dan 2 jendela aplikasi wireshark.

c. Program juga dapat membuka aplikasi dengan menggunakan file konfigurasi dengan menggunakan 
```
setup -f file.conf
``` 
Format file konfigurasi dibebaskan, namun pastikan dapat menjalankan fitur dari poin 2.
Contoh isi file.conf:
```
Firefox 2
Wireshark 3
```
Ketika menjalankan command contoh, program akan membuka 2 jendela aplikasi firefox dan 3 jendela aplikasi wireshark.

d. Program dapat mematikan semua aplikasi yg dijalankan oleh program tersebut dengan: 
```
setup -k
```
e. Program juga dapat mematikan aplikasi yang dijalankan sesuai dengan file konfigurasi. 
Contohnya: 
```
setup -k file.conf 
```
Command ini hanya mematikan aplikasi yang dijalankan dengan 
```
setup -f file.conf
```
## ***PENGERJAAN***
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

// Prototipe fungsi
void save_running_app(pid_t pid);
// Fungsi membuka aplikasi
void open_app(char *app_name, int num_windows, pid_t *pids, int *pid_count) {
    for (int i = 0; i < num_windows; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child proses, ngejalanin aplikasi
            execlp(app_name, app_name, NULL);
            perror("execlp");
            exit(1);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        } else {
            // Simpan PID kedalam file log
            pids[*pid_count] = pid;
            (*pid_count)++;
            usleep(10000); 
            save_running_app(pid);
        }
    }
}
// Fungsi membaca konfigurasi dari file
void read_config_file(char *file_name, pid_t *pids, int *pid_count) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    char app_name[256];
    int num_windows;
    while (fscanf(file, "%s %d", app_name, &num_windows) != EOF) {
        open_app(app_name, num_windows, pids, pid_count);
    }
    fclose(file);
}
// Fungsi mematikan aplikasi berdasarkan PID
void kill_apps(pid_t *pids, int pid_count) {
    if (pid_count == 0) {
        printf("Tidak ada aplikasi.\n");
        return;
    }    
    for (int i = 0; i < pid_count; i++) {
        kill(pids[i], SIGTERM);
    }   
    // Menunggu child proses selesai
    while (wait(NULL) > 0);
}
// Fungsi menyimpan PID aplikasi yang di run kedalam file log
void save_running_app(pid_t pid) {
    FILE *log_file = fopen("Berjalan", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(1);
    }
    fprintf(log_file, "%d\n", pid);
    fclose(log_file);
}
// Fungsi membaca PID aplikasi dari file log dan melakukan kill
void kill_apps_from_log() {
    FILE *log_file = fopen("Berjalan", "r");
    if (log_file == NULL) {
        printf("Tidak ada aplikasi.\n");
        return;
    }
    pid_t pid;
    while (fscanf(log_file, "%d", &pid) != EOF) {
        if (kill(pid, SIGTERM) == -1) {
            perror("Error killing process");
        } else {
            printf("Killed process with PID: %d\n", pid);
        }
    }
    fclose(log_file);
    remove("berjalan"); // Hapus file log setelah selesai melakukan kill
}
// Fungsi Main untuk memilih opsi program yang dijalankan
int main(int argc, char *argv[]) {
    pid_t pids[100]; 
    int pid_count = 0;

    // Menghilangkan peringatan Gtk
    putenv("GTK_MODULES=");

    if (argc < 2) {
        printf("Usage: setup -o <app1> <num1> <app2> <num2>... or setup -f <file.conf> or setup -k\n");
        return 1;
    }
    if (strcmp(argv[1], "-o") == 0) {
        // Membuka aplikasi dengan perintah
        for (int i = 2; i < argc; i += 2) {
            int num_windows = atoi(argv[i + 1]);
            open_app(argv[i], num_windows, pids, &pid_count);
        }
    } else if (strcmp(argv[1], "-f") == 0) {
        // Membaca konfigurasi dari file
        if (argc != 3) {
            printf("Usage: setup -f <file.conf>\n");
            return 1;
        }
        read_config_file(argv[2], pids, &pid_count);
    } else if (strcmp(argv[1], "-k") == 0) {
        // Mematikan aplikasi
        kill_apps(pids, pid_count);
        // Menjalankan fungsi untuk melakukan kill berdasarkan file log
        kill_apps_from_log();
    } else {
        printf("Invalid option\n");
        return 1;
    }
return 0;
}
```
## ***PENJELASAN PENGERJAAN***
*Salomo ingin dapat membuka berbagai macam aplikasi dengan banyak jendela aplikasi dalam satu command. Namai file program tersebut setup.c*

Pertama mkdir untuk membuat directory, disini saya menamai direktori dengan soal4, kemudian nano untuk membuat text book yang dinamai dengan setup.c sesuai dengan keterangan soal
`mkdir soal4`
`nano setup.c`

*Program C sesuai yang diminta di soal beserta penjelasan fungsi fungsi pada programnya*
Pertama kita buat untuk program C
`Library`
- #include <stdio.h> = untuk input-output program c

- #include <stdlib.h> = untuk fungsi-fungsi umum seperti alokasi memori dan konversi

- #include <unistd.h> = untuk system call POSIX 

- #include <sys/types.h> = untuk tipe data khusus sistem

- #include <sys/wait.h> =  untuk fungsi-fungsi yang berhubungan dengan penungguan proses

- #include <string.h> =  untuk manipulasi string

- #include <signal.h> = untuk menangani sinyal dan pengontrolan proses

`void save_running_app(pid_t pid)`

Fungsi save_running_app(pid_t pid) berfungsi untuk menyimpan PID aplikasi yang sedang berjalan ke dalam file log
```
void open_app(char *app_name, int num_windows, pid_t *pids, int *pid_count) {
    for (int i = 0; i < num_windows; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child proses, ngejalanin aplikasi
            execlp(app_name, app_name, NULL);
            perror("execlp");
            exit(1);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        } else {
            // Simpan PID kedalam file log
            pids[*pid_count] = pid;
            (*pid_count)++;
            usleep(10000); 
            save_running_app(pid);
        }
    }
}
```
- Fungsi ini digunakan untuk membuka aplikasi. Menerima nama aplikasi, jumlah jendela, array PID, dan jumlah PID yang digunakan sebagai argumen.
- Menggunakan fork() untuk membuat proses baru dan execlp() untuk menjalankan aplikasi.
- Setelah menjalankan aplikasi, PID disimpan dan ditambahkan ke file log.
```
void read_config_file(char *file_name, pid_t *pids, int *pid_count) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    char app_name[256];
    int num_windows;
    while (fscanf(file, "%s %d", app_name, &num_windows) != EOF) {
        open_app(app_name, num_windows, pids, pid_count);
    }
    fclose(file);
}
```
- Fungsi ini membaca konfigurasi dari file.
- Menerima nama file, array PID, dan jumlah PID sebagai argumen.
- Setiap baris file membaca nama aplikasi dan jumlah jendela, dan kemudian memanggil open_app().
```
void kill_apps(pid_t *pids, int pid_count) {
    if (pid_count == 0) {
        printf("Tidak ada aplikasi.\n");
        return;
    }    
    for (int i = 0; i < pid_count; i++) {
        kill(pids[i], SIGTERM);
    }   
    // Menunggu child proses selesai
    while (wait(NULL) > 0);
}
```
- Fungsi ini digunakan untuk mematikan aplikasi berdasarkan PID.
- Menerima array PID dan jumlah PID sebagai argumen.
- Mengirimkan sinyal SIGTERM ke setiap PID yang disimpan.
```
void save_running_app(pid_t pid) {
    FILE *log_file = fopen("Berjalan", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(1);
    }
    fprintf(log_file, "%d\n", pid);
    fclose(log_file);
}
```
- Fungsi ini menyimpan PID aplikasi yang dijalankan ke dalam file log.
- Menerima PID aplikasi sebagai argumen.
- Membuka file log, menulis PID, dan menutup file log.
```
void kill_apps_from_log() {
    FILE *log_file = fopen("Berjalan", "r");
    if (log_file == NULL) {
        printf("Tidak ada aplikasi.\n");
        return;
    }
    pid_t pid;
    while (fscanf(log_file, "%d", &pid) != EOF) {
        if (kill(pid, SIGTERM) == -1) {
            perror("Error killing process");
        } else {
            printf("Killed process with PID: %d\n", pid);
        }
    }
    fclose(log_file);
    remove("berjalan"); // Hapus file log setelah selesai melakukan kill
}
```
- Fungsi ini membaca PID aplikasi dari file log dan mematikan aplikasi.
- Membuka file log, membaca PID, dan mengirimkan sinyal SIGTERM ke setiap PID.
- Setelah selesai, file log dihapus.
```
int main(int argc, char *argv[]) {
    pid_t pids[100]; 
    int pid_count = 0;

    // Menghilangkan peringatan Gtk
    putenv("GTK_MODULES=");

    if (argc < 2) {
        printf("Usage: setup -o <app1> <num1> <app2> <num2>... or setup -f <file.conf> or setup -k\n");
        return 1;
    }
    if (strcmp(argv[1], "-o") == 0) {
        // Membuka aplikasi dengan perintah
        for (int i = 2; i < argc; i += 2) {
            int num_windows = atoi(argv[i + 1]);
            open_app(argv[i], num_windows, pids, &pid_count);
        }
    } else if (strcmp(argv[1], "-f") == 0) {
        // Membaca konfigurasi dari file
        if (argc != 3) {
            printf("Usage: setup -f <file.conf>\n");
            return 1;
        }
        read_config_file(argv[2], pids, &pid_count);
    } else if (strcmp(argv[1], "-k") == 0) {
        // Mematikan aplikasi
        kill_apps(pids, pid_count);
        // Menjalankan fungsi untuk melakukan kill berdasarkan file log
        kill_apps_from_log();
    } else {
        printf("Invalid option\n");
        return 1;
    }
return 0;
}
```
Fungsi utama program.

- Memeriksa argumen baris perintah untuk menentukan tindakan apa yang diambil.
- Jika argumen adalah -o, program membuka aplikasi dengan argumen baris perintah.
- Jika argumen adalah -f, program membaca konfigurasi dari file.
- Jika argumen adalah -k, program mematikan aplikasi.
- Jika argumen tidak valid, program mencetak pesan kesalahan.
  
## ***Dokumentasi***

NB: Semua soal tidak boleh dikerjakan menggunakan fungsi **system()**. Di utamakan menggunakan fungsi **fork()** atau **exec()**.
