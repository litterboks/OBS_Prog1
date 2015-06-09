#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

char *program_name;

void print_usage();

void search(char  *path, char *filename, int countOpti, int countOptR);

void searchRecurse();


int main(int argc, char* argv[])
{
	program_name = argv[0];
	pid_t pid, wpid;
	int status = 0;
	int c;
	int countOpti = 0;
	int countOptR = 0;
	while ((c = getopt(argc, argv, "iR")) != EOF) {
		switch (c) {

		case 'i':
			if (countOpti >= 1) {
				print_usage();
			} else {
				countOpti = 1;
			}
			break;
		case 'R':
			if (countOptR >= 1) {
				print_usage();
			} else {
				countOptR = 1;
			}
			break;
		case '?':
			print_usage();
			break;
		default:
			print_usage();
		}
	}
//Wenn kein Pfad und Dateiname angegeben wurden usage ausgeben
	if (argc < optind + 2) {
		print_usage();
	}

//Absoluten Pfad ermitteln
	char buf[PATH_MAX + 1];
	char* absolute_path = realpath(argv[optind++], buf);

//Prozesse für jedes Suchwort angeben
	while (optind < argc) {

		switch (pid = fork()) {
		case -1:
			printf("Error at fork()");
			break;
		case 0:
			search(absolute_path, argv[optind], countOpti, countOptR);
			return 0;

		default:
			;
		}
		//Nächstes Suchwort
		optind++;
	}
	while ((wpid = wait(&status)) > 0) {
	//	printf("Exit status of %d was %d\n", (int)wpid, status);
	}
	return 0;
}

void search(char* directory, char* filename, int countOpti, int countOptR)
{
	//Gewünschtes Format:
	//<pid>: <filename>: <complete-path-to-found-file>\n
	struct dirent *direntp;
	DIR *dirp;
	char subdir[PATH_MAX + 1];

	//Verzeichnis öffnen mit Fehlerabfrage
	if ((dirp = opendir(directory)) == NULL) {
		printf("%d: Failed to open directory %s\n",(int)getpid(), directory);
		return;
	}
	//Verzeichnis lesen
	while ((direntp = readdir(dirp))) {
		//Case insensitive
		if (countOpti < 0) {
			if (strcmp(direntp->d_name, filename) == 0) {
				if (strcmp(directory, "/") != 0) {
					printf("%d: %s: %s/%s\n", (int)getpid() , direntp->d_name, directory, direntp->d_name);
				} else {
					printf("%d: %s: /%s\n", (int)getpid(), direntp->d_name, direntp->d_name);
				}
			}
		//Case sensitive
		} else {

			if (strcasecmp(direntp->d_name, filename) == 0) {
				if (strcmp(directory, "/") != 0) {
					printf("%d: %s: %s/%s\n", (int)getpid(), direntp->d_name, directory, direntp->d_name);
				} else {
					printf("%d: %s: /%s\n", (int)getpid(), direntp->d_name, direntp->d_name);
				}
			}
		}
		//Rekursion
		if (countOptR != 0) {
			if (direntp->d_type == 4) {
				if (strcmp(direntp->d_name, "..") != 0 && strcmp(direntp->d_name, ".") != 0) {
					strcpy(subdir, directory);
					if (strcmp(directory, "/") != 0) {
						strcat(subdir, "/");
					}
					strcat(subdir, direntp->d_name);
					search(subdir, filename, countOpti, countOptR);
				}
			}
		}
	}
	closedir(dirp);
	return;
}

void print_usage()
{
	fprintf(stderr, "Usage: %s directory_name\n", program_name);
	exit(1);
}
