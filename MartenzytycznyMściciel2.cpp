// MartenzytycznyMściciel2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include "Base.h"
#include "Austenite.h"
#include "Martensite.h"
#include "option.h"

using namespace sf;
using namespace std;

const unsigned int WIDTH = 1200, HEIGHT = 600, frameWidth = 150, frameHeight = 200;

template <class Left, class Right> int checkIfUpdateStatsIsRequired(Left& left, Right& right)
{
	if ((abs(left.position.x - right.position.x) < (frameWidth / 2))
		&& ((left.isAttack) || (right.isAttack)))
	{
		if (left.isAttack) return updateStats(left, right);
		else if(right.isAttack) return updateStats(right, left);
	}

	return 0;
}

template <class Winner, class Loser> int updateStats(Winner& winner, Loser& loser)
{
	if ((int)winner.a[winner.attackIndex].frame == 3 && winner.attackStats)
	{
		winner.attackStats = false;

		if (loser.isRey)
		{
			loser.isRey = false;
			loser.isReleaseRey = true;
			loser.cooldownCounter[loser.reyIndex] = loser.cooldown[loser.reyIndex];
			winner.setSpeed(winner.constVelo);
		}

		if (loser.isBlock && (loser.direction != winner.direction))
		{
			winner.isAttack = false;
			winner.attackStats = true;
			winner.a[winner.attackIndex].frame = 0;
			loser.hp -= (winner.dmg / 3);

			return 1;
		}
		else
		{
			loser.hp -= winner.dmg;
			loser.isAttack = false;

			return 2;
		}
	}
}

int main()
{
	RenderWindow window{ VideoMode{ WIDTH,HEIGHT },"Martenzytycny Mściciel 2 v1.0.0 alpha" };
	window.setFramerateLimit(60);

	Event e;

	bool played = false, win;

	//TITLE SCREEN
	while (true)
	{
		{
			Music titleMusic;
			titleMusic.openFromFile("sounds/title_music.wav");
			titleMusic.setLoop(true);
			titleMusic.play();

			bool start = false;

			Texture t;
			Sprite logo;

			Font font;
			Text text, desc;

			if (!played)
			{
				t.loadFromFile("img/logo.png");
				logo.setTexture(t);
			}
			else
			{
				font.loadFromFile("fonts/arial.ttf");
				text.setFont(font);
				text.setCharacterSize(100);
				text.setFillColor(Color::White);
				text.setOrigin(200, 0);
				text.setPosition(WIDTH / 2, 20);

				desc.setFont(font);
				desc.setCharacterSize(20);
				desc.setFillColor(Color::White);
				//desc.setOrigin(200, 0);
				desc.setPosition(10, 170);


				if (win)
				{
					text.setString("Zwyciestwo");
					desc.setString("Jedna z wielu bitew Matrenzytycznego Msciciela przeciwko Austenitycznym Rycerzom zakonczona zwycieztwem bohatera. \n		Jednak nawet ta jedna wygrana, choc nie wiele zmienia, jest powodem do dumy. Ale czy napewno...");
				}
				else
				{
					text.setString("Porazka");
					desc.setString("Rycerze Austenitu pokonali Martenzytycznego Msciciela. Dziely bohater polegl, lecz w jego miejsce przyjda kolejni. \n		Kolejni beda probowali przywrocic rownowage na wykresie zelazo cementyt niszczac Austenit. Czy napewno tedy droga ?");
				}
			}

			int selected = 0;
			const int optAmount = 3;
			Option o1[optAmount] = { { 20, "Nowa gra", 500, 250 },{ 20, "Wczytaj gre", 500, 300 },{ 20, "Wyjscie", 500, 350 } };
			o1[selected].select();

			while (!start)
			{
				while (window.pollEvent(e))
				{
					if (e.type == Event::Closed)
						window.close();

					if (e.type == Event::KeyPressed)
					{
						if (e.key.code == Keyboard::Up && selected > 0)
						{
							o1[selected].select();
							selected--;
							o1[selected].select();
						}

						if (e.key.code == Keyboard::Down && selected < optAmount - 1)
						{
							o1[selected].select();
							selected++;
							o1[selected].select();
						}

						if (e.key.code == Keyboard::Return)
						{
							switch (selected)
							{
							case 0:
								start = true;
								break;
							case 1:
								break;
							case 2:
								exit(0);
							}
						}
					}
				}

				window.clear();
				window.draw(logo);
				window.draw(text);
				window.draw(desc);

				for (int i = 0; i < optAmount; i++)
				{
					window.draw(o1[i]);
				}

				window.display();
			}

			titleMusic.stop();

		}

		//GAME
		{
			Music fightMusic;
			fightMusic.openFromFile("sounds/fight_music.wav");
			fightMusic.setLoop(true);
			fightMusic.play();

			Music blockSound, strikeSound, reySound;
			blockSound.openFromFile("sounds/block.wav");
			strikeSound.openFromFile("sounds/attack.wav");
			reySound.openFromFile("sounds/rey.wav");
			int soundPlayed = 0;

			bool isReyPlaying = false;
			reySound.setLoop(true);

			int ms[] = { 600, 1200, 50, 6 }; //HP, Temp, Damage, Movement
			int as[] = { 600, 1200, 50, 4 };

			Texture back;
			back.loadFromFile("img/background.png");
			Sprite sBackground(back);

			Martensite player{ NULL, ms[0], ms[1], ms[2], ms[3], "img/player.png", true, 200, 400 };
			Austenite oponent{ &player, as[0], as[1], as[2], as[3], "img/player.png", false, 1000, 400 };
			player.changeEnemy(&oponent);

			while (window.isOpen())
			{
				while (window.pollEvent(e))
				{
					if (e.type == Event::Closed)
						window.close();

					if (!player.isDead)
					{
						if (e.type == Event::KeyPressed)
						{
							if (e.key.code == Keyboard::S && !player.isRey && !player.isReleaseRey)
							{
								player.isBlock = true;
							}
							else if (e.key.code == Keyboard::Q && !player.isBlock && !player.isReleaseBlock)
							{
								if(player.direction) player.rey.setPosition(player.position.x + 35, player.position.y - 36);
								else player.rey.setPosition(player.position.x - 35, player.position.y - 36);
								player.isRey = true;
							}
						}
						else if (e.type == Event::KeyReleased)
						{
							if (e.key.code == Keyboard::S)
							{
								player.isReleaseBlock = true;
								player.isBlock = false;
							}
							else if (e.key.code == Keyboard::Q && player.cooldownCounter[player.reyIndex] <= 0)
							{
								player.isRey = false;
								player.isReleaseRey = true;
								oponent.setSpeed(oponent.constVelo);
							}
						}
					}
				}

				if (!player.isBlock && !player.isReleaseBlock && !player.isAttack 
					&& !player.isDead && !player.isRey && !player.isReleaseRey)
				{
					if (Keyboard::isKeyPressed(Keyboard::D))
					{
						player.rightMove();
					}
					else if (Keyboard::isKeyPressed(Keyboard::A))
					{
						player.leftMove();
					}
					else if (Keyboard::isKeyPressed(Keyboard::W))
					{
						if (player.cooldownCounter[0] == 0)
						{
							player.isAttack = true;
							player.cooldownCounter[0] = player.cooldown[0];
						}
					}
				}

				soundPlayed = checkIfUpdateStatsIsRequired(player, oponent);

				oponent.actionDecision();

				player.updateActions();
				
				switch (soundPlayed)
				{
				case 1:
					blockSound.play();
					break;

				case 2:
					strikeSound.play();
					break;
				}

				if ((player.isRey) && !isReyPlaying)
				{
					isReyPlaying = true;
					reySound.play();
				}
				if ((!player.isRey) && isReyPlaying)
				{
					isReyPlaying = false;
					reySound.stop();
				}

				player.updateGraphix();
				oponent.updateGraphix();

				player.updateCooldown();
				oponent.updateCooldown();

				window.clear();

				if (player.hp <= 0 || player.temp <= 0)
				{
					player.isDead = true;
					player.isBlock = false; 
					player.isReleaseBlock = false; 
					player.isAttack = false;
					player.isRey = false;
					player.isReleaseRey = false;
					
					player.die();

					if (Keyboard::isKeyPressed(Keyboard::Return))
					{
						win = false;
						played = true;
						break;
					}
				}
				if (oponent.hp <= 0 || oponent.temp <= 0)
				{
					oponent.isDead = true;
					oponent.isBlock = false;
					oponent.isReleaseBlock = false;
					oponent.isAttack = false;
					oponent.isRey = false;
					oponent.isReleaseRey = false;

					oponent.die();

					if (Keyboard::isKeyPressed(Keyboard::Return))
					{
						win = true;
						played = true;
						break;
					}
				}

				window.draw(sBackground);
				window.draw(player);
				window.draw(oponent);
				window.display();
			}

			fightMusic.stop();

		}
	}
	return 0;
}


