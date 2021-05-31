#include "Hanoi.h"
#include <QPainter>
#include <cmath>
#include <QPainterPath>

const double PI = 4.0 * atan(1.0);

Hanoi::Hanoi(int diskCount) 
	: m_diskCount(diskCount) 
{
	Q_ASSERT(0 <= diskCount && diskCount <= 4);
	for (int i = 0; i < 3; i++)
	{
		double phi = 2 * PI * i / 3;
		m_cc[i] = QPointF(sin(phi), -cos(phi));
	}
	m_colors = QList<QColor>{
		QColor(200, 100, 200),
		QColor(175, 125, 200),
		QColor(150, 150, 200),
		QColor(125, 200, 200)
	};
}


double Hanoi::getDiskRadius(int k) const
{
	return 8 + 10 * k;
}

QPointF Hanoi::getPolePos(int j) const
{
	const double r = 50.0;
	QPointF p = m_cc[j] * r;
	return p;
}


void Hanoi::drawStatus(QPainter& pa, HanoiStatus& status)
{
	pa.setPen(Qt::NoPen);
	pa.setBrush(Qt::white);
	pa.drawEllipse(QPointF(0, 0), outradius, outradius);

	for (int i = 0; i < 3; i++) {
		// double r = 50.0;
		QPointF p = getPolePos(i);
		for (int j = 0; j < status.poles[i].count(); j++) {
			int k = status.poles[i][j];
			Q_ASSERT(1 <= k && k <= m_diskCount);
			pa.setBrush(m_colors[k - 1]);
			pa.setPen(QColor(50, 50, 50));
			int diskRadius = getDiskRadius(k);
			pa.drawEllipse(p, diskRadius, diskRadius);
		}
		pa.setBrush(Qt::black);
		pa.drawEllipse(p, 5, 5);
	}

	

	pa.setPen(Qt::black);
	pa.setBrush(Qt::NoBrush);
	pa.drawEllipse(QPointF(0, 0), outradius, outradius);
}


void Hanoi::drawMove(QPainter& pa, HanoiMove& move)
{
	drawStatus(pa, move);
	if (move.from < 0 || move.to < 0) return;
	
	
	QPointF p0 = getPolePos(move.from);
	QPointF p2 = getPolePos(move.to);

	QPointF e0 = (p2 - p0);
	e0 = e0 * (1.0 / sqrt(pow(e0.x(), 2) + pow(e0.y(), 2)));
	QPointF e1(-e0.y(), e0.x());

	double diskRadius = getDiskRadius(move.poles[move.to].back());
	p2 -= e0 * (diskRadius + 2.0);
	p0 += e0 * 10.0;

	QPointF p1 = p2 - 15 * e0;

	const double h1 = 5, h2 = 10;

	QPainterPath pp;
	pp.moveTo(p0 + e1 * h1);
	pp.lineTo(p1 + e1 * h1);
	pp.lineTo(p1 + e1 * h2);
	pp.lineTo(p2);
	pp.lineTo(p1 - e1 * h2);
	pp.lineTo(p1 - e1 * h1);
	pp.lineTo(p0 - e1 * h1);

	pp.closeSubpath();
	pa.setPen(QPen(Qt::black, 1));
	pa.setBrush(Qt::white);
	// pa.setBrush(Qt::NoBrush);
	pa.drawPath(pp);

}

HanoiMove Hanoi::getFirstMove() const
{
	HanoiMove move;
	for (int j = 0; j < m_diskCount; j++) move.poles[0].push_back(m_diskCount - j);
	move.from = move.to = -1;
	return move;
}

// ============================================================================


void HanoiPath::buildPath()
{
	m_path.clear();
	m_path.append(getFirstMove());
	int cc[] = { 0,1,2 };
	doBuildPath(m_diskCount - 1, cc);
}

void HanoiPath::addMove(int a, int b)
{
	Q_ASSERT(0 <= a && a < 3 && 0 <= b && b < 3 && a != b);
	Q_ASSERT(!m_path.isEmpty());
	HanoiMove move = m_path.back();
	Q_ASSERT(!move.poles[a].isEmpty());
	Q_ASSERT(move.poles[b].isEmpty() || move.poles[a].back() < move.poles[b].back());
	move.poles[b].append(move.poles[a].back());
	move.poles[a].pop_back();
	move.from = a;
	move.to = b;
	m_path.append(move);
}


void drawArrow(QPainter& pa)
{
	QPainterPath pp;
	double w = 5;
	pp.moveTo(- 20 - w,- 40);
	pp.lineTo( + 20 - w, 0);
	pp.lineTo(- 20 - w, 40);

	pa.setPen(QPen(Qt::white, 15, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
	pa.setBrush(Qt::NoBrush);
	pa.drawPath(pp);
}


void HanoiPath::paint(QPainter& pa, int w, int h)
{
	buildPath();
	double dy = outradius * 2.2;
	double dx = dy * 1.2;

	int m = m_path.count();
	int ncol = (int)ceil(sqrt((double)m));
	int nrow = (m + ncol - 1) / ncol;
	double mrg = outradius;


	double sc = qMin(w / (ncol * dx + 2*mrg), h / (nrow * dy + 2 * mrg));

	double x0 = 0.5 * dx + 0.5 * (w / sc - ncol * dx);
	double y0 = 0.5 * dy + 0.5 * (h / sc - nrow * dy);

	pa.save();
	pa.scale(sc, sc);


	QPainterPath pp;
	double xa = x0 - outradius * 0.5;
	double xb = x0 + (ncol - 1) * dx + outradius * 0.5;

	pp.moveTo(x0, y0);
	for (int i = 0; i+1 < nrow; i++) 
	{
		double y = y0 + dy * i;
		if ((i & 1) == 0)
		{
			pp.lineTo(xb, y0 + i * dy);
			pp.arcTo(QRectF(xb - dy / 2, y, dy, dy), 90, -180);
		}
		else
		{
			pp.lineTo(xa, y0 + i * dy);
			pp.arcTo(QRectF(xa - dy / 2, y, dy, dy), 90, 180);
		}
	}
	int lastRow = nrow - 1;
	int lastCol = (m - 1) % ncol;
	if (lastRow & 1) lastCol = ncol - 1 - lastCol;
	pp.lineTo(x0 + dx * lastCol, y0 + lastRow * dy);

	pa.setPen(QPen(QColor(220, 220, 220), 50));
	pa.setBrush(Qt::NoBrush);
	pa.drawPath(pp);

	for (int i = 0; i < m; i++) {
		int row = i / ncol;
		int col = i % ncol;
		if (row & 1) col = ncol - 1 - col;

		pa.save();
		pa.translate(x0 + dx * col, y0 + dy * row);
		drawMove(pa, m_path[i]);
		pa.restore();

		/*
		if ((i % ncol) > 0)
		{
			double sgn = (row & 1) ? -1 : 1;
			double delta = -0.5 * sgn;

			QPointF p(x0 + dx * (col + delta), y0 + dy * row);

			QPainterPath pp;
			double w = 5;
			pp.moveTo(p.x() - 20*sgn - w, p.y() - 40);
			pp.lineTo(p.x() + 20*sgn - w, p.y());
			pp.lineTo(p.x() - 20*sgn - w, p.y() + 40);

			pa.setPen(QPen(Qt::white, 10, Qt::SolidLine,Qt::SquareCap, Qt::MiterJoin));
			pa.setBrush(Qt::NoBrush);
			pa.drawPath(pp);

		}
		*/

	}

	// draw arrows
	for (int i = 0; i < m; i++) {
		int row = i / ncol;
		int col = i % ncol;
		if (row & 1) col = ncol - 1 - col;
		if ((i % ncol) > 0)
		{
			double sgn = (row & 1) ? -1 : 1;
			double delta = -0.5 * sgn;

			QPointF p(x0 + dx * (col + delta), y0 + dy * row);
			pa.save();
			pa.translate(p);
			if (sgn < 0) pa.rotate(180);
			::drawArrow(pa);
			pa.restore();
		}
		else if (row < nrow - 1)
		{
			double x = (row & 1) == 0 ? xb - dy / 2 : xa - dy / 2;


			QPointF p = ((row & 1) == 0)
				? QPointF(xb - dy / 2 + dy, y0 + dy * row + dy * 0.5)
				: QPointF(xa - dy / 2, y0 + dy * row + dy * 0.5);

			//pa.setPen(Qt::magenta);
			//pa.drawEllipse(p, 5, 5);

			pa.save();
			pa.translate(p + QPointF(0,-3));
			pa.rotate(90);
			::drawArrow(pa);
			pa.restore();

		}

	}




	pa.restore();

}


void HanoiPath::drawArrow(QPainter& pa, double dir)
{

}

void HanoiPath::drawEndlineArrow(QPainter& pa, double dir)
{

}

// ============================================================================

/*
void HanoiShortestPath::paint(QPainter& pa, int w, int h)
{
	buildPath();
	pa.save();
	pa.scale(0.25, 0.25);


	double x0 = outradius * 2.5;
	double y0 = outradius * 2.5;

	double x1 = x0 + 2000;
	double curveRadius = outradius * 2;
	double d = 2 * curveRadius;

	double y = y0;
	QPainterPath pp;
	pp.moveTo(x0, y);
	for (int i = 0; i < 3; i++)
	{
		pp.lineTo(x1, y);
		pp.arcTo(QRectF(x1 - curveRadius, y, d, d), 90, -180);
		y += 2 * curveRadius;
		pp.lineTo(x0, y);
		pp.arcTo(QRectF(x0 - curveRadius, y, d, d), 90, 180);
		y += 2 * curveRadius;
	}



	pa.setPen(QPen(QColor(100, 100, 100), 10));
	pa.setBrush(Qt::NoBrush);
	pa.drawPath(pp);

	double s = outradius * 1.5;

	double dd = outradius * 2.1;
	for (int i = 0; i < m_path.count(); i++) {
		QPointF p = pp.pointAtPercent(pp.percentAtLength(s)); s += outradius * 2.3;
		pa.save();
		pa.translate(p.x(),p.y());
		drawMove(pa, m_path[i]);
		pa.restore();
	}
	pa.restore();
}
*/

void HanoiShortestPath::doBuildPath(int level, int cc[3])
{
	if (level == 0)
	{
		addMove(cc[0], cc[1]);
	}
	else
	{
		int cc_1[3] = { cc[0], cc[2], cc[1] };
		doBuildPath(level - 1, cc_1);
		addMove(cc[0], cc[1]);
		int cc_2[3] = { cc[2], cc[1], cc[0] };
		doBuildPath(level - 1, cc_2);
	}
}


// ============================================================================

void HanoiLongestPath::doBuildPath(int level, int cc[3])
{
	if (level == 0) 
	{
		addMove(cc[0], cc[1]);
		// addMove(cc[2], cc[1]);
	}
	else
	{
		int cc_1[3] = { cc[0], cc[1], cc[2] };
		doBuildPath(level - 1, cc_1);
		addMove(cc[0], cc[2]);
		int cc_2[3] = { cc[1], cc[0], cc[2] };
		doBuildPath(level - 1, cc_2);
		addMove(cc[2], cc[1]);
		int cc_3[3] = { cc[0], cc[1], cc[2] };
		doBuildPath(level - 1, cc_3);
	}
}

// ============================================================================

void HanoiGraph::paint(QPainter& pa, int w, int h)
{
	double r = ((1 << m_diskCount) - 1) * unit;
	double y0 = m_cc[0].y() * r - outradius * 2;
	double y1 = m_cc[1].y() * r + outradius * 2;


	double sc = h/(y1 - y0);
	double ycenter = (y0 + y1) / 2;

	pa.save();
	pa.translate(w / 2, h / 2 - ycenter * sc);
	pa.scale(sc, sc);
	draw(pa, m_diskCount-1);


	pa.setPen(Qt::red);
	pa.drawLine(-w*4, y0, w*4, y0);
	pa.drawLine(-w * 4, y1, w*4, y1);

	pa.restore();
}


void HanoiGraph::draw(QPainter& pa, int level)
{

	int cc[] = { 0,1,2 };
	HanoiStatus status;
	pa.save();
	
	pa.setPen(QPen(QColor(100, 100, 100), 15));
	pa.setBrush(Qt::NoBrush);
	drawLinks(pa, level);

	pa.setPen(QPen(QColor(200, 200, 200), 12));
	pa.setBrush(Qt::NoBrush);
	drawLinks(pa, level);

	draw(pa, level, cc, status);
	pa.restore();
}


void HanoiGraph::draw(QPainter& pa, int level, int cc[3], HanoiStatus status)
{
	if (level == 0) {
		for (int j = 0; j < 3; j++) {
			status.poles[cc[j]].push_back(1);
			pa.save();
			pa.translate(unit * m_cc[j].x(), unit * m_cc[j].y());
			drawStatus(pa, status);
			pa.restore();
			status.poles[cc[j]].pop_back();
		}
	}
	else
	{
		double unit2 = unit * (1 << level);
		for (int j = 0; j < 3; j++) {
			status.poles[cc[j]].push_back(level + 1);
			pa.save();
			pa.translate(unit2 * m_cc[j].x(), unit2 * m_cc[j].y());
			int cc2[3];
			cc2[j] = cc[j];
			cc2[(j + 1) % 3] = cc[(j + 2) % 3];
			cc2[(j + 2) % 3] = cc[(j + 1) % 3];
			draw(pa, level - 1, cc2, status);
			pa.restore();
			status.poles[cc[j]].pop_back();
		}
	}
}


void HanoiGraph::drawLinks(QPainter& pa, int level)
{
	if (level == 0) {
		QPainterPath pp;
		pp.moveTo(unit * m_cc[2].x(), unit * m_cc[2].y());
		for (int j = 0; j < 3; j++)
			pp.lineTo(unit * m_cc[j].x(), unit * m_cc[j].y());
		pa.drawPath(pp);
	}
	else
	{
		double unit2 = unit * (1 << level);
		double unit3 = unit2 - unit;

		for (int j = 0; j < 3; j++)
		{
			pa.save();
			pa.translate(unit2 * m_cc[j].x(), unit2 * m_cc[j].y());
			drawLinks(pa, level - 1);
			pa.restore();

			int j1 = (j + 1) % 3, j2 = (j + 2) % 3;

			pa.drawLine(
				m_cc[j1] * unit2 + m_cc[j2] * unit3,
				m_cc[j2] * unit2 + m_cc[j1] * unit3);

		}
	}

}

