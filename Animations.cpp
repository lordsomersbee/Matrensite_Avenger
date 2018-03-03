#include "stdafx.h"
#include "Animations.h"


Animations::Animations() {}

Animations::~Animations() {}

void Animations::changeAttributes(float s, int fc)
{
	frame = 0;
	animSpeed = s;
	frameCount = fc;
}