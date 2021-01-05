#include"conio.h"
#include"stdio.h"
#include"string.h"
#include"iostream"
#include <fstream>
#include<vector>
#include "../SuperMarioBros3/Utils.cpp"
using namespace std;

#define MAX_SCENE_LINE 1024
#define TILESET_WIDTH	16
#define TILESET_HEIGHT	16
#define CELL_WIDTH		177
#define CELL_HEIGHT		144


void CreateGridFile(int sceneId);
void ReadMapInformation(int sceneID, int& mapWidth, int& mapHeight);
void WriteGridFile(int sceneID,int gridCols, int gridRows);


int main()
{
	int sceneID;
	/*const char* newDir = R"(D:\CastlevaniaUit\CastlevaniaUit\)";*/
	//if (!SetCurrentDirectory(newDir)) {
	//	std::cerr << "Error setting current directory: #" << GetLastError();
	//	return 1; // quit if we couldn't set the current directory
	//}
	cout << "Insert Scene you want to create grid: ";
	cin >> sceneID;
	CreateGridFile(sceneID);
	cout << "\n";
	system("Pause");
	return 0;
}



void CreateGridFile(int sceneID)
{
	int mapWidth = -1, mapHeight = -1;

	ReadMapInformation(sceneID, mapWidth, mapHeight);


	if (mapWidth == -1 || mapHeight == -1)
		return;


	int gridCols = mapWidth / CELL_WIDTH;
	int gridRows = mapHeight / CELL_HEIGHT;

	cout << "\nColumns of Grid:\t" << gridCols << "\nRows of Grid:\t" << gridRows;

	

	WriteGridFile(sceneID,gridCols, gridRows);

};




void ReadMapInformation(int sceneID, int& mapWidth, int& mapHeight)
{
	char fileMapName[100];

	sprintf_s(fileMapName, "..\\SuperMarioBros3\\Resources\\Map\\World1-%d\\World1-%d_Inf.txt", sceneID, sceneID);
	ifstream mapFile(fileMapName);
	if (!mapFile)
		cout << "Could not open the map file!";
	else
	{
		cout << "Open the map file sucessfully!" << endl;
		char str[MAX_SCENE_LINE];
		mapFile.getline(str, MAX_SCENE_LINE);
		string line(str);

		vector<string> tokens = split(line);
		if (tokens.size() < 19) return; // skip invalid lines

		int tolRowMap = atoi(tokens[3].c_str());
		int tolColMap = atoi(tokens[4].c_str());


		mapWidth = tolRowMap * TILESET_WIDTH;
		mapHeight = tolColMap * TILESET_HEIGHT;


		cout << "\nMapWidth:\t" << mapWidth << "\nMapHeight:\t" << mapHeight;
	}
	mapFile.close();
};

void WriteGridFile(int sceneID,int gridCols, int gridRows)
{
	char gridFileName[100];
	sprintf_s(gridFileName, "..\\SuperMarioBros3\\Resources\\Map\\World1-%d\\World1-%d_Grid.txt", sceneID, sceneID);
	ofstream gridFile;
	gridFile.open(gridFileName);
	gridFile << "Cols of Grid\tRows of Grid\n";
	gridFile << gridCols << "\t" << gridRows;

	char objectsFileName[100];
	sprintf_s(objectsFileName, "..\\SuperMarioBros3\\Resources\\Map\\World1-%d\\World1-%d_Obj.txt", sceneID, sceneID);
	ifstream objFile(objectsFileName);
	char str[1024];

	while (objFile.getline(str, 1024))
	{
		string line(str);
		vector<string> tokens = split(line);
		
		
		if (line[0] != '#' && line[0] != '0' && atoi(tokens[0].c_str()) != 13)
		{
			if (tokens.size() < 3)
				continue;

			float x = (float)atof(tokens[1].c_str());
			float y = (float)atof(tokens[2].c_str());

			int row = (int)(y / CELL_HEIGHT);
			int col = (int)(x / CELL_WIDTH);


			if (row == gridRows)
				row = gridRows - 1;
			if (col == gridCols)
				col = gridCols - 1;

			gridFile << "\n" << line << "\t" << row << "\t" << col;
		}
		else
		{
			gridFile << "\n" << line;
		}
		cout <<"\n" << line ;
	}
	objFile.close();
	gridFile.close();

}