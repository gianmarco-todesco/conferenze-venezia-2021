#pragma once

#include <QWidget>
#include <QImage>
#include <QList>
#include "MipMap.h"


class QPainter;

class HilbertImage : public QWidget
{
    Q_OBJECT
    int m_level;
    MipMap m_target;
    QList<int> m_refScale;
    double m_gridSize;
    double m_cellSize;


public:
    HilbertImage(QWidget *parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent*) override;
    QSize sizeHint() const override { return QSize(1024, 1024); }
    void keyPressEvent(QKeyEvent* e) override;

private:
    void draw(QPainter& pa, int w, int h) const;
    void draw_1(QPainter& pa, int w, int h) const;
    void draw_2(QPainter& pa, int w, int h) const;
    void drawGrid(QPainter& pa, double gridSize, int n) const;
    void build();
    void save();
};
