#ifndef _OUTLINE_H
#define _OUTLINE_H

#include <tuple>
#include <list>
#include <string>
#include "tile.h"


struct HSplit
{
    Tile* upper;
    Tile* lower;
};

struct VSplit
{
    Tile* left;
    Tile* right;
};

struct NeighborCount
{
    int solid_count;
    int space_count;
};

class Outline {
private:
    int width;
    int height;
public:
    Tile* start;
    std::list<Tile*> blocks;

    //================================================================
    // Constructors and Destructors
    //================================================================
    Outline(int width, int height): width(width), height(height) {
        start = new Tile(
            {
                {width, height},// topRight
                {0, 0},         // bottomLeft
            }, -1               // id
        );

        blocks.push_back(start);
    }
    ~Outline() {
        for (Tile* block : blocks) {
            delete block;
        }
    }

    //================================================================
    // Getters and Setters
    //================================================================
    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    //================================================================
    // Public Methods
    //================================================================
    Tile* findTileatPoint(Tile* start, Point point) {
        // Protection
        if (start == nullptr) {
            return nullptr;
        }
        if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height) {
            return nullptr;
        }

        Tile* tile = start;
        while (
            point.y < tile->getRect().bottom_left.y || point.y >= tile->getRect().top_right.y ||
            point.x < tile->getRect().bottom_left.x || point.x >= tile->getRect().top_right.x
        ) {
            // 1) First move up or down, using right top (rt) and left bottom
            // (lb) stitches, until a tile is found whose vertical range contains
            // the desired point.
            while (point.y < tile->getRect().bottom_left.y || point.y >= tile->getRect().top_right.y) {
                if (point.y < tile->getRect().bottom_left.y) {
                    tile = tile->getBelow();
                } else {
                    tile = tile->getAbove();
                }
            }
            // 2) Then move left or right, using tr and lb stitches, until a
            // tile is found whose horizontal range contains the desired point.
            while (point.x < tile->getRect().bottom_left.x || point.x >= tile->getRect().top_right.x) {
                if (point.x < tile->getRect().bottom_left.x) {
                    tile = tile->getLeft();
                } else {
                    tile = tile->getRight();
                }
            }
            // 3) Since the horizontal motion may have introduced a ver-
            // tical misalignment, steps l) and 2) may have to be iterated
            // several times to locate the tile containing the point. The con-
            // vexity of the tiles guarantees that the algorithm will converge.
        }

        return tile;
    }
    HSplit splitTileHorizontally(Tile* tile, int y){
        // Protection
        if (tile == nullptr) {
            return {nullptr, nullptr};
        }
        if (y <= tile->getRect().bottom_left.y || y >= tile->getRect().top_right.y) {
            return {nullptr, nullptr};
        }

        // Create new tiles
        Tile* upper = new Tile(
            {
                tile->getRect().top_right,           // topRight
                {tile->getRect().bottom_left.x, y}   // bottomLeft
            }, -1                                    // id
        );
        Tile* lower = tile;
        tile->setRect(
            {
                {tile->getRect().top_right.x, y},    // topRight
                tile->getRect().bottom_left          // bottomLeft
            }
        );

        // Update stitches
        upper->setBelow(lower);
        upper->setAbove(lower->getAbove());
        upper->setRight(lower->getRight());

        Tile* tile_it;
        // adjust corner stitches along top edge
        for (tile_it = lower->getAbove(); tile_it != nullptr && tile_it->getBelow() == lower; tile_it = tile_it->getLeft()) {
            tile_it->setBelow(upper);
        }
        lower->setAbove(upper);

        // adjust corner stitches along right edge
        for (tile_it = lower->getRight(); tile_it != nullptr && tile_it->getRect().bottom_left.y >= y; tile_it = tile_it->getBelow()) {
            tile_it->setLeft(upper);
        }
        lower->setRight(tile_it);

        // adjust corner stitches along left edge (lower)
        for (tile_it = lower->getLeft(); tile_it != nullptr && tile_it->getRect().top_right.y <= y; tile_it = tile_it->getAbove()) {
            // Nothing to do, because `Tile* lower = tile;`
        }
        upper->setLeft(tile_it);

        // adjust corner stitches along left edge (upper)
        for (; tile_it != nullptr && tile_it->getRight() == lower; tile_it = tile_it->getAbove()) {
            tile_it->setRight(upper);
        }

        // Add the new tiles to the list
        blocks.push_back(upper);

        return {upper, lower};
    }
    VSplit splitTileVertically(Tile* tile, int x) {
        // Protection
        if (tile == nullptr) {
            return {nullptr, nullptr};
        }
        if (x <= tile->getRect().bottom_left.x || x >= tile->getRect().top_right.x) {
            return {nullptr, nullptr};
        }

        // Create new tiles
        Tile* right = new Tile(
            {
                tile->getRect().top_right,           // topRight
                {x, tile->getRect().bottom_left.y}   // bottomLeft
            }, -1                                    // id
        );
        Tile* left = tile;
        tile->setRect(
            {
                {x, tile->getRect().top_right.y},    // topRight
                tile->getRect().bottom_left          // bottomLeft
            }
        );

        // Update stitches
        right->setLeft(left);
        right->setRight(left->getRight());
        right->setAbove(left->getAbove());

        Tile* tile_it;
        // adjust corner stitches along right edge
        for (tile_it = left->getRight(); tile_it != nullptr && tile_it->getLeft() == left; tile_it = tile_it->getBelow()) {
            tile_it->setLeft(right);
        }
        left->setRight(right);

        // adjust corner stitches along top edge
        for (tile_it = left->getAbove(); tile_it != nullptr && tile_it->getRect().bottom_left.x >= x; tile_it = tile_it->getLeft()) {
            tile_it->setBelow(right);
        }
        left->setAbove(tile_it);

        // adjust corner stitches along bottom edge (left)
        for (tile_it = left->getBelow(); tile_it != nullptr && tile_it->getRect().top_right.x <= x; tile_it = tile_it->getRight()) {
            // Nothing to do, because `Tile* left = tile;`
        }
        right->setBelow(tile_it);

        // adjust corner stitches along bottom edge (right)
        for (; tile_it != nullptr && tile_it->getAbove() == left; tile_it = tile_it->getRight()) {
            tile_it->setAbove(right);
        }

        // Add the new tiles to the list
        blocks.push_back(right);

        return {left, right};
    }
    Tile* mergeDown(Tile* tile) {
        // Protection
        if (tile == nullptr) {
            return nullptr;
        }
        Tile* lower = tile->getBelow();
        if (lower == nullptr) {
            return tile;
        }

        // Check if the two tiles are horizontally aligned
        if (tile->getRect().bottom_left.x != lower->getRect().bottom_left.x || 
            tile->getRect().top_right.x != lower->getRect().top_right.x) {
            return tile;
        }

        // Check the id of the two tiles
        if (tile->getId() != lower->getId()) {
            return tile;
        }

        // Change the size of the tile
        lower->setRect(
            {
                tile->getRect().top_right,           // topRight
                lower->getRect().bottom_left         // bottomLeft
            }
        );

        // Update stitches
        lower->setAbove(tile->getAbove());
        lower->setRight(tile->getRight());

        Tile* tile_it;
        // adjust corner stitches along right edge
        for (tile_it = tile->getRight(); tile_it != nullptr && tile_it->getLeft() == tile; tile_it = tile_it->getBelow()) {
            tile_it->setLeft(lower);
        }
        
        // adjust corner stitches along left edge
        for (tile_it = tile->getLeft(); tile_it != nullptr && tile_it->getRight() == tile; tile_it = tile_it->getAbove()) {
            tile_it->setRight(lower);
        }

        // adjust corner stitches along top edge
        for (tile_it = tile->getAbove(); tile_it != nullptr && tile_it->getBelow() == tile; tile_it = tile_it->getLeft()) {
            tile_it->setBelow(lower);
        }

        // Free the tile
        blocks.remove(tile);
        delete tile;

        return lower;
    }
    Tile* mergeUp(Tile* tile) {
        // Protection
        if (tile == nullptr) {
            return nullptr;
        }
        Tile* upper = tile->getAbove();
        if (upper == nullptr) {
            return tile;
        }

        // Use mergeDown
        return mergeDown(upper);
    }
    Tile* createBlock(Rect rect, int id) {
        // 1) Find the space tile containing the top edge of the area
        // to be occupied by the new tile (because of the strip property,
        // a single space tile must contain the entire edge).
        Tile* top_strip_tile = findTileatPoint(start, {rect.bottom_left.x, rect.top_right.y});

        // 2) Split the top space tile along a horizontal line into a piece
        // entirely above the new tile and a piece overlapping the new
        // tile. Update corner stitches in the tiles adjoining the new tile.
        splitTileHorizontally(top_strip_tile, rect.top_right.y);

        // 3) Find the space tile containing the bottom edge of the
        // new solid tile, split it in the same fashion, and update stitches
        // around it.
        Tile* bottom_strip_tile = findTileatPoint(start, rect.bottom_left);
        HSplit h_split_bottom = splitTileHorizontally(bottom_strip_tile, rect.bottom_left.y);

        // 4 ) Work down along the left side of the area of the new tile, 
        // as for the area-search algorithm. Each tile along this edge 
        // must be a space tile that spans the entlre width Of the new
        // solid tile. Split the space tile into a Piece entirely to the left
        // of the new tile, a piece entirely to the right of the new tile,
        // and a piece entirely within the new tile. This splitting may
        // make it possible to merge the left and right remainders verti-
        // cally with the tiles just above them: merge whenever possible.
        Tile* next_tile = nullptr;
        Tile* ret_tile = nullptr;
        Tile* t_left = nullptr;
        Tile* t_right = nullptr;
        for (
            Tile* tile = h_split_bottom.upper == nullptr ? bottom_strip_tile : h_split_bottom.upper;
            tile != nullptr && tile->getRect().top_right.y <= rect.top_right.y; //TODO: Check this condition
            tile = next_tile
        ) {
            // next_tile = tile->getAbove();
            next_tile = findTileatPoint(tile, {rect.bottom_left.x, tile->getRect().top_right.y});
            Tile* tt = tile;

            // Split left
            VSplit v_split_l = splitTileVertically(tt, rect.bottom_left.x);
            if (v_split_l.left != nullptr) {
                t_left = mergeDown(v_split_l.left);
                tt = v_split_l.right;
            }

            // Split right
            VSplit v_split_r = splitTileVertically(tt, rect.top_right.x);
            if (v_split_r.right != nullptr) {
                t_right = mergeDown(v_split_r.right);
                tt = v_split_r.left;
            }

            // Set id of the tile
            tt->setId(id);

            // Merge down
            ret_tile = mergeDown(tt);
        }
        if (t_left != nullptr) {
            mergeUp(t_left);
        }
        if (t_right != nullptr) {
            mergeUp(t_right);
        }

        return ret_tile;
    }
    NeighborCount neighborFinding(Tile* tile) {
        // Protection
        if (tile == nullptr) {
            return {0, 0};
        }

        int solid_count = 0;
        int space_count = 0;

        // Check above
        for (Tile* tile_it = tile->getAbove(); tile_it != nullptr && tile_it->getRect().top_right.x > tile->getRect().bottom_left.x; tile_it = tile_it->getLeft()) {
            if (tile_it->getId() == -1) {
                space_count++;
            } else {
                solid_count++;
            }
        }

        // Check right
        for (Tile* tile_it = tile->getRight(); tile_it != nullptr && tile_it->getRect().top_right.y > tile->getRect().bottom_left.y; tile_it = tile_it->getBelow()) {
            if (tile_it->getId() == -1) {
                space_count++;
            } else {
                solid_count++;
            }
        }

        // Check below
        for (Tile* tile_it = tile->getBelow(); tile_it != nullptr && tile_it->getRect().bottom_left.x < tile->getRect().top_right.x; tile_it = tile_it->getRight()) {
            if (tile_it->getId() == -1) {
                space_count++;
            } else {
                solid_count++;
            }
        }

        // Check left
        for (Tile* tile_it = tile->getLeft(); tile_it != nullptr && tile_it->getRect().bottom_left.y < tile->getRect().top_right.y; tile_it = tile_it->getAbove()) {
            if (tile_it->getId() == -1) {
                space_count++;
            } else {
                solid_count++;
            }
        }

        return {solid_count, space_count};
    }

};

#endif