#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char description[256];
    char due_date[10];
    int priority;
    int status;
} Task;

typedef struct Node {
    Task task;
    struct Node* next;
} Node;

void initialize_Task(Node* head, Task newTask); 
void delete_Task(Node* head, int taskId);  
void save_Task(Node* head, const char* filename);
void load_Task(Node* head, const char* filename);  

#endif
