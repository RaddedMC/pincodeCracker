#include <iostream>
#include <string>
#include <fstream>
#include <csignal>
#include <unistd.h>
#include <string.h>
using namespace std;

int monitorFile(char * code, string fileLocation) {

	//Create file, this will delete any existing instances of it
	fstream crackData(fileLocation, ios::out);
	if (crackData.fail()) {
		cerr << "Unable to create file." << endl;
		exit(5);
	}
	crackData.close();
	if (crackData.fail()) {
		cerr << "Unable to close created file." << endl;
	}

	int attempts = 0;

	while (true) {

		// Open file and read from it
		fstream crackData(fileLocation, ios::in);
		if (crackData.fail()) {
			cerr << "Unable to open file." << endl;
			exit(5);
		}
		string data;
		crackData >> data;
		if (crackData.fail() && !crackData.eof()) {
			cerr << "Unable to read from file." << endl;
		}
		crackData.close();
		if (crackData.fail() && !crackData.eof()) {
			cerr << "Unable to close file." << endl;
		}

		// If empty, wait a bit and try again
		// If incorrect, wait a bit and try again
		// If correct, exit loop and show user attempts.
		if (data.empty()) {
			continue;
		} else if (strncmp(data.c_str(), code, 7) == 0) {
			cout << data << " is the correct passcode!\a" << endl; // \a is a bell character. It makes my computer go
																   // beep so that if I fall asleep while basicAlg is
																   // counting to 999999 I don't miss the attempt count
			attempts++;
			return attempts;
		} else {
			cout << "Incorrect passcode. Attempt " << attempts << ". " << data << endl;
			attempts++;
		}

		// Wipe file
		fstream fileWipr(fileLocation, ios::out);
		if (fileWipr.fail()) {
			cerr << "Unable to clear file." << endl;
		}
		fileWipr.close();
		if (fileWipr.fail()) {
			cerr << "Unable to close cleared file." << endl;
		}
		//usleep(1); // 1 us read delay, to prevent SSD lockup
	}

	return 0;
	// return number of attempts
}

int main(int argc, char * argv[]) {
	char code[7];
	bool argumentFailed = false;
	while (true) {
		if (argc <= 1 || argumentFailed) {
			cout << "What do you want the secret code to be? (6 digits only!): ";
			fgets(code, 7, stdin);
		} else {
			for (int i = 0; i != 7; i++) {
				code [i] = argv[1][i];
			}
		}
		bool codeValid = true;
		for (int i = 0; i != 6 && codeValid; i++) {
			if (code[i] < 48 || code[i] > 57) {
				cout << "You have entered an invalid code. Please enter a 6-digit PIN code." << endl;
				codeValid = false;
				argumentFailed = true;
			}
		}
		if (codeValid) {
			break;
		}
	}

	string ioFile = getenv("HOME"); ioFile += "/passwdfile";

	cout << "Your pin code of " << code << " is valid. ";
	while (true) {
		cout << "Please execute a brute-force command in another terminal, and point it to " << ioFile << ", or press Ctrl+C to exit the program." << endl;
		int attempts = monitorFile(code, ioFile);
		cout << "That algorithm cracked code " << code << " in " << attempts << " tries!" << endl;
		usleep(5000000); //usleep for 5 seconds after a successful crack
	}

	// Read every ms
	// If nothing, check again
	// If something, verify code and cout either valid or false.
	// If false, add to failed attempts and keep logging.
	// If true, halt program and delete file.
	// If SIGKILL, delete file.
}