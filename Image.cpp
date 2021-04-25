#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

Image Level::background{"./resources/back.png"};
Image Level::end_back{"./resources/end_screen.png"};
Image Level::win_back{"./resources/win.png"};

Image::Image(const char *a_path) {
    data = (Pixel*)stbi_load(a_path, &width, &height, &channels, sizeof(Pixel));
    if (data != nullptr) {
        size = width * height * channels;
    }
}

Image::Image(int a_width, int a_height, int a_channels) {
    data = new Pixel[a_width * a_height ]{};

    if (data != nullptr) {
        width = a_width;
        height = a_height;
        size = a_width * a_height * a_channels;
        channels = a_channels;
        self_allocated = true;
    }
}


bool Image::Save(const std::string &a_path)
{
    auto extPos = a_path.find_last_of('.');
    if (a_path.substr(extPos, std::string::npos) == ".png" ||
            a_path.substr(extPos, std::string::npos) == ".PNG") {
        stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
    } else if (a_path.substr(extPos, std::string::npos) == ".jpg" ||
            a_path.substr(extPos, std::string::npos) == ".JPG" ||
            a_path.substr(extPos, std::string::npos) == ".jpeg" ||
            a_path.substr(extPos, std::string::npos) == ".JPEG") {
        stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
    } else {
        std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
        return true;
    }

    return false;
}

Image::~Image()
{
  if(self_allocated) delete [] data;
  else stbi_image_free(data);
}

void Image::Blit(int dstx, int dsty, Image &src, int x0, int y0, int width, int height, double xscale, double yscale)
{
    if (xscale == 1 && yscale == 1) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                auto x_1 = src.GetPixel(x0 + x, y0 + y);
                auto x_2 = GetPixel(dstx + x, dsty + height - y);
                PutPixel(dstx + x, dsty + height - y, Blend(x_2, x_1));
            }
        }
    } else {
        auto sheight = height*yscale, swidth = width*xscale;
        yscale = 1/yscale, xscale = 1/xscale;
        for (int y = 0; y < sheight; y++) {
            for (int x = 0; x < swidth; x++) {
                auto x_1 = src.GetPixel(x0 + x * xscale, y0 + y * yscale);
                auto x_2 = GetPixel(dstx + x, dsty + sheight - y);
                PutPixel(dstx + x, dsty + sheight - y, Blend(x_2, x_1));
            }
        }
    }
}

void Image::Fill(int dstx, int dsty, int width, int height, Pixel color) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                auto x_2 = GetPixel(dstx + x, dsty + height - y);
                PutPixel(dstx + x, dsty + height - y, Blend(x_2, color));
            }
        }
}

Level::Level(const std::string &file_name, const std::string &lab_name)
{
    std::ifstream file;
    file.open(lab_name);
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            char c;
            file >> c;
            lab_arr[i][j] = c;
            if (lab_arr[i][j] == 'A') {
                x_init = j;
                y_init = i;
            }
        }
    }
    file.close();
    spawn_time = glfwGetTime();

    /*std::cout << " MAZE Config: \n";
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++)
        {
            std::cout << lab_arr[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n" << "INIT ROOM: " << lab_arr[y_init][x_init] << "\nCUR LEVEL: " << lev_num <<  "\n\n";
    */
    for (int k = 0; k < ROOMS; k++) {
        file.open(file_name + std::to_string(k) + ".txt");
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                file >> level_arr[i][j][k];
                switch (level_arr[i][j][k]) {
                case '@':
                case 'U':
                    x_hero[0][k] = j;
                    y_hero[0][k] = i + 1;
                    break;
                case 'D':
                    x_hero[1][k] = j;
                    y_hero[1][k] = i + 1;
                    break;
                case 'L':
                    x_hero[2][k] = j;
                    y_hero[2][k] = i + 1;
                    break;
                case 'R':
                    x_hero[3][k] = j;
                    y_hero[3][k] = i + 1;
                    break;
                }
            }
        }
        /*std::cout << "ROOM #" << k << " DOWNLOADED\n";
        for (int i = 0; i < 4; i++)
            std::cout << y_hero[i][k] << " " << x_hero[i][k] << std::endl;
        << " " << level_arr[y_hero[k]][x_hero[k]][k] << "\n";
        */
        file.close();
    }
    /*std::cout << "\n" << "INIT ROOM: " << lab_arr[y_init][x_init] << "\nCUR LEVEL: " << lev_num <<  "\n\n";
    std::cout << "CUR LEV CONFIG: \n";
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            std::cout << level_arr[i][j][lev_num] << " (" << i << " " << j << ") ";
        }
        std::cout << "\n";
    }*/
}

void Level::Level_Draw (Image & screenBuffer, bool is_finish, bool is_win)
{
    int x_add, y_add;
    lev_num = lab_arr[y_init][x_init] - 'A';
    for (int i = MAP_WIDTH * tileSize; i < WINDOW_WIDTH; i++) {
        for (int j = 0; j < WINDOW_HEIGHT; j++) {
            screenBuffer.PutPixel(i, j, backgroundColor);
        }
    }
    for (int i = 0; i < MAP_WIDTH ; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            int xsize = srcSize, ysize = srcSize;
            bool is_blend = false;
            switch (level_arr[j][i][lev_num]) {
            case '_':
                x_add = 3 * srcSize;
                y_add = 7 * srcSize;
                is_blend = true;
                break;
            case 'W':
                x_add = 11 * srcSize;
                y_add = 24 * srcSize;
                is_blend = true;
                break;
            case 'G':
                x_add = 10 * srcSize;
                y_add = 16 * srcSize;

                is_blend = true;
                break;
            case '#':
                x_add = 4 * srcSize;
                y_add = 8 * srcSize;
                is_blend = true;
                if (i == 0) {
                    is_blend = false;
                    x_add = 11.5 * srcSize;
                    if (j == 0 || j == MAP_HEIGHT - 1) {
                        x_add = 22 * srcSize;
                        y_add = 27 * srcSize;
                        is_blend = true;
                    } else {
                        y_add = 41 * srcSize;
                    }
                } else if (i == MAP_HEIGHT - 1) {
                    is_blend = false;
                    x_add = 12.5 * srcSize;
                    if (j == 0 || j == MAP_HEIGHT - 1) {
                        x_add = 22 * srcSize;
                        y_add = 27 * srcSize;
                        is_blend = true;
                    } else {
                        y_add = 41 * srcSize;
                    }

                } else if (j == 0) {
                    is_blend = false;

                    x_add = 14.5 * srcSize;
                    y_add = 41 * srcSize;
                } else if (j == MAP_HEIGHT - 1) {
                    is_blend = false;

                    x_add = srcSize;
                    y_add = 16 * srcSize;
                }
                break;
            case '@':
            case 'L':
            case 'R':
            case 'U':
            case 'D':
            case '.':
                x_add = 2 * srcSize;
                y_add = 32 * srcSize;
                break;
            case 'x':
                if (j == MAP_HEIGHT - 1) {
                    x_add = 9 * srcSize;
                    y_add = 13 * srcSize;
                } else if (!j) {
                    x_add = 29 * srcSize - 1;
                    y_add = 0 * srcSize;
                } else if (!i) {
                    x_add = 28 * srcSize - 1;
                    y_add = 2 * srcSize;

                } else if (i == MAP_WIDTH - 1) {
                    x_add = 29 * srcSize - 1;
                    y_add = 5 * srcSize;
                }
                break;
            case 'Q':
                x_add = 9 * srcSize;
                y_add = 3 * srcSize;
                break;
            case 'K':
                is_blend = true;
                xsize = 12;
                ysize = 8;
                y_add = 0 * srcSize;
                x_add = 30 * srcSize;
                break;
            case 'C':
                is_blend = true;
                if (j == MAP_HEIGHT - 1) {
                    x_add = 10 * srcSize;
                    y_add = 13 * srcSize;
                } else if (!j) {
                    x_add = 28 * srcSize - 1;
                    y_add = 0 * srcSize;
                } else if (!i) {
                    x_add = 28 * srcSize - 1;
                    y_add = 3 * srcSize;
                } else if (i == MAP_WIDTH - 1) {
                    x_add = 29 * srcSize - 1;
                    y_add = 4 * srcSize;
                }
                break;
            }

            if (is_blend) screenBuffer.Blit(i*tileSize, WINDOW_HEIGHT - (j + 1)*tileSize, background,
                                            2*srcSize, 32*srcSize, srcSize, srcSize, (double)tileSize/srcSize, (double)tileSize/srcSize);
            screenBuffer.Blit(i*tileSize, WINDOW_HEIGHT - (j + 1)*tileSize, background,
                              x_add, y_add, xsize, ysize, (double)tileSize/xsize, (double)tileSize/ysize);
        }
    }


    constexpr double fade_duration = 0.8;
    double time = glfwGetTime();
    if (time - spawn_time < fade_duration) {
        Pixel winbackgroundColor{0, 0, 0, uint8_t(255*(1 - (time - spawn_time)/fade_duration))};
        screenBuffer.Fill(0, 0, screenBuffer.Width(), screenBuffer.Height(), winbackgroundColor);

    }

    if (is_finish) {
        screenBuffer.Blit(WINDOW_WIDTH / 2 - ((483 - 105) / 2),
                          WINDOW_HEIGHT - (tileSize * MAP_HEIGHT) / 2 - (430 - 130) / 2, end_back,
                          105, 130, 483-104, 430-130);
    }


    if (is_win) {
        Pixel winbackgroundColor{152, 251, 152, 255};
        screenBuffer.Fill(0, 0, screenBuffer.Width(), screenBuffer.Height(), winbackgroundColor);
        screenBuffer.Blit(WINDOW_WIDTH / 2 - (512 / 2), WINDOW_HEIGHT - (tileSize * MAP_HEIGHT) / 2 - 500 / 2, win_back);
    }
}

void Level::Key_Draw (Image &screenBuffer, int key_num)
{
    double xscale = tileSize/12.0;
    double yscale = tileSize/8.0;
    int y_add = 0 * srcSize;
    int x_add = 30 * srcSize - 1;

    int keys_per_row = (WINDOW_WIDTH / tileSize - MAP_WIDTH);

    for (int k = 0; k < key_num; k++) {
        auto dx = ((k % keys_per_row) + MAP_WIDTH) * tileSize;
        auto dy = WINDOW_HEIGHT - (k / keys_per_row + 1) * tileSize;
        screenBuffer.Blit(dx, dy, background, x_add, y_add, 12, 8, xscale, yscale);
    }
}
