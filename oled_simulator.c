/*******************************************************************************
** �ļ����ƣ�oled_simulator.c
** �ļ����ã�OLEDģ����
** ��д���ߣ�Tom Free �����
** ��дʱ�䣺2020-06-24
** �ļ���ע��OLEDģ����Դ�ļ�
**			 
**			 
** ���¼�¼��
**          2020-06-24 -> �����ļ�                          <Tom Free �����>
**
**
**       Copyright (c) �������������ܿƼ����޹�˾ All Rights Reserved
**
**       1 Tab == 4 Spaces     UTF-8     ANSI C Language(C99)
*******************************************************************************/

#include "oled_simulator.h"
#include "sdl.h"
#include "windows.h"

/* �������� */
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
/* ���Ƽ� */
#define OLED_KEY_OPTION_CTRL        (0x1000u)
#define OLED_KEY_OPTION_ALT         (0x2000u)
#define OLED_KEY_OPTION_SHIFT       (0x4000u)
/* ��ϼ�ֵ���� */
#define OLED_KEY_CODE_VALUE(CODE)   (CODE & 0x0FFFu)
#define OLED_KEY_CODE_OPT(CODE)     (CODE & 0xF000u)

/* һ�����ؿ�� */
#define OLED_PIXEL_WIDTH    ((OLED_POINT_WIDTH) + (OLED_LINE_WIDTH))
/* ��Ļʵ�ʳߴ� */
#define OLED_REAL_WIDTH     ((OLED_WIDTH) * (OLED_PIXEL_WIDTH) + (OLED_LINE_WIDTH))
#define OLED_REAL_HEIGHT    ((OLED_HEIGHT) * (OLED_PIXEL_WIDTH) + (OLED_LINE_WIDTH))

static SDL_Thread* thread_ptr;
/* ���� */
static SDL_Window* window_ptr;
/* ��Ⱦ�� */
static SDL_Renderer* render_ptr;
/* ���� */
static SDL_Texture* texture_ptr;
/* �Դ� */
static int oled_simulator_buf[OLED_REAL_WIDTH * OLED_REAL_HEIGHT];
/* �����¼� */
static int key_event_flag = 0;
/* ������ֵ */
static int key_event_value = 0;
/* �˳���־ */
static int sdl_quit_flag = 0;

/*******************************************************************************
** �������ƣ�OLED_Simulator_FillColor
** �������ã���ɫ��䵽buf
** ���������buf - �Դ棬color - ��ɫ��length - ����
** �����������
** ʹ�÷�����OLED_Simulator_FillColor(oled_simulator_buf, 0xFFFFFF00, 1024);
** ������ע��
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
** �������ƣ�OLED_Simulator_PollEvent
** �������ã��¼���ȡ�߳�
** �����������
** �����������
** ʹ�÷�����OLED_Simulator_PollEvent();
** ������ע��
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
** �������ƣ�OLED_Simulator_Init
** �������ã���ʼ��ģ����
** �����������
** �����������
** ʹ�÷�����OLED_Simulator_Init();
** ������ע��
*******************************************************************************/
static void OLED_Simulator_Init(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        MessageBox(NULL, "SDL2 initialize fail! System will exit.", "ERROR", MB_OK);
        exit(0);
    }

    /* ����window */
    window_ptr = SDL_CreateWindow("OLED Simulator - SDL2",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  OLED_REAL_WIDTH,
                                  OLED_REAL_HEIGHT,
                                  SDL_WINDOW_SHOWN);
    /* ����render */
    render_ptr = SDL_CreateRenderer(window_ptr, 
                                    -1, 
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    /* ����texture��ע�����ظ�ʽ�ͷ��ʷ�ʽ */
    texture_ptr = SDL_CreateTexture(render_ptr, 
                                    SDL_PIXELFORMAT_RGBA8888, 
                                    SDL_TEXTUREACCESS_STREAMING,
                                    OLED_REAL_WIDTH,
                                    OLED_REAL_HEIGHT);
    
    /* �����Ļ����ʾ */
    OLED_Simulator_Clear();
    OLED_Simulator_Flush();
}

/*******************************************************************************
** �������ƣ�OLED_Simulator_UnInit
** �������ã�����ģ����
** �����������
** �����������
** ʹ�÷�����OLED_Simulator_UnInit();
** ������ע��
*******************************************************************************/
static void OLED_Simulator_UnInit(void)
{
    /* �ͷŴ�����Դ */
    SDL_DestroyTexture(texture_ptr);
    SDL_DestroyRenderer(render_ptr);
    SDL_DestroyWindow(window_ptr);

    /* ���SDL��Դ */
    SDL_Quit();
}

/*******************************************************************************
** �������ƣ�OLED_Simulator_Setpixel
** �������ã�д����
** ���������x,y - ���꣬color - ��ɫ��0 - ������1 - ǰ��
** �����������
** ʹ�÷�����OLED_Simulator_Setpixel(10, 45, 1);
** ������ע��
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
** �������ƣ�OLED_Simulator_Clear
** �������ã�����Դ�
** �����������
** �����������
** ʹ�÷�����OLED_Simulator_Clear();
** ������ע��
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
** �������ƣ�OLED_Simulator_GetPixel
** �������ã�������
** ���������x,y - ����
** �����������
** ʹ�÷�����OLED_Simulator_GetPixel(12, 45);
** ������ע��
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
** �������ƣ�OLED_Simulator_Flush
** �������ã�ˢ���Դ浽��Ļ
** �����������
** �����������
** ʹ�÷�����OLED_Simulator_Flush();
** ������ע��
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
** �������ƣ�OLED_Simulator_GetTicks
** �������ã���ȡticks
** �����������
** ���������ticks��32λ
** ʹ�÷�����OLED_Simulator_GetTicks();
** ������ע��
*******************************************************************************/
int OLED_Simulator_GetTicks(void)
{
    return SDL_GetTicks();
}

/*******************************************************************************
** �������ƣ�OLED_Simulator_GetKeyEvent
** �������ã���ȡ�¼�
** �����������
** ����������Ƿ����¼�
** ʹ�÷�����OLED_Simulator_GetKeyEvent();
** ������ע��
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
** �������ƣ�OLED_Simulator_GetKeyValue
** �������ã���ȡ��ֵ
** �����������
** �����������ֵ
** ʹ�÷�����OLED_Simulator_GetKeyValue();
** ������ע��
*******************************************************************************/
int OLED_Simulator_GetKeyValue(void)
{
    int key_value;

    /* ֵת�� */
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
** �������ƣ�OLED_Simulator_Run
** �������ã�����
** ���������task - ������
** �����������
** ʹ�÷�����OLED_Simulator_Run(task);
** ������ע��
*******************************************************************************/
void OLED_Simulator_Run(void(*task)(void))
{
    /* ��ʼ�� */
    OLED_Simulator_Init();

    /* �����������߳� */
    thread_ptr = SDL_CreateThread((SDL_ThreadFunction)task, "Task", NULL);

    for (;;)
    {
        /* �¼�ѭ�� */
        OLED_Simulator_PollEvent();
        /* �鿴�Ƿ��˳� */
        if (sdl_quit_flag == 1)
        {
            break;
        }
    }

    /* ���� */
    OLED_Simulator_UnInit();
}
