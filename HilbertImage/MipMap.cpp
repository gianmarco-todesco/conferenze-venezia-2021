#include "MipMap.h"
#include <assert.h>


MipMap::MipMap(QImage img)
{
	assert(!img.isNull());
	int w = img.width(), h = img.height();
	assert(w == h);
	assert((w & (w - 1)) == 0); // w=h deve essere una potenza di due
	m_images.append(img);
	int sz = w;
	for (;;)
	{
		sz = sz / 2;
		if (sz == 0) break;
		QImage child = img.scaled(sz, sz, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		Q_ASSERT(child.width() == sz);
		Q_ASSERT(child.height() == sz);
		//Q_ASSERT(child.format() == QImage::Format_Grayscale8);
		m_images.insert(0, child);
		// child.save(QString("data/img%1.png").arg(sz));
		
	}
	sz = w;
	QVector<int> q(sz * sz, 0);
	for (int iy = 0; iy < sz; iy++)
	{
		for (int ix = 0; ix < sz; ix++) 
			q[iy * sz + ix] = img.pixel(ix, iy) & 0xff;
	}

	m_data.append(q);
	for (;;)
	{
		int sz1 = sz / 2;
		if (sz1 == 0) break;
		QVector<int> q1(sz1 * sz1, 0);
		for (int iy = 0; iy < sz1; iy++)
		{
			for (int ix = 0; ix < sz1; ix++)
			{
				int v00 = q[2 * iy * sz + 2 * ix];
				int v01 = q[2 * iy * sz + 2 * ix + 1];
				int v10 = q[(2 * iy + 1) * sz + 2 * ix];
				int v11 = q[(2 * iy + 1) * sz + 2 * ix + 1];

				q1[iy * sz1 + ix] = qMin(qMin(v00, v01), qMin(v10, v11));
			}
		}
		m_data.insert(0, q1);
		q = q1;
		sz = sz1;


		QImage img2(sz, sz, QImage::Format_Grayscale8);
		for (int iy = 0; iy < sz1; iy++)
		{
			for (int ix = 0; ix < sz1; ix++)
			{
				int v = q[iy * sz + ix];
				img2.setPixel(ix,iy,qRgb(v, v, v));
			}
		}
		img2.save(QString("data/img-%1.png").arg(sz));
	}
	assert(m_images.count() == m_data.count());
	for (int i = 0; i < m_images.count(); i++)
	{
		const QImage& img = m_images.at(i);
		assert(img.width() == img.height());
		assert(img.width() == (1 << i));
		assert(m_data[i].size() == img.width() * img.height());
	}
}
