#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

// Fungsi membuka aplikasi
void open_app(char *app_name, int num_windows, pid_t *pids, int *pid_count) {
 for (int i = 0; i < num_windows; i++) {
     pid_t pid = fork();
     if (pid == 0) {
            execlp(app_name, app_name, NULL);
            perror("execlp");
            exit(1);
     } else if (pid < 0) {
            perror("fork");
            exit(1);
     } else {
            // Simpan PID
            pids[*pid_count] = pid;
            (*pid_count)++;
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
        return;
    }
    
    for (int i = 0; i < pid_count; i++) {
        kill(pids[i], SIGTERM);
    }
    while (wait(NULL) > 0);
}
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
    // Membuka aplikasi dengan baris perintah
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
    } else {
        printf("Invalid option\n");
        return 1;
    }
return 0;
}
