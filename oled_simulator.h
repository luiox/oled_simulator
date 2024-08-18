/*******************************************************************************
** 文件名称：oled_simulator.h
** 文件作用：OLED模拟器
** 编写作者：Tom Free 付瑞彪
** 编写时间：2020-06-24
** 文件备注：OLED模拟器头文件，可以配置模拟器的相关参数
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
#ifndef __OLED_SIMULATOR_H__
#define __OLED_SIMULATOR_H__

/* 预设颜色模式选项 */
#define OLED_COLOR_USER_MODE        (0u)
#define OLED_COLOR_SSD1306_WHITE    (1u)
#define OLED_COLOR_SSD1306_BLUE     (2u)
#define OLED_COLOR_LCD12864         (3u)

/* 颜色混色定义 */
#define OLED_COLOR_RGB(R, G, B)     ((R << 24) | (G << 16) | (B << 8))

/* OLED尺寸 */
#define OLED_WIDTH                  (128u)
#define OLED_HEIGHT                 (64u)

/* 显示一个像素亮点的宽度 */
#define OLED_POINT_WIDTH            (6u)
/* 显示一个像素的间隔线的宽度 */
#define OLED_LINE_WIDTH             (2u)

/* 颜色模式 */
#define OLED_COLOR_MODE             OLED_COLOR_SSD1306_WHITE

/* 预设颜色定义 */
#if (OLED_COLOR_MODE == OLED_COLOR_SSD1306_WHITE)
/* OLED背景颜色定义 */
#define OLED_BACKGROUND_COLOR       OLED_COLOR_RGB(0, 0, 0)
/* OLED前景颜色定义 */
#define OLED_FOREGROUND_COLOR       OLED_COLOR_RGB(255, 255, 255)
/* OLED间隔线颜色定义 */
#define OLED_SPACELINE_COLOR        OLED_COLOR_RGB(10, 10, 10)
#elif (OLED_COLOR_MODE == OLED_COLOR_SSD1306_BLUE)
/* OLED背景颜色定义 */
#define OLED_BACKGROUND_COLOR       OLED_COLOR_RGB(10, 10, 10)
/* OLED前景颜色定义 */
#define OLED_FOREGROUND_COLOR       OLED_COLOR_RGB(10, 10, 200)
/* OLED间隔线颜色定义 */
#define OLED_SPACELINE_COLOR        OLED_COLOR_RGB(10, 10, 10)
#elif (OLED_COLOR_MODE == OLED_COLOR_LCD12864)
/* OLED背景颜色定义 */
#define OLED_BACKGROUND_COLOR       OLED_COLOR_RGB(0, 0, 0)
/* OLED前景颜色定义 */
#define OLED_FOREGROUND_COLOR       OLED_COLOR_RGB(255, 255, 255)
/* OLED间隔线颜色定义 */
#define OLED_SPACELINE_COLOR        OLED_COLOR_RGB(10, 10, 10)
#elif (OLED_COLOR_MODE == OLED_COLOR_USER_MODE)
#error please set color
/* OLED背景颜色定义 */
#define OLED_BACKGROUND_COLOR       //OLED_COLOR_RGB(0, 0, 0)
/* OLED前景颜色定义 */
#define OLED_FOREGROUND_COLOR       //OLED_COLOR_RGB(255, 255, 255)
/* OLED间隔线颜色定义 */
#define OLED_SPACELINE_COLOR        //OLED_COLOR_RGB(10, 10, 10)
#endif  /* OLED_COLOR_MODE == OLED_COLOR_SSD1306_WHITE */

/*******************************************************************************
** 函数名称：OLED_Simulator_Setpixel
** 函数作用：写像素
** 输入参数：x,y - 坐标，color - 颜色，0 - 背景，1 - 前景
** 输出参数：无
** 使用范例：OLED_Simulator_Setpixel(10, 45, 1);
** 函数备注：
*******************************************************************************/
void OLED_Simulator_Setpixel(int x, int y, int color);

/*******************************************************************************
** 函数名称：OLED_Simulator_Clear
** 函数作用：清除显存
** 输入参数：无
** 输出参数：无
** 使用范例：OLED_Simulator_Clear();
** 函数备注：
*******************************************************************************/
void OLED_Simulator_Clear(void);

/*******************************************************************************
** 函数名称：OLED_Simulator_GetPixel
** 函数作用：读像素
** 输入参数：x,y - 坐标
** 输出参数：无
** 使用范例：OLED_Simulator_GetPixel(12, 45);
** 函数备注：
*******************************************************************************/
int OLED_Simulator_GetPixel(int x, int y);

/*******************************************************************************
** 函数名称：OLED_Simulator_Flush
** 函数作用：刷新显存到屏幕
** 输入参数：无
** 输出参数：无
** 使用范例：OLED_Simulator_Flush();
** 函数备注：
*******************************************************************************/
void OLED_Simulator_Flush(void);

/*******************************************************************************
** 函数名称：OLED_Simulator_GetTicks
** 函数作用：读取ticks
** 输入参数：无
** 输出参数：ticks，32位
** 使用范例：OLED_Simulator_GetTicks();
** 函数备注：
*******************************************************************************/
int OLED_Simulator_GetTicks(void);

/*******************************************************************************
** 函数名称：OLED_Simulator_GetKeyEvent
** 函数作用：获取事件
** 输入参数：无
** 输出参数：是否有事件
** 使用范例：OLED_Simulator_GetKeyEvent();
** 函数备注：
*******************************************************************************/
int OLED_Simulator_GetKeyEvent(void);

/*******************************************************************************
** 函数名称：OLED_Simulator_GetKeyValue
** 函数作用：读取键值
** 输入参数：无
** 输出参数：键值
** 使用范例：OLED_Simulator_GetKeyValue();
** 函数备注：
*******************************************************************************/
int OLED_Simulator_GetKeyValue(void);

/*******************************************************************************
** 函数名称：OLED_Simulator_Run
** 函数作用：运行
** 输入参数：
** 输出参数：
** 使用范例：OLED_Simulator_Run();
** 函数备注：
*******************************************************************************/
void OLED_Simulator_Run(void(*task)(void));

#endif	/* __OLED_SIMULATOR_H__ */
