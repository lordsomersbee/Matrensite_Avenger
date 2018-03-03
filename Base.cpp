#include "stdafx.h"
#include "Base.h"


Base::Base(Base* tar, int t_hp, int t_t, int t_d, int t_v, string path, bool dir, int t_x, int t_y)
	: target(tar), hp(t_hp), temp(t_t), dmg(t_d), moveVelo(t_v), COOLDOWN_ABILITIES(2), PLAYER_POSITION{t_x,t_y}
{
	t.loadFromFile(path);
	character.setTexture(t);
	character.setOrigin(frameWidth / 2, frameHeight / 2);
	position.setCordinates(PLAYER_POSITION.x, PLAYER_POSITION.y);
	character.setPosition((int)position.x, (int)position.y);
	pack.setCordinates(5 * frameWidth, 3 * frameHeight);
	
	isAttack = false;
	isBlock = false;
	isReleaseBlock = false;
	attackStats = true;
	isDead = false;
	isRey = false;
	isReleaseRey = false;
	direction = dir;

	reyLength = 0;
	constHp = hp;
	constTemp = temp;
	constDmg = dmg;
	constVelo = moveVelo;
	
	a = new Animations[5];
	
	attackIndex = 0;
	reyIndex = 1;
	moveIndex = 2;
	blockIndex = 3;
	dieIndex = 4;

	a[moveIndex].changeAttributes(0.3f, 6);
	a[attackIndex].changeAttributes(0.2, 6);
	a[blockIndex].changeAttributes(0.2f, 3);
	a[dieIndex].changeAttributes(0.2f, 6);
	a[reyIndex].changeAttributes(0.2f, 3);

	cooldown = new int[COOLDOWN_ABILITIES];

	cooldown[attackIndex] = 2 * FPS;
	cooldown[reyIndex] = 15 * FPS;

	cooldownCounter = new int[COOLDOWN_ABILITIES];
	for (int i = 0; i < COOLDOWN_ABILITIES; i++)
	{
		cooldownCounter[i] = 0;
	}	

	displayHp.setFillColor(Color::Color(210, 9, 0));
	displayTemp.setFillColor(Color::Color(45, 84, 210));

	displayCooldown.setFillColor(Color::Color(34,22,158));
	displayCooldown.setOrigin(30, 0);

	rey.setFillColor(Color::Color(198, 0, 13));
}

Base::~Base() {}

void Base::attack()
{
	pack.x = int(a[attackIndex].frame) * frameWidth;

	if (direction) pack.y = 2 * frameHeight;
	else pack.y = 3 * frameHeight;
	
	a[attackIndex].frame += a[attackIndex].animSpeed;
	
	if (a[attackIndex].frame > a[attackIndex].frameCount)
	{
		a[attackIndex].frame -= a[attackIndex].frameCount;
		isAttack = false;
		attackStats = true;
	}
}

void Base::movementAnimation()
{
	pack.x = int(a[moveIndex].frame) * frameWidth;
	a[moveIndex].frame += a[moveIndex].animSpeed;
	if (a[moveIndex].frame > a[moveIndex].frameCount) a[moveIndex].frame -= a[moveIndex].frameCount;
}

void Base::rightMove()
{
	direction = true;
	position.x += moveVelo;
	pack.y = 0;

	
	movementAnimation();
}

void Base::leftMove()
{
	direction = false;
	position.x -= moveVelo;
	pack.y = frameHeight;

	movementAnimation();
}

void Base::block()
{
	pack.x = (int)a[blockIndex].frame* frameWidth;

	if (direction) pack.y = 4 * frameHeight;
	else pack.y = 5 * frameHeight;

	if (a[blockIndex].frame < a[blockIndex].frameCount - 1)
	{
		a[blockIndex].frame += a[blockIndex].animSpeed;
	}
	else
	{
		a[blockIndex].frame = 2;
	}
}

void Base::releaseBlock()
{
	pack.x = (int)a[blockIndex].frame * frameWidth;
	
	if (direction) pack.y = 4 * frameHeight;
	else pack.y = 5 * frameHeight;

	if (a[blockIndex].frame > 0)
	{
		a[blockIndex].frame -= a[blockIndex].animSpeed;
	}
	else
	{
		a[blockIndex].frame = 0;
		isReleaseBlock = false;
	}
}

void Base::kineticRey()
{
	if (cooldownCounter[reyIndex] > 0 || !isRey)
	{
		isRey = false;
		return;
	}

	pack.x = (int)getFrame(reyIndex) * frameWidth;

	if (direction) pack.y = 8 * frameHeight;
	else pack.y = 9 * frameHeight;

	if (getFrame(reyIndex) < getFrameCount(reyIndex) - 1) setFrame(reyIndex, getFrame(reyIndex) + getAnimSpeed(reyIndex));
	else setFrame(reyIndex, 2);

	if (direction) rey.setSize(Vector2f(reyLength, 5));
	else rey.setSize(Vector2f(-reyLength, 5));

	int dist = abs(target->getPosition().x - this->position.x) - 40;

	reyLength += 70;

	if (checkSide() && (reyLength >= dist))
	{
		reyLength = dist;
		target->setSpeed(2);
		if (target->getTemp() > 0)
		{
			target->changeTemp(-3);

			this->temp -= 5;

			if (!target->isDead)
			{
				if ((this->hp < this->constHp)) this->hp += 0.5;
				else this->hp = constHp;
			}
		}
	}
}

void Base::releaseKineticRey()
{
	reyLength = 0;
	rey.setSize(Vector2f(reyLength, 5));

	pack.x = (int)getFrame(reyIndex) * frameWidth;

	if (direction) pack.y = 8 * frameHeight;
	else pack.y = 9 * frameHeight;

	if (getFrame(reyIndex) > 0) setFrame(reyIndex, getFrame(reyIndex) - getAnimSpeed(reyIndex));
	else
	{
		setFrame(reyIndex, 0);
		cooldownCounter[reyIndex] = cooldown[reyIndex];
		isReleaseRey = false;
	}
}

void Base::tempRegeneration()
{
	if (temp < constTemp) temp += 0.2;
}

void Base::die()
{
	pack.x = (int)a[dieIndex].frame * frameWidth;

	if (direction) pack.y = 6 * frameHeight;
	else pack.y = 7 * frameHeight;

	if (a[dieIndex].frame > a[dieIndex].frameCount) a[dieIndex].frame = 5;
	else a[dieIndex].frame += a[dieIndex].animSpeed;
}

void Base::draw(RenderTarget& target, RenderStates state) const
{
	target.draw(character, state);
	target.draw(displayHp, state);
	target.draw(displayTemp, state);
	target.draw(displayCooldown, state);
	target.draw(displayRey, state);
	target.draw(rey, state);
}

void Base::updateActions()
{
	if (this->isAttack) this->attack();
	else if (this->isBlock) this->block();
	else if (this->isReleaseBlock) this->releaseBlock();
	else if (this->isRey) this->kineticRey();
	else if (this->isReleaseRey) this->releaseKineticRey();
}

void Base::updateCooldown()
{
	tempRegeneration();

	for (int i = 0; i < COOLDOWN_ABILITIES; i++)
	{
		if(cooldownCounter[i] > 0) cooldownCounter[i]--;
	}
}

bool Base::checkSide()
{
	if ((this->direction && (target->getPosition().x > this->getPosition().x)) 
		|| (!this->direction && (target->getPosition().x < this->getPosition().x))) return true;
	else return false;
}

void Base::changeEnemy(Base* t)
{
	target = t;
}

Point Base::getPosition()
{
	return position;
}

int Base::getTemp()
{
	return temp;
}

float Base::getFrame(int i)
{
	return a[i].frame;
}

float Base::getAnimSpeed(int i)
{
	return a[i].animSpeed;
}

float Base::getFrameCount(int i)
{
	return a[i].frameCount;
}

void Base::setSpeed(int v)
{
	moveVelo = v;
}

void Base::changeTemp(int t)
{
	temp += t;
}

void Base::setFrame(int i, float s)
{
	a[i].frame = s;
}

template <class T> void checkFastBlock(T& t)
{
	if(this->isAttack)
}