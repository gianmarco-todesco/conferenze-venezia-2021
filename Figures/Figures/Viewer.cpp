#include "Viewer.h"
#include <QPainter>
#include "Hanoi.h"
#include "Dragon.h"
#include "Hilbert.h"
#include "Peano.h"
#include "Sierpinski.h"
#include "ChaosGame.h"
#include "BarnsleyFern.h"
#include "Tree.h"
// #include "Cloud.h"
#include "MengerSpongeSections.h"


#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

Viewer::Viewer(QWidget *parent)
	: QWidget(parent)
{
	// HanoiShortestPath(4).save("fig1.png");
	// HanoiLongestPath(4).save("fig2.png");
	// Dragon1().save("fig6a.png");
	// Dragon2().save("fig6b.png");
	// Dragon4(15).save("fig8.png");
	// Hilbert().save("fig9.png");
	
	Peano().save("fig10.png");

	// Sierpinski().save("fig11.png");
	// Sierpinski2().save("fig12.png");
	// HanoiGraph(4).save("fig13.png");
	// ChaosGame(0).save("fig14.png");
	// ChaosGame(1).save("fig15.png");
	// BarnsleyFern(1).save("fig16a.png");
	// BarnsleyFern(2).save("fig16b.png");
	// Tree().save("fig17.png");
	// Cloud().save("fig18.png");
	// MengerSpongeSections().save("fig19.png");

}

Viewer::~Viewer()
{
}

// double S=0.9, A=30;
int Seed = 1234581;

void Viewer::paintEvent(QPaintEvent*)
{
	QPainter pa(this);
	pa.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

	int w = width(), h = height();
	// pa.translate(width() / 2, height() / 2);
	pa.fillRect(0, 0, w, h, Qt::white);


	// HanoiShortestPath(4).paint(pa, w, h);
	// HanoiLongestPath(4).paint(pa, w, h);
	// HanoiGraph(3).paint(pa, w, h);
	
	// Dragon2().paint(pa, w, h);
	// Dragon4(15).paint(pa, w, h);

	// Hilbert().paint(pa, w, h);
	Peano().paint(pa, w, h);
	// Hilbert2().paint(pa, w, h);

	// Sierpinski().paint(pa, w, h);
	// Sierpinski2().paint(pa, w, h);

	// HanoiGraph(4).paint(pa, w, h);

	// ChaosGame(1).paint(pa, w, h);
	// BarnsleyFern(1).paint(pa, w, h);
	// BarnsleyFern(2).paint(pa, w, h);
	// fern.paint(pa, w, h);
	// Tree().paint(pa,w,h);
	// Cloud(Seed).paint(pa, w, h);

	// MengerSpongeSections().paint(pa, w, h);
}

void Viewer::keyPressEvent(QKeyEvent*e)
{
	//if (e->key() == Qt::Key_A) Seed -= 1;
	//else if (e->key() == Qt::Key_S) Seed += 1;
	qDebug() << Seed;
	update();

}

void Viewer::mousePressEvent(QMouseEvent*e)
{
	// fern.click(e->pos().x(), e->pos().y());
	update();
}
