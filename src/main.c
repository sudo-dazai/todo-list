#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char description[256];
    char due_date[10];
    int priority;
    int status;  // 0 for incomplete, 1 for complete
} Task;

typedef struct Node {
    Task task;
    struct Node* next;
} Node;

Node* initialize_Task(Node* head, Task newTask) {
    Node* newNode = (Node *)malloc(sizeof(Node));
    newNode->task = newTask;
    newNode->next = head;
    head = newNode;
    return head;
}

Node* delete_Task(Node* head, int taskId) {
    Node* temp = head;
    Node* prev = NULL;

    if(temp != NULL && temp->task.id == taskId) {
        head = temp->next;
        free(temp);
        return head;
    }

    while(temp != NULL && temp->task.id != taskId) {
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL) return head;

    prev->next = temp->next;
    free(temp);
    return head;
}

void save_Task(Node* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        Task task = current->task;
        if (fprintf(file, "%d,%s,%s,%d,%d\n", task.id, task.description, task.due_date, task.priority, task.status) < 0) {
            perror("Error writing to file");
            fclose(file);
            return;
        }
        current = current->next;
    }

    if (fclose(file) != 0) {
        perror("Error closing file");
    } else {
        printf("Tasks successfully saved to %s\n", filename);
    }
    
}

void load_Task(Node** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        perror("Could not open file");
        return;
    }

    char line[300];
    while(fgets(line, sizeof(line), file)) {
        Task task;
        sscanf(line, "%d,%255[^,],%9[^,],%d,%d", &task.id, task.description, task.due_date, &task.priority, &task.status);
        *head = initialize_Task(*head, task);
    }

    fclose(file);
}

void view_Tasks(Node* head) {
    Node* current = head;
    if (current == NULL) {
        printf("No tasks available.\n");
        return;
    }
    while(current != NULL) {
        Task task = current->task;
        printf("ID: %d\nDescription: %s\nDue Date: %s\nPriority: %d\nStatus: %s\n\n",
               task.id, task.description, task.due_date, task.priority,
               task.status ? "Completed" : "Incomplete");
        current = current->next;
    }
}

void update_Task_Status(Node* head, int taskId, int status) {
    Node* current = head;
    while (current != NULL) {
        if (current->task.id == taskId) {
            current->task.status = status;
            return;
        }
        current = current->next;
    }
    printf("Task ID %d not found.\n", taskId);
}



int main(int argc, char **argv) {
    Node* head = NULL;
    char filename[] = "tasks.txt";

    load_Task(&head, filename);

    char command[10];
    Task newTask;
    int taskId;
    int status;
    char input[256]; // Buffer for user input

    while (1) {
        printf("\nEnter command (add, remove, update, view, save, exit): ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("Error reading command");
            break;
        }
        command[strcspn(command, "\n")] = '\0';  // Remove newline character

        if (strcmp(command, "add") == 0) {
            printf("Enter task description: ");
            if (fgets(newTask.description, sizeof(newTask.description), stdin) == NULL) {
                perror("Error reading description");
                break;
            }
            newTask.description[strcspn(newTask.description, "\n")] = '\0';  // Remove newline character

            printf("Enter due date (YYYY-MM-DD): ");
            if (fgets(newTask.due_date, sizeof(newTask.due_date), stdin) == NULL) {
                perror("Error reading due date");
                break;
            }
            newTask.due_date[strcspn(newTask.due_date, "\n")] = '\0';  // Remove newline character

            printf("Enter priority (1-3): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading priority");
                break;
            }
            sscanf(input, "%d", &newTask.priority);

            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                // Discard character
            }

            newTask.status = 0;  // New task is incomplete
            newTask.id = (head == NULL) ? 1 : head->task.id + 1;  // Set task ID

            head = initialize_Task(head, newTask);
        } else if (strcmp(command, "remove") == 0) {
            printf("Enter task ID to remove: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading task ID");
                break;
            }
            sscanf(input, "%d", &taskId);

            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                // Discard character
            }

            head = delete_Task(head, taskId);
        } else if (strcmp(command, "update") == 0) {
            printf("Enter task ID to update: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading task ID");
                break;
            }
            sscanf(input, "%d", &taskId);

            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                // Discard character
            }

            printf("Enter new status (0 for incomplete, 1 for complete): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading status");
                break;
            }
            sscanf(input, "%d", &status);

            // Clear the input buffer
            while ((c = getchar()) != '\n' && c != EOF) {
                // Discard character
            }

            update_Task_Status(head, taskId, status);
        } else if (strcmp(command, "view") == 0) {
            view_Tasks(head);
        } else if (strcmp(command, "save") == 0) {
            save_Task(head, filename);
        } else if (strcmp(command, "exit") == 0) {
            save_Task(head, filename);
            printf("Tasks saved. Exiting...\n");
            break;
        } else {
            printf("Invalid command. Please try again.\n");
        }
    }

    // Clean up memory
    Node* current = head;
    Node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    return 0;
}
