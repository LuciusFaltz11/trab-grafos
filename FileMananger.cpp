#include "FileMananger.h"

FileMananger::FileMananger()
{
}
FileMananger::~FileMananger()
{
}

void FileMananger::ListAvailableFiles(char grafoPonderado)
{
	string path;
	int i = 0;

	this->ponderado = grafoPonderado;

	if(grafoPonderado == 's'){
		path = "./files/grafosPonderados/";
		for (const auto &entry : filesystem::directory_iterator(path))
		{
			cout << "[ " << i << " ] " << entry.path().filename() << endl;
			i++;
		}
	} else {
		path = "./files/grafosNaoPonderados/";
		for (const auto &entry : filesystem::directory_iterator(path))
		{
			cout << "[ " << i << " ] " << entry.path().filename() << endl;
			i++;
		}
	}
}

string FileMananger::GetFileNameByIndex(int index)
{
	string path;
	int i = 0;

	if(ponderado == 's'){
		path = "./files/grafosPonderados/";
		for (const auto &entry : filesystem::directory_iterator(path))
		{
			if (i == index)
			{
				return entry.path().filename().string();
			}
			i++;
		}
	} else{
		path = "./files/grafosNaoPonderados/";
		for (const auto &entry : filesystem::directory_iterator(path))
		{
			if (i == index)
			{
				return entry.path().filename().string();
			}
			i++;
		}
	}
	return "";
}

void FileMananger::Read(string fileName, void (*func)(string, Grafo*), Grafo *grafo)
{
	if(ponderado == 's'){
		fileName = "./files/grafosPonderados/" + fileName;	
	}else{
		fileName = "./files/grafosNaoPonderados/" + fileName;
	}

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
	if(ponderado == 's'){
		fileName = "./files/grafosPonderados/" + fileName;	
	}else{
		fileName = "./files/grafosNaoPonderados/" + fileName;
	}

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