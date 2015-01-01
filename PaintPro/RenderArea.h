#pragma once

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <iostream>

class Stroke {
public:
	enum { TYPE_POINT = 0, TYPE_LINE };

public:
	Stroke(int type, const QPoint& start, const QPoint& end, const QColor& color, int penWidth) : type(type), start(start), end(end), color(color), penWidth(penWidth) {}
	
	friend std::ostream& operator<<(std::ostream& out, const Stroke& s);

private:
	int type;
	QPoint start;
	QPoint end;
	QColor color;
	int penWidth;
};

class RenderArea : public QWidget {
Q_OBJECT

public:
	RenderArea(QWidget* parent = 0);

	void setScale(float factor);
	void setPenWidth(int penWidth);
	void saveImage(const QString& filename);
	void setImage(const QString& filename);
	void setBgImage(const QString& filename);
	void resizeImage(const QSize &newSize);
	void drawPoint(const QPoint &point);
	void drawLineTo(const QPoint &endPoint);
	QColor getAveragedColor(const QPoint& point);
	void setLayerFlipped(bool flipped);

protected:
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);
	void resizeEvent(QResizeEvent *event);
	void keyPressEvent(QKeyEvent* keyEvent);
	void keyReleaseEvent(QKeyEvent* keyEvent);

private:
	QImage bgImage;
	QImage fgImage;
	float scale;
	bool scribbling;
	QPoint lastPoint;
	QColor color;
	int penWidth;
	bool layerFlipped;
	int life;

	std::vector<Stroke> history;
};

