#include "Stroke.h"

std::ostream& operator<<(std::ostream& out, const Stroke& s) {
	return out << s.type << "\t" << s.start.x() << "\t" << s.start.y() << "\t" << s.end.x() << "\t" << s.end.y() << "\t" << s.color.red() << "\t" << s.color.green() << "\t" << s.color.blue() << "\t" << s.penWidth;
}