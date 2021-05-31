#include "Hilbert.h"
#include <QPainter>
#include <QPainterPath>



struct IMat {
	int a, b, c, d, tx, ty;
	IMat() : a(1), b(0), c(0), d(1), tx(0), ty(0) {}
	IMat(int _a, int _b, int _c, int _d, int _tx, int _ty) : a(_a), b(_b), c(_c), d(_d), tx(_tx), ty(_ty) {}

	QPointF operator*(const QPointF& p) const {
		return QPointF(p.x() * a + p.y() * b + tx, p.x() * c + p.y() * d + ty);
	}
	QPoint operator*(const QPoint& p) const {
		return QPoint(p.x() * a + p.y() * b + tx, p.x() * c + p.y() * d + ty);
	}

	IMat operator*(const IMat& mat) const {
		return IMat(
			a * mat.a + b * mat.c,
			a * mat.b + b * mat.d,
			c * mat.a + d * mat.c,
			c * mat.b + d * mat.d,
			a * mat.tx + b * mat.ty + tx,
			c * mat.tx + d * mat.ty + ty);
	}
};

struct Node {
	QPointF pts[4];
};

//=============================================================================


void Hilbert::paint(QPainter& pa, int w, int h)
{
	pa.save();
	pa.translate(w / 2, h / 2);

	int ncol = 3;
	int nrow = 2;

	const double size2 = qMin(w / ncol, h / nrow);
	const double size = size2 * 0.9;

	for (int i = 0; i < ncol*nrow; i++)
	{
		int col = i % ncol;
		int row = i / ncol;
		pa.save();
		pa.translate(size2 * (col - (ncol - 1.) / 2.), size2 * (row - (nrow - 1.) / 2.));

		draw(pa, i, size);

		pa.restore();
	}
	pa.restore();
}


void Hilbert::draw(QPainter& pa, int level, double size)
{
	drawGrid(pa, level, size);
	QList<Node> nodes;
	build(nodes, level, IMat());

	const double unit = size / (2.0 * (2 << level));


	QPainterPath pp;
	for (int i = 0; i < nodes.count(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			QPointF p = nodes[i].pts[j] * unit;
			if (i == 0 && j == 0) pp.moveTo(p); else pp.lineTo(p);
		}
	}
	pa.setPen(QPen(Qt::black, 3));
	pa.drawPath(pp);
}

void Hilbert::drawGrid(QPainter& pa, int level, double size)
{
	int m = 2 << level;

	const double unit = size / (2.0 * m);
	double limit = size / 2.0;

	pa.setPen(QPen(QColor(220, 220, 220), 0));
	pa.setBrush(Qt::NoBrush);
	QPainterPath pp;
	for (int i = -m; i <= m; i++)
	{
		pp.moveTo(-limit, unit * i);
		pp.lineTo(limit, unit* i);
		pp.moveTo(unit * i, -limit);
		pp.lineTo(unit* i, limit);
	}
	pa.drawPath(pp);
}

void Hilbert::build(QList<Node>& nodes, int level, const IMat& mat)
{
	if (level == 0)
	{
		Node node;
		node.pts[0] = mat * QPointF(-1.0,  1.0);
		node.pts[1] = mat * QPointF(-1.0, -1.0);
		node.pts[2] = mat * QPointF( 1.0, -1.0);
		node.pts[3] = mat * QPointF( 1.0,  1.0);
		nodes.push_back(node);
	}
	else
	{
		double unit = 1 << (level);
		build(nodes, level - 1, mat * IMat(0,-1, -1, 0, -unit,  unit));
		build(nodes, level - 1, mat * IMat(1, 0,  0, 1, -unit, -unit));
		build(nodes, level - 1, mat * IMat(1, 0,  0, 1,  unit, -unit));
		build(nodes, level - 1, mat * IMat(0, 1,  1, 0,  unit,  unit));


	}
}