#include "Dragon.h"
#include <QPainterPath>
#include <QPainter>

const double PI = 4.0 * atan(1.0);

void Dragon::makeDragon(QList<QPointF>& pts, int level, QPointF p0, QPointF p1, double sgn)
{
	if (level == 0)
	{
		pts.append(p1);
	}
	else
	{
		QPointF pm = (p0 + p1) * 0.5;
		QPointF d = (p1 - p0) * 0.5;
		QPointF p = pm + QPointF(d.y(), -d.x())*sgn;
		makeDragon(pts, level - 1, p0, p, 1);
		makeDragon(pts, level - 1, p, p1, -1);
	}
}

void Dragon::makeDragon(QPainterPath& pp, int level, QPointF p0, QPointF p1, double t)
{
	QList<QPointF> pts;
	pts.append(p0);
	makeDragon(pts, level, p0, p1, 1.0);
	pp.moveTo(p0);
	if (t > 0.0)
	{
		for (int i = 1; i + 1 < pts.count(); i++)
		{
			QPointF p0 = pts[i - 1], p1 = pts[i], p2 = pts[i + 1];
			QPointF p01 = t * p0 + (1 - t) * p1, p12 = t * p2 + (1 - t) * p1;
			pp.lineTo(p01);
			pp.quadTo(p1, p12);
		}
		pp.lineTo(pts.back());
	}
	else {
		for (int i = 1; i < pts.count(); i++)
		{
			pp.lineTo(pts[i]);
		}
	}
}

// ============================================================================


void Dragon1::draw(QPainter& pa, int level, QPointF p0, QPointF p1)
{
	pa.setBrush(Qt::NoBrush);

	if (level > 0 && level < 8)
	{
		QPainterPath pp;
		makeDragon(pp, level - 1, p0, p1, 0.0);
		QPen pen(QColor(200, 200, 200), 1, Qt::DashLine);
		pa.setPen(pen);
		pa.drawPath(pp);
	}
	QPainterPath pp;
	makeDragon(pp, level, p0, p1, level < 4 ? 0.0 : 0.2);
	if (level < 4)
	{
		QPen pen(Qt::black, 2);
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);
		pa.setPen(pen);
	}
	else
	{
		pa.setPen(Qt::black);
	}

	pa.drawPath(pp);

	pa.setPen(Qt::black);
	pa.setBrush(Qt::black);
	pa.drawEllipse(p0, 2, 2);
	pa.drawEllipse(p1, 2, 2);

}

void Dragon1::paint(QPainter& pa, int w, int h)
{
	double mrg = 100.0;
	int ncol = 4;
	int nrow = 3;

	double d = (w - mrg * (ncol + 1)) / ncol;
	double dx = d + mrg;
	double dy = (h - 2 * mrg) / (nrow - 1);
	double x0 = mrg;
	double y0 = mrg;

	for (int i = 0; i < ncol * nrow; i++)
	{
		int row = i / ncol;
		int col = i % ncol;
		pa.save();
		pa.translate(x0 + dx * col, y0 + dy * row);

		pa.setBrush(Qt::NoBrush);

		QPointF p0(0, 0);
		QPointF p1(d, 0);
		draw(pa, i, p0, p1);

		pa.restore();

	}
}

// ============================================================================

void Dragon2::paint(QPainter& pa, int w, int h)
{
	

	QList<QPointF> pts;
	makeDragon(pts, 18, QPointF(0, 0), QPointF(100.0, 0.0), 1.0);
	double x0, y0, x1, y1;
	x0 = x1 = pts[0].x();
	y0 = y1 = pts[0].y();
	for (int i = 1; i < pts.count(); i++)
	{
		double x = pts[i].x();
		if (x < x0)x0 = x; else if (x > x1) x1 = x;
		double y = pts[i].y();
		if (y < y0)y0 = y; else if (y > y1) y1 = y;
	}

	double mrg = 100.0;
	double sc = qMin((w - mrg * 2) / (x1 - x0), (h - mrg * 2) / (y1 - y0));

	double xcenter = (x0 + x1) / 2;
	double ycenter = (y0 + y1) / 2;


	pa.save();
	pa.translate(w/2 - xcenter * sc, h/2 - ycenter * sc);
	pa.scale(sc, sc);


	QPainterPath pp;
	pp.moveTo(pts.at(0));
	for (int i = 1; i < pts.count(); i++) pp.lineTo(pts.at(i));
	pa.setPen(QPen(Qt::black,0));
	pa.setBrush(Qt::NoBrush);
	pa.drawPath(pp);

	pa.restore();


}


// ============================================================================

Dragon3::Dragon3()
{

}

void Dragon3::paint(QPainter& pa, int w, int h)
{


	QList<QPointF> pts;
	makeDragon(pts, 19, QPointF(0, 0), QPointF(100.0, 0.0), 1.0);
	double x0, y0, x1, y1;
	x0 = x1 = pts[0].x();
	y0 = y1 = pts[0].y();
	for (int i = 1; i < pts.count(); i++)
	{
		double x = pts[i].x();
		if (x < x0)x0 = x; else if (x > x1) x1 = x;
		double y = pts[i].y();
		if (y < y0)y0 = y; else if (y > y1) y1 = y;
	}

	double mrg = 100.0;
	double sc = qMin((w - mrg * 2) / (x1 - x0), (h - mrg * 2) / (y1 - y0));

	double xcenter = (x0 + x1) / 2;
	double ycenter = (y0 + y1) / 2;


	pa.save();
	pa.translate(w / 2 - xcenter * sc, h / 2 - ycenter * sc);
	pa.scale(sc, sc);

	QColor colors[] = {
		QColor(255,0,0),
		QColor(255,127,0),
		QColor(255,255,0),
		QColor(127,255,0),
		QColor(0,255,127),
		QColor(0,255,255),
		QColor(0,127,255),
		QColor(0,0,255)
	};

	int n = pts.count();
	int pp[] = { 0, (int)(n * 0.025), (int)(n * 0.05), (int)(n * 0.1), (int)(n * 0.2), (int)(n * 0.4), (int)(n * 0.8), n };
	for (int j = 0; j < 7; j++)
	{
		int i0 = pp[j], i1 = pp[j+1];
		QPainterPath pp;
		pp.moveTo(pts.at(i0));
		for (int i = i0 + 1; i < i1; i++) pp.lineTo(pts.at(i));
		pa.setPen(QPen(colors[j], 0));
		pa.setBrush(Qt::NoBrush);
		pa.drawPath(pp);

	}


	pa.restore();


}


// ============================================================================

void Dragon4::paint(QPainter& pa, int w, int h)
{
	double d = 400.0;
	QPainterPath pp;
	makeDragon(pp, 17, QPointF(0, 0), QPointF(d, 0.0), 0.0);

	QColor colors[] = {
		QColor(200,200,100),
		QColor(100,200,100),
		QColor(100,200,200),
		QColor(100,180,200),
		QColor(20,50,200)
	};
	pa.save();
	pa.translate(w / 2 - d/2, h / 2);
	

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int t = 0; t < 2; t++)
			{
				pa.save();
				pa.translate(d * 2 * (i - 2) + t * d, d * 2 * (j - 2) + t * d);

				for (int k = 0; k < 4; k++) {
					if ((m_mask >> k) & 1) {
						QColor color = colors[k];
						
						pa.setPen(QPen(color, 0));
						pa.save();
						pa.rotate(k * 90);
						pa.drawPath(pp);
						pa.restore();
					}
				}
				pa.restore();
			}
		}
	}
	pa.setPen(QPen(Qt::black, 0));
	pa.drawPath(pp);


	pa.restore();
}