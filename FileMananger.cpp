#include "FileMananger.h"

FileMananger::FileMananger()
{
}
FileMananger::~FileMananger()
{
}

void FileMananger::ListAvailableFiles(char tipo)
{
	string path;
	int i = 0;

	this->tipoAresta = tipo;

	if(tipoGrafo == 's'){
		path = "./files/grafoDirecionado/";
		for (const auto &entry : filesystem::directory_iterator(path))
		{
			cout << "[ " << i << " ] " << entry.path().filename() << endl;
			i++;
		}
	} else if(tipoGrafo == 'n'){
		path = "./files/grafoNaoDirecionado/";
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

	if(tipoGrafo == 's'){
		path = "./files/grafoDirecionado/";
		for (const auto &entry : filesystem::directory_iterator(path))
		{
			if (i == index)
			{
				return entry.path().filename().string();
			}
			i++;
		}
	} else if(tipoGrafo == 'n'){
		path = "./files/grafoNaoDirecionado/";
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
	if(tipoGrafo == 's'){
		fileName = "./files/grafoDirecionado/" + fileName;	
	}else{
		fileName = "./files/grafoNaoDirecionado/" + fileName;
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
	if(tipoGrafo == 's'){
		fileName = "./files/grafoDirecionado/" + fileName;	
	}else{
		fileName = "./files/grafoNaoDirecionado/" + fileName;
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