#include "RenderArea.h"
#include <QPainter>
#include <QRgb>
#include <QVector3D>

#define LIFE_FACTOR 10

RenderArea::RenderArea(QWidget* parent) : QWidget(parent) {
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	setBgImage("morgan-freeman.jpg");

	penWidth = 20;
	layerFlipped = false;

	this->setFocusPolicy(Qt::StrongFocus);
}

void RenderArea::setPenWidth(int penWidth) {
	this->penWidth = penWidth;
}

void RenderArea::saveImage(const QString& filename) {
	fgImage.save(filename);
}

void RenderArea::setImage(const QString& filename) {
	fgImage.load(filename);

	this->setFixedSize(fgImage.width(), fgImage.height());

	update();
	this->adjustSize();
}

void RenderArea::setBgImage(const QString& filename) {
	bgImage.load(filename);

	this->setFixedSize(bgImage.width(), bgImage.height());

	update();
	this->adjustSize();
}

void RenderArea::resizeImage(const QSize &newSize) {
	if (fgImage.size() == newSize) return;
	
	QImage newImage(newSize, QImage::Format_ARGB32);
	newImage.fill(qRgba(255, 255, 255, 0));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), fgImage);
	//*image = newImage;
	fgImage = newImage;
}

void RenderArea::drawPoint(const QPoint &point) {
	QPainter painter(&fgImage);
	painter.setPen(QPen(color, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawPoint(point);
	
	int rad = (penWidth / 2) + 2;
	update(QRect(point, point).normalized().adjusted(-rad, -rad, +rad, +rad));
}

void RenderArea::drawLineTo(const QPoint &endPoint) {
	QPainter painter(&fgImage);
	painter.setPen(QPen(color, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(lastPoint, endPoint);

	QPoint diff = endPoint - lastPoint;
	life -= (int)sqrtf(diff.x() * diff.x() + diff.y() * diff.y());
	if (life < 0) {
		life = penWidth * LIFE_FACTOR;
		color = getAveragedColor(lastPoint.x(), lastPoint.y());
	}
	
	int rad = (penWidth / 2) + 2;
	update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

	lastPoint = endPoint;
}

QColor RenderArea::getAveragedColor(int x, int y) {
	QVector3D rgb(0, 0, 0);
	int count = 0;
	for (int u = x - penWidth * 0.5; u <= x + penWidth * 0.5; ++u) {
		for (int v = y - penWidth * 0.5; v <= y + penWidth * 0.5; ++v) {
			if (x < 0 || x >= bgImage.width() || y < 0 || y >= bgImage.height()) continue;

			QColor c = QColor(bgImage.pixel(lastPoint.x(), lastPoint.y()));

			rgb += QVector3D(c.red(), c.green(), c.blue());
			count++;
		}
	}

	rgb /= count;

	return QColor(rgb.x(), rgb.y(), rgb.z());
}

void RenderArea::setLayerFlipped(bool flipped) {
	layerFlipped = flipped;
	update();
}

void RenderArea::paintEvent(QPaintEvent * /* event */) {
	QPainter painter(this);

	if (!layerFlipped) {
		painter.setOpacity(0.2);
		painter.drawImage(QPoint(0, 0), bgImage);

		painter.setOpacity(1.0);
		painter.drawImage(QPoint(0, 0), fgImage);
	} else {
		painter.setOpacity(0.2);
		painter.drawImage(QPoint(0, 0), fgImage);

		painter.setOpacity(1.0);
		painter.drawImage(QPoint(0, 0), bgImage);
	}
}

void RenderArea::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		lastPoint = event->pos();
		scribbling = true;
		life = penWidth * LIFE_FACTOR;

		color = getAveragedColor(lastPoint.x(), lastPoint.y());
		drawPoint(event->pos());
	}
}

void RenderArea::mouseMoveEvent(QMouseEvent *event) {
	if (scribbling)
		drawLineTo(event->pos());
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event) {
	if (scribbling) {
		drawLineTo(event->pos());
		scribbling = false;
	}
}

void RenderArea::resizeEvent(QResizeEvent *event) {
	if (width() > fgImage.width() || height() > fgImage.height()) {
		resizeImage(QSize(width(), height()));
		update();
	}

	QWidget::resizeEvent(event);
}

void RenderArea::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) {
		lastPoint = this->mapFromGlobal(QCursor::pos());
		scribbling = true;
		life = penWidth * LIFE_FACTOR;

		color = getAveragedColor(lastPoint.x(), lastPoint.y());
		drawPoint(lastPoint);

		this->setMouseTracking(true);
	} else if (event->key() == Qt::Key_F) {
		layerFlipped = true;
		update();
	}
}

void RenderArea::keyReleaseEvent(QKeyEvent* event) {
	scribbling = false;
	this->setMouseTracking(false);

	layerFlipped = false;
	update();
}
