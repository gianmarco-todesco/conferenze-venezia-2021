#pragma once
#include "Figure.h"
#include <QList>
#include <QPointF>

class QPointF;
class QPainterPath;

class Sierpinski : public Figure
{
public:
	void paint(QPainter& pa, int w, int h) override;
	void draw(QPainter& pa, int level, double r);
	void build(QPainterPath& pp, int level, const QPointF &p0, const QPointF& p1, const QPointF& p2);
};

class Sierpinski2 : public Figure
{
public:
	void paint(QPainter& pa, int w, int h) override;
	void draw(QPainter& pa, int level, double r);
	void drawLast(QPainter& pa, int level, double r);
	void build(QList<QPointF> & pts, int level, const QPointF& p0, const QPointF& p1, const QPointF& p2);

	void buildGrid(QPainterPath& pp, int level, const QPointF& p0, const QPointF& p1, const QPointF& p2);
};

