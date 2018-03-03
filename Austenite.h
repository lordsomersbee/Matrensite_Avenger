#pragma once
#include "Base.h"

class Austenite : public Base
{
	int distance, probability;
public:
	Austenite(Base*, int, int, int, int, string, bool, int, int);
	~Austenite();

	void actionDecision();

	void updateGraphix() override;

	friend class Martensite;
};

