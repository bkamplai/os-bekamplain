#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    //  Check to see if the correct number of arguments is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: ./process <int>\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    //   Check to see if the provide argument is a valid integer
    if (n == 0 && argv[1][0] != '0') {
        fprintf(stderr, "Usage: ./process <int>\n");
        exit(1);
    }

    printf("Initial Process: PID = %d\n", getpid());

    for(int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            //  Fork failed
            fprintf(stderr, "Fork failed\n");
            exit(1);
        } else if (pid == 0) {
            //  Child(ren) process
            printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
            exit(0);    //  Prevent child(ren) from spawning own child(ren)
        }
    }

    //  Wait for child(ren) process(es) to complete
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}
