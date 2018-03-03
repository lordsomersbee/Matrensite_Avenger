#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include "Animations.h"
#include "Point.h"

using namespace sf;
using namespace std;

const int FPS = 60;

class Base : public Drawable
{
protected:
	int dmg, moveVelo, reyLength, constHp, constDmg, constVelo;
	float hp, temp, constTemp;
	bool isAttack, isBlock, isReleaseBlock, isFastBlock, isCritical, attackStats, isDead, isRey, isReleaseRey, direction; //0 - left; 1 - right
	int* cooldown;
	int* cooldownCounter;
	Base* target;
	Texture t;
	Sprite character;
	Animations* a;
	int attackIndex, reyIndex, moveIndex, blockIndex, dieIndex;
	const Point PLAYER_POSITION{ 200, 400 };
	Point position, pack;
	const int frameWidth = 150, frameHeight = 200, COOLDOWN_ABILITIES = 1; //number of them
	RectangleShape displayHp;
	RectangleShape displayTemp;
	RectangleShape displayCooldown;
	RectangleShape rey;
	CircleShape displayRey;

public:
	Base(Base*, int, int, int, int, string, bool, int, int);
	~Base();
	void attack();
	void movementAnimation();
	void rightMove();
	void leftMove();
	void block();
	void releaseBlock();
	void kineticRey();
	void releaseKineticRey();
	void tempRegeneration();
	void die();
	void draw(RenderTarget& Target, RenderStates State) const override;
	void updateActions();
	void updateCooldown();

	bool checkSide();
	
	virtual void updateGraphix() = 0;
	
	void changeEnemy(Base*);
	Point getPosition();
	int getTemp();
	float getFrame(int);
	float getAnimSpeed(int);
	float getFrameCount(int);
	void setSpeed(int);
	void changeTemp(int);
	void setFrame(int, float);

	template <class T> void checkFastBlock(T& t);

	friend int main();
	template <class Left, class Right> friend int checkIfUpdateStatsIsRequired(Left& left, Right& right);
	template <class Winner, class Loser> friend int updateStats(Winner& winner, Loser& loser);
	friend class Austenite;
};

