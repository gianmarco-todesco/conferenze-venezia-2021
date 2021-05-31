#pragma once

#include "Figure.h"

#include <QList>
#include <QColor>

class HanoiStatus {
public:
	QList<int> poles[3];
};

class HanoiMove : public HanoiStatus {
public:
	int from, to;
	HanoiMove() : from(-1), to(-1) {}
};


class Hanoi : public Figure
{
protected:
	int m_diskCount;
	QPointF m_cc[3];
	QList<QColor> m_colors;
	const double outradius = 110.0;

public:
	Hanoi(int diskCount);

	void drawStatus(QPainter& pa, HanoiStatus& status);
	void drawMove(QPainter& pa, HanoiMove& status);

	HanoiMove getFirstMove() const;
	double getDiskRadius(int k) const; // k = 1..m_diskCount
	QPointF getPolePos(int j) const;
};

class HanoiPath : public Hanoi {
protected:
	QList<HanoiMove> m_path;
public:
	HanoiPath(int diskCount) : Hanoi(diskCount) {};

	void buildPath();
	virtual void doBuildPath(int level, int cc[3]) = 0;
	void addMove(int a, int b);

	void paint(QPainter& pa, int w, int h) override;

	void drawArrow(QPainter& pa, double dir);
	void drawEndlineArrow(QPainter& pa, double dir);
};

class HanoiShortestPath : public HanoiPath {
	
public:
	HanoiShortestPath(int diskCount) : HanoiPath(diskCount) {}
	void doBuildPath(int level, int cc[3]) override;
	
};

class HanoiLongestPath : public HanoiPath {
public:
	HanoiLongestPath(int diskCount) : HanoiPath(diskCount) {}
	void doBuildPath(int level, int cc[3]) override;
};

class HanoiGraph : public Hanoi {
	const double unit = 140.0;
public:
	HanoiGraph(int diskCount) : Hanoi(diskCount) {}
	void paint(QPainter& pa, int w, int h) override;

private:
	void draw(QPainter& pa, int level);
	void draw(QPainter& pa, int level, int cc[3], HanoiStatus status);
	void drawLinks(QPainter& pa, int level);
};

