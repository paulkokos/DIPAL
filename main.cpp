#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

int main () {
    ifstream myfile;
    myfile.open("C:\\search.png");

    if (myfile.is_open()) {
        myfile.close();
        cout<< "function success";
    } else {
        cout<< "unable to open file";
    }
    system("pause");
    return 0;
}