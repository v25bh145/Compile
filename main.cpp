#include <iostream>
#include <fstream>
#include "compile/compile.h"
using namespace std;

int main(int argc, char* argv[]) {

    streambuf* coutBuf = cout.rdbuf();
    ofstream of("log.txt");
    streambuf* fileBuf = of.rdbuf();
    cout.rdbuf(fileBuf);

    Compile(argv[1], argv[2]);
    cout<<"YES BANGDREAM!"<<endl;
    
    of.flush();
    of.close();
    cout.rdbuf(coutBuf);
}