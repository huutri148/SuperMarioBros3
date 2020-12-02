#pragma once
#include"Sprites.h"
#include <map>
class Font
{
	std::map<char, LPSPRITE> fonts;

public:
	Font();
	void Init();
	LPSPRITE mapping(char c);
	vector<LPSPRITE> StringToSprite(string str);
};

