#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;

fstream openFileForWrite() {
	string ioFile = getenv("HOME"); ioFile += "/Dev/School/C++/FINAL/driveThing/passwdfile";
	fstream fout(ioFile, ios::out);
	if (fout.fail()) {
		cerr << "Unable to create file." << endl;
		exit(5);
	}
	return fout;
}

int main() {
	int code = 0; //000000
	cout << "Starting lazybrute..." << endl;
	while (true) {
		fstream outFile = openFileForWrite();
		char codeStr[7]; sprintf(codeStr, "%06d", code);

		cout << "Trying code " << codeStr << "..." << endl;
		// outFile << codeStr;
		// usleep(1); // 1 us write delay, to prevent SSD lockup
		// if (outFile.fail()) {
		// 	cerr << "Unable to write to file. Trying again." << endl;
		// 	continue;
		// }
		// outFile.close();
		// if (outFile.fail()) {
		// 	cerr << "Unable to close file." << endl;
		// }
		// if (code == 999999) {
		// 	cout << "Reached code 999999. Exiting lazybrute" << endl;
		// 	return 5;
		// }
		code++;
	}
}