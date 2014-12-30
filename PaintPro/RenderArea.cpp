#include "RenderArea.h"
#include <QPainter>
#include <QRgb>
#include <QVector3D>

#define LIFE_FACTOR 10

RenderArea::RenderArea(QWidget* parent) : QWidget(parent) {
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	scale = 1.0f;
	penWidth = 20;
	layerFlipped = false;

	setBgImage("morgan-freeman.jpg");

	this->setFocusPolicy(Qt::StrongFocus);
}

void RenderArea::setScale(float scale) {
	this->scale = scale;

	this->setFixedSize(bgImage.width() * scale, bgImage.height() * scale);

	update();
	this->adjustSize();
}

void RenderArea::setPenWidth(int penWidth) {
	this->penWidth = penWidth;
}

void RenderArea::saveImage(const QString& filename) {
	fgImage.save(filename);
}

void RenderArea::setImage(const QString& filename) {
	fgImage.load(filename);

	this->setFixedSize(fgImage.width() * scale, fgImage.height() * scale);

	update();
	this->adjustSize();
}

void RenderArea::setBgImage(const QString& filename) {
	bgImage.load(filename);

	this->setFixedSize(bgImage.width() * scale, bgImage.height() * scale);

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
	QPoint pt = point / scale;

	QPainter painter(&fgImage);
	painter.setPen(QPen(color, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	//painter.drawEllipse(pt, penWidth / 2, penWidth / 2);
	painter.drawPoint(pt);
	
	int rad = (penWidth / 2) + 2;
	rad *= scale;
	//update(QRect(point - QPoint(penWidth / 2 + 2, penWidth / 2 + 2), point + QPoint(penWidth / 2 + 2, penWidth / 2 + 2)));
	update(QRect(point, point).normalized().adjusted(-rad, -rad, +rad, +rad));
}

void RenderArea::drawLineTo(const QPoint &endPoint) {
	QPoint pt1 = lastPoint / scale;
	QPoint pt2 = endPoint / scale;

	QPainter painter(&fgImage);
	painter.setPen(QPen(color, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(pt1, pt2);

	QPoint diff = pt2 - pt1;;
	life -= (int)sqrtf(diff.x() * diff.x() + diff.y() * diff.y());
	if (life < 0) {
		life = penWidth * LIFE_FACTOR;
		color = getAveragedColor(lastPoint);
	}
	
	int rad = (penWidth / 2) + 2;
	rad *= scale;
	update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

	lastPoint = endPoint;
}

QColor RenderArea::getAveragedColor(const QPoint& point) {
	QPoint pt = point / scale;

	QVector3D rgb(0, 0, 0);
	int count = 0;
	for (int u = pt.x() - penWidth * 0.5; u <= pt.x() + penWidth * 0.5; ++u) {
		for (int v = pt.y() - penWidth * 0.5; v <= pt.y() + penWidth * 0.5; ++v) {
			if (u < 0 || u >= bgImage.width() || v < 0 || v >= bgImage.height()) continue;

			QColor c = QColor(bgImage.pixel(u, v));

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

	painter.scale(scale, scale);

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

		color = getAveragedColor(lastPoint);
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

void RenderArea::wheelEvent(QWheelEvent* event) {
	if (event->modifiers() & Qt::ControlModifier) {
		scale += event->delta() / 120;
		if (scale < 1) scale = 1;
		if (scale > 8) scale = 8;

		setScale(scale);
	} else {
		QWidget::wheelEvent(event);
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

		color = getAveragedColor(lastPoint);
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
