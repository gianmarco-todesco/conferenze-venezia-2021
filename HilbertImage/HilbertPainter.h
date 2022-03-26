#pragma once

#include <QPointF>
#include <QPainterPath>
#include <QPainter>
#include <QTransform>

class QPainter;
class MipMap;

class HilbertBasePainter {
	QPointF m_last;
	QTransform m_mat1, m_mat2, m_mat3, m_mat4;
	double m_gridSize;
	int m_currentLevel;
	QPainterPath m_pp;
public:
	HilbertBasePainter();
	void drawArc(QPainter& pa, double gridSize);

protected:
	virtual bool stop(int level, int ix, int iy) = 0;

private:
	void drawArc(QPainter& pa, QTransform mat, bool isFirst = true);

};


class HilbertSimplePainter : public HilbertBasePainter {
	int m_level;
public:
	HilbertSimplePainter(int level) : m_level(level) {};
protected:
	bool stop(int level, int ix, int iy) override {
		return level >= m_level;
	}
};


class HilbertCirclePainter : public HilbertBasePainter {
	
public:
	HilbertCirclePainter() {}
protected:
	bool stop(int level, int ix, int iy) override {
		int sz = 1 << level;
		double r = 0.5 * (sz - 1);
		double xx = ix - 0.5 * (sz - 1), yy = iy - 0.5 * (sz - 1);
		return level > 8 || xx * xx + yy * yy < r* r;
	}
};


class HilbertImagePainter : public HilbertBasePainter {
	const MipMap* m_mipMap;
	QList<int> m_refScale;
public:
	HilbertImagePainter(const MipMap*mipMap, QList<int> refScale) : m_mipMap(mipMap), m_refScale(refScale) {}
protected:
	bool stop(int level, int ix, int iy) override;
};

