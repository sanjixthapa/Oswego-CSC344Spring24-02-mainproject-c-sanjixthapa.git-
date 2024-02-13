#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//function prototypes to allow compiler to recognize functions before actual definitions
void push(int data);
int pop();
void swap();
void in();
void dup(int n);
void execute_instruction(char* opcode, int argument, int* counter);
void ifeq(int argument, int *counter);
void iflt(int argument, int *counter);
void jump(int argument, int *counter);


//this struct represents a node in a linked list, each node contains an integer value and a pointer to next node
struct Node {
    int data;
    struct Node* next;
};

//declares a pointer top to a Node structure and initializes it to NULL
struct Node* top = NULL;

void push(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = top;
    top = newNode;
}

int pop() {
    if (top == NULL) {
        printf("Stack is empty\n");
        return -1;
    }
    struct Node* temp = top;
    int data = top->data;
    top = top->next;
    free(temp); // Free memory of popped node
    return data;
}

void swap() {
    if (top != NULL && top->next != NULL) {
        int temp = top->data;
        top->data = top->next->data;
        top->next->data = temp;
    } else {
        printf("Not enough elements to perform swap\n");
    }
}

void in() {
    int data;
    printf("Enter an integer: ");
    scanf("%d", &data);
    push(data);
}
void dup(int n) {
    if (n <= 0) {
        printf("Invalid value of N for DUP\n");
        return;
    }

    struct Node* current = top;
    struct Node* tempStack = NULL;

    // Push the top N elements onto a temporary stack
    for (int i = 0; i < n && current != NULL; i++) {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        newNode->data = current->data;
        newNode->next = tempStack;
        tempStack = newNode;
        current = current->next;
    }

    // Push the elements from the temporary stack back onto the original stack
    while (tempStack != NULL) {
        struct Node* temp = tempStack;
        push(tempStack->data);
        tempStack = tempStack->next;
        free(temp);
    }
}

void execute_instruction(char* opcode, int argument, int* counter) {
    int data, data1, data2, result;
    if (strcmp(opcode, "IN") == 0) {
        in();
    } else if (strcmp(opcode, "OUT") == 0) {
        data = pop();
        printf("%d\n", data);

    } else if (strcmp(opcode, "DROP") == 0) {
        pop();

    } else if (strcmp(opcode, "ADD") == 0) {
        data1 = pop();
        data2 = pop();
        result = data1 + data2;
        push(result);

    } else if (strcmp(opcode, "SUB") == 0) {
        data1 = pop();
        data2 = pop();
        result = data2 - data1;
        push(result);

    } else if (strcmp(opcode, "MOD") == 0) {
        data1 = pop();
        data2 = pop();
        result = data1 % data2;
        push(result);

    } else if (strcmp(opcode, "DIV") == 0) {
        data1 = pop();
        data2 = pop();
        result = data2 / data1;
        push(result);

    } else if (strcmp(opcode, "MUL") == 0) {
        data1 = pop();
        data2 = pop();
        result = data1 * data2;
        push(result);

    } else if (strcmp(opcode, "AND") == 0) {
        data1 = pop();
        data2 = pop();
        result = data1 & data2;
        push(result);

    } else if (strcmp(opcode, "OR") == 0) {
        data1 = pop();
        data2 = pop();
        result = data1 | data2;
        push(result);

    } else if (strcmp(opcode, "LIT") == 0) {
        push(argument);
    } else if (strcmp(opcode, "SWAP") == 0) {
        swap();
    } else if (strcmp(opcode, "DUP") == 0) {
        dup(argument);
    } else if (strcmp(opcode, "IFEQ") == 0) {
        ifeq(argument, counter);
    } else if (strcmp(opcode, "IFLT") == 0) {
        iflt(argument, counter);
    } else if (strcmp(opcode, "JUMP") == 0) {
        jump(argument, counter);
    }
}

void ifeq(int argument, int* counter) {
    if (top == NULL || top->next == NULL) {
        printf("Not enough elements\n");
        return;
    }
    int secondElement = pop();
    int firstElement = pop();
    if (secondElement == firstElement) {
        *counter = argument; // Jump to the specified instruction
    }
}

void iflt(int argument, int *counter) {
    if (top == NULL || top->next == NULL) {
        printf("Not enough elements\n");
        return;
    }
    int secondElement = pop();
    int firstElement = pop();
    if (firstElement < secondElement) {
        *counter = argument; // Jump to the specified instruction
    }
}

void jump(int argument, int *counter) {
    *counter = argument; // Unconditional jump to the specified instruction
}

int main() {
    char line[256];
    char opcode[255];
    int argument;
    int counter = -1;

    //open text file
    FILE *pF = fopen("input.txt", "r");
    if (pF == NULL) {
        printf("Error opening file");
        return 1;
    }

    // Read the entire file into memory as an array of strings
    char instructions[1000][256]; // Assuming maximum 1000 lines
    int instructionCount = 0;

    //reads each line from input file using and stores it in 'line' array
    //then copies contents of 'line' array into 'instructions' at the specific index 'instructionCount'
    while (fgets(line, sizeof(line), pF) != NULL) {
        strcpy(instructions[instructionCount], line);
        instructionCount++;
    }
    fclose(pF); // Close the file

    //while loop iterates over 'instructions' array
    //sscanf to parse the opcode and argument from each instruction string
    //increments counter to next instruction
    // Execute instructions based on the counter value
    while (counter < instructionCount) {
        sscanf(instructions[counter], "%s %d", opcode, &argument);
        counter++;
        execute_instruction(opcode, argument, &counter);
    }
    return 0;
}