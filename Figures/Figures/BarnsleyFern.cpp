#include "BarnsleyFern.h"

#include <QPainter>
#include <QVector>
#include <QTransform>
#include <QPainterPath>
#include <QPainterPath>
#include <QDebug>


class Transform {
public:
	double a,b,c,d,tx,ty;
	double prob;

	Transform(double _a, double _b, double _c, double _d, double _tx, double _ty, double _p)
		: a(_a), b(_b), c(_c), d(_d), tx(_tx), ty(_ty), prob(_p)
	{}

	QPointF operator*(const QPointF& p) const {
		return QPointF(
			a * p.x() + b * p.y() + tx,
			c * p.x() + d * p.y() + ty);
	}
};

const Transform f1( 0.00,  0.00,  0.00, 0.16, 0.00,  0.00,  3);
const Transform f2( 0.85,  0.04, -0.04, 0.85, 0.00, -1.60, 73);
const Transform f3(-0.15,  0.28,  0.26, 0.24, 0.00, -0.44, 13);
const Transform f4( 0.20, -0.26,  0.23, 0.22, 0.00, -1.60, 11);


const QList<QPointF> base{
	QPointF(0.0154321,0.0154321),
	QPointF(-2.40741,-3.45679),
	QPointF(-2.57716,-9.9537),
	QPointF(2.0679,-4.36728)
};

const QRectF bounds(base[2].x(), base[2].y(), base[3].x() - base[2].x(), base[0].y() - base[2].y());


void drawPolygon(QPainter& pa, double unit, const QList<QPointF>& pts, const Transform* f = nullptr)
{
	QPainterPath pp;
	QList<QPointF> pts2;
	for (QPointF p : pts)
	{
		QPointF q = p;
		if (f) q = (*f) * q;
		pts2.append(q * unit);
	}
	pp.moveTo(pts2.back());
	for (QPointF p : pts2) pp.lineTo(p);
	pa.drawPath(pp);

}


void BarnsleyFern::paint(QPainter& pa, int w, int h)
{

	if (m_mode == 1)
	{
		double unit = h * 0.95 / bounds.height();
		m_scale = unit;
		m_center = QPointF(w / 2, h - (h - unit * bounds.height())*0.5);

		pa.save();
		pa.translate(m_center.x(), m_center.y());

		QList<QPointF> pts;
		makePoints(pts, 1'000'000);
		drawSimple(pa, pts, unit);

		// pa.setPen(Qt::black);
		//pa.drawRect(bounds.left() * unit, bounds.top() * unit, bounds.width() * unit, bounds.height() * unit);
		pa.restore();


	}
	else
	{
		QList<QPointF> pts;
		makePoints(pts, 100'000);

		for (int i = 0; i < 4; i++) {
			int col = i % 2;
			int row = i / 2;
			QRectF rect(w * 0.5 * col, h * 0.5 * row, w / 2, h / 2);
			double unit = rect.height() * 0.95 / bounds.height();

			pa.save();
			pa.translate(rect.center().x(), rect.bottom() - (rect.height() - unit * bounds.height()) * 0.5);
			draw(pa, pts, unit, i);
			pa.restore();
		}

		pa.restore();
	}


	//pa.setPen(Qt::black);
	//pa.drawRect(10, 10, w - 20, h - 20);
}


void BarnsleyFern::drawSimple(QPainter& pa, QList<QPointF>& pts, double unit)
{
	QColor green(0, 255, 0, 25);
	for (QPointF pt : pts)
	{
		QPointF p = pt * unit;
		pa.fillRect(p.x(), p.y(), 1, 1, green);
	}

}


void BarnsleyFern::draw(QPainter& pa, QList<QPointF>& pts, double unit, int mode)
{
	QColor green(0, 255, 0, 25);
	QColor gray(220, 220, 220, 100);

	double offset = 100;

	// left image
	pa.save();
	pa.translate(-offset, 0);

	for (QPointF pt : pts)
	{
		QPointF p = pt * unit;
		pa.fillRect(p.x(), p.y(), 1, 1, green);
	}
	pa.setPen(QPen(QColor(100,100,100,100),0));
	drawPolygon(pa, unit, base);

	pa.restore();


	// right image
	const Transform* fs[] = { &f1,&f2,&f3,&f4 };
	const Transform& f = *fs[mode];

	pa.save();
	pa.translate(offset, 0);

	// bg
	for (QPointF pt : pts)
	{
		QPointF p = pt * unit;
		pa.fillRect(p.x(), p.y(), 1, 1, gray);
	}

	// fg
	int mm = pts.count();
	if (mode > 1) mm = mm / 8;
	for(int i=0;i<mm;i++)
	{
		QPointF p = (f*pts[i]) * unit;
		pa.fillRect(p.x(), p.y(), 1, 1, green);
	}

	pa.setPen(QPen(QColor(100, 100, 100), 0));
	drawPolygon(pa, unit, base, &f);
	pa.restore();

	for (int i = 0; i < base.count(); i+=2)
	{
		QPointF p0 = base.at(i) * unit + QPointF(-offset, 0);
		QPointF p1 = (f * base.at(i)) * unit + QPointF(offset, 0);
		pa.setPen(QPen(QColor(50, 50, 50), 0, Qt::DotLine));
		pa.drawLine(p0, p1);
		QPointF e0 = p1 - p0;
		e0 *= (1.0 / sqrt(e0.x() * e0.x() + e0.y() * e0.y()));
		QPointF e1(-e0.y(), e0.x());
		pa.setPen(Qt::black);
		pa.setBrush(Qt::black);
		QPainterPath pp;
		pp.moveTo(p1);
		pp.lineTo(p1 - e0 * 5 + e1 * 2);
		pp.lineTo(p1 - e0 * 5 - e1 * 2);
		pp.closeSubpath();
		pa.drawPath(pp);

	}
}

void BarnsleyFern::makePoints(QList<QPointF>& pts, int n)
{
	srand(1234567);
	pts.reserve(n);
	pts.clear();
	int pp1 = f1.prob, pp2 = pp1 + f2.prob, pp3 = pp2 + f3.prob, pp4 = pp3 + f4.prob;
	Q_ASSERT(pp4 == 100);
	QPointF pt(10, 10);
	for (int i = 0; i < n; i++)
	{
		int prod = rand() % 100;
		if (prod < pp1) pt = f1 * pt;
		else if (prod < pp2) pt = f2 * pt;
		else if (prod < pp3) pt = f3 * pt;
		else pt = f4 * pt;
		pts.append(pt);
	}
}


void BarnsleyFern::click(int x, int y)
{
	QPointF p = (QPointF(x, y) - m_center) * (1.0 / m_scale);
	m_pos = QPointF(p.x(), p.y());
	qDebug() << m_pos;
}