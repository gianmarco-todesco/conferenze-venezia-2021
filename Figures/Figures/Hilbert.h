#pragma once
#include "Figure.h"

#include <QPointF>
#include <QList>
#include <QImage>

struct IMat;
struct Node;

class Hilbert : public Figure
{
	
public:
	void paint(QPainter& pa, int w, int h) override;
	void build(QList<Node>& nodes, int level, const IMat& mat);
	void draw(QPainter& pa, int level, double size);
	void drawGrid(QPainter& pa, int level, double size);
};


class Hilbert2 : public Figure
{
	QImage m_img, m_img2, m_img3;
public:
	Hilbert2();
	void paint(QPainter& pa, int w, int h) override;
	void build(QList<Node>& nodes, int level, int maxLevel, const IMat& mat);
	void draw(QPainter& pa, int level, double size);
	void drawGrid(QPainter& pa, int level, double size);
};



class Hilbert3 : public Figure
{
	int m_level;
public:
	Hilbert3(int level = 0) : m_level(level) {}
	void paint(QPainter& pa, int w, int h) override;
	void foobar();
	double paperScaleFactor() const override { return 4.0; }
};


class Hilbert4 : public Figure
{
	int m_level;
public:
	Hilbert4(int level) : m_level(level) {};
	void paint(QPainter& pa, int w, int h) override;
	double paperScaleFactor() const override { return 4.0; }

};

