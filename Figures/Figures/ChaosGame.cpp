#include "ChaosGame.h"
#include <QPainter>
#include <QPainterPath>
#include <QList>
#include <QPointF>

const double PI = 4.0 * atan(1.0);


void ChaosGame::paint(QPainter& pa, int w, int h)
{

	Params params;
	params.w = w;
	params.h = h;
	if (m_mode == 0)
	{
		params.pointsCount = 10'000;
		params.vCount = 3;
	}
	else
	{
		params.pointsCount = 5'000'000;
		params.vCount = 5;
	}
	paint(pa, params);
}

void ChaosGame::paint(QPainter& pa, const Params& params)
{
	srand(123457);
	double wy = 1.0 - cos(PI * 2 * (params.vCount / 2) / params.vCount);
	double x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	QList<QPointF> pts;
	for (int i = 0; i < params.vCount; i++)
	{
		double phi = 2 * PI * i / params.vCount;
		QPointF p(-sin(phi), -cos(phi));
		pts.append(p);
		double x = p.x(), y = p.y();
		if (x < x0)x0 = x; else if (x > x1)x1 = x;
		if (y < y0)y0 = y; else if (y > y1)y1 = y;
	}
	double mrg = 0.1 * qMin(params.w, params.h);
	double sc = qMin((params.w - 2 * mrg) / (x1 - x0), (params.h - 2 * mrg) / (y1 - y0));
	double r = sc;
	double yCenter = (params.h - sc * (y1 - y0)) / 2 + r;
	for (int i = 0; i < pts.count(); i++) pts[i] *= r;


	pa.save();
	pa.translate(params.w / 2, yCenter);

	QPainterPath pp;
	pp.moveTo(pts.back());
	for (QPointF p : pts) pp.lineTo(p);
	pa.setPen(Qt::NoPen); //  QPen(QColor(100, 100, 100), 5));
	pa.setBrush(QColor(250, 250, 250)); //  Qt::NoBrush);
	pa.drawPath(pp);
	
	QPointF p = QPointF(0, 0);
	int m = params.pointsCount; 
	QColor color;
	if (m_mode == 0) color = QColor(0, 0, 0, 50);
	else color = QColor(0, 0, 0, 10);

	for (int i = 0; i < m; i++)
	{
		int j = rand() % params.vCount;
		p = (p + pts[j]) * 0.5;
		if (i > 10) pa.fillRect(p.x(), p.y(), 1, 1, color);
	}
	

	pa.setPen(Qt::black);
	pa.setBrush(Qt::black);
	for (QPointF p : pts) pa.drawEllipse(p, 3, 3);


	// draw path
	if (m_mode == 0)
	{
		p = QPointF();
		QList<QPointF> path;
		path.append(p);
		for (int i = 0; i < 10; i++)
		{
			int j = rand() % params.vCount;
			p = (p + pts[j]) * 0.5;
			path.append(p);
		}

		pa.setPen(Qt::NoPen);
		pa.setBrush(QColor(100, 200, 250));
		for (QPointF p : path) pa.drawEllipse(p, 2, 2);
		pa.setPen(QPen(QColor(100, 200, 250), 0));
		pa.setBrush(Qt::NoBrush);
		for (int i = 0; i + 1 < path.count(); i++)
		{
			QPointF p0 = path[i], p1 = path[i + 1];
			QPointF e0 = p1 - p0;
			e0 *= 1.0 / sqrt(e0.x() * e0.x() + e0.y() * e0.y());
			QPointF e1(-e0.y(), e0.x());
			p0 += 4 * e0;
			p1 -= 4 * e0;
			pa.drawLine(p0, p1);
			pa.drawLine(p1, p1 - e0 * 10 + e1 * 5);
			pa.drawLine(p1, p1 - e0 * 10 - e1 * 5);
		}

	}




	pa.restore();




}


