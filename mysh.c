#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX 20

void tokenize(char *input, char *A[MAX]) {
    char *p;
    int i;

    p = strtok(input, " ");
    for (i = 0; p != NULL && i < (MAX - 1); ++i) {
        A[i] = p;
        p = strtok(NULL, " \t");
    }
    A[i] = NULL;
}

int main() {
    char input[256];
    char *A[MAX];
    char prompt[50] = "$ ";

    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0;  /* Remove newline character */

        tokenize(input, A);

        if (A[0] == NULL) {
            continue;
        } else if (strcmp(A[0], "exit") == 0) {
            break;
        } else if (strcmp(A[0], "echo") == 0) {
            int i = 1;
            while (A[i] != NULL) {
                printf("%s ", A[i]);
                i++;
            }
            if (strcmp(A[1], "-n") != 0) {
                printf("\n");
            }
        } else if (strcmp(A[0], "PS1") == 0) {
            strcpy(prompt, A[1]);
        } else if (strcmp(A[0], "cat") == 0) {
            FILE *fp = fopen(A[1], "r");
            if (fp != NULL) {
                char c;
                while ((c = fgetc(fp)) != EOF) {
                    putchar(c);
                }
                fclose(fp);
                printf("\n");
            } else {
                printf("File not found\n");
            }
        } else if (strcmp(A[0], "cp") == 0) {
            FILE *source, *target;
            char ch;

            source = fopen(A[1], "r");
            if (source == NULL) {
                printf("Source file not found\n");
                continue;
            }

            target = fopen(A[2], "w");
            if (target == NULL) {
                fclose(source);
                printf("Error creating target file\n");
                continue;
            }

            while ((ch = fgetc(source)) != EOF) {
                fputc(ch, target);
            }

            fclose(source);
            fclose(target);
            printf("File copied successfully\n");
        } else if (strcmp(A[0], "rm") == 0) {
            if (remove(A[1]) == 0) {
                printf("File deleted successfully\n");
            } else {
                printf("Error deleting file\n");
            }
        } else if (strcmp(A[0], "mkdir") == 0) {
            if (mkdir(A[1], 0777) == 0) {
                printf("Directory created successfully\n");
            } else {
                printf("Error creating directory\n");
            }
        } else if (strcmp(A[0], "rmdir") == 0) {
            if (rmdir(A[1]) == 0) {
                printf("Directory removed successfully\n");
            } else {
                printf("Error removing directory\n");
            }
        } else {
            printf("Usage: echo, PS1, cat, cp, rm, mkdir, rmdir, exit\n");
        }
    }

    return 0;
}
