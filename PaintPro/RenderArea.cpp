#include "RenderArea.h"
#include <QPainter>
#include <QRgb>
#include <QVector3D>

RenderArea::RenderArea(QWidget* parent) : QWidget(parent) {
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	penWidth = 20;
}

void RenderArea::setImage(const QString& filename) {
	bgPixmap.load(filename);

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
	QPainter painter(&fgPixmap);
	painter.setPen(QPen(color, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
			if (x < 0 || x >= bgPixmap.width() || y < 0 || y >= bgPixmap.height()) continue;

			QColor c = QColor(bgPixmap.pixel(lastPoint.x(), lastPoint.y()));

			rgb += QVector3D(c.red(), c.green(), c.blue());
			count++;
		}
	}

	rgb /= count;

	return QColor(rgb.x(), rgb.y(), rgb.z());
}

void RenderArea::paintEvent(QPaintEvent * /* event */) {
     static const QPoint points[4] = {
         QPoint(10, 80),
         QPoint(20, 10),
         QPoint(80, 30),
         QPoint(90, 70)
     };


     QRect rect(10, 20, 80, 60);

     QPainterPath path;
     path.moveTo(20, 80);
     path.lineTo(20, 30);
     path.cubicTo(80, 0, 50, 50, 80, 80);

     int startAngle = 20 * 16;
     int arcLength = 120 * 16;

     QPainter painter(this);

	 painter.setOpacity(0.2);
 	 painter.drawImage(QPoint(0, 0), bgPixmap);

	 painter.setOpacity(1.0);
	 painter.drawImage(QPoint(0, 0), fgPixmap);
	 //painter.drawLine(10, 10, 100, 100);

	 /*
     QPainter painter(this);
     painter.setPen(pen);
     painter.setBrush(brush);*/
     /*if (antialiased)
         painter.setRenderHint(QPainter::Antialiasing, true);*/

     painter.setRenderHint(QPainter::Antialiasing, false);
     painter.setPen(palette().dark().color());
     painter.setBrush(Qt::NoBrush);
     painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
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
	if (width() > fgPixmap.width() || height() > fgPixmap.height()) {
		int newWidth = qMax(width() + 128, fgPixmap.width());
		int newHeight = qMax(height() + 128, fgPixmap.height());
		resizeImage(&fgPixmap, QSize(newWidth, newHeight));
		update();
	}

	QWidget::resizeEvent(event);
}