#include "Tree.h"
#include <QPainter>
#include <QPainterPath>
#include <QTransform>


void drawTree(QPainter& pa, int level, double childScale, double angle)
{
	QPainterPath pp;
	pp.moveTo(0, 0);
	pp.lineTo(0, -100);
	pa.drawPath(pp);
	if(level>0)
	{
		const double sc = childScale;
		pa.save();
		pa.translate(0, -100);
		pa.scale(sc, sc);
		pa.rotate(-angle);
		for (int i = 0; i<2; i++)
		{
			drawTree(pa, level - 1, childScale, angle);
			pa.rotate(angle * 3 * 0.61803398875);
		}
		pa.restore();
	}
}

void Tree::paint(QPainter& pa, int w, int h)
{
	QPainterPath pp;
	pa.save();
	pa.translate(w / 2, h*0.9);
	double sc = h * 0.0016;
	pa.scale(sc,sc);
	pa.setPen(QPen(Qt::black, 5));
	drawTree(pa, 18, m_s, m_a);
	pa.restore();
	//pa.setPen(Qt::black);
	//pa.drawRect(2, 2, w-4, h-4);
}