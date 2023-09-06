#include <iostream>

using namespace std;
#include "FileMananger.h"

int main(int argc, char const *argv[])
{
    FileMananger fileMananger;

    fileMananger.ListAvailableFiles();
    // fileMananger.Read("teste.txt");

    return 0;
}
