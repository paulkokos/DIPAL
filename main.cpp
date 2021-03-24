#include <iostream>
#include <fstream>
//#include <windows.h>
#include <vector>
using namespace std;

int main () {
    ifstream myfile;
    myfile.open("../etc/lena_color.tiff",std::ios::binary);
    int *filename;
    if (myfile.is_open()) {
        std::vector<int> buffer(std::istreambuf_iterator<char>(myfile), {});
        cout << buffer.size() << endl;
//        filename = &buffer[0];
//        cout << &filename << endl;
        for (int i=0;i< buffer.size();i++) {
            cout << std::hex << buffer[i]<< endl;
        }
        myfile.close();
        cout<< "function success";
    } else {
        cout<< "unable to open file";
    }
    system("pause");
    return 0;
}