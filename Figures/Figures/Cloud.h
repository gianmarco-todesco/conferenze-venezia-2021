#pragma once
#include "Figure.h"
class Cloud : public Figure
{
	int m_seed;
public:
	Cloud(int seed = 1234581) : m_seed(seed) {}
	void paint(QPainter& pa, int w, int h) override;
};

