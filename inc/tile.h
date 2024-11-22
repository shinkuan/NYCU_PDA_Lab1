#ifndef _TILE_H
#define _TILE_H

#include <tuple>
#include <list>
#include <string>


struct Point
{
    int x;
    int y;
};

struct Rect
{
    Point top_right;
    Point bottom_left;
};


class Tile {
private:
    Rect rect;
    int id;
    Tile* above;
    Tile* right;
    Tile* below;
    Tile* left;

public:
    //================================================================
    // Constructors and Destructors
    //================================================================
    Tile(Rect rect, int id): rect(rect), id(id) {
        above = nullptr;
        right = nullptr;
        below = nullptr;
        left = nullptr;
    }
    ~Tile() {
        // Do nothing
    }

    //================================================================
    // Getters and Setters
    //================================================================
    Rect getRect() {
        return rect;
    }

    void setRect(Rect rect) {
        this->rect = rect;
    }

    int getId() {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }

    Tile* getAbove() {
        return above;
    }

    Tile* getRight() {
        return right;
    }

    Tile* getBelow() {
        return below;
    }

    Tile* getLeft() {
        return left;
    }

    void setAbove(Tile* above) {
        this->above = above;
    }

    void setRight(Tile* right) {
        this->right = right;
    }

    void setBelow(Tile* below) {
        this->below = below;
    }

    void setLeft(Tile* left) {
        this->left = left;
    }

    //================================================================
    // Tile Index
    //================================================================
    bool isSpace() {
        return id < 0;
    }

    bool isTile() {
        return id > 0;
    }


};

#endif