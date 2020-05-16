//E' data la stringa di caratteri:
//char * text_to_write = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. At ultrices mi tempus imperdiet nulla malesuada pellentesque elit. Non enim praesent elementum facilisis leo vel fringilla est ullamcorper. Non quam lacus suspendisse faucibus interdum. Metus vulputate eu scelerisque felis imperdiet. At ultrices mi tempus imperdiet nulla malesuada pellentesque. At tempor commodo ullamcorper a lacus vestibulum. Consequat semper viverra nam libero justo laoreet sit amet. Facilisis magna etiam tempor orci eu. Convallis tellus id interdum velit laoreet id donec ultrices. Interdum velit euismod in pellentesque massa placerat duis ultricies. Senectus et netus et malesuada fames. Pharetra vel turpis nunc eget lorem dolor. Nisi porta lorem mollis aliquam ut porttitor leo a. Euismod elementum nisi quis eleifend quam adipiscing vitae proin.";
//
//- il processo padre crea un file output.txt (se il file esiste già, va troncato a zero e sovrascritto).
//- il processo padre tiene aperto il file output.txt, salvare il descrittore in una variabile fd
//- il processo padre crea un processo figlio: l'obiettivo finale è che il file output.txt contenga tutta la stringa
//  text_to_write e che i due processi (processo padre e processo figlio) si dividano il lavoro di scrittura nel file.
//  La scrittura sul file avviene in "parallelo".
//- il processo figlio apre per conto suo il file output.txt.
//- il processo padre scrive la prima metà di text_to_write nel file, cominciando a scrivere dalla posizione iniziale del file.
//- il processo figlio scrive l'altra metà di text_to_write del file, cominciando a scrivere da dove il processo padre ha
//  terminato di scrivere.
//- il processo padre attende la conclusione del processo figlio e poi termina.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
	char * file_name = "output.txt";
	char * text_to_write = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. At ultrices mi tempus imperdiet nulla malesuada pellentesque elit. Non enim praesent elementum facilisis leo vel fringilla est ullamcorper. Non quam lacus suspendisse faucibus interdum. Metus vulputate eu scelerisque felis imperdiet. At ultrices mi tempus imperdiet nulla malesuada pellentesque. At tempor commodo ullamcorper a lacus vestibulum. Consequat semper viverra nam libero justo laoreet sit amet. Facilisis magna etiam tempor orci eu. Convallis tellus id interdum velit laoreet id donec ultrices. Interdum velit euismod in pellentesque massa placerat duis ultricies. Senectus et netus et malesuada fames. Pharetra vel turpis nunc eget lorem dolor. Nisi porta lorem mollis aliquam ut porttitor leo a. Euismod elementum nisi quis eleifend quam adipiscing vitae proin.";
	int parent_length= strlen(text_to_write)/2;
	int child_length= strlen(text_to_write)-parent_length;

	int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("open() error\n");
		exit(EXIT_FAILURE);
	}

	switch (fork()) {
		case 0:
			if (lseek(fd, parent_length, SEEK_SET) == -1) {
				perror("lseek() filed in child\n");
				exit(EXIT_FAILURE);
			}

			if (write(fd, text_to_write + parent_length, child_length) == -1) {
				perror("write() failed in child\n");
				exit(EXIT_FAILURE);
			}
			break;

		case -1:
			printf("fork() failed\n");
			exit(1);
			break;

		default:
			if (write(fd, text_to_write, strlen(text_to_write)/2) == -1) {
				perror("write() failed in parent\n");
				exit(EXIT_FAILURE);
			}

			if (wait(NULL) == -1) {
				perror("wait() failed\n");
				exit(EXIT_FAILURE);
			}

			close(fd);
			exit(0);
	}

	return 0;
}
