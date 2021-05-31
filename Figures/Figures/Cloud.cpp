#include "Cloud.h"
#include <QPainter>
#include <QImage>
#include <QRandomGenerator>
#include <memory>
#include <QPainterPath>
#include <math.h>

const double pi = 4.0 * atan(1.0);

double rnd()
{
	return -1.0 + 2.0*QRandomGenerator::global()->generateDouble();
}

double smooth(double t)
{
	return t * t * (3.0 - 2.0 * t);
}

class FImage {
	std::unique_ptr<double[]> m_buffer;
	int m_bits;
	int m_n, m_n2;
public:
	FImage(int bits) 
		: m_buffer(std::make_unique<double[]>(1<<(2*bits)))
		, m_bits(bits) 
		, m_n(1<<bits)
		, m_n2(1<<(2*bits))
	{
	}
	~FImage() {}

	FImage(const FImage& img) = delete;
	FImage&operator=(const FImage& img) = delete;

	void clear() { for (int i = 0; i < m_n2; i++) m_buffer[i] = 0.0; }

	QImage makeImg() {
		QImage img(m_n, m_n, QImage::Format_ARGB32);
		for (int y = 0; y < m_n; y++)
		{
			uchar* scanline = img.scanLine(y);
			for (int x = 0; x < m_n; x++)
			{
				double v = m_buffer[(size_t )y * (size_t)m_n + (size_t)x];
				v = sin(v * pi) * 0.5 + 0.5;
				// v = 0.5 + 0.5 * v;
				int iv = qMax(0, qMin(255, (int)(255.0 * v)));
				QColor color(iv, 255, 255);
				scanline[2] = color.red();
				scanline[1] = color.green();
				scanline[0] = color.blue();
				scanline[3] = 255;
				scanline += 4;
			}
		}
		return img;
	}

	void build(int cellBits, QRandomGenerator* randomGenerator)
	{
		int m = (1 << (m_bits - cellBits)) + 1;
		int m2 = m * m;
		auto arr = std::make_unique<double[]>(m2);
		for (int i = 0; i < m2; i++) arr[i] = -1.0 + 2.0 * randomGenerator->generateDouble();
		double den = 1.0 / (1 << cellBits);
		int bitMask = (1 << cellBits) - 1;
		for (int y = 0; y < m_n; y++)
		{
			int iy = y >> cellBits;
			double ty = (y & bitMask) * den;
			for (int x = 0; x < m_n; x++)
			{
				int ix = x >> cellBits;
				double tx = (x & bitMask) * den;
				double v00 = arr[m * iy + ix];
				double v01 = arr[m * iy + ix + 1];
				double v10 = arr[m * (iy+1) + ix];
				double v11 = arr[m * (iy+1) + ix + 1];
				double v = (1 - tx) * ((1 - ty) * v00 + ty * v10) + tx * ((1 - ty) * v01 + ty * v11);
				m_buffer[y * m_n + x] = v;
			}
		}
	}

	void sum(const FImage& other, double factor) {
		Q_ASSERT(m_n == other.m_n);
		for (int i = 0; i < m_n2; i++) m_buffer[i] += other.m_buffer[i] * factor;
	}
};

void drawGrid(QPainter& pa, const QRectF& rect, int level)
{
	QColor colors[] = {
		QColor::fromHsl(0,0,0),
		QColor::fromHsl(0,0,32),
		QColor::fromHsl(0,0,64),
		QColor::fromHsl(0,0,96),
		QColor::fromHsl(0,0,128)
	};
	for (int i = qMin(5, level); i >= 0; i--)
	{
		int m = (1 << i);
		pa.setPen(QPen(colors[i], 0.5*(6 - i)));
		
		for (int j = 1; j < m; j++)
		{
			double t = (double)j / (double)m;
			double x = (1 - t) * rect.left() + t * rect.right();
			pa.drawLine(x, rect.top(), x, rect.bottom());
			double y = (1 - t) * rect.top() + t * rect.bottom();
			pa.drawLine(rect.left(),y, rect.right(), y);

		}

	}
	pa.setPen(QPen(Qt::black, 3));
	pa.setBrush(Qt::NoBrush);
	pa.drawRect(rect);
}


void Cloud::paint(QPainter& pa, int w, int h)
{

	std::vector<std::unique_ptr<FImage> > images;
	int bits = 10;
	FImage img(bits);
	for (int i = 0; i < 5; i++) images.push_back(std::make_unique<FImage>(bits));

	QRandomGenerator randomGenerator(0x12347);
	for (int i = 0; i < 5; i++)
	{
		images[i]->build(10-i, &randomGenerator);
	}

	img.clear();
	QImage qimg;

	double sz = h * 0.7;
	double sz2 = h * 0.2;

	double y0 = (h - sz - sz2) / 3;
	double y1 = y0*2 + sz2;
	
	int m = 5;
	double mrg = (w - sz2 * m) / (m + 1);

	double factor = 1.0;
	for (int i = 0; i < m; i++)
	{
		img.sum(*images[i], factor);
		factor *= 0.5;
		QRectF imgRect(mrg + i * (sz2+mrg), y0, sz2, sz2);
		qimg = img.makeImg();
		pa.drawImage(imgRect, qimg);
		drawGrid(pa, imgRect, i);
	}

	FImage img2(bits);
	for (int i = 5; i < 10; i++)
	{
		img2.build(10-i, &randomGenerator);
		img.sum(img2, factor);
		factor *= 0.5;
	}

	qimg = img.makeImg();
	QRectF rect((w - sz) * 0.5, y1, sz, sz);
	pa.drawImage(rect, qimg);
	pa.setPen(QPen(Qt::black, 0));
	pa.setBrush(Qt::NoBrush);
	pa.drawRect(rect);


}