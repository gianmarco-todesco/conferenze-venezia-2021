#include "Peano.h"

#include <QPainter>
#include <QPainterPath>
#include <QList>

Peano::Peano()
{
	buildImages();
}


void Peano::buildImages()
{
	QImage img;
	img.load("data/michele_243.png");
	Q_ASSERT(img.width() == 243);
	Q_ASSERT(img.height() == 243);

	QImage q(243, 243, QImage::Format_Grayscale8);
	for (int i = 0; i < q.height(); i++)
	{
		for (int j = 0; j < q.width(); j++) q.setPixelColor(i, j, img.pixelColor(i, j));
	}

	m_images.append(q);

	for (int k = 0; k < 4; k++)
	{
		int sz = qRound(243.0 / pow(3.0, k + 1));
		QImage q2(243, 243, QImage::Format_Grayscale8);
		for (int i = 0; i < sz; i++)
		{
			for (int j = 0; j < sz; j++)
			{
				int v = q.pixelColor(i * 3, j * 3).value();
				for (int di = 0; di < 3; di++)
					for (int dj = 0; dj < 3; dj++)
						v = qMin(v, q.pixelColor(i * 3 + di, j * 3 + dj).value());
				q2.setPixelColor(i, j, QColor(v, v, v));
			}
		}
		m_images.append(q2);
		q = q2;
	}

}

void Peano::makePath(QPainterPath&pp, QPointF p0, QPointF p3, const QTransform& tr, int level, bool first)
{
	/*
	if (level == 1)
	{
		QPointF p = (p0 + p3) * 0.5;
		int ix = qRound(p.x()/9.0);
		int iy = qRound(p.y()/9.0);
		int k = 2;
		if (0 <= ix && ix < m_images[k].width() && 0 <= iy && iy < m_images[k].height())
		{
			int value = m_images[k].pixelColor(ix, iy).value();
			if (value >120) level = 0;
		}

	}
	*/

	if (0<level && level-1<m_images.count())
	{
		QPointF p = (p0 + p3) * 0.5;
		double unit = 1.0/pow(3, level - 1);
		int ix = qRound(p.x() * unit);
		int iy = qRound(p.y() * unit);
		int k = level - 1;
		int thresholds[] = { 70,110,150,190,240,240 };
		if (0 <= ix && ix < m_images[k].width() && 0 <= iy && iy < m_images[k].height())
		{
			int value = m_images[k].pixelColor(ix, iy).value();
			if (value > thresholds[k]) level = 0;
		}
	}




	if (level == 0)
	{
		double t = 0.3;
		QPointF pa = (1 - t) * p0 + t * p3;
		QPointF pb = (1 - t) * p3 + t * p0;

		if (first) pp.moveTo(tr.map(p0));
		else pp.quadTo(tr.map(p0), tr.map(pa));
		pp.lineTo(tr.map(pb));
	}
	else
	{
		double t = 1.0 / 3.0;
		double x0 = p0.x(), y0 = p0.y();
		double x3 = p3.x(), y3 = p3.y();
		double x1 = (1 - t) * p0.x() + t * p3.x();
		double x2 = (1 - t) * p3.x() + t * p0.x();
		double y1 = (1 - t) * p0.y() + t * p3.y();
		double y2 = (1 - t) * p3.y() + t * p0.y();
		QPointF pts[] = {
			p0, QPointF(x1, y1), QPointF(x0, y2),
			QPointF(x1,y3), QPointF(x2,y2),QPointF(x1,y1),
			QPointF(x2,y0), QPointF(x3,y1),QPointF(x2,y2),
			p3
		};
		for(int i=0; i<9; i++) 
			makePath(pp, pts[i], pts[i+1], tr, level - 1, i==0 && first);
	}

}

void Peano::paint(QPainter& pa, int w, int h)
{
	double size = qMin(w, h) * 0.9;
	QRectF rect((w - size) / 2, (h - size) / 2, size, size);
	QPainterPath pp;
	QTransform tr;
	double sc = size / 243.0;
	tr.translate((w - size) / 2, (h - size) / 2);
	tr.scale(sc, sc);
	makePath(pp, QPointF(0,0), QPointF(243.0,243.0), tr, 6);
	pa.setPen(QPen(Qt::black, 3));
	pa.setBrush(Qt::NoBrush);
	pa.drawPath(pp);
}
