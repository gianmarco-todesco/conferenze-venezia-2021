#include "Viewer.h"
#include <QPainter>
#include "Hanoi.h"
#include "Dragon.h"
#include "Hilbert.h"
#include "Sierpinski.h"
#include "ChaosGame.h"
#include "BarnsleyFern.h"
#include "Cloud.h"


#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

Viewer::Viewer(QWidget *parent)
	: QWidget(parent)
{
	// HanoiShortestPath(4).save("fig1.png");
	// HanoiLongestPath(4).save("fig2.png");
	// HanoiGraph(4).save("fig13.png");
	// Dragon1().save("fig6a.png");
	// Dragon2().save("fig6b.png");
	// Dragon4(15).save("fig8.png");
	// Hilbert().save("fig9.png");
	// Sierpinski().save("fig11.png");
	// Sierpinski2().save("fig12.png");
	// ChaosGame(0).save("fig14.png");
	// ChaosGame(1).save("fig15.png");
	// BarnsleyFern(1).save("fig16a.png");
	// BarnsleyFern(2).save("fig16b.png");
}

Viewer::~Viewer()
{
}

// A = n*0.4
// B = n*0.8
int A = (1<<16)*0.4, B = (1<<16)*0.8;
int imask = 15;
int level = 0;

BarnsleyFern fern(2);

void Viewer::paintEvent(QPaintEvent*)
{
	QPainter pa(this);
	pa.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

	int w = width(), h = height();
	// pa.translate(width() / 2, height() / 2);
	pa.fillRect(0, 0, w, h, Qt::white);


	// HanoiGraph(3).paint(pa, w, h);
	// HanoiShortestPath(4).paint(pa, w, h);
	// HanoiLongestPath(4).paint(pa, w, h);
	// Dragon4(imask).paint(pa, w, h);

	// Hilbert().paint(pa, w, h);
	// Sierpinski().paint(pa, w, h);
	// Sierpinski2().paint(pa, w, h);

	// ChaosGame(1).paint(pa, w, h);
	// fern.paint(pa, w, h);
	Cloud().paint(pa, w, h);
}

void Viewer::keyPressEvent(QKeyEvent*e)
{
	if (e->key() == Qt::Key_A)level++; 
	else if (e->key() == Qt::Key_S) { if (level > 0) level--; }
	update();
}

void Viewer::mousePressEvent(QMouseEvent*e)
{
	fern.click(e->pos().x(), e->pos().y());
	update();
}
