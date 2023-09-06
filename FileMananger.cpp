#include "FileMananger.h"

FileMananger::FileMananger()
{
}
FileMananger::~FileMananger()
{
}


void FileMananger::ListAvailableFiles()
{
    string path = "./files";
    for (const auto &entry : filesystem::directory_iterator(path))
        cout << entry.path().filename() << endl;
}

void FileMananger::Read(string fileName){
    fileName = "./files/"+fileName;
    fstream file;
    file.open(fileName, ios::in);
    if (!file) {
		cout << "Arquivo '"<< fileName << "' nao encontrado! " << endl;
	}
	else {
		char ch;
		while (1) {
			file >> ch;
			if (file.eof())
				break;
			cout << ch;
		}

	}

}