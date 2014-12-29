#include "RenderArea.h"
#include <QPainter>
#include <QRgb>
#include <QVector3D>

RenderArea::RenderArea(QWidget* parent) : QWidget(parent) {
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	setImage("morgan-freeman.jpg");

	penWidth = 20;
}

void RenderArea::setPenWidth(int penWidth) {
	this->penWidth = penWidth;
}

void RenderArea::saveImage(const QString& filename) {
	fgImage.save(filename);
}

void RenderArea::setImage(const QString& filename) {
	bgImage.load(filename);

	this->setFixedSize(bgImage.width(), bgImage.height());

	update();
}

void RenderArea::resizeImage(QImage *image, const QSize &newSize) {
	if (image->size() == newSize) return;
	
	QImage newImage(newSize, QImage::Format_ARGB32);
	newImage.fill(qRgba(255, 255, 255, 0));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}

void RenderArea::drawLineTo(const QPoint &endPoint) {
	QPainter painter(&fgImage);
	painter.setPen(QPen(color, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(lastPoint, endPoint);
	
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

void RenderArea::paintEvent(QPaintEvent * /* event */) {
	QPainter painter(this);

	painter.setOpacity(0.2);
	painter.drawImage(QPoint(0, 0), bgImage);

	painter.setOpacity(1.0);
	painter.drawImage(QPoint(0, 0), fgImage);
}

void RenderArea::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		lastPoint = event->pos();
		scribbling = true;

		color = getAveragedColor(lastPoint.x(), lastPoint.y());
	}
}

void RenderArea::mouseMoveEvent(QMouseEvent *event) {
	if ((event->buttons() & Qt::LeftButton) && scribbling)
		drawLineTo(event->pos());
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton && scribbling) {
		drawLineTo(event->pos());
		scribbling = false;
	}
}

void RenderArea::resizeEvent(QResizeEvent *event) {
	if (width() > fgImage.width() || height() > fgImage.height()) {
		//int newWidth = qMax(width() + 128, fgImage.width());
		int newWidth = width();
		//int newHeight = qMax(height() + 128, fgImage.height());
		int newHeight = height();
		resizeImage(&fgImage, QSize(newWidth, newHeight));
		update();
	}

	QWidget::resizeEvent(event);
}