#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

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

  close(STDIN_FILENO);
  //close(STDOUT_FILENO);
  //close(STDERR_FILENO);

  // program disini

    chdir("/home/abhi/sisop_proc");

    int id_download = fork();

    if (id_download == 0){
        execlp("wget", "wget", "-q", "--no-check-certificate", "https://docs.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup", "-O", "library.zip", NULL);
    } else {
        wait(NULL);
        int id_unzip = fork();
        if (id_unzip == 0){
            execlp("unzip", "unzip", "-q", "library.zip", NULL);    
        } else {
            wait(NULL);
        }
    }


  /*
  program tersebut belum menyelesaikan tahap dekripsi menggunakan ROT19, program ini juga belum menyelesaikan tahap merename sesuai kode nama filenya
  dan menghapus sesuai kode nama filenya, tahap backup dan restore file belum selesai, tahap mengganti mode sesuai signal belum selesai,
  belum menyelesaikan code untuk dimatikan secara aman, dan pencatatan log file belum selesai
  */


    return 0;
}
