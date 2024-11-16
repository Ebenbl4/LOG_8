#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>
#include <queue>

typedef struct Node {
	int data;
	struct Node* next;
} Node;

int_fast8_t** create_graph(int size);
void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void print_graph(int size, int_fast8_t** graph_pointer);
void kls_queue_BFS(int v, int size, int_fast8_t** graph_pointer, bool* visited);
void kls_queue_breadth_search(int size, int_fast8_t** graph_pointer);
void manually_enqueueBFS(Node** head, int data);
int manually_dequeue(Node** head);
void manually_BFS(int_fast8_t** graph, int size, int start);

int main(void) {
	setlocale(LC_ALL, "Russian");
	int_fast8_t** graph;
	int size;
	float edge_probability;
	clock_t search1_start, search1_end, search2_start, search2_end;
	double search_time1, search_time2;

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

	printf("\nПорядок обхода вершин в ширину (стандартный класс queue): ");
	search1_start = clock();
	kls_queue_breadth_search(size, graph);
	search1_end = clock();
	search_time1 = ((double)(search1_end - search1_start)) / CLOCKS_PER_SEC * 1000;
	printf("\nВремя обхода: %f ms\n", search_time1);

	search2_start = clock();
	manually_BFS(graph, size, 0);
	search2_end = clock();
	search_time2 = ((double)(search2_end - search2_start)) / CLOCKS_PER_SEC * 1000;
	printf("Время обхода: %f ms\n", search_time2);

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

void kls_queue_BFS(int v, int size, int_fast8_t** graph_pointer, bool* visited) {
	std::queue<int> Q;

	Q.push(v);
	visited[v] = true;

	while (!Q.empty()) {
		v = Q.front();
		Q.pop();
		printf("%d ", v + 1);

		for (int i = 0; i < size; i++) {
			if (graph_pointer[v][i] == 1 && !visited[i]) {
				Q.push(i);
				visited[i] = true;
			}
		}
	}
}

void kls_queue_breadth_search(int size, int_fast8_t** graph_pointer) {
	bool* visited = new bool[size];
	for (int i = 0; i < size; i++) {
		visited[i] = false;
	}

	for (int i = 0; i < size; i++) {
		if (!visited[i]) {
			kls_queue_BFS(i, size, graph_pointer, visited);
		}
	}

	delete[] visited;
}

void manually_enqueueBFS(Node** head, int data) {
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

int manually_dequeue(Node** head) {
	if (*head == NULL) {
		return -1;
	}

	Node* temp = *head;
	int data = temp->data;
	*head = (*head)->next;
	free(temp);
	return data;
}

void manually_BFS(int_fast8_t** graph, int size, int start) {
	int* visited = (int*)calloc(size, sizeof(int));
	if (visited == NULL) {
		fprintf(stderr, "Ошибка выделения памяти");
		exit(1);
	}

	Node* queue = NULL;
	visited[start] = 1;
	manually_enqueueBFS(&queue, start);
	printf("\nПорядок обхода вершин в ширину (структура данных список): ");
	printf("%d ", start + 1);

	while (queue != NULL) {
		int current = manually_dequeue(&queue);

		for (int i = 0; i < size; i++) {
			if (graph[current][i] == 1 && !visited[i]) {
				visited[i] = 1;
				manually_enqueueBFS(&queue, i);
				printf("%d ", i + 1);
			}
		}
	}
	printf("\n");
	free(visited);
}