#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <string>

using namespace sf;
using namespace std;

class Option : public Drawable 
{
	Font font;
	Text text;
	bool isSelected;
public:
	Option(int, string, int, int);
	~Option();
	void draw(RenderTarget& Target, RenderStates State) const override;
	void select();
};

