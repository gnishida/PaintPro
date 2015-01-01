#pragma once

#include <QPoint>
#include <QColor>
#include <iostream>

class Stroke {
public:
	enum { TYPE_POINT = 0, TYPE_LINE };

public:
	Stroke2(int type, const QPoint& start, const QPoint& end, const QColor& color, int penWidth) : type(type), start(start), end(end), color(color), penWidth(penWidth) {}
	
	friend std::ostream& operator<<(std::ostream& out, const Stroke2& s);

private:
	int type;
	QPoint start;
	QPoint end;
	QColor color;
	int penWidth;
};

