#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Gate {INPUT, OUTPUT, NOT, AND, OR, NAND, NOR, XOR, DECODER, MULTIPLEXER, PASS};

typedef struct VarNode {
    int value;
    int isAnOutput;
    struct VarNode *next;
    char name[128];
} VarNode;

typedef struct InstructionNode {
    int numIns;
    int numOuts;
    struct InstructionNode *next;
    enum Gate type;
    char inputs[50][256];
    char outputs[50][256];
    char switches[50][256];
} InstructionNode;

typedef struct Hashtable {
    int size;
    struct VarNode **table;
} Hashtable;

int **TruthTable;
Hashtable *varTable;
InstructionNode *front;

void MallocTable(int);
void ScanFile(FILE*, int*, int*);
void MakeTruthTable(int);
void IntToBin(int, int, int*);
void PrintTable(int, int);
int HashIt(char*);
int GetIndex(VarNode**, int);
void NewInstruction(InstructionNode*);
void InsertVar(VarNode*);
struct VarNode *GetValue(char*);

int main(int argc, char **argv) {
    if (argc != 2) {
        return -1;
    }
    FILE *fp=fopen(argv[1], "r");
    if (fp==NULL) {
        return -1;
    }
    int ins = 0;
    int outs = 0;
    MallocTable(128);
    ScanFile(fp, &ins, &outs);
    fclose(fp);
    MakeTruthTable(ins);
    PrintTable(ins, outs);
    return 0;
}

void MallocTable(int size) {
    varTable = (Hashtable *)malloc(sizeof(Hashtable));
    varTable->size = size;
    varTable->table = malloc(sizeof(VarNode *) * size);
    int i;
    for (i = 0; i < size; i++) {
        varTable->table[i] = NULL;
    }
}

void ScanFile(FILE *fp, int *inCount, int *outCount) {
    char current[128];
    while (fscanf(fp, "%s", current) != EOF) {
        if (strcmp(current, "INPUT") == 0) {
            fscanf(fp, "%s", current);
            *inCount= atoi(current);
            fscanf(fp, "%s", current);
            if (strcmp(current, ":") == 0) {
                fscanf(fp, "%s", current);
            }
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= *inCount;
            node->numOuts = 0;
            node->type = INPUT;
            int inputIndex = 0;
            int i;
            for (i = 0; i < *inCount; i++) {
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                }
                VarNode *temp = malloc(sizeof(VarNode));
                strcpy(temp->name, current);
                temp->value = 0;
                temp->isAnOutput = 0;
                temp->next = NULL;
                InsertVar(temp);
                fscanf(fp, "%s", current);
            }
            NewInstruction(node);
        }
        if (strcmp(current, "OUTPUT") == 0) {
            fscanf(fp, "%s", current);
            *outCount= atoi(current);
            fscanf(fp, "%s", current);
            if (strcmp(current, ":") == 0) {
                fscanf(fp, "%s", current);
            }
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= *outCount;
            node->numOuts = 0;
            node->type = OUTPUT;
            int inputIndex = 0;
            int i;
            for (i = 0; i < *outCount; i++) {
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                }
                VarNode *temp= malloc(sizeof(VarNode));
                strcpy(temp->name, current);
                temp->value = 0;
                temp->isAnOutput = 1;
                temp->next = NULL;
                InsertVar(temp);
                fscanf(fp, "%s", current);
            }
            NewInstruction(node);
        }
        if (strcmp(current, "AND") == 0) {
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= 2;
            node->numOuts = 1;
            node->type = AND;

            int inputIndex = 0;
            int i;
            for (i = 0; i < 3; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current); 
                }
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                }else {
                    strcpy(node->outputs[0], current);
                }
                VarNode *temp = malloc(sizeof(VarNode));
                strcpy(temp->name, current);
                if (strcmp(current, "1") == 0) {
                    temp->value = 1;
                }else {
                    temp->value = 0;
                }
                temp->isAnOutput = 0;
                temp->next = NULL;
                InsertVar(temp);
            }
            NewInstruction(node);
        }
        if (strcmp(current, "OR") == 0) {
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= 2;
            node->numOuts = 1;
            node->type = OR;
            int inputIndex = 0;
            int i;
            for (i = 0; i < 3; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current); 
                }
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                }else {
                    strcpy(node->outputs[0], current);
                }
                VarNode *temp = malloc(sizeof(VarNode));
                strcpy(temp->name, current);
                if (strcmp(current, "1") == 0) {
                    temp->value = 1;
                }else {
                    temp->value = 0;
                }
                temp->isAnOutput = 0;
                temp->next = NULL;
                InsertVar(temp);
            }
            NewInstruction(node);
        }
        if (strcmp(current, "NOT") == 0) {
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= 1;
            node->numOuts = 1;
            node->type = NOT;
            int inputIndex = 0;
            int i;
            for (i = 0; i < 2; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current); 
                }
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                } else {
                    strcpy(node->outputs[0], current);
                }
                VarNode *temp = malloc(sizeof(VarNode));
                strcpy(temp->name, current);
                if (strcmp(current, "1") == 0) {
                    temp->value = 1;
                }else {
                    temp->value = 0;
                }
                temp->isAnOutput = 0;
                temp->next = NULL;
                InsertVar(temp);
            }
            NewInstruction(node);
        }
        if (strcmp(current, "NAND") == 0) {
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= 2;
            node->numOuts = 1;
            node->type = NAND;
            int inputIndex = 0;
            int i;
            for (i = 0; i < 3; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current); 
                }
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                } else {
                    strcpy(node->outputs[0], current);
                }
                VarNode *temp = malloc(sizeof(VarNode));
                strcpy(temp->name, current);
                if (strcmp(current, "1") == 0) {
                    temp->value = 1;
                }
                else {
                    temp->value = 0;
                }
                temp->isAnOutput = 0;
                temp->next = NULL;
                InsertVar(temp);
            }
            NewInstruction(node);
        }
        if (strcmp(current, "NOR") == 0) {
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= 2;
            node->numOuts = 1;
            node->type = NOR;
            int inputIndex = 0;
            int i;
            for (i = 0; i < 3; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current); 
                }
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                }else {
                    strcpy(node->outputs[0], current);
                }
                VarNode *temp = malloc(sizeof(VarNode));
                strcpy(temp->name, current);
                if (strcmp(current, "1") == 0) {
                    temp->value = 1;
                }
                else {
                    temp->value = 0;
                }
                temp->isAnOutput = 0;
                temp->next = NULL;
                InsertVar(temp);
            }
            NewInstruction(node);
        }
        if (strcmp(current, "XOR") == 0) {
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= 2;
            node->numOuts = 1;
            node->type = XOR;
            int inputIndex = 0;
            int i;
            for (i = 0; i < 3; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current); 
                }
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                }else {
                    strcpy(node->outputs[0], current);
                }
                VarNode *temp = malloc(sizeof(VarNode));
                strcpy(temp->name, current);
                if (strcmp(current, "1") == 0) {
                    temp->value = 1;
                }
                else {
                    temp->value = 0;
                }
                temp->isAnOutput = 0;
                temp->next = NULL;
                InsertVar(temp);
            }
            NewInstruction(node);
        }
        if (strcmp(current, "DECODER") == 0) {
            int ins;
            fscanf(fp, "%s", current);
            ins = atoi(current);
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= ins;
            node->numOuts = 1<<ins;
            node->next = NULL;
            node->type = DECODER;
            int i;
            int inputIndex = 0;
            for (i = 0; i < ins; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current);
                }
                strcpy(node->inputs[inputIndex], current);
                inputIndex++;
                VarNode *a= malloc(sizeof(VarNode));
                strcpy(a->name, current);
                if (strcmp(a->name, "1") == 0) {
                    a->value = 1;
                }else {
                    a->value = 0;
                }
                a->isAnOutput = 0;
                a->next = NULL;
                InsertVar(a);
            }
            int outputIndex = 0;
            int j;
            for (j = 0; j < node->numOuts; j++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current);
                }
                strcpy(node->outputs[outputIndex], current);
                outputIndex++;
                VarNode *b= malloc(sizeof(VarNode));
                strcpy(b->name, current);
                if (strcmp(b->name, "1") == 0) {
                    b->value = 1;
                }else {
                    b->value = 0;
                }
                b->isAnOutput = 0;
                b->next = NULL;
                InsertVar(b);
            }
            NewInstruction(node);
        }
        if (strcmp(current, "MULTIPLEXER") == 0) {
            int ins;
            fscanf(fp, "%s", current);
            ins = atoi(current);
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns= ins;
            node->numOuts = 1;
            node->next = NULL;
            node->type = MULTIPLEXER;
            int switchIndex = 0;
            int switchCount= 1<<ins;
            int i;
            for (i = 0; i < switchCount; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current);
                }
                strcpy(node->switches[switchIndex], current);
                switchIndex++;
                VarNode *a= malloc(sizeof(VarNode));
                strcpy(a->name, current);
                if (strcmp(a->name, "1") == 0) {
                    a->value = 1;
                }else {
                    a->value = 0;
                }
                a->isAnOutput = 0;
                a->next = NULL;
                InsertVar(a);
            }
            int inputIndex = 0;
            for (i = 0; i < ins; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current); 
                }
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                }
                VarNode *b= malloc(sizeof(VarNode));
                strcpy(b->name, current);
                b->value = 0;
                b->isAnOutput = 0;
                b->next = NULL;
                InsertVar(b);
            }
            fscanf(fp, "%s", current);
            if (strcmp(current, ":") == 0) {
                fscanf(fp, "%s", current); 
            }
            VarNode *c= malloc(sizeof(VarNode));
            strcpy(c->name, current);
            c->value = 0;
            c->isAnOutput = 0;
            c->next = NULL;
            InsertVar(c);
            strcpy(node->outputs[0], current);
            NewInstruction(node);
        }
        if (strcmp(current, "PASS") == 0) {
            InstructionNode *node = malloc(sizeof(InstructionNode));
            node->numIns = 1;
            node->numOuts = 1;
            node->type = PASS;
            int inputIndex = 0;
            int i;
            for (i = 0; i < 2; i++) {
                fscanf(fp, "%s", current);
                if (strcmp(current, ":") == 0) {
                    fscanf(fp, "%s", current); 
                }
                if (inputIndex < node->numIns) {
                    strcpy(node->inputs[inputIndex], current);
                    inputIndex++;
                }else {
                    strcpy(node->outputs[0], current);
                }
                VarNode *temp = malloc(sizeof(VarNode));
                strcpy(temp->name, current);
                if (strcmp(current, "1") == 0) {
                    temp->value = 1;
                }else {
                    temp->value = 0;
                }
                temp->isAnOutput = 0;
                temp->next = NULL;
                InsertVar(temp);
            }
            NewInstruction(node);
        }
    }
}

void MakeTruthTable(int numIns) {
    int rows=1<<numIns;
    int cols=numIns;
    TruthTable=(int **)malloc(sizeof(int *)*rows);
    int i;
    for(i=0;i<rows;i++) {
        TruthTable[i]=(int *)malloc(sizeof(int)*cols);
    }
    int j;
    int *num=malloc(sizeof(int)*cols);
    for(i=0;i<rows;i++) {
        IntToBin(i,cols,num);
        for(j=0;j<cols;j++) {
            TruthTable[i][j]=num[j];
        }
    }
}

void IntToBin(int number, int length, int *ptr) {
    int i;
    int remainder;
    for(i=length-1;i>=0;i--) {
        remainder=number%2;
        ptr[i]=remainder;
        number/=2;
    }
    return;
}

void PrintTable(int inputs, int outputs) {
    int truth_rows = 1<<inputs, truth_cols = inputs;
    int i,j,k;
    for (i=0;i<truth_rows;i++) {
        for (j=0;j<truth_cols;j++) {
            printf("%d ", TruthTable[i][j]);
            VarNode *n = GetValue(front->inputs[j]);
            n->value = TruthTable[i][j];
        }
        InstructionNode *ptr = front;
        while (ptr != NULL) {
            enum Gate t = ptr->type;
            if(t==NOT) {
                VarNode *a= GetValue(ptr->inputs[0]);
                VarNode *b= GetValue(ptr->outputs[0]);
                b->value = !(a->value);
            }else if(t==AND) {
                VarNode *a= GetValue(ptr->inputs[0]);
                VarNode *b= GetValue(ptr->inputs[1]);
                VarNode *c= GetValue(ptr->outputs[0]);
                int result = a->value && b->value;
                c->value = result;
            }else if(t==OR) {
                VarNode *a= GetValue(ptr->inputs[0]);
                VarNode *b= GetValue(ptr->inputs[1]);
                VarNode *c= GetValue(ptr->outputs[0]);
                int result = a->value || b->value;
                c->value = result;
            }else if(t==NAND) {
                VarNode *a= GetValue(ptr->inputs[0]);
                VarNode *b= GetValue(ptr->inputs[1]);
                VarNode *c= GetValue(ptr->outputs[0]);
                int result = !(a->value && b->value);
                c->value = result;
            }else if(t==NOR) {
                VarNode *a= GetValue(ptr->inputs[0]);
                VarNode *b= GetValue(ptr->inputs[1]);
                VarNode *c= GetValue(ptr->outputs[0]);
                int result = !(a->value || b->value);
                c->value = result;
            }else if(t==XOR) {
                VarNode *a= GetValue(ptr->inputs[0]);
                VarNode *b= GetValue(ptr->inputs[1]);
                VarNode *c= GetValue(ptr->outputs[0]);
                int result = a->value ^ b->value;
                c->value = result;
            }else if(t==PASS) {
                VarNode *a= GetValue(ptr->inputs[0]);
                VarNode *b= GetValue(ptr->outputs[0]);
                b->value = a->value;
            }else if(t==DECODER) {
                int numIns= ptr->numIns;
                VarNode **nodes = malloc(sizeof(VarNode *) * numIns);
                for (k=0;k<numIns;k++) {
                    VarNode *a=GetValue(ptr->inputs[k]);
                    nodes[k] = a;
                }
                int index = GetIndex(nodes, ptr->numIns);
                for (k=0;k<ptr->numOuts;k++) {
                    VarNode *b= GetValue(ptr->outputs[k]);
                    if (k==index) {
                        b->value = 1;
                    }else {
                        b->value = 0;
                    }
                }
            }else if(t==MULTIPLEXER) {
                int numIns= ptr->numIns;
                VarNode **nodes = malloc(sizeof(VarNode *) * numIns);
                for (k=0;k<numIns;k++) {
                    VarNode *c= GetValue(ptr->inputs[k]);
                    nodes[k]=c;
                }
                int index = GetIndex(nodes, ptr->numIns);
                VarNode *x = GetValue(ptr->outputs[0]);
                VarNode *y = GetValue(ptr->switches[index]);
                x->value = y->value;
            }
            ptr = ptr->next;
        }
        printf("| ");
        for (k=0;k<front->next->numIns;k++) {
            VarNode *x = GetValue(front->next->inputs[k]);
            if (k==front->next->numIns- 1) {
                printf("%d", x->value);
            }else {
                printf("%d ", x->value);
            }
        }
        printf("\n");
    }
}

int HashIt(char *key) {
    unsigned int hash = 0;
    while(*key) {
        hash = (hash*10)+*key++-'0';
    }
    return hash%varTable->size;
}

int GetIndex(VarNode **vars, int inputs) {
    int i;
    int total=0;
    int counter=0;
    for (i=inputs-1; i>=0; i--) {
        if (vars[i]->value == 0) {
            counter++;
            continue;
        }
        total += 1<<counter;
        counter++;
    }
    return total;
}

void NewInstruction(InstructionNode *node) {
    if (front == NULL) {
        front = node;
        return;
    }
    InstructionNode *ptr = front;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = node;
}

void InsertVar(VarNode *newNode) {
    int index = HashIt(newNode->name);
    if (varTable->table[index] == NULL) {
        varTable->table[index] = newNode;
        return;
    }
    VarNode *ptr = varTable->table[index];
    while (ptr->next != NULL) {
        if (strcmp(ptr->name, newNode->name) == 0) {
            ptr->value = newNode->value;
            free(newNode);
            return;
        }
        ptr = ptr->next;
    }
    if (strcmp(ptr->name, newNode->name) == 0) {
        ptr->value = newNode->value;
        free(newNode);
        return;
    }
    ptr->next = newNode;
}

VarNode *GetValue(char *key) {
    int index = 0;
    index = HashIt(key);
    if (varTable->table[index] == NULL) {
        return NULL;
    }
    VarNode *ptr = varTable->table[index];
    while (ptr != NULL) {
        if (strcmp(key, ptr->name) == 0) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}
