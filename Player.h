#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

struct Point {
    int x;
    int y;
};

enum class MovementDir {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE
};

struct Player {
    void swap(Player &other) {
        std::swap(key_num, other.key_num);
        std::swap(is_finish, other.is_finish);
        std::swap(coords, other.coords);
        std::swap(old_coords, other.old_coords);
        std::swap(color, other.color);
    }

    Player(Player &other) {
        swap(other);
    }

    explicit Player(Point pos = {
        .x = 10,
        .y = 10
    }):
        coords(pos), old_coords(coords) {};

    bool Moved() const;
    void ProcessInput(MovementDir dir, Level & level_cur, Image & screenBuffer);
    void Draw(Image & screen);
    int key_num = 0;
    bool is_finish = false, is_win = false;
    static Image hero;
private:

    Point coords {
        .x = 10, .y = 10
    };
    Point old_coords {
        .x = 10, .y = 10
    };
    Pixel color {
        .r = 255, .g = 255, .b = 0, .a = 255
    };
    int move_speed = 3;

};

#endif //MAIN_PLAYER_H
