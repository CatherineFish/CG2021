#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H
#include "common.h"

#include <string>
#include <utility>


constexpr int tileSize = 32;
constexpr int srcSize = 16;

constexpr int MAP_HEIGHT = 20;
constexpr int MAP_WIDTH = 20;
constexpr int ROOMS = 13;
constexpr int MAZE_SIZE = 7;
constexpr GLsizei WINDOW_WIDTH = (MAP_WIDTH + 5) * tileSize, WINDOW_HEIGHT = MAP_HEIGHT * tileSize;



struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

constexpr Pixel backgroundColor{221, 160, 221, 255};

class Image
{
    int width = -1;
    int height = -1;
    int channels = 3;
    size_t size = 0;
    Pixel *data = nullptr;
    bool self_allocated = false;
public:
    void swap(Image &other) {
        std::swap(width, other.width);
        std::swap(height, other.height);
        std::swap(channels, other.channels);
        std::swap(size, other.size);
        std::swap(data, other.data);
        std::swap(self_allocated, other.self_allocated);
    }
    explicit Image(const char *a_path);
    Image(int a_width, int a_height, int a_channels);
    Image(Image &&other) { swap(other); }
    Image &operator=(Image &&other) { swap(other); return *this; }
    Image(const Image &other) = delete;
    Image &operator=(const Image &other) = delete;

    bool Save(const std::string &a_path);
    void Blit(int dstx, int dsty, Image &src, int x0, int y0, int width, int height, double xscale = 1, double yscale = 1);
    void Fill(int dstx, int dsty, int width, int height, Pixel color);
    void Blit(int dstx, int dsty, Image &src, double xscale = 1, double yscale = 1)
    {
        Blit(dstx, dsty, src, 0, 0, src.Width(), src.Height(), xscale, yscale);
    }

    int Width()    const { return width; }
    int Height()   const { return height; }
    int Channels() const { return channels; }
    size_t Size()  const { return size; }
    Pixel* Data()        { return  data; }

    Pixel GetPixel(int x, int y)
    {
        if (x < 0 || x >= width) return {};
        if (y < 0 || y >= height) return {};
        return data[width * y + x];
    }

    void  PutPixel(int x, int y, const Pixel &pix)
    {
        if (x < 0 || x >= width) return;
        if (y < 0 || y >= height) return;
        data[width* y + x] = pix;
    }

    ~Image();
};

class Level
{
    static Image background;
    static Image end_back;
    static Image win_back;
    char  level_arr[MAP_HEIGHT][MAP_WIDTH][13]{};
public:
    int x_hero[4][ROOMS], y_hero[4][ROOMS], x_init, y_init;
    char lab_arr[MAZE_SIZE][MAZE_SIZE];
    int enter = 0;
    double spawn_time{};

    int lev_num = 0;
    char getCell(int x, int y) {
        y = (WINDOW_HEIGHT / tileSize) - 1 - y;
        if (x < 0 || x >= MAP_WIDTH) return '#';
        if (y < 0 || y >= MAP_HEIGHT) return '#';
        return level_arr[y][x][lev_num];
    }
    void setCell(int x, int y, char cell) {
        y = (WINDOW_HEIGHT / tileSize) - 1 - y;
        if (x < 0 || x >= MAP_WIDTH) return;
        if (y < 0 || y >= MAP_HEIGHT) return;
        level_arr[y][x][lev_num] = cell;
    }

    void swap(Level &other) {
        std::swap(x_hero, other.x_hero);
        std::swap(y_hero, other.y_hero);
        std::swap(x_init, other.x_init);
        std::swap(y_init, other.y_init);
        std::swap(lab_arr, other.lab_arr);
        std::swap(enter, other.enter);
        std::swap(level_arr, other.level_arr);
        std::swap(lev_num, other.lev_num);
        std::swap(spawn_time, other.spawn_time);
    }

    Level(Level &&other) { swap(other); }
    Level &operator=(Level &&other) { swap(other); return *this; }
    Level(const Level &other) = delete;
    Level &operator=(const Level &other) = delete;

    Level(const std::string &file_name, const std::string &lab_name);
    void Key_Draw (Image & screenBuffer, int key_num);
    void Level_Draw (Image & screenBuffer, bool is_finish, bool is_win);
};


inline static constexpr Pixel Blend(Pixel a, Pixel b) {
    return {
        uint8_t((a.r * (255 - b.a) + b.r * b.a) / 255),
        uint8_t((a.g * (255 - b.a) + b.g * b.a) / 255),
        uint8_t((a.b * (255 - b.a) + b.b * b.a) / 255),
        uint8_t((a.r * (255 - b.a) + b.a) / 255)
    };
}




#endif //MAIN_IMAGE_H
