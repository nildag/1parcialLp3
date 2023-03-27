#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


#define MAX_LENGTH 100

// Funciones para el algoritmo de ordenación mergesort
void merge(int* array, int left, int middle, int right);
void mergesort(int* array, int left, int right);
void mergesort_parallel(int* array, int length, int process_id, int num_processes);

// Funciones para la impresión de resultados
void print_mapping(int* array, int length, int process_id, int num_processes);
void print_tree(int depth, int node_id, int num_processes);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s num_processes input_array\n", argv[0]);
        exit(1);
    }
    int num_processes = atoi(argv[1]);
    char* input_array = argv[2];
    int length = 0;
    int array[MAX_LENGTH];
    char* token = strtok(input_array, ",");
    while (token != NULL) {
        array[length] = atoi(token);
        length++;
        token = strtok(NULL, ",");
    }
    if (length == 0) {
        printf("Error: empty input array.\n");
        exit(1);
    }
    if (num_processes > length) {
        printf("Error: number of processes must be less than or equal to the length of the input array.\n");
        exit(1);
    }
    // Imprimir la lista de entrada
    printf("Input list: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    // Realizar la ordenación mergesort en paralelo
    mergesort_parallel(array, length, 0, num_processes);
    // Imprimir la lista de salida
    printf("Output list: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}

// Función para imprimir el mapeo de cada sublista de entrada de cada subproceso
void print_mapping(int* array, int length, int process_id, int num_processes) {
    int start_index = process_id * length / num_processes;
    int end_index = (process_id + 1) * length / num_processes - 1;
    printf("Process %d mapping: ", process_id);
    for (int i = start_index; i <= end_index; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

// Función para imprimir el esquema gráfico en árbol
void print_tree(int depth, int node_id, int num_processes) {
if (depth == 0) {
printf("Root process\n");
return;
}
for (int i = 0; i < depth -1; i++) {
printf("|");
}
printf("|");
for (int i = 0; i < num_processes; i++) {
if (i == node_id) {
printf("*");
}
printf("*");
}
printf("\n");
}

// Función para mezclar dos sublistas ordenadas en una sola lista ordenada
void merge(int* array, int left, int middle, int right) {
int i, j, k;
int n1 = middle - left + 1;
int n2 = right - middle;
int left_array[n1], right_array[n2];
for (i = 0; i < n1; i++) {
left_array[i] = array[left + i];
}
for (j = 0; j < n2; j++) {
right_array[j] = array[middle + 1 + j];
}
i = 0;
j = 0;
k = left;
while (i < n1 && j < n2) {
if (left_array[i] <= right_array[j]) {
array[k] = left_array[i];
i++;
} else {
array[k] = right_array[j];
j++;
}
k++;
}
while (i < n1) {
array[k] = left_array[i];
i++;
k++;
}
while (j < n2) {
array[k] = right_array[j];
j++;
k++;
}
}

// Función para ordenar una sublista usando el algoritmo mergesort
void mergesort(int* array, int left, int right) {
if (left < right) {
int middle = left + (right - left) / 2;
mergesort(array, left, middle);
mergesort(array, middle + 1, right);
merge(array, left, middle, right);
}
}

// Función para ordenar una lista usando el algoritmo mergesort en paralelo
void mergesort_parallel(int* array, int length, int process_id, int num_processes) 
{
	if (num_processes == 1)
	{
		mergesort(array, 0, length - 1);
		return;
	}
	// Crear dos procesos hijos
	int fd[2];
	if (pipe(fd) == -1) 
	{
		perror("pipe");
		exit(1);
	}
	int child_id_left = fork();
	if (child_id_left == -1) 
	{
		perror("fork");
		exit(1);
	}
	if (child_id_left == 0) 
	{
		// Cerrar el extremo de escritura del pipe
		if (close(fd[1]) == -1)
		{
			perror("close");
			exit(1);
		}
		// Ordenar la primera mitad de la lista en el proceso hijo izquierdo
		mergesort_parallel(array, length / 2, 0, num_processes / 2);
		// Leer la lista ordenada del proceso hijo izquierdo
		int left_array[length / 2];
		if (read(fd[0], left_array, (length / 2) * sizeof(int)) == -1) 
		{
			perror("read");
			exit(1);
		}
	}
}	
