#pragma once
#include "Figure.h"
class Cloud : public Figure
{
public:
	void paint(QPainter& pa, int w, int h) override;
};

