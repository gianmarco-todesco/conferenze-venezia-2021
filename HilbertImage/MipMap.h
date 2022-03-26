#pragma once

#include <QImage>
#include <QList>
#include <QVector>

class MipMap {
public:
    QList<QVector<int>> m_data;
    QList<QImage> m_images;
    MipMap(QImage img);
};

