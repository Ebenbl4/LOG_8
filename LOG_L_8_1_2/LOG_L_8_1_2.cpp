#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <locale>
#include <stdbool.h>

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

Node** create_adj_list(int size);
void initialize_adj_list(int size, Node** adj_list, float edge_prob);
void print_adj_list(int size, Node** adj_list);
void free_adj_list(int size, Node** adj_list);
void add_edge(Node** adj_list, int src, int dest);
void BFS(Node** adj_list, int size, int start);

int main(void) {
    setlocale(LC_ALL, "Russian");
    Node** adj_list;
    int size;
    float edge_prob;

    printf("Введите количество вершин графа: ");
    if (!scanf("%d", &size) || size <= 1) {
        fprintf(stderr, "Ошибка ввода размера\n");
        exit(1);
    }

    printf("Введите вероятность создания ребра (от 0 до 100): ");
    if (!scanf("%f", &edge_prob) || edge_prob < 0 || edge_prob > 100) {
        fprintf(stderr, "Ошибка ввода вероятности\n");
        exit(1);
    }

    adj_list = create_adj_list(size);
    initialize_adj_list(size, adj_list, edge_prob);
    printf("\nСписок смежности графа:\n\n");
    print_adj_list(size, adj_list);

    printf("\nПорядок обхода вершин в ширину: ");
    BFS(adj_list, size, 0);
    printf("\n");

    free_adj_list(size, adj_list);
    return 0;
}

void print_adj_list(int size, Node** adj_list) {
    for (int i = 0; i < size; i++) {
        printf("%d: ", i + 1);
        Node* current = adj_list[i];
        while (current != NULL) {
            printf("%d ", current->vertex + 1);
            current = current->next;
        }
        printf("\n");
    }
}

void free_adj_list(int size, Node** adj_list) {
    for (int i = 0; i < size; i++) {
        Node* current = adj_list[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(adj_list);
}

Node** create_adj_list(int size) {
    Node** adj_list = (Node**)malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        adj_list[i] = NULL;
    }
    return adj_list;
}

void add_edge(Node** adj_list, int src, int dest) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->next = NULL;

    if (adj_list[src] == NULL) {
        adj_list[src] = newNode;
    }
    else {
        Node* current = adj_list[src];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void initialize_adj_list(int size, Node** adj_list, float edge_prob) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if ((float)rand() / (RAND_MAX / 100) < edge_prob) {
                add_edge(adj_list, i, j);
                add_edge(adj_list, j, i);
            }
        }
    }
}

void BFS(Node** adj_list, int size, int start) {
    bool* visited = (bool*)calloc(size, sizeof(bool));
    if (!visited) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return;
    }

    int* queue = (int*)malloc(size * sizeof(int));
    if (!queue) {
        free(visited);
        fprintf(stderr, "Ошибка выделения памяти\n");
        return;
    }

    int front = 0, rear = 0;

    queue[rear++] = start;
    visited[start] = true;
    printf("%d ", start + 1);

    if (adj_list[start] == NULL) {
        free(visited);
        free(queue);
        return;
    }

    while (front < rear) {
        int current = queue[front++];

        Node* temp = adj_list[current];
        while (temp != NULL) {
            int adj_vertex = temp->vertex;
            if (!visited[adj_vertex]) {
                queue[rear++] = adj_vertex;
                visited[adj_vertex] = true;
                printf("%d ", adj_vertex + 1);
            }
            temp = temp->next;
        }
    }

    bool found_unvisited = false;
    for (int i = 0; i < size; i++) {
        if (!visited[i]) {
            if (!found_unvisited) {
                found_unvisited = true;
            }
            BFS(adj_list, size, i);
        }
    }

    free(visited);
    free(queue);
}