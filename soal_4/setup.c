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
