#pragma once
#include "Base.h"

class Martensite : public Base
{
	int graphRatio, graphWidth;
public:
	Martensite(Base*, int, int, int, int, string, bool, int, int);
	~Martensite();

	void updateGraphix() override;

	friend class Austenite;
};

