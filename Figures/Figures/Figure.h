#pragma once

class QPainter;
class QString;

class Figure
{
public:
	virtual void paint(QPainter& pa, int w, int h) = 0;
	void save(const QString& fname);
	virtual double paperScaleFactor() const { return 1.0; }
};

