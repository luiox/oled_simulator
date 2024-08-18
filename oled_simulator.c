/*******************************************************************************
** 文件名称：oled_simulator.c
** 文件作用：OLED模拟器
** 编写作者：Tom Free 付瑞彪
** 编写时间：2020-06-24
** 文件备注：OLED模拟器源文件
**			 
**			 
** 更新记录：
**          2020-06-24 -> 创建文件                          <Tom Free 付瑞彪>
**
**
**       Copyright (c) 深圳市三派智能科技有限公司 All Rights Reserved
**
**       1 Tab == 4 Spaces     UTF-8     ANSI C Language(C99)
*******************************************************************************/

#include "oled_simulator.h"
#include "sdl.h"
#include "windows.h"

/* 按键定义 */
#define OLED_KEY_VALUE_NONE         (0x0000u)
#define OLED_KEY_VALUE_BACKSPACE    (0x0008u)
#define OLED_KEY_VALUE_TAB          (0x0009u)
#define OLED_KEY_VALUE_ENTER        (0x000Du)
#define OLED_KEY_VALUE_ESC          (0x001Bu)
#define OLED_KEY_VALUE_SPACE        (0x0020u)
#define OLED_KEY_VALUE_DEL          (0x007Fu)
#define OLED_KEY_VALUE_LEFT         (0x013Au)
#define OLED_KEY_VALUE_UP           (0x013Bu)
#define OLED_KEY_VALUE_RIGHT        (0x013Cu)
#define OLED_KEY_VALUE_DOWN         (0x013Du)
#define OLED_KEY_VALUE_INSERT       (0x0142u)
#define OLED_KEY_VALUE_F1           (0x0154u)
#define OLED_KEY_VALUE_F2           (0x0155u)
#define OLED_KEY_VALUE_F3           (0x0156u)
#define OLED_KEY_VALUE_F4           (0x0157u)
#define OLED_KEY_VALUE_F5           (0x0158u)
#define OLED_KEY_VALUE_F6           (0x0159u)
#define OLED_KEY_VALUE_F7           (0x015Au)
#define OLED_KEY_VALUE_F8           (0x015Bu)
#define OLED_KEY_VALUE_F9           (0x015Cu)
#define OLED_KEY_VALUE_F10          (0x015Du)
#define OLED_KEY_VALUE_F11          (0x015Eu)
#define OLED_KEY_VALUE_F12          (0x015Fu)
#define OLED_KEY_VALUE_ENTER_PAD    (0x0172u)
/* 控制键 */
#define OLED_KEY_OPTION_CTRL        (0x1000u)
#define OLED_KEY_OPTION_ALT         (0x2000u)
#define OLED_KEY_OPTION_SHIFT       (0x4000u)
/* 组合键值分离 */
#define OLED_KEY_CODE_VALUE(CODE)   (CODE & 0x0FFFu)
#define OLED_KEY_CODE_OPT(CODE)     (CODE & 0xF000u)

/* 一个像素宽度 */
#define OLED_PIXEL_WIDTH    ((OLED_POINT_WIDTH) + (OLED_LINE_WIDTH))
/* 屏幕实际尺寸 */
#define OLED_REAL_WIDTH     ((OLED_WIDTH) * (OLED_PIXEL_WIDTH) + (OLED_LINE_WIDTH))
#define OLED_REAL_HEIGHT    ((OLED_HEIGHT) * (OLED_PIXEL_WIDTH) + (OLED_LINE_WIDTH))

static SDL_Thread* thread_ptr;
/* 窗口 */
static SDL_Window* window_ptr;
/* 渲染器 */
static SDL_Renderer* render_ptr;
/* 纹理 */
static SDL_Texture* texture_ptr;
/* 显存 */
static int oled_simulator_buf[OLED_REAL_WIDTH * OLED_REAL_HEIGHT];
/* 按键事件 */
static int key_event_flag = 0;
/* 按键键值 */
static int key_event_value = 0;
/* 退出标志 */
static int sdl_quit_flag = 0;

/*******************************************************************************
** 函数名称：OLED_Simulator_FillColor
** 函数作用：颜色填充到buf
** 输入参数：buf - 显存，color - 颜色，length - 长度
** 输出参数：无
** 使用范例：OLED_Simulator_FillColor(oled_simulator_buf, 0xFFFFFF00, 1024);
** 函数备注：
*******************************************************************************/
static void OLED_Simulator_FillColor(int* buf, int color, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        buf[i] = color;
    }
}

/*******************************************************************************
** 函数名称：OLED_Simulator_PollEvent
** 函数作用：事件读取线程
** 输入参数：无
** 输出参数：无
** 使用范例：OLED_Simulator_PollEvent();
** 函数备注：
*******************************************************************************/
static void OLED_Simulator_PollEvent(void)
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            key_event_flag = 1;
            key_event_value = event.key.keysym.sym;
            break;

        case SDL_QUIT:
            sdl_quit_flag = 1;
            break;

        default:
            break;
        }
    }
}

/*******************************************************************************
** 函数名称：OLED_Simulator_Init
** 函数作用：初始化模拟器
** 输入参数：无
** 输出参数：无
** 使用范例：OLED_Simulator_Init();
** 函数备注：
*******************************************************************************/
static void OLED_Simulator_Init(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        MessageBox(NULL, "SDL2 initialize fail! System will exit.", "ERROR", MB_OK);
        exit(0);
    }

    /* 创建window */
    window_ptr = SDL_CreateWindow("OLED Simulator - SDL2",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  OLED_REAL_WIDTH,
                                  OLED_REAL_HEIGHT,
                                  SDL_WINDOW_SHOWN);
    /* 创建render */
    render_ptr = SDL_CreateRenderer(window_ptr, 
                                    -1, 
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    /* 创建texture，注意像素格式和访问方式 */
    texture_ptr = SDL_CreateTexture(render_ptr, 
                                    SDL_PIXELFORMAT_RGBA8888, 
                                    SDL_TEXTUREACCESS_STREAMING,
                                    OLED_REAL_WIDTH,
                                    OLED_REAL_HEIGHT);
    
    /* 清除屏幕并显示 */
    OLED_Simulator_Clear();
    OLED_Simulator_Flush();
}

/*******************************************************************************
** 函数名称：OLED_Simulator_UnInit
** 函数作用：销毁模拟器
** 输入参数：无
** 输出参数：无
** 使用范例：OLED_Simulator_UnInit();
** 函数备注：
*******************************************************************************/
static void OLED_Simulator_UnInit(void)
{
    /* 释放窗口资源 */
    SDL_DestroyTexture(texture_ptr);
    SDL_DestroyRenderer(render_ptr);
    SDL_DestroyWindow(window_ptr);

    /* 清除SDL资源 */
    SDL_Quit();
}

/*******************************************************************************
** 函数名称：OLED_Simulator_Setpixel
** 函数作用：写像素
** 输入参数：x,y - 坐标，color - 颜色，0 - 背景，1 - 前景
** 输出参数：无
** 使用范例：OLED_Simulator_Setpixel(10, 45, 1);
** 函数备注：
*******************************************************************************/
void OLED_Simulator_Setpixel(int x, int y, int color)
{
    int i, j;
    x = x * OLED_PIXEL_WIDTH + OLED_LINE_WIDTH;
    y = y * OLED_PIXEL_WIDTH + OLED_LINE_WIDTH;
    color = color ? OLED_FOREGROUND_COLOR : OLED_BACKGROUND_COLOR;
    for (i = y; i < y + OLED_POINT_WIDTH; i++)
    {
        for (j = x; j < x + OLED_POINT_WIDTH; j++)
        {
            oled_simulator_buf[j + i * OLED_REAL_WIDTH] = color;
        }
    }
}

/*******************************************************************************
** 函数名称：OLED_Simulator_Clear
** 函数作用：清除显存
** 输入参数：无
** 输出参数：无
** 使用范例：OLED_Simulator_Clear();
** 函数备注：
*******************************************************************************/
void OLED_Simulator_Clear(void)
{
    int i, j;
    OLED_Simulator_FillColor(oled_simulator_buf, OLED_SPACELINE_COLOR, OLED_REAL_HEIGHT * OLED_REAL_WIDTH);
    for (i = 0; i < OLED_HEIGHT; i++)
    {
        for (j = 0; j < OLED_WIDTH; j++)
        {
            OLED_Simulator_Setpixel(j, i, OLED_BACKGROUND_COLOR);
        }
    }
}

/*******************************************************************************
** 函数名称：OLED_Simulator_GetPixel
** 函数作用：读像素
** 输入参数：x,y - 坐标
** 输出参数：无
** 使用范例：OLED_Simulator_GetPixel(12, 45);
** 函数备注：
*******************************************************************************/
int OLED_Simulator_GetPixel(int x, int y)
{
    x = x * OLED_PIXEL_WIDTH + OLED_LINE_WIDTH;
    y = y * OLED_PIXEL_WIDTH + OLED_LINE_WIDTH;
    if (oled_simulator_buf[y * OLED_REAL_WIDTH + x] == OLED_BACKGROUND_COLOR)
    {
        return 0;
    }
    {
        return 1;
    }
}

/*******************************************************************************
** 函数名称：OLED_Simulator_Flush
** 函数作用：刷新显存到屏幕
** 输入参数：无
** 输出参数：无
** 使用范例：OLED_Simulator_Flush();
** 函数备注：
*******************************************************************************/
void OLED_Simulator_Flush(void)
{
    void* pix;
    int pitch;
    int i;

    if (SDL_LockTexture(texture_ptr, NULL, &pix, &pitch)) return;
    for (i = 0; i <= (OLED_REAL_WIDTH * OLED_REAL_HEIGHT); i++)
    {
        ((Uint32*)pix)[i] = oled_simulator_buf[i];
    }
    SDL_UnlockTexture(texture_ptr);

    SDL_RenderCopy(render_ptr, texture_ptr, NULL, NULL);
    SDL_RenderPresent(render_ptr);
}

/*******************************************************************************
** 函数名称：OLED_Simulator_GetTicks
** 函数作用：读取ticks
** 输入参数：无
** 输出参数：ticks，32位
** 使用范例：OLED_Simulator_GetTicks();
** 函数备注：
*******************************************************************************/
int OLED_Simulator_GetTicks(void)
{
    return SDL_GetTicks();
}

/*******************************************************************************
** 函数名称：OLED_Simulator_GetKeyEvent
** 函数作用：获取事件
** 输入参数：无
** 输出参数：是否有事件
** 使用范例：OLED_Simulator_GetKeyEvent();
** 函数备注：
*******************************************************************************/
int OLED_Simulator_GetKeyEvent(void)
{
    if (key_event_flag > 0)
    {
        key_event_flag = 0;
        return 1;
    }
    return 0;
}

/*******************************************************************************
** 函数名称：OLED_Simulator_GetKeyValue
** 函数作用：读取键值
** 输入参数：无
** 输出参数：键值
** 使用范例：OLED_Simulator_GetKeyValue();
** 函数备注：
*******************************************************************************/
int OLED_Simulator_GetKeyValue(void)
{
    int key_value;

    /* 值转换 */
    if (key_event_value == SDLK_UP)
    {
        key_value = OLED_KEY_VALUE_UP;
    }
    else if (key_event_value == SDLK_DOWN)
    {
        key_value = OLED_KEY_VALUE_DOWN;
    }
    else if (key_event_value == SDLK_LEFT)
    {
        key_value = OLED_KEY_VALUE_LEFT;
    }
    else if (key_event_value == SDLK_RIGHT)
    {
        key_value = OLED_KEY_VALUE_RIGHT;
    }
    else if (key_event_value == SDLK_KP_ENTER)
    {
        key_value = OLED_KEY_VALUE_ENTER;
    }
    else if (key_event_value == SDLK_ESCAPE)
    {
        key_value = OLED_KEY_VALUE_ESC;
    }
    else if (key_event_value == SDLK_TAB)
    {
        key_value = OLED_KEY_VALUE_TAB;
    }
    else if (key_event_value == SDLK_SPACE)
    {
        key_value = OLED_KEY_VALUE_SPACE;
    }
    else
    {
        key_value = OLED_KEY_VALUE_NONE;
    }

    return key_value;
}

/*******************************************************************************
** 函数名称：OLED_Simulator_Run
** 函数作用：运行
** 输入参数：task - 任务函数
** 输出参数：无
** 使用范例：OLED_Simulator_Run(task);
** 函数备注：
*******************************************************************************/
void OLED_Simulator_Run(void(*task)(void))
{
    /* 初始化 */
    OLED_Simulator_Init();

    /* 创建任务处理线程 */
    thread_ptr = SDL_CreateThread((SDL_ThreadFunction)task, "Task", NULL);

    for (;;)
    {
        /* 事件循环 */
        OLED_Simulator_PollEvent();
        /* 查看是否退出 */
        if (sdl_quit_flag == 1)
        {
            break;
        }
    }

    /* 销毁 */
    OLED_Simulator_UnInit();
}
