#pragma once
#include "Figure.h"
class Tree : public Figure
{
	double m_s, m_a;
public:
	Tree(double s = 0.82, double a = 31) : m_s(s), m_a(a) {}
	void paint(QPainter& pa, int w, int h) override;
};

