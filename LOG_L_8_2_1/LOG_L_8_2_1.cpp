#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>

typedef struct Node {
	int data;
	struct Node* next;
} Node;

int_fast8_t** create_graph(int size);
void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void print_graph(int size, int_fast8_t** graph_pointer);
void enqueueBFS(Node** head, int data);
int dequeue(Node** head);
void BFS(int_fast8_t** graph, int size, int start);

int main(void) {
	setlocale(LC_ALL, "Russian");
	int_fast8_t** graph;
	int size;
	float edge_probability;

	fprintf(stdout, "Введите количество вершин графа: ");
	if (!scanf("%d", &size) || size <= 1) {
		fprintf(stderr, "Ошибка ввода размера массива");
		exit(1);
	}

	fprintf(stdout, "Введите вероятность создания ребра (от 0 до 100): ");
	if (!scanf("%f", &edge_probability) || edge_probability < 0 || edge_probability > 100) {
		fprintf(stderr, "Ошибка ввода вероятности");
		exit(1);
	}

	graph = create_graph(size);
	srand((unsigned int)time(NULL));
	initialize_graph(size, graph, edge_probability);
	printf("Матрица смежности: \n\n");
	print_graph(size, graph);

	BFS(graph, size, 0);

	for (int i = 0; i < size; i++) {
		free(graph[i]);
	}
	free(graph);

	return 0;
}

int_fast8_t** create_graph(int size) {
	int_fast8_t** array = (int_fast8_t**)malloc(sizeof(int_fast8_t*) * size);
	for (int i = 0; i < size; i++) {
		array[i] = (int_fast8_t*)malloc(sizeof(int_fast8_t) * size);
	}
	if (array == NULL) {
		fprintf(stderr, "Ошибка создания массива");
		exit(1);
	}
	return array;
}

void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
				graph_pointer[i][j] = 1;
				graph_pointer[j][i] = 1;
			}
			else {
				graph_pointer[i][j] = 0;
				graph_pointer[j][i] = 0;
			}
		}
		graph_pointer[i][i] = 0;
	}
}

void print_graph(int size, int_fast8_t** graph_pointer) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(stdout, "%d ", graph_pointer[j][i]);
		}
		fprintf(stdout, "\n");
	}
}

void enqueueBFS(Node** head, int data) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->next = NULL;

	if (*head == NULL) {
		*head = newNode;
		return;
	}

	Node* current = *head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = newNode;
}

int dequeue(Node** head) {
	if (*head == NULL) {
		return -1;
	}

	Node* temp = *head;
	int data = temp->data;
	*head = (*head)->next;
	free(temp);
	return data;
}

void BFS(int_fast8_t** graph, int size, int start) {
	int* visited = (int*)calloc(size, sizeof(int));
	if (visited == NULL) {
		fprintf(stderr, "Ошибка выделения памяти");
		exit(1);
	}

	Node* queue = NULL;
	visited[start] = 1;
	enqueueBFS(&queue, start);
	printf("\nПорядок обхода вершин в ширину: ");
	printf("%d ", start + 1);

	while (queue != NULL) {
		int current = dequeue(&queue);

		for (int i = 0; i < size; i++) {
			if (graph[current][i] == 1 && !visited[i]) {
				visited[i] = 1;
				enqueueBFS(&queue, i);
				printf("%d ", i + 1);
			}
		}
	}
	printf("\n");
	free(visited);
}