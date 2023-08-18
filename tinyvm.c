#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int instructionMemory[129];
int dataMemory[11];
int programCounter = 0;
int accumulator = 0;
int MAR = 0;
int MDR = 0;
struct instructionRegister
{
    int op;
    int addr;
} IR;

void loadInstructions(char fileName[])
{
    FILE* inputFile = fopen(fileName, "r"); //open file to read values

    if (inputFile == NULL) {
        printf("Error opening the input file.\n");
        exit(1); //exit if file is unable to be opened
    }

    int opcode, address;
    int instructionCount = 0;

    //read instructions from the file until the end
    while (fscanf(inputFile, "%d %d", &opcode, &address) == 2) {
        //store the opcode and address in the instruction memory array
        instructionMemory[instructionCount] = opcode;
        instructionMemory[instructionCount + 1] = address;
        instructionCount += 2;

        //check if the instruction memory has reached its capacity
        if (instructionCount >= 128) {
            printf("Instruction memory overflow.\n");
            exit(1); //if there are more instructions from file than what can fit in instruction memory, exit
        }
    }
}

void fetch()//get the relevant op code and address based on the program counter
{
    IR.op = instructionMemory[programCounter]; //move op code from instruction memory to instruction register
    IR.addr = instructionMemory[programCounter + 1]; //move address from instruction memory to instruction register
    programCounter+= 2; //incrementing by 2 to get to the next op code
}

void execute(int opCode, int address)
{
    switch(opCode) //use op code to determine operation to be executed
    {
        case 1:
            //load
            //sets accumulator to the value at the address from IR
            printf("\nLOAD <%d>\n", address);
            MAR = address;
            MDR = dataMemory[MAR];
            accumulator = MDR;
            break;
        case 2:
            //add
            //adds the value in the accumulator and the value at the address from IR
            printf("\nADD <%d>\n", address);
            MAR = address;
            MDR = dataMemory[MAR];
            accumulator+= MDR;
            break;
        case 3:
            //store
            //stores the value in the accumulator at the address from the IR
            printf("\nSTORE <%d>\n", address);
            MAR = address;
            MDR = accumulator;
            dataMemory[MAR] = MDR;
            break;
        case 4:
            //sub
            //subtracts the value in the accumulator and the value at the address from IR
            printf("\nSUB <%d>\n", address);
            MAR = address;
            MDR = dataMemory[MAR];
            accumulator-= MDR;
            break;
        case 5:
            //in
            //sets accumulator to a number that is input from user's keyboard
            printf("\nIN <%d>\n", address);
            printf("\nInput a value: ");
            scanf("%d", &accumulator);
            break;
        case 6:
            //out
            //displays the current value of the accumulator
            printf("\nOUT <%d>\n", address);
            printf("\nResult is : %d\n", accumulator);
            break;
        case 7:
            //halt
            //ends the program
            printf("\nHALT\n");
            exit(1);
            break;
        case 8:
            //jmp
            //sets the value of the program counter to the address from the IR
            printf("\nJMP <%d>\n", address);
            programCounter = address;
            break;
        case 9:
            //skipz
            //skips the next instruction if the value in the accumulator equals 0
            printf("\nSKIPZ <%d>\n", address);
            if(accumulator == 0)
                programCounter += 2;
            break;
        case 10:
            //skipg
            //skips the next instruction if the value in the accumulator is greater than 0
            printf("\nSKIPG <%d>\n", address);
            if(accumulator > 0)
                programCounter += 2;
            break;
        case 11:
            //skipl
            //skips the next instruction if the value in the accumulator is less than 0
            printf("\nSKIPL <%d>\n", address);
            if(accumulator < 0)
                programCounter += 2;
            break;
    }
}

void displayState() {
    printf("\nPC = %d | A = %d | DM = [", programCounter, accumulator); //displaying values of the vm
    for (int i = 0; i < 10; i++) {
        printf("%d", dataMemory[i]);
        if (i < 9) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(int argCount, char *argVector[])
{
    if( argCount != 2 ) //check if correct number of arguments are supplied
    {
        //this will run if the number of command line arguments is more or less than 1
        printf("Incorrect number of arguments supplied. \n");
        return 0; //exit
    }
    dataMemory[10] = 1; //adding constant to subtract by to data memory
    loadInstructions(argVector[1]);
    printf("Initial values:\n");
    displayState();

    while(programCounter <= 128) //fetch and execute instructions until we've traversed through instruction memory array
    {
       fetch(programCounter);
       execute(IR.op, IR.addr);
       displayState();
    }

    return 0;
}
