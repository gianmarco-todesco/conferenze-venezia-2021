#include "Sierpinski.h"
#include <QPainter>
#include <QPainterPath>

#include <QPointF>
#include <math.h>
#include <QDebug>

const double PI = 4.0 * atan(1.0);

const double cs120 = cos(PI * 2 / 3);
const double sn120 = sin(PI * 2 / 3);

const QPointF corners[3] = {
	QPointF(0,-1),
	QPointF(-sn120, -cs120),
	QPointF( sn120, -cs120)
};

void Sierpinski::paint(QPainter& pa, int w, int h)
{
	int m = 5;

	double ar = 2.0/sqrt(3.0);
	double mrg = w * 0.03;

	double w1 = (w - (m+1.0)*mrg) / m;
	double h1 = w1 / ar;

	double x0, y0;
	x0 = mrg + w1 * 0.5;
	y0 = mrg + h1 * 2.0 / 3.0;
	double r1 = h1 * 2.0 / 3.0;
	
	for (int i = 0; i < m; i++)
	{
		pa.save();
		pa.translate(x0 + (mrg + w1) * i, y0);
		
		draw(pa, i, r1);
		pa.restore();
	}

	// r2 * 3 / 2 + 3 * mrg + h1 = h;
	// r2 * 3 / 2 * ar + 2 * mrg = w

	double r2 = qMin((w - 2*mrg)/(2*sn120), (h - h1 - 3*mrg) * (2.0/3.0));

	double h2 = r2 * 3.0 / 2.0;
	double mrgy = (h - mrg - h1 - h2) / 2;

	double y2 = h1 + mrg + mrgy + r2;

	pa.save();
	pa.translate(w / 2, y2);
	draw(pa, 8, r2);
	pa.restore();

	/*
	pa.setPen(Qt::blue);
	pa.drawLine(0, 1, w, 1);
	pa.setPen(Qt::red);
	pa.drawLine(0, mrg, w, mrg);
	pa.drawLine(0, mrg+h1, w, mrg+h1);

	pa.drawLine(0, y2 - r2, w, y2 - r2);
	pa.drawLine(0, y2 - r2 * cs120, w, y2 - r2 * cs120);
	*/

}

void Sierpinski::draw(QPainter& pa, int level, double r)
{
	QPainterPath pp;
	build(pp, level, corners[0] * r, corners[1] * r, corners[2] * r);
	pa.setPen(QPen(Qt::black, 0));
	pa.setBrush(QColor(100, 100, 100));
	pa.drawPath(pp);
}

void Sierpinski::build(QPainterPath& pp, int level, const QPointF& p0, const QPointF& p1, const QPointF& p2)
{
	if (level == 0)
	{
		pp.moveTo(p0);
		pp.lineTo(p1);
		pp.lineTo(p2);
		pp.closeSubpath();
	}
	else
	{
		QPointF p01 = (p0 + p1) / 2;
		QPointF p02 = (p0 + p2) / 2;
		QPointF p12 = (p1 + p2) / 2;
		build(pp, level - 1, p0, p01, p02);
		build(pp, level - 1, p1, p01, p12);
		build(pp, level - 1, p2, p02, p12);
	}
}

// ============================================================================

void Sierpinski2::paint(QPainter& pa, int w, int h)
{
	int nrow = 2, ncol = 3;
	double w1 = w / ncol, h1 = h / nrow;
	double r = qMin(w1 * 0.9 / (2.0 * sn120), h1 * 0.9 * 2.0 / 3.0);

	for (int i = 0; i < ncol * nrow; i++)
	{
		int col = i % ncol;
		int row = i / ncol;
		// qDebug() << col << row;
		pa.save();
		pa.translate(w1 * (col + 0.5), h1 * (row + 2.0/3.0));
		if (i == ncol * nrow - 1) drawLast(pa, 8, r);
		else draw(pa, i, r);
		pa.restore();
	}
}

void Sierpinski2::draw(QPainter& pa, int level, double r)
{
	QPainterPath grid;
	buildGrid(grid, 3, corners[1] * r, corners[0] * r, corners[2] * r);
	pa.setPen(QPen(QColor(120, 120, 120), 0));
	pa.setBrush(Qt::NoBrush);
	pa.drawPath(grid);

	QList<QPointF> cc = { corners[1] * r, corners[0] * r, corners[2] * r };
	QList<QPointF> pts;
	pts.append(cc[0]);
	build(pts, level, cc[0], cc[1], cc[2]);
	

	QPainterPath pp;
	pp.moveTo(pts[0]);
	const qreal t = 0.1;
	for (int i = 1; i + 1 < pts.count(); i++)
	{
		QPointF p0 = pts[i - 1], p1 = pts[i], p2 = pts[i + 1];
		QPointF p01 = t * p0 + (1 - t) * p1;
		QPointF p12 = t * p2 + (1 - t) * p1;
		pp.lineTo(p01);
		pp.quadTo(p1, p12);
	}
	pp.lineTo(pts.back());
	pa.setPen(QPen(Qt::black, 3));
	pa.setBrush(Qt::NoBrush);
	pa.drawPath(pp);
}


void Sierpinski2::drawLast(QPainter& pa, int level, double r)
{
	QList<QPointF> cc = { corners[1] * r, corners[0] * r, corners[2] * r };
	QList<QPointF> pts;
	pts.append(cc[0]);
	build(pts, level, cc[0], cc[1], cc[2]);
	QPainterPath pp;
	pp.moveTo(pts[0]);
	for (int i = 1; i < pts.count(); i++) pp.lineTo(pts[i]);
	pa.setPen(QPen(Qt::black, 0));
	pa.setBrush(Qt::NoBrush);
	pa.drawPath(pp);
}

void Sierpinski2::build(QList<QPointF>& pts, int level, const QPointF& p0, const QPointF& p1, const QPointF& p2)
{
	QPointF p01 = (p0 + p1) / 2;
	QPointF p02 = (p0 + p2) / 2;
	QPointF p12 = (p1 + p2) / 2;

	if (level == 0)
	{
		pts << p01 << p12 << p2;		
	}
	else
	{
		build(pts, level - 1, p0, p02, p01);
		build(pts, level - 1, p01, p1, p12);
		build(pts, level - 1, p12, p02, p2);
	}
}

void Sierpinski2::buildGrid(QPainterPath& pp, int level, const QPointF& p0, const QPointF& p1, const QPointF& p2)
{
	if (level == 0)
	{
		pp.moveTo(p0);
		pp.lineTo(p1);
		pp.lineTo(p2);
		pp.closeSubpath();
	}
	else
	{
		QPointF p01 = (p0 + p1) / 2;
		QPointF p02 = (p0 + p2) / 2;
		QPointF p12 = (p1 + p2) / 2;
		buildGrid(pp, level - 1, p0, p01, p02);
		buildGrid(pp, level - 1, p1, p01, p12);
		buildGrid(pp, level - 1, p2, p02, p12);
	}
}
