#pragma once

#include "Figure.h"

#include <QPointF>
#include <QList>
class QPainterPath;


class Dragon : public Figure
{
public:
	
	void makeDragon(QList<QPointF> &pts, int level, QPointF p0, QPointF p1, double sgn);
	void makeDragon(QPainterPath& pp, int level, QPointF p0, QPointF p1, double t);
	// void drawDragon(QPainter& pa, int level, QPointF p0, QPointF p1);
};

class Dragon1 : public Dragon
{
public:
	void paint(QPainter& pa, int w, int h) override;
	void draw(QPainter& pa, int level, QPointF p0, QPointF p1);
};

class Dragon2 : public Dragon
{
public:
	void paint(QPainter& pa, int w, int h) override;
};


class Dragon3 : public Dragon
{
public:
	Dragon3();

	void paint(QPainter& pa, int w, int h) override;
};

class Dragon4 : public Dragon
{
	int m_mask;
public:
	Dragon4(int mask) : m_mask(mask) {};
	void paint(QPainter& pa, int w, int h) override;
};