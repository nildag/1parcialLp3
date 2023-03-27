#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void send_signal(int process_id, int signal_number) {
    printf("Sending signal %d to process %d\n", signal_number, process_id);
    kill(process_id, signal_number);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        exit(1);
    }
    
    FILE* f = fopen(argv[1], "r");
    if (f == NULL) {
        perror("fopen");
        exit(1);
    }
    
    int process_id, signal_number, delay;
    while (fscanf(f, "%d %d %d", &process_id, &signal_number, &delay) != EOF) {
        sleep(delay);
        send_signal(process_id, signal_number);
    }
    
    fclose(f);
    return 0;
}

