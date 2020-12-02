#include "Font.h"
void Font::Init()
{
	Sprites* sprites = Sprites::GetInstance();
	fonts.insert(make_pair('0', sprites->Get(50013)));
	fonts.insert(make_pair('1', sprites->Get(50014)));
	fonts.insert(make_pair('2', sprites->Get(50015)));
	fonts.insert(make_pair('3', sprites->Get(50016)));
	fonts.insert(make_pair('4', sprites->Get(50017)));
	fonts.insert(make_pair('5', sprites->Get(50018)));
	fonts.insert(make_pair('6', sprites->Get(50019)));
	fonts.insert(make_pair('7', sprites->Get(50020)));
	fonts.insert(make_pair('8', sprites->Get(50021)));
	fonts.insert(make_pair('9', sprites->Get(50022)));
	fonts.insert(make_pair('A', sprites->Get(50023)));
	fonts.insert(make_pair('B', sprites->Get(50024)));
	fonts.insert(make_pair('C', sprites->Get(50025)));
	fonts.insert(make_pair('D', sprites->Get(50026)));
	fonts.insert(make_pair('E', sprites->Get(50027)));
	fonts.insert(make_pair('F', sprites->Get(50028)));
	fonts.insert(make_pair('G', sprites->Get(50029)));
	fonts.insert(make_pair('H', sprites->Get(50030)));
	fonts.insert(make_pair('I', sprites->Get(50031)));
	fonts.insert(make_pair('J', sprites->Get(50032)));
	fonts.insert(make_pair('K', sprites->Get(50033)));
	fonts.insert(make_pair('L', sprites->Get(50034)));
	fonts.insert(make_pair('M', sprites->Get(50035)));
	fonts.insert(make_pair('N', sprites->Get(50036)));
	fonts.insert(make_pair('O', sprites->Get(50037)));
	fonts.insert(make_pair('P', sprites->Get(50038)));
	fonts.insert(make_pair('Q', sprites->Get(50039)));
	fonts.insert(make_pair('R', sprites->Get(50040)));
	fonts.insert(make_pair('S', sprites->Get(50041)));
	fonts.insert(make_pair('T', sprites->Get(50042)));
	fonts.insert(make_pair('U', sprites->Get(50043)));
	fonts.insert(make_pair('V', sprites->Get(50044)));
	fonts.insert(make_pair('W', sprites->Get(50045)));
	fonts.insert(make_pair('X', sprites->Get(50046)));
	fonts.insert(make_pair('Y', sprites->Get(50047)));
	fonts.insert(make_pair('Z', sprites->Get(50048)));
}

LPSPRITE Font::mapping(char c)
{
	int ic = (int)c;
	if(!(ic >= 48 && ic <= 57  || ic >= 64 && ic <= 90 ))
		return NULL;
	return fonts.at(c);
}
vector<LPSPRITE> Font::StringToSprite(string str)
{
	vector<LPSPRITE> sprites;
	LPSPRITE sprite;
	char c;
	for (int i = 0; i < str.size(); i++)
	{
		c = (char)str[i];
		sprite = mapping(c);
		if(sprite != NULL)
			sprites.push_back(sprite);
	}
	return sprites;
}
Font::Font()
{
	Init();
}
