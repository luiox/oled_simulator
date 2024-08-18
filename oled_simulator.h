/*******************************************************************************
** �ļ����ƣ�oled_simulator.h
** �ļ����ã�OLEDģ����
** ��д���ߣ�Tom Free �����
** ��дʱ�䣺2020-06-24
** �ļ���ע��OLEDģ����ͷ�ļ�����������ģ��������ز���
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
#ifndef __OLED_SIMULATOR_H__
#define __OLED_SIMULATOR_H__

/* Ԥ����ɫģʽѡ�� */
#define OLED_COLOR_USER_MODE        (0u)
#define OLED_COLOR_SSD1306_WHITE    (1u)
#define OLED_COLOR_SSD1306_BLUE     (2u)
#define OLED_COLOR_LCD12864         (3u)

/* ��ɫ��ɫ���� */
#define OLED_COLOR_RGB(R, G, B)     ((R << 24) | (G << 16) | (B << 8))

/* OLED�ߴ� */
#define OLED_WIDTH                  (128u)
#define OLED_HEIGHT                 (64u)

/* ��ʾһ����������Ŀ�� */
#define OLED_POINT_WIDTH            (6u)
/* ��ʾһ�����صļ���ߵĿ�� */
#define OLED_LINE_WIDTH             (2u)

/* ��ɫģʽ */
#define OLED_COLOR_MODE             OLED_COLOR_SSD1306_WHITE

/* Ԥ����ɫ���� */
#if (OLED_COLOR_MODE == OLED_COLOR_SSD1306_WHITE)
/* OLED������ɫ���� */
#define OLED_BACKGROUND_COLOR       OLED_COLOR_RGB(0, 0, 0)
/* OLEDǰ����ɫ���� */
#define OLED_FOREGROUND_COLOR       OLED_COLOR_RGB(255, 255, 255)
/* OLED�������ɫ���� */
#define OLED_SPACELINE_COLOR        OLED_COLOR_RGB(10, 10, 10)
#elif (OLED_COLOR_MODE == OLED_COLOR_SSD1306_BLUE)
/* OLED������ɫ���� */
#define OLED_BACKGROUND_COLOR       OLED_COLOR_RGB(10, 10, 10)
/* OLEDǰ����ɫ���� */
#define OLED_FOREGROUND_COLOR       OLED_COLOR_RGB(10, 10, 200)
/* OLED�������ɫ���� */
#define OLED_SPACELINE_COLOR        OLED_COLOR_RGB(10, 10, 10)
#elif (OLED_COLOR_MODE == OLED_COLOR_LCD12864)
/* OLED������ɫ���� */
#define OLED_BACKGROUND_COLOR       OLED_COLOR_RGB(0, 0, 0)
/* OLEDǰ����ɫ���� */
#define OLED_FOREGROUND_COLOR       OLED_COLOR_RGB(255, 255, 255)
/* OLED�������ɫ���� */
#define OLED_SPACELINE_COLOR        OLED_COLOR_RGB(10, 10, 10)
#elif (OLED_COLOR_MODE == OLED_COLOR_USER_MODE)
#error please set color
/* OLED������ɫ���� */
#define OLED_BACKGROUND_COLOR       //OLED_COLOR_RGB(0, 0, 0)
/* OLEDǰ����ɫ���� */
#define OLED_FOREGROUND_COLOR       //OLED_COLOR_RGB(255, 255, 255)
/* OLED�������ɫ���� */
#define OLED_SPACELINE_COLOR        //OLED_COLOR_RGB(10, 10, 10)
#endif  /* OLED_COLOR_MODE == OLED_COLOR_SSD1306_WHITE */

/*******************************************************************************
** �������ƣ�OLED_Simulator_Setpixel
** �������ã�д����
** ���������x,y - ���꣬color - ��ɫ��0 - ������1 - ǰ��
** �����������
** ʹ�÷�����OLED_Simulator_Setpixel(10, 45, 1);
** ������ע��
*******************************************************************************/
void OLED_Simulator_Setpixel(int x, int y, int color);

/*******************************************************************************
** �������ƣ�OLED_Simulator_Clear
** �������ã�����Դ�
** �����������
** �����������
** ʹ�÷�����OLED_Simulator_Clear();
** ������ע��
*******************************************************************************/
void OLED_Simulator_Clear(void);

/*******************************************************************************
** �������ƣ�OLED_Simulator_GetPixel
** �������ã�������
** ���������x,y - ����
** �����������
** ʹ�÷�����OLED_Simulator_GetPixel(12, 45);
** ������ע��
*******************************************************************************/
int OLED_Simulator_GetPixel(int x, int y);

/*******************************************************************************
** �������ƣ�OLED_Simulator_Flush
** �������ã�ˢ���Դ浽��Ļ
** �����������
** �����������
** ʹ�÷�����OLED_Simulator_Flush();
** ������ע��
*******************************************************************************/
void OLED_Simulator_Flush(void);

/*******************************************************************************
** �������ƣ�OLED_Simulator_GetTicks
** �������ã���ȡticks
** �����������
** ���������ticks��32λ
** ʹ�÷�����OLED_Simulator_GetTicks();
** ������ע��
*******************************************************************************/
int OLED_Simulator_GetTicks(void);

/*******************************************************************************
** �������ƣ�OLED_Simulator_GetKeyEvent
** �������ã���ȡ�¼�
** �����������
** ����������Ƿ����¼�
** ʹ�÷�����OLED_Simulator_GetKeyEvent();
** ������ע��
*******************************************************************************/
int OLED_Simulator_GetKeyEvent(void);

/*******************************************************************************
** �������ƣ�OLED_Simulator_GetKeyValue
** �������ã���ȡ��ֵ
** �����������
** �����������ֵ
** ʹ�÷�����OLED_Simulator_GetKeyValue();
** ������ע��
*******************************************************************************/
int OLED_Simulator_GetKeyValue(void);

/*******************************************************************************
** �������ƣ�OLED_Simulator_Run
** �������ã�����
** ���������
** ���������
** ʹ�÷�����OLED_Simulator_Run();
** ������ע��
*******************************************************************************/
void OLED_Simulator_Run(void(*task)(void));

#endif	/* __OLED_SIMULATOR_H__ */
