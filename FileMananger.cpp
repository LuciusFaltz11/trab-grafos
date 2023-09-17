#include "Grafo.h"


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
		if (i == index)
		{
			return entry.path().filename().string();
		}
		i++;
	}
	return "";
}

void FileMananger::Read(string fileName, void (*func)(string, Grafo*), Grafo *grafo)
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
		string line;
		while (getline(file, line))
		{
			func(line, grafo);
		}
		file.close();
	}
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
		string line;
		while (getline(file, line))
		{
			cout << line << endl;
		}
		file.close();
	}
}