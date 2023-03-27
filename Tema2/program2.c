#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void sigint_handler(int signum) {
    printf("Received SIGINT\n");
}

void sigalrm_handler(int signum) {
    printf("Received SIGALRM\n");
}

void sigusr1_handler(int signum) {
    printf("Received SIGUSR1\n");
}

void sigusr2_handler(int signum) {
    printf("Received SIGUSR2\n");
}

int main() {
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalrm_handler);
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    
    while (1) {
        pause();
    }
    
    return 0;
}

