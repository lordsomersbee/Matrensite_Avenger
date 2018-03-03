#include "stdafx.h"
#include "Martensite.h"


Martensite::Martensite(Base* t, int t_hp, int t_t, int t_d, int t_v, string path, bool dir, int t_x, int t_y)
	: Base(t, t_hp, t_t, t_d, t_v, path, dir, t_x, t_y), graphRatio(300), graphWidth(12)
{
	displayHp.setPosition(200, 500);
	displayTemp.setPosition(200, 550);

	displayRey.setRadius(30);
	displayRey.setOrigin(30, 30);
	displayRey.setFillColor(Color::Black);
	displayRey.setOutlineColor(Color::Color(198, 0, 13));
	displayRey.setOutlineThickness(0);
	displayRey.setPosition(100, 525);

	/*font.loadFromFile("fonts/arial.ttf");
	tmp.setCharacterSize(20);
	tmp.setFillColor(Color::White);
	tmp.setPosition(2, 500);
	tmp.setString(to_string(temp));*/
}

Martensite::~Martensite() {}

void Martensite::updateGraphix()
{
	character.setTextureRect(IntRect(pack.x, pack.y, frameWidth, frameHeight));
	character.setPosition(position.x, position.y);

	if (!this->isDead)
	{
		displayHp.setSize(Vector2f(hp / (constHp / graphRatio), graphWidth));
		displayTemp.setSize(Vector2f(temp / (1200 / graphRatio), graphWidth));

		displayCooldown.setSize(Vector2f(cooldownCounter[0] / (cooldown[0] / 60), 3));
		displayCooldown.setPosition(position.x, position.y - 70);

		displayRey.setOutlineThickness(cooldownCounter[1] / (cooldown[1] / displayRey.getRadius()));

		if (direction) pack.setCordinates(5 * frameWidth, 4 * frameHeight);
		else pack.setCordinates(5 * frameWidth, 5 * frameHeight);
	}
	else
	{
		displayHp.setFillColor(Color::Transparent);
		displayTemp.setFillColor(Color::Transparent);
		displayCooldown.setFillColor(Color::Transparent);
		rey.setFillColor(Color::Transparent);
		displayRey.setFillColor(Color::Transparent);
	}
}