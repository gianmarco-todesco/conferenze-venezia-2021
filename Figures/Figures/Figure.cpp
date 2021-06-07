#include "Figure.h"
#include <QImage>
#include <QPainter>

void Figure::save(const QString& fname)
{
	int w=2048, h=1536;

	double s = paperScaleFactor();
	w *= s;
	h *= s;

	QImage image(w, h, QImage::Format_RGB32);
	QPainter pa;
	pa.begin(&image);
	pa.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
	pa.fillRect(0, 0, w, h, Qt::white);
	paint(pa, w, h);
	pa.end();
	image.save("images/" + fname);
}
