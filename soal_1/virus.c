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
