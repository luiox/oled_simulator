#include "oled_simulator.h"
#include <SDL.h>

SDL_Renderer* g_renderer = NULL;
int           g_scale    = 4;
int g_x = 0;
    int g_y = 0;
uint8_t**g_displaybuf;
    static int g_oled_width = 128, g_oled_height = 64;

    void oled_simulator_set_renderer(SDL_Renderer* renderer)
{
    g_renderer = renderer;
}

void oled_simulator_set_scale(uint8_t scale)
{
    g_scale = scale;
}

int* oled_simulator_get_scale()
{
    	return &g_scale;

}

// 设置oled仿真器的起始位置
void oled_simulator_set_xy(int x, int y)
{
    g_x = x;
	g_y = y;
}

// 绘制矩形，考虑到scale
static void draw_rect_ex(SDL_Renderer* renderer, SDL_Rect rect, int scale)
{
    for (int i = 0; i < scale; i++) {
        SDL_RenderDrawRect(renderer, &rect);
        rect.x++;
        rect.y++;
        rect.w -= 2;
        rect.h -= 2;
    }
}

static void draw_point_ex(SDL_Renderer* renderer, int x, int y, int scale)
{
	for (int i = 0; i < scale; i++) {
		for (int j = 0; j < scale; j++) {
			SDL_RenderDrawPoint(renderer, x + i, y + j);
		}
	}
}

void oled_simulator_set_wh(int width, int height)
{
	g_oled_width = width;
	g_oled_height = height;

}
// 获取oled仿真器的像素宽度
int* oled_simulator_get_width()
{

    return &g_oled_width;

}
// 获取oled仿真器的像素高度
int* oled_simulator_get_height()
{
    	return &g_oled_height;

}

void oled_simulator_set_displaybuf(uint8_t (*displaybuf)[])
{
    	g_displaybuf = displaybuf;

}

// 显示oled仿真器
void oled_simulator_show()
{
    //// 绘制边框
    //SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 255);   // 设置为蓝色
    //draw_rect_ex(g_renderer,
    //             (SDL_Rect){g_x, g_y, g_oled_width * g_scale, 
    //    g_oled_height * g_scale}, g_scale);

     //绘制缓存数组
    // 遍历OLED_DisplayBuf，将每个点放大scale*scale个像素点
    // height是位压缩

    // 先遍历行
    for (int32_t i = 0; i < g_oled_height/8; i++) {
        // 遍历位压缩下的每一位
        for (int32_t j = 0; j < 8; j++) {
            // 遍历列
            for (int32_t k = 0; k < g_oled_width; k++) {
                uint8_t* p   = (uint8_t*)g_displaybuf;
                int      val = *(p + i * g_oled_width + k) & (1 << j);
                if (val == 0) {
                    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
                }
                else {
                    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
                }
                draw_point_ex(g_renderer, k * g_scale, 
                    (i * 8 + j) * g_scale, 
                    g_scale);
            }
        }
    }

}

//void oled_simulator_show(uint8_t** display_buf, uint8_t width, uint8_t height)
//{
//    if (g_renderer == NULL) {
//        return;
//    }
//
//    // 绘制边框
//    SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 255);   // 设置为蓝色
//    draw_rect_ex(g_renderer, (SDL_Rect){g_x, g_y, width * g_scale, height * g_scale}, g_border_width);
//
//    return;
//    // 遍历OLED_DisplayBuf，将每个点放大scale*scale个像素点
//    // height是位压缩
//    for (uint8_t i = 0; i < height; i++) {
//        int idx_i = i / 8;   // OLED_DisplayBuf的行数
//        for (uint8_t j = 0; j < 128; j++) {
//            // printf("%p", OLED_DisplayBuf);
//            for (uint8_t k = 0; k < 8; k++) {
//                if ((display_buf[i][j] & 0xff) >> k == 0) {
//                    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
//                }
//                else {
//                    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
//                }
//                for (uint8_t k = 0; k < g_scale; k++) {
//                    for (uint8_t l = 0; l < g_scale; l++) {
//                        SDL_RenderDrawPoint(g_renderer, j * g_scale + l, i * g_scale + k);
//                    }
//                }
//            }
//        }
//    }
//
//    // for (uint8_t i = 0; i < height; i++)
//    //{
//    //	for (uint8_t j = 0; j < width; j++)
//    //	{
//    //		printf("%p", display_buf);
//    //		if (display_buf[i][j] == 0)
//    //		{
//    //			SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
//    //		}
//    //		else
//    //		{
//    //			SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
//    //		}
//    //		for (uint8_t k = 0; k < g_scale; k++)
//    //		{
//    //			for (uint8_t l = 0; l < g_scale; l++)
//    //			{
//    //				SDL_RenderDrawPoint(g_renderer, j * g_scale + l, i * g_scale + k);
//    //			}
//    //		}
//    //	}
//    // }
//}

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
