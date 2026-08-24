#include <iostream>
#include <fstream>

using namespace std;
int adress = 0x69;

int main() {

ifstream file("/dev/i2c-1", ios::in);
if (file.is_open()) {

    cout << "Datei ist geöffnet worden" << endl;
}



file.close();

return 0;

}





