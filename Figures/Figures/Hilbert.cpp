#include "Hilbert.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>


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



//=============================================================================

Hilbert2::Hilbert2()
{
	m_img.load("data/michele128.png");
	m_img2 = m_img.scaled(m_img.width() / 2, m_img.height() / 2);
	m_img3 = m_img.scaled(m_img.width() / 4, m_img.height() / 4);
	qDebug() << m_img.size();
}

void Hilbert2::paint(QPainter& pa, int w, int h)
{
	double size = qMin(w, h) * 0.9;
	pa.save();
	pa.translate(w/2,h/2);
	draw(pa, 9, size);
	pa.restore();
}


void Hilbert2::build(QList<Node>& nodes, int level, int maxLevel, const IMat& mat)
{
	if (level == 1)
	{
		int off = (2 << maxLevel) - 4;
		QPoint p = mat * QPoint(0, 0);
		int ix = (p.x() + off);
		int iy = (p.y() + off);
		Q_ASSERT(ix >= 0);
		Q_ASSERT(iy >= 0);
		Q_ASSERT((ix % 8) == 0);
		Q_ASSERT((iy % 8) == 0);
		Q_ASSERT(ix <= 2 * off);
		Q_ASSERT(iy <= 2 * off);
		Q_ASSERT(ix / 8 <= (1 << (maxLevel - 1)) - 1);
		Q_ASSERT(iy / 8 <= (1 << (maxLevel - 1)) - 1);

		int max = 1 << (maxLevel - 1);
		Q_ASSERT(max == 256);
		Q_ASSERT(m_img.width() == 256);
		Q_ASSERT(m_img.height() == 256);
		ix /= 8;
		iy /= 8;

		Q_ASSERT(0 <= ix && ix < max);
		Q_ASSERT(0 <= iy && iy < max);
		if (m_img.pixelColor(QPoint(ix, iy)).value() > 127) level = 0;
	}
	else if (level == 2)
	{
		int off = (2 << maxLevel) - 8;
		QPoint p = mat * QPoint(0, 0);
		int ix = (p.x() + off);
		int iy = (p.y() + off);
		Q_ASSERT(ix >= 0);
		Q_ASSERT(iy >= 0);
		Q_ASSERT((ix % 16) == 0);
		Q_ASSERT((iy % 16) == 0);
		Q_ASSERT(ix <= 2 * off);
		Q_ASSERT(iy <= 2 * off);
		Q_ASSERT(ix / 16 <= (1 << (maxLevel - 2)) - 1);
		Q_ASSERT(iy / 16 <= (1 << (maxLevel - 2)) - 1);

		int max = 1 << (maxLevel - 2);
		Q_ASSERT(max == 128);
		Q_ASSERT(m_img2.width() == max);
		Q_ASSERT(m_img2.height() == max);
		ix /= 16;
		iy /= 16;
		Q_ASSERT(0 <= ix && ix < max);
		Q_ASSERT(0 <= iy && iy < max);

		if (m_img2.pixelColor(QPoint(ix, iy)).value() > 200) level = 0;
	}
	else if (level == 3)
	{
		int off = (2 << maxLevel) - 16;
		QPoint p = mat * QPoint(0, 0);
		int ix = (p.x() + off);
		int iy = (p.y() + off);
		Q_ASSERT(ix >= 0);
		Q_ASSERT(iy >= 0);
		Q_ASSERT((ix % 32) == 0);
		Q_ASSERT((iy % 32) == 0);
		Q_ASSERT(ix <= 2 * off);
		Q_ASSERT(iy <= 2 * off);
		Q_ASSERT(ix / 32 <= (1 << (maxLevel - 3)) - 1);
		Q_ASSERT(iy / 32 <= (1 << (maxLevel - 3)) - 1);

		int max = 1 << (maxLevel - 3);
		Q_ASSERT(max == 64);
		Q_ASSERT(m_img3.width() == max);
		Q_ASSERT(m_img3.height() == max);
		ix /= 32;
		iy /= 32;
		Q_ASSERT(0 <= ix && ix < max);
		Q_ASSERT(0 <= iy && iy < max);

		if (m_img3.pixelColor(QPoint(ix, iy)).value() > 240) level = 0;

	}

	if (level == 0)
	{
		Node node;
		node.pts[0] = mat * QPointF(-1.0, 1.0);
		node.pts[1] = mat * QPointF(-1.0, -1.0);
		node.pts[2] = mat * QPointF(1.0, -1.0);
		node.pts[3] = mat * QPointF(1.0, 1.0);
		nodes.push_back(node);
	}
	else
	{
		double unit = 1 << (level);
		build(nodes, level - 1, maxLevel, mat * IMat(0, -1, -1, 0, -unit, unit));
		build(nodes, level - 1, maxLevel, mat * IMat(1, 0, 0, 1, -unit, -unit));
		build(nodes, level - 1, maxLevel, mat * IMat(1, 0, 0, 1, unit, -unit));
		build(nodes, level - 1, maxLevel, mat * IMat(0, 1, 1, 0, unit, unit));


	}
}

void Hilbert2::draw(QPainter& pa, int level, double size)
{
	// drawGrid(pa, level, size);
	QList<Node> nodes;
	build(nodes, level, level, IMat());

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
	pa.setPen(QPen(Qt::black, 0));
	pa.drawPath(pp);
}

void Hilbert2::drawGrid(QPainter& pa, int level, double size)
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
		pp.lineTo(limit, unit * i);
		pp.moveTo(unit * i, -limit);
		pp.lineTo(unit * i, limit);
	}
	pa.drawPath(pp);
}

// ============================================================================

class MipMap {

public:
	QList<QImage> m_images;

	// MipMap() { load("data/abate512.png");  }
	MipMap() { load("data/venezia512.png"); }
	void load(const QString& filename);
};

void MipMap::load(const QString& filename)
{
	QImage img(filename);
	Q_ASSERT(img.width() == 512);
	Q_ASSERT(img.height() == 512);
	// img.convertTo(QImage::Format_Grayscale8);
	m_images.append(img);
	int sz = img.width();
	for (;;)
	{
		sz = sz / 2;
		if (sz == 0) break;
		QImage child = img.scaled(sz, sz, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		Q_ASSERT(child.width() == sz);
		Q_ASSERT(child.height() == sz);
		//Q_ASSERT(child.format() == QImage::Format_Grayscale8);
		m_images.insert(0,child);
		// child.save(QString("data/img%1.png").arg(sz));
	}
}


class HilberPainter {
public:
	QPainter& pa;
	int maxLevel;
	double unit;
	bool first;
	QPointF lastPoint;
	QPainterPath pp;
	MipMap *mipMap;



	HilberPainter(QPainter& _pa, int _level, double _unit, MipMap *_mipMap = nullptr)
		: pa(_pa), maxLevel(_level), unit(_unit), first(true), mipMap(_mipMap) {}

	void paint();
	void buildArc(int level, IMat mat);

};

void HilberPainter::paint()
{
	double page = unit * (2 << maxLevel);

	pa.save();
	// pa.translate(0, -page);
	buildArc(maxLevel, IMat(1, 0, 0, 1, 0, 0));	
	pa.restore();

	pa.setPen(QPen(Qt::black, 0));
	pa.drawPath(pp);
}

void HilberPainter::buildArc(int level, IMat mat)
{

	int ix = mat.tx, iy = mat.ty;

	int dlevel = maxLevel - level;

	pa.setPen(QPen(Qt::red, 0));
	double r = (double)(2 + dlevel * 3);
	// pa.drawEllipse(QPointF(unit * ix, unit * iy), r, r);

	int jx, jy;

	if (level == maxLevel)
	{
		Q_ASSERT(ix == 0);
		Q_ASSERT(iy == 0);
		jx = jy = 0;
	}
	else
	{
		int m = 1 << dlevel;
		int halfStep = 2<<level;

		int limit = halfStep * (m - 1);

		jx = ix + limit;
		jy = iy + limit;
		Q_ASSERT(0 <= jx && jx <= 2 * limit);
		Q_ASSERT(0 <= jy && jx <= 2 * limit);


		int mask = (1 << (level+2)) - 1;
		Q_ASSERT((jx & mask) == 0);
		Q_ASSERT((jy & mask) == 0);

		jx >>= level + 2;
		jy >>= level + 2;

		//pa.setPen(QPen(Qt::blue, 0));
		//pa.drawEllipse(QPointF(unit * jx, unit * jy), r, r);
	}

	// if (level == maxLevel - 1 && jx == 0 && jy == 0) level = 0;

	bool drawIt = level == 0;

	if (mipMap && dlevel > 2 && level<mipMap->m_images.count())
	{
		int threshold[] = { 49,95,153,179,191,198,202,203,255,255 };
		int value = mipMap->m_images.at(maxLevel - level).pixelColor(jx, jy).value();
		if (value > threshold[level]) drawIt = true;
	}



	if (drawIt)
	{
		int delta = (int)(1 << level);
		QPointF p0 = (mat * QPointF(-delta, delta)) * unit;
		QPointF p1 = (mat * QPointF(-delta, -delta)) * unit;
		QPointF p2 = (mat * QPointF(delta, -delta)) * unit;
		QPointF p3 = (mat * QPointF(delta, delta)) * unit;

		const double t = 0.2;

		if (first) pp.moveTo(p0);
		else 
		{
			pp.cubicTo(lastPoint, p0, p0* (1 - t)  + p1*t);
		}
		first = false;
		pp.lineTo(p1 * (1 - t) + p0 * t);
		pp.quadTo(p1, p1 * (1 - t) + p2 * t);
		pp.lineTo(p2 * (1 - t) + p1 * t);
		pp.quadTo(p2, p2 * (1 - t) + p3 * t);
		pp.lineTo(p3 * (1 - t) + p2 * t);
		lastPoint = p3;
	}
	else
	{
		int delta = (int)(1 << level);
		buildArc(level - 1, mat * IMat(0, -1, -1, 0, -delta, delta));
		buildArc(level - 1, mat * IMat(1, 0, 0, 1, -delta, -delta));
		buildArc(level - 1, mat * IMat(1, 0, 0, 1, delta, -delta));
		buildArc(level - 1, mat * IMat(0, 1, 1, 0, delta, delta));

	}
}


QImage makeHilbertSample(int imgLevel, int hLevel)
{
	int sz = 1 << imgLevel;
	QImage img(sz, sz, QImage::Format_ARGB32);
	QPainter pa;
	pa.begin(&img);
	pa.save();
	pa.translate(sz / 2, sz / 2);

	int m = 2 << hLevel; // metà del numero di quadretti
	double unit = 0.25 * (1<<(imgLevel-hLevel)); // dimensioni di un quadretto

	HilberPainter hpa(pa, hLevel, unit);
	hpa.paint();

	pa.restore();
	pa.end();
	return img;
}

void Hilbert3::foobar()
{
	for (int i = 1; i<10; i++)
	{
		QImage img = makeHilbertSample(10, i);
		img.save(QString("data/h-%1.png").arg(i));
		int value = img.scaled(1, 1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).pixelColor(0, 0).value();
		qDebug() << value;
	}
}


void Hilbert3::paint(QPainter& pa, int w, int h)
{
	pa.save();
	pa.translate(w / 2, h / 2);


	int level = m_level;
	int m = 2 << level; // metà del numero di quadretti
	double unit = 0.499 * qMin(w / m, h / m); // dimensioni di un quadretto
	double limit = unit * m ;

	/*
	pa.setPen(QPen(QColor(220, 220, 220), 0));
	pa.setBrush(Qt::NoBrush);
	QPainterPath grid;
	for (int i = -m; i <= m; i++)
	{
		grid.moveTo(-limit, unit * i);
		grid.lineTo(limit, unit * i);
		grid.moveTo(unit * i, -limit);
		grid.lineTo(unit * i, limit);
	}
	pa.drawPath(grid);
	*/

	MipMap mipMap;
	HilberPainter hpa(pa, level, unit, &mipMap);
	hpa.paint();


	pa.restore();
}
