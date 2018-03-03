#include "stdafx.h"
#include "option.h"


Option::Option(int t_s, string t_t, int t_x, int t_y)
{
	font.loadFromFile("fonts/arial.ttf");

	text.setFont(font);
	text.setCharacterSize(t_s);
	text.setFillColor(Color::Magenta);

	text.setPosition(t_x, t_y);
	text.setString(t_t);

	isSelected = false;
}


Option::~Option() {}

void Option::draw(RenderTarget& target, RenderStates state) const
{
	target.draw(text, state);
}

void Option::select()
{
	if (!isSelected)
	{
		text.setFillColor(Color::White);
		isSelected = true;
	}
	else
	{
		text.setFillColor(Color::Magenta);
		isSelected = false;
	}
}