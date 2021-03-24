#include <iostream>
#include <fstream>
//#include <windows.h>
#include <vector>
using namespace std;

int main () {
    ifstream myfile;
    myfile.open("../etc/lena.png",std::ios::binary);

    if (myfile.is_open()) {
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(myfile), {});
        for (int i=0;i< buffer.size();i++) {
            cout << (int)buffer[i]<< endl;
        }
        myfile.close();
        cout<< "function success";
    } else {
        cout<< "unable to open file";
    }
    system("pause");
    return 0;
}