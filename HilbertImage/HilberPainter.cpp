#include "HilbertPainter.h"
#include "MipMap.h"
#include <QDebug>

HilbertBasePainter::HilbertBasePainter()
	: m_currentLevel(0)
	, m_gridSize(1)
{
	QTransform sc = QTransform::fromScale(0.5, 0.5);
	QTransform tr = QTransform::fromTranslate(-1, -1);
	m_mat1 = sc * tr * QTransform(0, 1, 1, 0, 1, 1);
	m_mat2 = sc * QTransform::fromTranslate(0, 2);
	m_mat3 = sc * QTransform::fromTranslate(2, 2);
	m_mat4 = sc * tr * QTransform(0, -1, -1, 0, 3, 1);
}

void HilbertBasePainter::drawArc(QPainter& pa, double gridSize)
{
	m_currentLevel = 0;
	m_gridSize = gridSize;
	m_pp = QPainterPath();
	drawArc(pa, QTransform(gridSize * 0.25, 0, 0, gridSize * 0.25, 0, 0));
	m_pp.lineTo(m_last);
	pa.drawPath(m_pp);
}


void HilbertBasePainter::drawArc(QPainter& pa, QTransform mat, bool isFirst)
{
	QPointF p0 = mat.map(QPointF(1.0, 1.0) );
	QPointF p1 = mat.map(QPointF(1.0, 3.0) );
	QPointF p2 = mat.map(QPointF(3.0, 3.0) );
	QPointF p3 = mat.map(QPointF(3.0, 1.0) );
	const double t = 0.2;
	QPointF center = mat.map(QPointF(2.0, 2.0));
	double factor = pow(2.0, m_currentLevel) / m_gridSize;
	int ix = (int)floor(center.x() * factor);
	int iy = (int)floor(center.y() * factor);
	if (stop(m_currentLevel, ix,iy))
	{
		QPointF p;
		if (isFirst)
		{
			p = p0;
			m_pp.moveTo(p0);
		}
		else
		{
			p = p0 * (1 - t) + p1 * t;
			m_pp.cubicTo(m_last, p0, p);
		}

		if (m_pp.elementCount() > 100)
		{
			pa.drawPath(m_pp);
			m_pp = QPainterPath();
			m_pp.moveTo(p);
		}
		m_pp.lineTo(p1 * (1 - t) + p0 * t);
		m_pp.quadTo(p1, p1 * (1 - t) + p2 * t);
		m_pp.lineTo(p2 * (1 - t) + p1 * t);
		m_pp.quadTo(p2, p2 * (1 - t) + p3 * t);
		m_pp.lineTo(p3 * (1 - t) + p2 * t);
		m_last = p3;
	}
	else
	{		
		m_currentLevel++;
		drawArc(pa, m_mat1 * mat, isFirst);
		drawArc(pa, m_mat2 * mat, false);
		drawArc(pa, m_mat3 * mat, false);
		drawArc(pa, m_mat4 * mat, false);
		m_currentLevel--;
	}
}




bool HilbertImagePainter::stop(int level, int ix, int iy)
{
	if (level >= m_refScale.count()) return true;
	if (level < 0 || level >= m_mipMap->m_data.count()) return true;
	const QImage& img = m_mipMap->m_images[level];
	if (ix < 0 || ix >= img.width() || iy < 0 || iy >= img.height()) return true;
	//int value = img.pixel(ix, iy)&0xff;
	//return value > m_refScale[level];
	int refValue = m_refScale[level];
	return m_mipMap->m_data[level][iy * img.width() + ix] >= refValue;
}