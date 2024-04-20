#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/inotify.h>

// Variabel flag global 
int running_flag = 1;
// Fungsi untuk menulis ke log
void write_to_log(const char *username, const char *activity, int status) {
    char log_filename[100];
    sprintf(log_filename, "%s.log", username);
    FILE *log_file = fopen(log_filename, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }
    time_t now;
    struct tm *local_time;
    time(&now);
    local_time = localtime(&now);

    fprintf(log_file, "[%02d:%02d:%04d]-[%02d:%02d:%02d]-%d-%s %s\n",
            local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900,
            local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
            getpid(), activity, status ? "JALAN" : "GAGAL");

    fclose(log_file);
}
// Fungsi untuk mengirim sinyal SIGTERM ke proses 
void stop_monitoring(const char *username) {
    char log_message[100];
    sprintf(log_message, "Menghentikan pemantauan untuk pengguna %s", username);
    write_to_log(username, log_message, 1);
    // Baca PID dari file log
    char log_filename[100];
    sprintf(log_filename, "%s.log", username);
    FILE *log_file = fopen(log_filename, "r");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }
    int pid;
    if (fscanf(log_file, "[%*d:%*d:%*d]-[%*d:%*d:%*d]-%d", &pid) != 1) {
        perror("Error reading PID from log file");
        fclose(log_file);
        return;
    }
    fclose(log_file);
    // Kirim sinyal SIGTERM ke proses 
    if (kill(pid, SIGTERM) == -1) {
        perror("Error sending SIGTERM signal");
        return;
    }
}
// Fungsi untuk memonitor segala aktivitas yang dilakukan oleh pengguna
void monitor_activities(const char *username) {
    // Membuat handle untuk memantau direktori home pengguna
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("Failed to initialize inotify");
        return;
    }
    const char *user_home = getenv("HOME");
    int wd = inotify_add_watch(inotify_fd, user_home, IN_ALL_EVENTS);
    if (wd == -1) {
        perror("Failed to add watch to user's home directory");
        close(inotify_fd);
        return;
    }
    // Membuat buffer untuk menampung event inotify
    char event_buf[4096];
    ssize_t num_bytes;
    printf("Memulai pemantauan untuk pengguna %s\n", username);
    while (1) {
        // Membaca event inotify
        num_bytes = read(inotify_fd, event_buf, sizeof(event_buf));
        if (num_bytes <= 0) {
            perror("Error reading inotify events");
            break;
        }
        // Mengolah setiap event
        int i = 0;
        while (i < num_bytes) {
            struct inotify_event *event = (struct inotify_event *) &event_buf[i];
            if (event->len > 0) {
                // Mencatat aktivitas ke file log
                write_to_log(username, event->name, 1); // 1 untuk Jalan
                printf("Aktivitas: %s. PID: %d\n", event->name, getpid());
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }
    // Menghapus watch dan menutup handle inotify
    inotify_rm_watch(inotify_fd, wd);
    close(inotify_fd);
}
// Fungsi menggagalkan kegiatan pengguna
void fail_activities(const char *username) {
    running_flag = 0;
    write_to_log(username, "Gagalkan aktivitas", 0); // 0 Untuk Gagal
}
// Fungsi mengaktifkan kembali kegiatan pengguna
void activate_activities(const char *username) {
    running_flag = 1;
    write_to_log(username, "Aktifkan kembali kegiatan", 1); // 1 Untuk Jalan
}
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [-m | -s | -c | -a] <username>\n", argv[0]);
        return 1;
    }
    const char *username = argv[2];
    if (strcmp(argv[1], "-m") == 0) {
        printf("Memulai pemantauan untuk pengguna %s\n", username);
        // Jalankan fitur daemon
        pid_t pid = fork();
        if (pid < 0) {
            perror("Error forking");
            exit(EXIT_FAILURE);
        }
        if (pid > 0) {
            printf("Pemantauan dimulai. PID proses pemantauan: %d\n", pid);
            exit(EXIT_SUCCESS); // Keluar parent process
        }
        // Child process jadi leader
        if (setsid() < 0) {
            perror("Error creating new session");
            exit(EXIT_FAILURE);
        }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        // Pemilihan proses pengguna
        monitor_activities(username);
    } else if (strcmp(argv[1], "-s") == 0) {
        printf("Menghentikan pemantauan untuk pengguna %s\n", username);
        // Mematikan fitur daemon
        stop_monitoring(username);
    } else if (strcmp(argv[1], "-c") == 0) {
        printf("Menggagalkan kegiatan pengguna %s\n", username);
        // Menggagalkan kegiatan pengguna
        fail_activities(username);
    } else if (strcmp(argv[1], "-a") == 0) {
        printf("Mengaktifkan kembali kegiatan pengguna %s\n", username);
        // Mengaktifkan kembali kegiatan pengguna
        activate_activities(username);
    } else {
        fprintf(stderr, "Invalid option\n");
        return 1;
    }
return 0;
}
// Kekurangannya kadang Error tidak bisa untuk melakukan ./admin -s, program tidak berhenti
