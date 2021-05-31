#pragma once

#include <QWidget>

class Viewer : public QWidget
{
	Q_OBJECT

public:
	Viewer(QWidget *parent = Q_NULLPTR);
	~Viewer();

	QSize sizeHint() const override { return QSize(1024, 768); }
	void paintEvent(QPaintEvent*) override; 

	void keyPressEvent(QKeyEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
};
