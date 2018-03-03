#pragma once
class Animations
{
	float frame, animSpeed;
	int frameCount;
public:
	Animations();
	~Animations();
	void changeAttributes(float, int);

	friend class Base;
	//template <class Left, class Right> friend void checkIfUpdateStatsIsRequired(Left& left, Right& right);
	template <class Winner, class Loser> friend int updateStats(Winner& winner, Loser& loser);
};

