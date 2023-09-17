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
	int i = 0;
	for (const auto &entry : filesystem::directory_iterator(path))
	{
		cout << "[ " << i << " ] " << entry.path().filename() << endl;
		i++;
	}
}

string FileMananger::GetFileNameByIndex(int index)
{
	string path = "./files";
	int i = 0;
	for (const auto &entry : filesystem::directory_iterator(path))
	{
		// cout << "[ " << i << " ] " << entry.path().filename() << endl;
		if(i == index){
			return entry.path().filename().string();
		}
		i++;
	}
	return "";
}

void FileMananger::Read(string fileName)
{
	fileName = "./files/" + fileName;
	fstream file;
	file.open(fileName, ios::in);
	if (!file)
	{
		cout << "Arquivo '" << fileName << "' nao encontrado! " << endl;
	}
	else
	{
		char ch;
		while (1)
		{
			file >> ch;
			if (file.eof())
				break;
			cout << ch;
		}
	}
}