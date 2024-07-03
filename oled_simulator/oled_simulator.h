#ifndef OLED_SIMULATOR_H
#define OLED_SIMULATOR_H

#include <stdint.h>
#include <SDL.h>
//#include <nuklear.h>
//#include <nuklear_sdl_renderer.h>

void oled_simulator_set_renderer(SDL_Renderer* renderer);

void oled_simulator_set_scale(uint8_t scale);

void oled_simulator_show(uint8_t** display_buf, uint8_t width, uint8_t height);

//void oled_simulator_draw_point(struct nk_context* ctx, struct nk_command_buffer* cmd, int x, int y, struct nk_color color);

#endif // !OLED_SIMULATOR_H
