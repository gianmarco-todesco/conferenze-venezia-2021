#include "MengerSpongeSections.h"
#include <QPainter>
#include <QImage>
#include <memory>

using namespace std;

void compute3Digits(char d3[], int level, double pos)
{
	for (int i = 0; i < level; i++)
	{
		double d = floor(pos * 3.0);
		pos = pos * 3.0 - d;
		d3[i] = (char)d;
	}
}

QImage makeSection(int size, int level, const char pattern[])
{
	using namespace std;
	QImage img(size, size, QImage::Format_ARGB32);
	unique_ptr<char[]> y3 = make_unique<char[]>(level);
	unique_ptr<char[]> x3 = make_unique<char[]>(level);
	double den = 1.0 / ((double)size - 1.0);

	for (int iy = 0; iy < size; iy++)
	{
		double y = (double)iy * den;
		compute3Digits(y3.get(), level, y);
		unsigned char* pixel = img.scanLine(iy);
		for (int ix = 0; ix < size; ix++)
		{
			double x = (double)ix * den;
			compute3Digits(x3.get(), level, x);
			bool inside = true;
			for (int i = 0; i < level; i++)
			{
				int count = pattern[i];
				if (x3[i] == 1) count++;
				if (y3[i] == 1) count++;
				if (count >= 2) { inside = false; break; }
			}
			int v = inside ? 0 : 230;
			pixel[0] = v;
			pixel[1] = v;
			pixel[2] = v;
			pixel[3] = 255;
			pixel += 4;
		}
	}
	return img;
}



void MengerSpongeSections::paint(QPainter& pa, int w, int h)
{
	int m = m_images.count();
	int ncol = 4;
	int nrow = (m+ncol-1) / ncol;
	double size = 0.9 * qMin(w / ncol, h / nrow);
	double mrg = size * 0.1;
	double x0 = (w - ((mrg + size) * ncol - mrg)) / 2;
	double y0 = (h - ((mrg + size) * nrow - mrg)) / 2;

	for (int i = 0; i < m; i++)
	{
		int row = i/ncol;
		int col = i%ncol;
		double x = x0 + (size + mrg) * col;
		double y = y0 + (size + mrg) * row;

		pa.drawImage(QRectF(x,y,size,size), m_images.at(i));
	}
}



MengerSpongeSections::MengerSpongeSections()
{
	buildImages(4);
}


void MengerSpongeSections::buildImages(int level)
{
	for (int i = 0; i < (1 << level); i++)
	{
		unique_ptr<char[]> pattern = make_unique<char[]>(level);
		for (int j = 0; j < level; j++) pattern[j] = (i >> j) & 1;
		QImage img = makeSection(2187, level, pattern.get());
		m_images.append(img);
	}

}
