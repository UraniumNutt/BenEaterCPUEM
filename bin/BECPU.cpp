 #include <iostream>
 #include <unistd.h>
 #include <cstdlib>
 #include <math.h>
 #include <fstream>
using namespace std;


typedef unsigned char uint8;

uint8 PC = 0;
uint8 MAR = 0;
uint8 A = 0;
uint8 B = 0;
uint8 Sum = 0;
uint8 IR = 0;
uint8 OUT = 0;
uint8 StepCounter = 0;

bool HLT = false;
bool zeroFlag = false;
bool carryFlag = false;

char charRAM[15];
uint8 RAM[15];

void initRAM() {

    streampos size;
    char *inputBIN = &charRAM[0];

    ifstream file("output.bin", ios::in|ios::binary|ios::ate);

    if (file.is_open()) {

        size = file.tellg();
        file.seekg(0 , ios::beg);
        file.read(inputBIN, size);
        file.close();

    }

    for (int i = 0; i < 16; i++) {
        RAM[i] = (uint8) charRAM[i];
    }



}



void showState() {

    // use escape sequence to clear the screen
    cout << "\033[2J\033[1;1H";

    cout << "----------------------" << endl;
    // use printf to format ouput in hex
    // print the state of CPU
    printf("PC: 0x%02X\n", PC);
    printf("MAR: 0x%02X\n", MAR);
    printf("A: 0x%02X\n", A);
    printf("B: 0x%02X\n", B);
    printf("Sum: 0x%02X\n", Sum);
    printf("Flags: Z: %d, C: %d\n", zeroFlag, carryFlag);
    printf("IR: 0x%02X\n", IR);
    // the output should be a decimal number and step counter should be in decimal
    printf("OUT: %d\n", OUT);
    printf("Step Counter: %d\n", StepCounter);

    // now to print the RAM
    printf("\n");
    printf("RAM:\n");
    for (int i = 0; i < 16; i++) {
        printf("0x%02X ", RAM[i]);
    }
    printf("\n");

}


void showOUT() {
    cout << dec;
    cout <<  (uint) OUT << endl;
}

void showAndWait(float microsecondsPeriod) {
    showState();
    usleep(microsecondsPeriod);
}


void executeInstruction(float clock) {

    int microsecondsPeriod = (1.0 / clock) * 1000000.0;

    showAndWait(microsecondsPeriod);
    MAR = (PC & 0b00001111); // Fetch 1
    StepCounter++;
    
    showAndWait(microsecondsPeriod);
    IR = (RAM[MAR]); // Fetch 2
    PC++;
    StepCounter++;
    
    switch ((IR & 0b11110000) >> 4) { // Top halve of IR has instruction, bottom argument

        case (0x00): // NOP

            showAndWait(microsecondsPeriod);
            StepCounter++; // Step 3
            
            showAndWait(microsecondsPeriod);
            StepCounter++; // Step 4
            
            showAndWait(microsecondsPeriod);
            StepCounter++; // Step 5
            
            showAndWait(microsecondsPeriod);
            StepCounter++; // Step 6
        
        break;

        case (0x01): // LDA

            showAndWait(microsecondsPeriod); // Step 3
            MAR = (IR & 0b00001111);
            StepCounter++;
            
            showAndWait(microsecondsPeriod); // Step 4
            A = RAM[MAR];
            StepCounter++;
            
            showAndWait(microsecondsPeriod); // Step 5
            StepCounter++;

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;

        break;

        case (0x02): // ADD

            showAndWait(microsecondsPeriod); // Step 3
            MAR = (IR & 0b00001111);
            StepCounter++;
            
            showAndWait(microsecondsPeriod); // Step 4
            B = RAM[MAR];
            StepCounter++;
            
            showAndWait(microsecondsPeriod); // Step 5
            if ((int) A + (int) B > 255) {
                carryFlag = true; // if overflow set carry flag
            }
            else {
                carryFlag = false;
            }
            Sum = A + B;
            A = Sum;
            if (Sum == 0) {
                zeroFlag = true; // if sum is 0 set zero flag
            }
            else {
                zeroFlag = false;
            }
            
            StepCounter++;

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;
            
        break;

        case (0x03): // SUB

            showAndWait(microsecondsPeriod); // Step 3
            MAR = (IR & 0b00001111);
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 4
            B = RAM[MAR];
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 5
            if ((int) A - (int) B < 0) {
                carryFlag = true; // if overflow set carry flag
            }
            else {
                carryFlag = false;
            }
            Sum = A - B;
            A = Sum;
            if (Sum == 0) {
                zeroFlag = true; // if sum is 0 set zero flag
            }
            else {
                zeroFlag = false;
            }
            
            StepCounter++;
             

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;
            

        break;

        case (0x04): // STA

            showAndWait(microsecondsPeriod); // Step 3
            MAR = (IR & 0b00001111);
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 4
            RAM[MAR] = A;
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 5
            StepCounter++;
             

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;
            

        break;

        case (0x05): // LDI

            showAndWait(microsecondsPeriod); // Step 3
            A = (IR & 0b00001111);
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 4
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 5
            StepCounter++;
             

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;
            

        break;

        case (0x06): // JMP

            showAndWait(microsecondsPeriod); // Step 3
            PC = (IR & 0b00001111);
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 4
            StepCounter++;
                        

            showAndWait(microsecondsPeriod); // Step 5
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;
            


        break;

        case (0x07): // JC

            if (carryFlag == false) {

                showAndWait(microsecondsPeriod); // Step 3
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 4
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 5
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 6
                StepCounter++;
                


            }
            if (carryFlag == true) {

                showAndWait(microsecondsPeriod); // Step 3
                PC = (IR & 0b00001111);
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 4
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 5
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 6
                StepCounter++;
                

            }


        break;

        case (0x08): // JZ

            if (zeroFlag == false) {

                            showAndWait(microsecondsPeriod); // Step 3
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 4
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 5
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 6
                            StepCounter++;
                            


                        }
                        if (zeroFlag == true) {

                            showAndWait(microsecondsPeriod); // Step 3
                            PC = (IR & 0b00001111);
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 4
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 5
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 6
                            StepCounter++;
                            

                        }

        break;

        case (0x09): // ADD #

            showAndWait(microsecondsPeriod); // Step 3
            B = (IR & 0b00001111);
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 4
            if ((int) A + (int) B > 255) {
                carryFlag = true; // if overflow set carry flag
            }
            else {
                carryFlag = false;
            }
            Sum = A + B;
            A = Sum;
            if (Sum == 0) {
                zeroFlag = true; // if sum is 0 set zero flag
            }
            else {
                zeroFlag = false;
            }
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 5
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;
            

        break;

        case (0x0A): // SUB #

            showAndWait(microsecondsPeriod); // Step 3
            B = (IR & 0b00001111);
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 4
            if ((int) A - (int) B < 0) {
                carryFlag = true; // if overflow set carry flag
            }
            else {
                carryFlag = false;
            }
            Sum = A - B;
            A = Sum;
            if (Sum == 0) {
                zeroFlag = true; // if sum is 0 set zero flag
            }
            else {
                zeroFlag = false;
            }
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 5
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;
            


        break;

        case (0x0B): // LDB

            showAndWait(microsecondsPeriod); // Step 3
            MAR = (IR & 0b00001111);
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 4
            B = RAM[MAR];
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 5
            StepCounter++;
             

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;
            


        break;

        case (0x0C): // JNC

            if (carryFlag == true) {

                showAndWait(microsecondsPeriod); // Step 3
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 4
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 5
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 6
                StepCounter++;
                


            }
            if (carryFlag == false) {

                showAndWait(microsecondsPeriod); // Step 3
                PC = (IR & 0b00001111);
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 4
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 5
                StepCounter++;
                

                showAndWait(microsecondsPeriod); // Step 6
                StepCounter++;
                

            }

        break;

        case (0x0D): // JNZ

            if (zeroFlag == true) {

                            showAndWait(microsecondsPeriod); // Step 3
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 4
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 5
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 6
                            StepCounter++;
                            


                        }
                        if (zeroFlag == false) {

                            showAndWait(microsecondsPeriod); // Step 3
                            PC = (IR & 0b00001111);
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 4
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 5
                            StepCounter++;
                            

                            showAndWait(microsecondsPeriod); // Step 6
                            StepCounter++;
                            

                        }

        break;

        case (0x0E): // OUT

            showAndWait(microsecondsPeriod); // Step 3
            OUT = A;
            StepCounter++;
            

            showAndWait(microsecondsPeriod); // Step 4
            StepCounter++;
             

            showAndWait(microsecondsPeriod); // Step 5
            StepCounter++;
             

            showAndWait(microsecondsPeriod); // Step 6
            StepCounter++;
             
            

        break;


        case (0x0F): // HLT

            showAndWait(microsecondsPeriod); // Step 3
            StepCounter++;
            HLT = true;
            

        break;




    }

    if (!HLT) {
        StepCounter = 0; // Reset instruction
    }
}



int main() {

    // uses escape sequence to clear screen
    cout << "\033[2J\033[1;1H";
    

    initRAM();

    //cout << 0xE0 << endl;

    while (!HLT) {
        
        executeInstruction(10.0);
        
    }
    
 
    return 0;
}
