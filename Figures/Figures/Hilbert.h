#pragma once
#include "Figure.h"

#include <QPointF>
#include <QList>

struct IMat;
struct Node;

class Hilbert : public Figure
{
	
public:
	void paint(QPainter& pa, int w, int h) override;
	void build(QList<Node>& nodes, int level, const IMat& mat);
	void draw(QPainter& pa, int level, double size);
	void drawGrid(QPainter& pa, int level, double size);
};

