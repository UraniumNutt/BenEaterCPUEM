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

	cout << "\033[2J\033[1;1H";
    cout << hex;
    //cout << dec;
    cout << "----------------------" << endl;
    cout << "PC: " << (uint) (PC & 0b00001111) << endl;
    cout << "MAR: " << (uint) (MAR & 0b00001111) << endl;
    cout << "A: " << (uint) A << endl;
    cout << "B: " << (uint) B << endl;
    cout << "Sum: " << (uint) Sum << endl;
    cout << "Flags, Zero, Carry: " << zeroFlag << carryFlag << endl; 
    cout << "IR: " << (uint) IR  << endl;
    cout << dec;
    cout << "OUT: " << (uint) OUT << endl; // out SHOULD be in decimal
    cout << hex;
    cout << "StepCounter: " << (uint) StepCounter  << endl;

    cout << "RAM: " << endl;

    for (int i = 0; i < 16; i++) {
        cout << i << ": " << (uint) RAM[i] << endl;
    }
}

void showOUT() {
    cout << dec;
    cout <<  (uint) OUT << endl;
}

void executeInstruction(float clock) {

    int microsecondsPeriod = (1.0 / clock) * 1000000.0;

    showState();
    MAR = (PC & 0b00001111); // Fetch 1
    StepCounter++;
    usleep(microsecondsPeriod);

    showState();
    IR = (RAM[MAR]); // Fetch 2
    PC++;
    StepCounter++;
    usleep(microsecondsPeriod);

    

    switch ((IR & 0b11110000) >> 4) { // Top halve of IR has instruction, bottom argument

        case (0x00): // NOP

            showState();
            StepCounter++; // Step 3
            usleep(microsecondsPeriod);
            showState();
            StepCounter++; // Step 4
            usleep(microsecondsPeriod);
            showState();
            StepCounter++; // Step 5
            usleep(microsecondsPeriod);
            showState();
            StepCounter++; // Step 6
            usleep(microsecondsPeriod);

        break;

        case (0x01): // LDA

            showState(); // Step 3
            MAR = (IR & 0b00001111);
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 4
            A = RAM[MAR];
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 5
            StepCounter++;
            usleep(microsecondsPeriod); 

            showState(); // Step 6
            StepCounter++;
            usleep(microsecondsPeriod); 

        break;

        case (0x02): // ADD

            showState(); // Step 3
            MAR = (IR & 0b00001111);
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 4
            B = RAM[MAR];
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 5
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
            usleep(microsecondsPeriod); 

            showState(); // Step 6
            StepCounter++;
            usleep(microsecondsPeriod);

        break;

        case (0x03): // SUB

            showState(); // Step 3
            MAR = (IR & 0b00001111);
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 4
            B = RAM[MAR];
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 5
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
            usleep(microsecondsPeriod); 

            showState(); // Step 6
            StepCounter++;
            usleep(microsecondsPeriod);

        break;

        case (0x04): // STA

            showState(); // Step 3
            MAR = (IR & 0b00001111);
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 4
            RAM[MAR] = A;
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 5
            StepCounter++;
            usleep(microsecondsPeriod); 

            showState(); // Step 6
            StepCounter++;
            usleep(microsecondsPeriod);

        break;

        case (0x05): // LDI

            showState(); // Step 3
            A = (IR & 0b00001111);
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 4
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 5
            StepCounter++;
            usleep(microsecondsPeriod); 

            showState(); // Step 6
            StepCounter++;
            usleep(microsecondsPeriod);

        break;

        case (0x06): // JMP

            showState(); // Step 3
            PC = (IR & 0b00001111);
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 4
            StepCounter++;
            usleep(microsecondsPeriod);            

            showState(); // Step 5
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 6
            StepCounter++;
            usleep(microsecondsPeriod);


        break;

        case (0x07): // NOP

        break;

        case (0x08): // NOP

        break;

        case (0x09): // NOP

        break;

        case (0x0A): // NOP

        break;

        case (0x0B): // NOP

        break;

        case (0x0C): // NOP

        break;

        case (0x0D): // NOP

        break;

        case (0x0E): // OUT

            showState(); // Step 3
            OUT = A;
            StepCounter++;
            usleep(microsecondsPeriod);

            showState(); // Step 4
            StepCounter++;
            usleep(microsecondsPeriod); 

            showState(); // Step 5
            StepCounter++;
            usleep(microsecondsPeriod); 

            showState(); // Step 6
            StepCounter++;
            usleep(microsecondsPeriod); 
            

        break;


        case (0x0F): // HLT

            showState();
            StepCounter++;
            HLT = true;
            usleep(microsecondsPeriod);

        break;




    }

    if (!HLT) {
        StepCounter = 0; // Reset instruction
    }
}



int main() {

    
    initRAM();

    

    //cout << 0xE0 << endl;

    while (!HLT) {
        
        executeInstruction(10.0);
        
    }
    
 
    return 0;
}
