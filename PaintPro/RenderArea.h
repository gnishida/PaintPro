#pragma once

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>

class RenderArea : public QWidget {
Q_OBJECT

public:
	RenderArea(QWidget* parent = 0);

	void setImage(const QString& filename);
	void resizeImage(QImage *image, const QSize &newSize);
	void drawLineTo(const QPoint &endPoint);

protected:
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	QPixmap bgPixmap;
	QImage fgPixmap;
	bool scribbling;
	QPoint lastPoint;
};

