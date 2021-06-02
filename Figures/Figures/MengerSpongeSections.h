#pragma once
#include "Figure.h"
#include <QImage>
#include <QList>

class MengerSpongeSections : public Figure
{
	QList<QImage> m_images;
public:
	MengerSpongeSections();
	void paint(QPainter& pa, int w, int h) override;

	void buildImages(int level);
};

