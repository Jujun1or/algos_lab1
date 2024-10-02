#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 500
#define MAX_NAME_LENGTH 9

typedef struct Node {
    char name[MAX_NAME_LENGTH];
    struct Node* children[MAX_CHILDREN];
    int child_count;
} Node;

Node* create_node(const char* name) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Ошибка при выделении памяти\n");
        exit(1);
    }
    strncpy(new_node->name, name, MAX_NAME_LENGTH - 1);
    new_node->name[MAX_NAME_LENGTH - 1] = '\0'; 
    new_node->child_count = 0;
    return new_node;
}

int add_path(Node* root, const char* path) {
    char* folder_name;
    char* path_copy = malloc(256 * sizeof(char));
    strcpy(path_copy, path);
    Node* current = root;

    folder_name = strtok(path_copy, "\\");
    while (folder_name != NULL) {
        int found_name = 0;
        for (int i = 0; i < current->child_count; i++) {
            if (strcmp(current->children[i]->name, folder_name) == 0) {
                current = current->children[i];
                found_name = 1;
                break;
            }
        }
        if (!found_name) {
            Node* new_child = create_node(folder_name);
            current->children[current->child_count] = new_child;
            current->child_count++;
            current = new_child;
        }
        folder_name = strtok(NULL, "\\");
    }

    free(path_copy); 
    return 0; 
}

int compare(const void* a, const void* b) {
    Node* node_a = *(Node**)a;
    Node* node_b = *(Node**)b;
    return strcmp(node_a->name, node_b->name);
}

void print_tree(Node* node, int level, FILE * filename) {
    
    if (node == NULL) return;

    qsort(node->children, node->child_count, sizeof(Node*), compare);

    for (int i = 0; i < node->child_count; i++) {
        for (int j = 0; j < level; j++) {
            fprintf(filename, " ");
        }
        fprintf(filename, "%s\n", node->children[i]->name);
        print_tree(node->children[i], level + 1, filename); 
    }
}

void free_tree(Node* node) {
    if (node == NULL) return;
    for (int i = 0; i < node->child_count; i++) {
        free_tree(node->children[i]);
    }
    free(node);
}

int main() {
    FILE* input_file = fopen("input.txt", "r");
    FILE* output_file = fopen("output.txt", "w");

    if (!input_file || !output_file) {
        perror("Ошибка открытия файла\n");
        exit(1);
    }


    int N;
    fscanf(input_file, "%d\n", &N);

    Node* tree = create_node(""); 

    for (int i = 0; i < N; i++) {
        char path[256];
        fgets(path, sizeof(path), input_file);

        for (int pos = 0; path[pos] != '\0'; pos++) {
            if (path[pos] == '\n') {
                path[pos] = '\0'; 
                break; 
            }
        }
        add_path(tree, path);
    }

    print_tree(tree, 0, output_file); 

    fclose(input_file);
    fclose(output_file);

    free_tree(tree);
    return 0;
}
