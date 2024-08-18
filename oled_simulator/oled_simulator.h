#ifndef OLED_SIMULATOR_H
#define OLED_SIMULATOR_H

#include <stdint.h>
#include <SDL.h>
//#include <nuklear.h>
//#include <nuklear_sdl_renderer.h>

// 设置oled仿真器的渲染器
void oled_simulator_set_renderer(SDL_Renderer* renderer);
// 设置oled仿真器的缩放倍数
void oled_simulator_set_scale(uint8_t scale);
// 获取oled仿真器的缩放倍数
int* oled_simulator_get_scale();
// 设置oled仿真器的起始位置
void oled_simulator_set_xy(int x, int y);
// 设置oled仿真器的像素宽度和高度
void oled_simulator_set_wh(int width, int height);
// 获取oled仿真器的像素宽度
int* oled_simulator_get_width();
// 获取oled仿真器的像素高度
int* oled_simulator_get_height();
// oled仿真器设置像素缓冲区
void oled_simulator_set_displaybuf(uint8_t (*displaybuf)[]);
// 显示oled仿真器
void oled_simulator_show();

//void oled_simulator_draw_point(struct nk_context* ctx, struct nk_command_buffer* cmd, int x, int y, struct nk_color color);

#endif // !OLED_SIMULATOR_H
