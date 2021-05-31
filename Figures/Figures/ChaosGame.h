#pragma once
#include "Figure.h"


class ChaosGame : public Figure
{
	int m_mode;
public:
	ChaosGame(int mode) : m_mode(mode) {}
	void paint(QPainter& pa, int w, int h) override;

	struct Params {
		double w, h;
		int vCount;
		int pointsCount;
	};
	void paint(QPainter& pa, const Params &params);
};

