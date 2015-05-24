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
	pid_t pid;//, wpid;
	program_name = argv[0];
	char buf[PATH_MAX + 1];
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
			if (countOpti >= 1) {
				print_usage();
			} else {
				countOptR = 1;
			}
			break;
		default:
			print_usage();
		}
	}
	for (int i = 0; optind + 1 + i < argc; i++) {

		switch (pid = fork()) {
		case 1:
			printf("Error at fork()");
			break;
		case 0:
			//printf("Child with PID %d\n", getpid());
			getpid();
			realpath(argv[optind], buf);
			search(buf, argv[optind + 1 + i], countOpti, countOptR);
			return 0;

		default:
			//printf("Parent Process PID=%d\n", getpid());
			/*while ((wpid = wait(&status)) != pid) {
				if (wpid != -1) {
					continue;
					fprintf(stderr, "myfork: Fehler bei wait\n");
					return EXIT_FAILURE;
				}
				if (WIFEXITED(status)) {
					printf("Kindprozess normal beendet, Exit Code: %d\n", WEXITSTATUS(status));
				} else {
					printf("Kindprozess mit Fehler oder durch Signal beendet\n");
				}
			}*/
			wait(NULL);
		}
	}
	return 0;
}


void search(char* directory, char* filename, int countOpti, int countOptR)
{
	struct dirent *direntp;
	pid_t pid;
	DIR *dirp;
	char subdir[PATH_MAX + 1];
	dirp = opendir(directory);
//	printf("Search for: %s \nIn directory: %s\n\n", filename, directory);
	while ((direntp = readdir(dirp))) {
		//printf("Direntp: %s\nFilename: %s\n", direntp->d_name, filename);
		if (countOpti < 0) {
			if (strcmp(direntp->d_name, filename) == 0) {
				if (strcmp(directory, "/") != 0) {
					printf("%s/%s\n", directory, direntp->d_name);
				} else {
					printf("/%s\n", direntp->d_name);
				}
			}
		} else {
			if (strcasecmp(direntp->d_name, filename) == 0) {
				if (strcmp(directory, "/") != 0) {
					printf("%s/%s\n", directory, direntp->d_name);
				} else {
					printf("/%s\n", direntp->d_name);
				}
			}
		}
		if (countOptR != 0) {
			if (direntp->d_type == 4) {
				if (strcmp(direntp->d_name, "..") != 0 && strcmp(direntp->d_name, ".") != 0) {
					switch (pid = fork()) {
					case 0:
						strcpy(subdir, directory);
						if (strcmp(directory, "/") != 0) {
							strcat(subdir, "/");
						}
						strcat(subdir, direntp->d_name);
						search(subdir, filename, countOpti, countOptR);
						return;
					default:
						wait(NULL);
					}
				}
			}
		}
	}
	closedir(dirp);
}

void print_usage()
{
	fprintf(stderr, "Usage: %s directory_name\n", program_name);
	exit(1);
}


