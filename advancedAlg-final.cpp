#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <cmath> // Used for pow()
// https://www.programiz.com/cpp-programming/library-function/cmath/pow#:~:text=The%20pow()%20function%20computes,in%20header%20file.
using namespace std;

// function from Stack Overflow : https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
template <class T>
int numDigits(T number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

int numTestedCombos = 0;
int * testedCombos = new int[1000000];

fstream openFileForWrite() {
	string ioFile = getenv("HOME"); ioFile += "/passwdfile";
	fstream fout(ioFile, ios::out);
	if (fout.fail()) {
		cerr << "Unable to create file." << endl;
		exit(5);
	}
	return fout;
}

void writeFile(fstream * outFile, char * codeStr) {
	while (true) {
		*(outFile) << codeStr;
		usleep(1); // write delay, to prevent SSD lockup
		if (outFile -> fail()) {
			cerr << "Unable to write to file. Trying again." << endl;
			continue;
		}
		outFile -> close();
		if (outFile -> fail()) {
			cerr << "Unable to close file." << endl;
		}
		break;
	}
}

void tryCode(int code) {
	bool codeTried = false;
	for (int i = 0; i != numTestedCombos; i++) {
		if (testedCombos[i] == code) {
			cout << "You have already tried code " << code << endl;
			codeTried = true;
			break;
		}
	}
	if (!codeTried) {
		fstream outFile = openFileForWrite();
		char * codeStr = new char[7]; sprintf(codeStr, "%06d", code);
		cout << "Trying code " << codeStr << "..." << endl;
		numTestedCombos++;
		testedCombos[numTestedCombos] = code;
		writeFile(&outFile, codeStr);
	}
}

void tryAllCombosIncluding(int numberToTry) {
	// Figure out how many digits are in numberToTry
	int digits = numDigits(numberToTry);
	cout << "There are " << digits << " digits in " << numberToTry << "." << endl;
	int remainingDigits = 6-digits;

	if (digits < 6) {
		for (int i = 0; i <= (6 - digits); i++) {
			cout << "Digit offset = " << i << ". " << numTestedCombos << " combinations have been tested." << endl;
			//usleep(1000000);
			int digitsOnLeft = remainingDigits - i; // one digit on the right
			int digitsOnRight = 6 - digits - digitsOnLeft;
			cout << digitsOnRight << endl;
			// Generate lCap and rCap
			int rCap = 0;
			for (int j = 0; j != digitsOnRight; j++) {
				rCap += (9 * pow(10, j));
			}
			int lCap = 0;
			for (int j = 0; j != digitsOnLeft; j++) {
				lCap += (9 * pow(10, j));
			}
			for (int k = 0; k <= rCap; k++) {
				for (int l = 0; l <= lCap; l++) {
					tryCode((numberToTry * pow(10, i)) + (k) + (l * pow(10, digits+i)));
				}
			}
			// Iterate through left and right up to their respective caps
		}
	} else {
		tryCode(numberToTry);
	}
}

int main() {
	cout << "Starting advanced brute..." << endl;
	// List of most common pin codes https://www.theguardian.com/money/blog/2012/sep/28/debit-cards-currentaccounts#:~:text=1234%20accounts%20for%2010.7%25%20of,of%20all%20pins%20in%20use.
	// https://lifehacker.com/the-most-and-least-common-pin-numbers-and-numeric-pas-5944567
	tryCode(000000);
	tryAllCombosIncluding(1234); // Why do people still set this as their pin code lmao
	tryAllCombosIncluding(69); tryAllCombosIncluding(420); // nice
	tryAllCombosIncluding(4321);
	tryAllCombosIncluding(2580); // middle of standard numeric keypad
	tryAllCombosIncluding(360);
	tryAllCombosIncluding(3828); // T9 for the f-word
	tryAllCombosIncluding(980); tryAllCombosIncluding(1080); tryAllCombosIncluding(2080); // High-end Nvidia GPU's :D
	tryAllCombosIncluding(1004); // This is common?
	for (int i = 1; i != 10; i++) {
		tryAllCombosIncluding(i + (i * 10) + (i * 100));
	}
	for (int i = 1; i != 100; i++) {
		tryAllCombosIncluding(i + (i * 100));
	}
	for (int i = 0; i != 100; i++) { // All years from 1900 to 1999
		tryAllCombosIncluding(1900 + i);
	}
	for (int i = 0; i != 20; i++) { // All years from 2000 to 2019 (2020 would've been tested already)
		tryAllCombosIncluding(2000 + i);
	}
	for (int i = 1; i != 8; i++) {
		tryAllCombosIncluding(i + ((i+1) * 10) + ((i+2) * 100));
		tryAllCombosIncluding((i+2) + ((i+1) * 10) + ((i) * 100));
	}
	for (int i = 1; i != 10; i++) {
		tryAllCombosIncluding(i + (i * 10));
	}
	cout << "Ran out of things that make sense -- let's just iterate through everything." << endl;
	for (int i = 0; i != 1000000; i++) {
		tryCode(i);
	}
	cout << "Finished! We've tested " << numTestedCombos << " combinations." << endl;
	delete[] testedCombos;
}