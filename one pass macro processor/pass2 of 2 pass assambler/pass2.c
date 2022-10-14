#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stripTabspacesAndNewLine(char *line) {
  int size = strlen(line);
  for (int i = 0; i < size; i++) {
      if (line[i] == '\t') {
          line[i] = ' ';
      }
  }

  if (line[size - 1] == '\n') {
      line[size - 1] = '\0';
  }
}

// To get words in a line "COPY START 1000" should give array [COPY,START,1000]
void getInstructions(char *line, char *instructions[], int *instructionCount) {
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL) {
        if (strlen(token) > 0) {
            instructions[i] = token;
            i++;
        }
        token = strtok(NULL, " ");
    }
    *instructionCount = i;
}

void readOPTAB(FILE *optabFile, char optab[100][100][10], int *optabSize) {
    int i = 0;
    while (fscanf(optabFile, "%s%s", optab[i][0], optab[i][1]) != EOF) {
        i++;
    }
    *optabSize = i;
}

void readSYMTAB(FILE *symtabFile, char symtab[100][100][10], int *symtabSize) {
    int i = 0;
    while (fscanf(symtabFile, "%s%s", symtab[i][1], symtab[i][0]) != EOF) {
        i++;
    }
    *symtabSize = i;
}

void writeHeader(FILE *outputFile, char *instructions[], int instructionCount, int length) {
    // Instructions should be in the format: "COPY START 1000" (3 things)
    if (instructionCount != 3) {
        printf("error: invalid header\n");
        exit(1);
    }
    // Write the header (Name , Start address, Length)
    fprintf(outputFile, "H^%s^%s^%d\n", instructions[0], instructions[2],length);
}


void writeText(FILE *outputFile, FILE *inputFile, int startAddress) {
    FILE  *optabFile,  *symtabFile;
    optabFile = fopen("optab.txt", "r");
    symtabFile = fopen("symtab.txt", "r");

    char optab[100][100][10];  // 2d string array to hold opcode and its value
    char symtab[100][100][10]; // 2d string array to hold symbol and its address

    int symtabSize, optabSize;

    // Read optab and symtab
    readOPTAB(optabFile, optab, &optabSize);
    readSYMTAB(symtabFile, symtab, &symtabSize);
    fprintf(outputFile, "T^%d", startAddress);
    char line[100];
    char *instructions[4]; // Instructions per line
    int instructionCount;
    while (fgets(line, 100, inputFile)) {
        // strip newline
        stripTabspacesAndNewLine(line);
        getInstructions(line, instructions, &instructionCount);
        char *opcode = instructions[2];
        char *operand = instructions[3];

        // if only 3 words per line
        if (instructionCount == 3) {
            opcode = instructions[1];
            operand = instructions[2];
        }

        if (strcmp(opcode, "END") == 0) {
            break;
        } else if (strcmp(opcode, "RESB") == 0) {

        } else if (strcmp(opcode, "WORD") == 0) {
            fprintf(outputFile, "^%s",  operand); // Write text data
        } else if (strcmp(opcode, "RESW") == 0) {

        } else if (strcmp(opcode, "BYTE") == 0) {
            fprintf(outputFile, "^"); 
            int size = strlen(operand);
            for (int i = 2; i < size; i++)
                fprintf(outputFile, "%d",  operand[i]); // Write ASCII of char
        } else {
            // search opcode in optab
            for (int i = 0; i < optabSize; i++) {
                if (strcmp(opcode, optab[i][0]) == 0) {
                    // search operand in symtab
                    for (int j = 0; j < symtabSize; j++) {
                        if (strcmp(operand, symtab[j][0]) == 0) {
                            char *opcodeVal = optab[i][1];
                            char *symbolAddress = symtab[j][1];
                            // Write text data
                            fprintf(outputFile, "^%s%s", opcodeVal,  symbolAddress);
                            break;
                        }
                    }
                    break;
                }
            }
        }

    }
}


void writeEND(FILE *outputFile, int startAddress) {
    fprintf(outputFile, "\nE^%d", startAddress);
}

int main() {
    FILE *inputFile, *outputFile, *lengthFile;
    inputFile = fopen("input.txt", "r");
    lengthFile = fopen("length.txt", "r");
    outputFile = fopen("output.txt", "w");

    int programLength = 0;

    // Line (we will read the file line by line)
    char line[100];
    char *instructions[4]; // Instructions per line
    int instructionCount; // Instruction count per line

    // Read length of program
    fscanf(lengthFile, "%d", &programLength);
    // Read the first line
    fgets(line, 100, inputFile);
    // strip newline
    stripTabspacesAndNewLine(line);
    // Get the instructions in first line
    getInstructions(line, instructions, &instructionCount);
    // Write the header using the first line
    writeHeader(outputFile, instructions, instructionCount, programLength);

    // Get start address
    int address = atoi(instructions[2]);
    writeText(outputFile, inputFile, address);
    writeEND(outputFile, address);
    printf("Completed! Written to output.txt\n");
    return 0;
}