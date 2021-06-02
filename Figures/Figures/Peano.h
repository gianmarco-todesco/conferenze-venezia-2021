#pragma once
#include "Figure.h"
#include <QList>
#include <QImage>

class Peano : public Figure
{
	QList<QImage> m_images;
public:
	Peano();
	void buildImages();
	void paint(QPainter& pa, int w, int h) override;
	void makePath(QPainterPath& pp, QPointF p0, QPointF p3, const QTransform &tr, int level, bool first = true);

};

