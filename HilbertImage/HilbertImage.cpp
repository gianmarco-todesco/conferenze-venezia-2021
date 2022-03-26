#include "HilbertImage.h"
#include "HilbertPainter.h"
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>
#include <QDebug>

// ho comprato (per 11$) l'immagine originale il 26marzo2022 su https://www.alamy.com/
// https://www.alamy.com/search/imageresults.aspx?imgt=0&qt=PB8F35

HilbertImage::HilbertImage(QWidget *parent)
    : QWidget(parent)
	, m_level(0)
	, m_target(QImage("data\\venice-4096.png"))
	, m_gridSize(512)
	, m_cellSize(32)
{
	build();
	save();
}

void HilbertImage::paintEvent(QPaintEvent*)
{
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing);
    pa.fillRect(0, 0, width(), height(), Qt::white);
    draw(pa, width(), height());
}

void HilbertImage::draw(QPainter& pa, int w, int h) const
{
	draw_1(pa, w, h);
}

void HilbertImage::draw_1(QPainter& pa, int w, int h) const
{
	/*
	int n = 8;
	double unit = (qMin(w, h) - 100) / n;
	double gridSize = unit * n;
	*/
	double gridSize = m_gridSize;

	pa.save();
	pa.translate((w - gridSize) / 2, (h - gridSize) / 2);

	pa.setPen(Qt::black);
	HilbertImagePainter hp(&m_target, m_refScale);
	hp.drawArc(pa, gridSize);

	// drawGrid(pa, unit, n);

	pa.restore();
}

void HilbertImage::draw_2(QPainter& pa, int w, int h) const
{
	int n = 8;
	double unit = (qMin(w, h) - 100) / n;
	double gridSize = unit * n;
	pa.save();
	pa.translate((w - gridSize) / 2, (h - gridSize) / 2);


	pa.drawImage(QRectF(0, 0, gridSize, gridSize), m_target.m_images[m_level]);
	drawGrid(pa, unit, n);
	/*
	pa.setPen(Qt::black);
	QPainterPath pp;
	HilberPainter hp;
	hp.buildArc(pp, gridSize, m_level);
	pa.drawPath(pp);
	*/
	pa.restore();
}

void HilbertImage::drawGrid(QPainter& pa, double unit, int n) const
{
	pa.setPen(QPen(QColor(220, 220, 220), 0));
	pa.setBrush(Qt::NoBrush);
	QPainterPath grid;
	double limit = unit * n;
	for (int i = 0; i <= n; i++)
	{
		grid.moveTo(0, unit * i);
		grid.lineTo(limit, unit * i);
		grid.moveTo(unit * i, 0);
		grid.lineTo(unit * i, limit);
	}
	pa.drawPath(grid);
}


void HilbertImage::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_A) m_level++;
	else if(e->key() == Qt::Key_S && m_level>0) m_level--;
	update();
}

void HilbertImage::build()
{
	m_refScale.clear();
	double gridSize = m_gridSize * m_cellSize;
	for (int i = 0; gridSize >= m_cellSize; i++)
	{
		int sz = (int)(0.5+gridSize);
		QImage img(sz, sz, QImage::Format_RGB32);
		QPainter pa;
		pa.begin(&img);
		pa.setRenderHints(QPainter::Antialiasing);
		pa.fillRect(0, 0, sz, sz, Qt::white);
		pa.setPen(QPen(Qt::black, 0.15 * m_cellSize));
		HilbertSimplePainter hp(1);
		hp.drawArc(pa, sz);
		pa.end();
		img.save(QString("data\\H-%1.png").arg(sz));
		QImage one = img.scaled(1, 1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		int gray = one.pixel(0, 0) & 0xff;
		m_refScale.append(gray);
		gridSize *= 0.5;
	}
	/*
	for (int i = 4; i <= 10; i++)
	{
		for (int j = 0; j < i; j++) 
		{
			int sz = 1 << i;
			QImage img(sz, sz, QImage::Format_RGB32);
			QPainter pa;
			pa.begin(&img);
			pa.setRenderHints(QPainter::Antialiasing);
			pa.fillRect(0, 0, sz, sz, Qt::white);
			pa.setPen(Qt::black);
			QPainterPath pp;
			HilbertSimplePainter hp(j);
			hp.buildArc(pp, sz);
			pa.drawPath(pp);
			pa.end();
			img.save(QString("data\\H-%1-%2.png").arg(i).arg(j));
		}
	}
	*/
	
	
	qDebug() << m_refScale;
	
	/*
	int v0 = m_refScale.first();
	int v1 = m_refScale.back();
	int v2 = 255;
	for (int x : m_target.m_data.back()) if (x < v2)v2 = x;

	qDebug() << v1 << v2;
	v2 = 30; //  (v1 + v2) / 2;
	for (int i = 0; i < m_refScale.count(); i++) 
	{
		int v = m_refScale[i];
		v = v0 - (v0 - v) * (v0 - v2) / (v0 - v1);
		m_refScale[i] = v;
	}
	qDebug() << m_refScale;
	*/
	
}

void HilbertImage::save()
{
	int gridSize = m_gridSize;
	int factor = m_cellSize;
	QImage img(gridSize * factor, gridSize * factor, QImage::Format_Grayscale8);
	QPainter pa;
	pa.begin(&img);
	pa.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
	pa.fillRect(0, 0, img.width(), img.height(), Qt::white);
	pa.save();
	pa.scale(factor, factor);
	HilbertImagePainter hp(&m_target, m_refScale);
	pa.setPen(QPen(Qt::black,0.15));
	pa.setBrush(Qt::NoBrush);
	hp.drawArc(pa, gridSize);
	pa.restore();
	pa.end();
	img.save("data\\output.png");
}
