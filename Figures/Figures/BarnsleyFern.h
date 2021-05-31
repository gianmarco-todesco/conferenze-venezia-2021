#pragma once
#include "Figure.h"
#include <QPointF>

class BarnsleyFern : public Figure
{
	QPointF m_center;
	double m_scale;
	QPointF m_pos;
	int m_mode;

public:
	BarnsleyFern(int mode=1) : m_center(), m_scale(1), m_mode(mode) {}
	void paint(QPainter& pa, int w, int h) override;

	void drawSimple(QPainter& pa, QList<QPointF>& pts, double unit);

	void draw(QPainter& pa, QList<QPointF>& pts, double unit, int mode);
	void makePoints(QList<QPointF>& pts, int n);

	void click(int x, int y);

};

