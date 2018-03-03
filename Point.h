#pragma once
class Point
{
	int x, y;
public:
	Point();
	Point(int, int);
	~Point();
	void setCordinates(int, int);

	friend class Base;
	friend class Austenite;
	friend class Martensite;
	friend int main();
	template <class Left, class Right> friend int checkIfUpdateStatsIsRequired(Left& left, Right& right);
};

