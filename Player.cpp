#include "Player.h"

#include <iostream>

#include <cmath>

Image Player::hero("./resources/4.png");

bool Player::Moved() const {
    if (coords.x == old_coords.x && coords.y == old_coords.y)
        return false;
    else
        return true;
}

void Player::ProcessInput(MovementDir dir, Level & cur_lev, Image & screenBuffer) {
    if (is_win || is_finish) return;
    int move_dist = move_speed * 1;
    switch (dir) {
    case MovementDir::UP:
        old_coords.y = coords.y;
        coords.y += move_dist;
        break;
    case MovementDir::DOWN:
        old_coords.y = coords.y;
        coords.y -= move_dist;
        break;
    case MovementDir::LEFT:
        old_coords.x = coords.x;
        coords.x -= move_dist;
        break;
    case MovementDir::RIGHT:
        old_coords.x = coords.x;
        coords.x += move_dist;
        break;
    default:
        break;
    }

    int x_0 = (coords.x + tileSize / 2) / tileSize;
    int y_0 = (coords.y + tileSize / 2) / tileSize;
    char under_player = cur_lev.getCell(x_0, y_0);
    switch (under_player) {
    case 'K':
        cur_lev.setCell(x_0, y_0, '.');
        key_num++;
        break;
    case 'x':
        if (x_0 >= MAP_WIDTH - 1) {
            cur_lev.x_init += 1;
            cur_lev.enter = 2;
        } else if (x_0 <= 1) {
            cur_lev.x_init -= 1;
            cur_lev.enter = 3;
        } else if (y_0 >= (WINDOW_HEIGHT / tileSize) - 1) {
            cur_lev.y_init -= 1;
            cur_lev.enter = 1;
        } else {
            cur_lev.y_init += 1;
            cur_lev.enter = 0;
        }
        //std::cout << "OLD PLAYER COORDS: " << coords.x / tileSize  << " " << coords.y  / tileSize << "\n";
        coords.x = cur_lev.x_hero[cur_lev.enter][cur_lev.lab_arr[cur_lev.y_init][cur_lev.x_init] - 'A'] * tileSize;
        coords.y = WINDOW_HEIGHT - cur_lev.y_hero[cur_lev.enter][cur_lev.lab_arr[cur_lev.y_init][cur_lev.x_init] - 'A'] * tileSize;
        //std::cout << "NEW PLAYER COORDS:  " << coords.x / tileSize << " " << coords.y  / tileSize << "\n";
        cur_lev.spawn_time = glfwGetTime();
        break;
    case '_':
        is_finish = true;
        cur_lev.spawn_time = glfwGetTime();
        break;
    case 'Q':
        is_win = true;
        cur_lev.spawn_time = glfwGetTime();
        break;
    }
    for (int y = y_0-1; y <= y_0+1; y++) {
        for (int x = x_0-1; x <= x_0+1; x++) {
            int box_x = x * tileSize;
            int box_y = y * tileSize;
            double y_h = coords.y < box_y ? std::max(0, coords.y + tileSize - box_y) : std::min(0, coords.y - box_y - tileSize);
            double x_h = coords.x < box_x ? std::max(0, coords.x + tileSize - box_x) : std::min(0, coords.x - box_x - tileSize);
            char cell = cur_lev.getCell(x, y);
            if (cell == '#' || cell == 'C' || cell == 'W' || cell == 'G') {
                if (fabs(x_h) < fabs(y_h)) {
                    coords.x -= x_h;
                } else {
                    coords.y -= y_h;
                }
            }
            if (x || y) {
                if (cur_lev.getCell(x, y) == 'C') {
                    if (dir == MovementDir::SPACE && key_num) {
                        cur_lev.setCell(x, y, 'x');
                        key_num--;
                    }
                }
            }
        }
    }
}

void Player::Draw(Image & screen) {
    if (is_finish || is_win) return;

    double xscale = (double) tileSize/srcSize;
    double yscale = (double) tileSize/srcSize;
    screen.Blit(coords.x, coords.y, hero, 0, 0, srcSize, srcSize, xscale, yscale);
}
