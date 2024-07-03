#include "oled_simulator.h"
#include <SDL.h>

SDL_Renderer* g_renderer = NULL;
int           g_scale    = 1;

void oled_simulator_set_renderer(SDL_Renderer* renderer)
{
    g_renderer = renderer;
}

void oled_simulator_set_scale(uint8_t scale)
{
    g_scale = scale;
}

extern uint8_t OLED_DisplayBuf[8][128];
void           oled_simulator_show(uint8_t** display_buf, uint8_t width, uint8_t height)
{
    if (g_renderer == NULL) {
        return;
    }

    // 遍历OLED_DisplayBuf，将每个点放大scale*scale个像素点
    // height是位压缩
    for (uint8_t i = 0; i < height; i++) {
        int idx_i = i / 8;   // OLED_DisplayBuf的行数
        for (uint8_t j = 0; j < 128; j++) {
            // printf("%p", OLED_DisplayBuf);
            for (uint8_t k = 0; k < 8; k++) {
                if ((OLED_DisplayBuf[i][j] & 0xff) >> k == 0) {
                    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
                }
                else {
                    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
                }
                for (uint8_t k = 0; k < g_scale; k++) {
                    for (uint8_t l = 0; l < g_scale; l++) {
                        SDL_RenderDrawPoint(g_renderer, j * g_scale + l, i * g_scale + k);
                    }
                }
            }
        }
    }

    // for (uint8_t i = 0; i < height; i++)
    //{
    //	for (uint8_t j = 0; j < width; j++)
    //	{
    //		printf("%p", display_buf);
    //		if (display_buf[i][j] == 0)
    //		{
    //			SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    //		}
    //		else
    //		{
    //			SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    //		}
    //		for (uint8_t k = 0; k < g_scale; k++)
    //		{
    //			for (uint8_t l = 0; l < g_scale; l++)
    //			{
    //				SDL_RenderDrawPoint(g_renderer, j * g_scale + l, i * g_scale + k);
    //			}
    //		}
    //	}
    // }
}

// void oled_simulator_draw_point(struct nk_context* ctx, struct nk_command_buffer* cmd, int x, int
// y, struct nk_color color)
//{
//	/* 设置绘制颜色 */
//	nk_command_buffer_push_color(cmd, color);
//
//	/* 绘制一个单像素点 */
//	nk_draw_list_push_point(cmd, x, y, 1.0f);
//
//	/* 提交命令到命令缓冲区 */
//	nk_flush(ctx, cmd);
// }
