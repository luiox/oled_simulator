/* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <SDL.h>
#include "oled_simulator.h"
#include "../oled/device_oled.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include <nuklear.h>
#include <nuklear_sdl_renderer.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

extern uint8_t OLED_DisplayBuf[8][128];

bool should_hide = false;

int main(int argc, char* argv[])
{
    /* Platform */
    SDL_Window*   win;
    SDL_Renderer* renderer;
    int           running    = 1;
    int           flags      = 0;
    float         font_scale = 1;

    /* GUI */
    struct nk_context* ctx;
    struct nk_colorf   bg;

    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Demo",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           WINDOW_WIDTH,
                           WINDOW_HEIGHT,
                           SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (win == NULL) {
        SDL_Log("Error SDL_CreateWindow %s", SDL_GetError());
        exit(-1);
    }

    flags |= SDL_RENDERER_ACCELERATED;
    flags |= SDL_RENDERER_PRESENTVSYNC;


    renderer = SDL_CreateRenderer(win, -1, flags);

    if (renderer == NULL) {
        SDL_Log("Error SDL_CreateRenderer %s", SDL_GetError());
        exit(-1);
    }

    /* scale the renderer output for High-DPI displays */
    {
        int   render_w, render_h;
        int   window_w, window_h;
        float scale_x, scale_y;
        SDL_GetRendererOutputSize(renderer, &render_w, &render_h);
        SDL_GetWindowSize(win, &window_w, &window_h);
        scale_x = (float)(render_w) / (float)(window_w);
        scale_y = (float)(render_h) / (float)(window_h);
        SDL_RenderSetScale(renderer, scale_x, scale_y);
        font_scale = scale_y;
    }

    /* GUI */
    ctx = nk_sdl_init(win, renderer);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {
        struct nk_font_atlas* atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font*       font;

        /* set up the font atlas and add desired font; note that font sizes are
         * multiplied by font_scale to produce better results at higher DPIs */
        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14 *
         * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16 *
         * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf",
         * 13 * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12 *
         * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10 *
         * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13 *
         * font_scale, &config);*/
        nk_sdl_font_stash_end();

        /* this hack makes the font appear to be scaled down to the desired
         * size and is only necessary when font_scale > 1 */
        font->handle.height /= font_scale;
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(ctx, &font->handle);
    }

    // 设置渲染器
    oled_simulator_set_renderer(renderer);
    
    // 设置oled的像素缓冲区
    oled_simulator_set_displaybuf(OLED_DisplayBuf);
    
    // 初始化oled
    OLED_Init();

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (running) {
        /* Input */
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            if (evt.type == SDL_KEYDOWN) {
                // 检查是否为右边Shift键
                if (evt.key.keysym.sym == SDLK_RSHIFT) {
                    should_hide = !should_hide;
                    printf("Right Shift key pressed!\n");
                }
            }
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);

        if (should_hide) goto skip;

        /* GUI */
        if (nk_begin(ctx,
                     "Setting",
                     nk_rect(500, 300, 230, 250),
                     NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                         NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
            // oled 屏幕大小
            static int oled_width = 128, oled_height = 64;
            static int oled_scale = 4;   // 缩放倍数
            static int oled_x = 0, oled_y = 0;

            // 绘制设置
            nk_layout_row_static(ctx, 30, 100, 2);
            nk_label(ctx, "Width:", NK_TEXT_LEFT);
            nk_property_int(ctx, "", 32, &oled_width, 1024, 1, 1);

            nk_layout_row_static(ctx, 30, 100, 2);
            nk_label(ctx, "Height:", NK_TEXT_LEFT);
            nk_property_int(ctx, "", 32, &oled_height, 1024, 1, 1);

            nk_layout_row_static(ctx, 30, 100, 2);
            nk_label(ctx, "Scale:", NK_TEXT_LEFT);
            nk_property_int(ctx, "", 1, &oled_scale, 16, 1, 1);

            nk_layout_row_static(ctx, 30, 100, 2);
            nk_label(ctx, "X:", NK_TEXT_LEFT);
            nk_property_int(ctx, "", 0, &oled_x, 1024, 1, 1);

            nk_layout_row_static(ctx, 30, 100, 2);
            nk_label(ctx, "Y:", NK_TEXT_LEFT);
            nk_property_int(ctx, "", 0, &oled_y, 1024, 1, 1);

            // 设置scale
            oled_simulator_set_scale(oled_scale);
            // 设置oled的宽高
            oled_simulator_set_wh(oled_width, oled_height);
            // 设置oled的起始位置
            oled_simulator_set_xy(oled_x, oled_y);

        }
        nk_end(ctx);
    skip:
        /* ----------------------------------------- */

        SDL_SetRenderDrawColor(renderer, bg.r * 255, bg.g * 255, bg.b * 255, bg.a * 255);
        SDL_RenderClear(renderer);

        nk_sdl_render(NK_ANTI_ALIASING_ON);

        // oled的绘制要在nukler之后 

        // 测试代码
        //OLED_ShowString(0, 0, "Hello, World!", OLED_6X8);
        //OLED_ShowString(0, 8, "Hello, World!", OLED_6X8);
        //OLED_ShowString(0, 16, "Hello, World!", OLED_6X8);
        //OLED_ShowString(0, 24, "Hello, World!", OLED_6X8);
        //OLED_ShowString(0, 32, "Hello, World!", OLED_6X8);
        //OLED_ShowString(0, 40, "Hello, World!", OLED_6X8);
        //OLED_ShowString(0, 48, "Hello, World!", OLED_6X8);
        //OLED_ShowString(0, 56, "123456789101112131415", OLED_6X8);

        // 菜单绘制
        OLED_ShowString(2, 2, "-option1", OLED_8X16);
        OLED_ShowString(0, 16, "-option2", OLED_8X16);
        OLED_ShowString(0, 32, "-option3", OLED_8X16);
        OLED_ShowString(0, 48, "-option4", OLED_8X16);

        // 绘制当前选中的矩形
        OLED_DrawRectangle(0, 0, 128, 18, OLED_UNFILLED);

        // 通过仿真器显示缓存数组
        oled_simulator_show();

        

        //// 测试的代码
        //for (uint8_t k = 0; k < 100; k++) {
        //    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //    SDL_RenderDrawPoint(renderer, 200, k);
        //}

        SDL_RenderPresent(renderer);
    }

cleanup:
    nk_sdl_shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
