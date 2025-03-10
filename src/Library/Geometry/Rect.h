#pragma once

#include <algorithm>

#include "Point.h"
#include "Size.h"

template<class T>
struct Rect {
    T x = 0;
    T y = 0;
    T w = 0;
    T h = 0;

    Rect() = default;
    Rect(T x, T y, T w, T h): x(x), y(y), w(w), h(h) {}
    Rect(Point<T> topLeft, Size<T> size): x(topLeft.x), y(topLeft.y), w(size.w), h(size.h) {}
    Rect(Point<T> topLeft, Point<T> bottomRight) : x(topLeft.x), y(topLeft.y), w(bottomRight.x - topLeft.x), h(bottomRight.y - topLeft.y) {}

    bool contains(const Point<T> &point) {
        return x <= point.x && point.x < x + w && y <= point.y && point.y < y + h;
    }

    bool contains(const Rect<T> &rect) {
        return x <= rect.x && rect.x + rect.w <= x + w && y <= rect.y && rect.y + rect.h <= y + h;
    }

    Point<T> topLeft() const {
        return {x, y};
    }

    Point<T> bottomRight() const {
        return {x + w, y + h};
    }

    Point<T> topRight() const {
        return {x + w, y};
    }

    Point<T> bottomLeft() const {
        return {x, y + h};
    }

    Point<T> center() const {
        return {x + w / 2, y + h / 2};
    }

    Size<T> size() const {
        return {w, h};
    }

    bool intersects(const Rect<T> &other) const {
        // Rect, unlike BBox, doesn't include its bottom/right border. So an empty rect (w=0, h=0) intersects nothing.
        return
            x < other.x + other.w && x + w > other.x &&
            y < other.y + other.h && y + h > other.y;
    }

    Rect<T> intersection(const Rect<T> &other) const {
        T x1 = std::max(x, other.x);
        T y1 = std::max(y, other.y);
        T x2 = std::min(x + w, other.x + other.w);
        T y2 = std::min(y + h, other.y + other.h);

        return Rect<T>{x1, y1, x2 - x1, y2 - y1}; // Can return an empty rect with negative size.
    }

    bool isEmpty() const {
        return w <= 0 || h <= 0;
    }

    friend bool operator==(const Rect &l, const Rect &r) = default;
};

using Recti = Rect<int>;
