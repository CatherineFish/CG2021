
#include "Player.h"
#include <iostream>
#include <cmath>
/*
struct state_of_game {
    struct tilemap *map;
    struct tilemap *lose;
    struct tilemap *win;
};

static struct tilemap *create_death_screen(void) {
    struct tilemap *map = create_screen(STATIC_SCREEN_WIDTH, STATIC_SCREEN_HEIGHT);
    draw_message(map, 6, 2, "YOU DIED");
    draw_message(map, 1, 3, "Press R to restart");
    draw_message(map, 3, 4, "or ESC to exit");
    for (size_t y = 0; y < map->height; y++) {
        for (size_t x = 0; x < map->width; x++) {
            int r = rand();
            if (r/2 % 7 == 0) {
                tilemap_set_tile(map, x, y, 1, r & 1 ? TILE_BONES_1 : TILE_BONES_2);
            }

        }
    }
    tilemap_refresh(map);
    return map;
}*/

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}



void Player::ProcessInput(MovementDir dir, Level &cur_lev)
{
  int move_dist = move_speed * 1;
  //std::cout << 20 - coords.y / tileSize  << " " << coords.x / tileSize << std::endl;
  //std::cout << 20 - coords.y / tileSize  << " " << WINDOW_HEIGHT - coords.y << std::endl;

  switch(dir)
  {

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
/*  int x_0 = (coords.x + tileSize / 2) / tileSize;
  int y_0 = (coords.y + tileSize / 2) / tileSize;

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            int box_x = (x_0 + x) * tileSize;
            int box_y = (y_0 + y) * tileSize;
            double y_h = coords.y < box_y ? std::max(0, coords.y + tileSize - box_y) : std::min(0, coords.y - box_y - tileSize);
            double x_h = coords.x < box_x ? std::max(0, coords.x + tileSize - box_x) : std::min(0, coords.x - box_x - tileSize);
            if (cur_lev.level_arr[20 - y_0][x_0] == '#')
            {
                std::cout << "y_h = " <<  y_h << " x_h = " << x_h << std::endl;

                if (fabs(x_h) < fabs(y_h))
                {
                    coords.x -= x_h;
                }
                else
                {
                    coords.y -= y_h;
                }
            }
            else if (cur_lev.level_arr[20 - y_0][x_0] == '_')
            {
/*                if (fmin(fabs(hx), fabs(hy)) > 0) {
                    state.state = s_game_over;
                    want_redraw = 1;
                }
                break;
*/                /* Попадание в пустое место
                    КОГДА_НИБУДЬ Я ЭТО НАПИШУ
                 */
/*            } else if (cur_lev.level_arr[20 - y_0][x_0] == 'x')

            {

            }


        }
    }*/
    //tilemap_refresh(state.map);


    /*
      int x_n = (coords.x + tileSize/2) / tileSize;
      int y_n = (coords.y + tileSize/2) / tileSize;

       std::cout << "\n\n";
      for (int y = -1; y <= 1; y++) {
          for (int x = -1; x <= 1; x++) {
              int box_x = (x + x_n) * tileSize;
              int box_y = (y + y_n) * tileSize;
              int b_h = tileSize, b_w = tileSize;
              if (cur_lev.level_arr[20 - (y + y_n + 1)][x + x_n] != '#')
              {
                  b_h /= 2;
              }
              if ((cur_lev.level_arr[20 - (y + y_n)][x + x_n - 1] != '#') &&
                (cur_lev.level_arr[20 - (y + y_n)][x + x_n + 1] == '#'))
              {
                  b_w /= 2;
                  box_x += tileSize / 2;
              } else if ((cur_lev.level_arr[21 - (y + y_n)][x + x_n - 1] == '#') &&
                (cur_lev.level_arr[20 - (y + y_n)][x + x_n + 1] != '#'))
              {
                    b_w /= 2;
              }
              double x0 = coords.x, y0 = coords.y;
              double hy = ((y0 < box_y ? std::fmax(0, y0 + tileSize - box_y) : std::fmin(0, y0 - box_y - b_h)));
              double hx = ((x0 < box_x ? std::fmax(0, x0 + tileSize - box_x) : std::fmin(0, x0 - box_x - b_w)));
              std::cout << y << " " << x << " :: " << 21 - (y + y_n) << " "<< x + x_n << " " << cur_lev.level_arr[21 - (y + y_n)][x + x_n] << std:: endl;
              if (cur_lev.level_arr[21 - (y + y_n)][x + x_n] == '#')
              {
                  if (fabs(hx) < fabs(hy))
                  {
                      coords.x -= hx;
                  } else
                  {
                      coords.y -= hy;
                  }

                  break;
              }
          }
      }
    */


  /*int x = coords.x / tileSize;
  int y = coords.y / tileSize;

  //std::cout << x << " " << y << " " <<cur_lev.level_arr[y][x] << std::endl;
  int box_x = x * tileSize;
  int box_y = y * tileSize;
  //std::cout << coords.y << " " << box_y << "  " << coords.x << " " << box_x << std::endl;

  double y_h = coords.y < box_y ? std::max(0, coords.y + tileSize - box_y) : std::min(0, coords.y - box_y - tileSize);
  double x_h = coords.x < box_x ? std::max(0, coords.x + tileSize - box_x) : std::min(0, coords.x - box_x - tileSize);
  if (cur_lev.level_arr[20 - y][x] == '#')
  {
      if (fabs(x_h) < fabs(y_h))
      {
          coords.x -= x_h;
      }
      else
      {
          coords.y -= y_h;
      }
  }


  x = (coords.x + tileSize - 1) / tileSize;
  y = coords.y / tileSize;
  box_x = x * tileSize;
  box_y = y * tileSize;
  y_h = coords.y < box_y ? std::max(0, coords.y + tileSize - box_y) : std::min(0, coords.y - box_y - tileSize);
  x_h = coords.x < box_x ? std::max(0, coords.x + tileSize - box_x) :std:: min(0, coords.x - box_x - tileSize);
  if (cur_lev.level_arr[20 - y][x] == '#')
  {
      if (fabs(x_h) < fabs(y_h))
      {
          coords.x -= x_h;
      }
      else
      {
          coords.y -= y_h;
      }
  }
  x = (coords.x) / tileSize;
  y = (coords.y + tileSize - 1) / tileSize;
  box_x = x * tileSize;
  box_y = y * tileSize;
  y_h = coords.y < box_y ? std::max(0, coords.y + tileSize - box_y) : std::min(0, coords.y - box_y - tileSize);
  x_h = coords.x < box_x ? std::max(0, coords.x + tileSize - box_x) : std::min(0, coords.x - box_x - tileSize);
  if (cur_lev.level_arr[20 - y][x] == '#')
  {
      if (fabs(x_h) < fabs(y_h))
      {
          coords.x -= x_h;
      }
      else
      {
          coords.y -= y_h;
      }
  }
  x = (coords.x - tileSize) / tileSize;
  y = coords.y / tileSize;
  box_x = x * tileSize;
  box_y = y * tileSize;
  y_h = coords.y < box_y ? std::max(0, coords.y + tileSize - box_y) : std::min(0, coords.y - box_y - tileSize);
  x_h = coords.x < box_x ? std::max(0, coords.x + tileSize - box_x) :std:: min(0, coords.x - box_x - tileSize);
  if (cur_lev.level_arr[20 - y][x] == '#')
  {
      if (fabs(x_h) < fabs(y_h))
      {
          coords.x -= x_h;
      }
      else
      {
          coords.y -= y_h;
      }
  }
  x = (coords.x) / tileSize;
  y = (coords.y - tileSize) / tileSize;
  box_x = x * tileSize;
  box_y = y * tileSize;
  y_h = coords.y < box_y ? std::max(0, coords.y + tileSize - box_y) : std::min(0, coords.y - box_y - tileSize);
  x_h = coords.x < box_x ? std::max(0, coords.x + tileSize - box_x) : std::min(0, coords.x - box_x - tileSize);
  if (cur_lev.level_arr[20 - y][x] == '#')
  {
      if (fabs(x_h) < fabs(y_h))
      {
          coords.x -= x_h;
      }
      else
      {
          coords.y -= y_h;
      }
  }
*/





}

Pixel blend(Pixel a, Pixel b) {
return {
    (a.r*(255 - b.a) + b.r*b.a)/255,
    (a.g*(255 - b.a) + b.g*b.a)/255,
    (a.b*(255 - b.a) + b.b*b.a)/255,
    (a.r*(255 - b.a) + b.a)/255
    };
}


void Player::Draw(Image &screen)
{
  /*if(Moved())
  {
    for(int y = old_coords.y; y < old_coords.y + tileSize; ++y)
    {
      for(int x = old_coords.x; x < old_coords.x + tileSize; ++x)
      {
        screen.PutPixel(x, y, backgroundColor);
      }
    }
    old_coords = coords;
}*/

  Image hero("./resources/Dungeon_Character.png");

    double xscale = 1/3.;
    double yscale = 1/3.;
    for (size_t y = 0; y < tileSize; y++)
    {
        for (size_t x = 0; x < tileSize; x++)
        {
            auto x1 = hero.GetPixel(3 * 16 + x * xscale, 3 * 16 - (int)(y * yscale) - 1);
            auto x2 = screen.GetPixel(x + coords.x, y + coords.y);
            screen.PutPixel(x + coords.x, y + coords.y, blend(x2, x1));
        }
    }
}
