#include "stdafx.h"
#include "Austenite.h"


Austenite::Austenite(Base* t, int t_hp, int t_t, int t_d, int t_v, string path, bool dir, int t_x, int t_y)
	: Base(t, t_hp, t_t, t_d, t_v, path, dir, t_x, t_y), probability(70) 
{
	displayHp.setOrigin(30, 0);
	displayTemp.setOrigin(30, 0);
}

Austenite::~Austenite() {}

void Austenite::actionDecision()
{
	if (this->isDead)
	{
		this->die();
	}
	else
	{
		//Check Direction
		if (target->position.x < this->position.x) this->direction = false;
		else this->direction = true;

		//Check COOLDOWN for ATTACK
		if ((this->cooldownCounter[this->attackIndex] - this->cooldown[this->attackIndex] / 2 > 0)
			&& !this->isAttack && !this->isBlock && !this->isReleaseBlock)
		{
			//RUN!!
			this->direction = !this->direction;

			if (this->direction) this->rightMove();
			else this->leftMove();
		}
		//Check Distance
		else
		{
			if (abs(target->position.x - this->position.x) < (frameWidth / 2))
			{
				//Check if PLAYER is ATTACK
				if (target->isAttack && (target->direction != this->direction) && (rand() % 100 <= this->probability)) this->isBlock = true;
				else
				{
					//Check if RELEASING BLOCK is required
					if (this->isBlock)      
					{
						this->isBlock = false;
						this->isReleaseBlock = true;
					}
					//Check if NPC can ATTACK
					else if (!this->isReleaseBlock /*&& (trial < this->probability)*/)
					{
						this->isAttack = true;
						this->cooldownCounter[this->attackIndex] = this->cooldown[this->attackIndex];
					}
				}
			}
			else if (!this->isBlock && !this->isReleaseBlock)
			{
				//Choose way to MOVE 
				if (this->direction) this->rightMove();
				else this->leftMove();
			}
		}

		this->updateActions();

	}
}

void Austenite::updateGraphix()
{
	character.setTextureRect(IntRect(pack.x, pack.y, frameWidth, frameHeight));
	character.setPosition(position.x, position.y);

	if (!this->isDead)
	{
		displayHp.setSize(Vector2f(hp / (constHp / 60), 3));
		displayHp.setPosition(position.x, position.y - 80);
		displayTemp.setSize(Vector2f(temp / (constTemp / 60), 3));
		displayTemp.setPosition(position.x, position.y - 75);

		displayCooldown.setSize(Vector2f(cooldownCounter[0] / (cooldown[0] / 60), 3));
		displayCooldown.setPosition(position.x, position.y - 70);

		if (direction) pack.setCordinates(5 * frameWidth, 4 * frameHeight);
		else pack.setCordinates(5 * frameWidth, 5 * frameHeight);
	}
	else
	{
		displayHp.setFillColor(Color::Transparent);
		displayTemp.setFillColor(Color::Transparent);
		displayCooldown.setFillColor(Color::Transparent);
	}
}
