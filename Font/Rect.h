#ifndef RECT_H
#define RECT_H

#include <glm/glm.hpp>



inline bool isIntersecting(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2) {
    return (((q1.x-p1.x)*(p2.y-p1.y) - (q1.y-p1.y)*(p2.x-p1.x))
            * ((q2.x-p1.x)*(p2.y-p1.y) - (q2.y-p1.y)*(p2.x-p1.x)) <= 0)
            &&
           (((p1.x-q1.x)*(q2.y-q1.y) - (p1.y-q1.y)*(q2.x-q1.x))
            * ((p2.x-q1.x)*(q2.y-q1.y) - (p2.y-q1.y)*(q2.x-q1.x)) <= 0);
}


// TODO: move to own class
struct Line2 {
    glm::vec2 start, end;

    Line2() {}
    Line2(const glm::vec2& start, const glm::vec2& end): start(start), end(end) {}
    Line2(float x1, float y1, float x2, float y2):
        start(glm::vec2(x1, y1)), end(glm::vec2(x2, y2)) {}

    // https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
    bool intersectLine(const Line2& line) const {
        return isIntersecting(start, end, line.start, line.end);
    }
};


struct Rect {
    Rect() {}
    Rect(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {}

    float x, y, w, h;

    bool collide(const glm::vec2& point) const {
        return point.x >= x && point.x < x + w && point.y >= y && point.y < y + h;
    }

    bool intersect(const Line2& line) const {
        glm::vec2 topLeft(x, y);
        glm::vec2 topRight(x + w, y);
        glm::vec2 bottomLeft(x, y + h);
        glm::vec2 bottomRight(x + w, y + h);

        return line.intersectLine(Line2(topLeft, topRight)) ||
               line.intersectLine(Line2(topLeft, bottomLeft)) ||
               line.intersectLine(Line2(topRight, bottomRight)) ||
               line.intersectLine(Line2(bottomLeft, bottomRight));
    }
};

#endif // RECT_H
