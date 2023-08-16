#ifndef	__GUI_MAIN_H
#define	__GUI_MAIN_H

#include "main.h"
#include "stdbool.h"
#include "string.h"
#include "math.h"

//��Ŀ¼��������ʾҳ��㼶
enum 
{
  M_WINDOW,
	M_CONFIRM_WINDOW,
  M_SLEEP,
    M_MAIN, 
      M_EDITOR,
        M_KNOB,
          M_KRF,
          M_KPF,
      M_VOLT,
			M_COUNT,
			M_COUNT_SHOW,
			M_RANGING,
      M_SETTING,
        M_ABOUT,
};

//״̬����ʼ����ǩ
enum
{
  S_FADE,       				//ת������
  S_WINDOW,     				//������ʼ��
	S_CONFIRM_WINDOW,     //������ʼ��
  S_LAYER_IN,   				//�㼶��ʼ��
  S_LAYER_OUT,  				//�㼶��ʼ��
  S_NONE,       				//ֱ��ѡ��ҳ��
};

// ����ID
enum
{
  BTN_ID_CC = 0, // ��ʱ����ת
	BTN_ID_CW = 1, // ˳ʱ����ת
	BTN_ID_SP = 2, // �̰�
	BTN_ID_LP = 3, // ����
};

// ��������
#define BTN_PARAM_TIMES 2 // ����uint8_t���ֵ���ܲ����������洢�������㣬����Ŵ�����ʹ��

//������Ϣ
typedef struct
{
  uint8_t id;
  bool pressed;
}M_BTN_INFO;


//�˵��ṹ��
typedef struct MENU
{
  char *m_select;
} M_SELECT;

/************************************* ҳ����� *************************************/

//OLED����
#define   DISP_H              64    //��Ļ�߶�
#define   DISP_W              128   //��Ļ���

//UI����
#define   UI_DEPTH            20    //����㼶��
#define   UI_MNUMB            50   //�˵�����

// �б����
// Ĭ�ϲ���

#define LIST_FONT u8g2_font_HelvetiPixel_tr // �б�����
#define LIST_TEXT_H 8                       // �б�ÿ����������ĸ߶�
#define LIST_LINE_H 16                      // �б��и߶�
#define LIST_TEXT_S 4                       // �б�ÿ�����ֵ��ϱ߾࣬��߾���ұ߾࣬�±߾�����������߶Ⱥ��и߶Ⱦ���
#define LIST_BAR_W 5                        // �б��������ȣ���Ҫ����������Ϊ���м���1���ؿ�ȵ���
#define LIST_BOX_R 0.5                      // �б�ѡ���Բ��

// ��խ�и߶Ȳ���
/*
#define LIST_FONT u8g2_font_4x6_tr // �б�����
#define LIST_TEXT_H 5              // �б�ÿ����������ĸ߶�
#define LIST_LINE_H 7              // �б��и߶�
#define LIST_TEXT_S 1              // �б�ÿ�����ֵ��ϱ߾࣬��߾���ұ߾࣬�±߾�����������߶Ⱥ��и߶Ⱦ���
#define LIST_BAR_W 7               // �б��������ȣ���Ҫ����������Ϊ���м���1���ؿ�ȵ���
#define LIST_BOX_R 0.5             // �б�ѡ���Բ��
*/

enum 
{
  DISP_BRI,     //��Ļ����
  TILE_ANI,     //���������ٶ�
  LIST_ANI,     //�б����ٶ�
  WIN_ANI,      //���������ٶ�
  SPOT_ANI,     //�۹⶯���ٶ�
  TAG_ANI,      //��ǩ�����ٶ�
  FADE_ANI,     //��ʧ�����ٶ�
  BTN_SPT,      //�����̰�ʱ��
  BTN_LPT,      //��������ʱ��
  TILE_UFD,     //����ͼ���ͷչ������
  LIST_UFD,     //�˵��б��ͷչ������
  TILE_LOOP,    //����ͼ��ѭ��ģʽ����
  LIST_LOOP,    //�˵��б�ѭ��ģʽ����
  WIN_BOK,      //���������黯����
  KNOB_DIR,     //��ť�����л�����
  DARK_MODE,    //�ڰ�ģʽ����
	UI_PARAM,			//16//��������
};

typedef struct
{
  bool init;
  uint8_t num[UI_MNUMB];
  uint8_t select[UI_DEPTH];
  uint8_t layer;
  uint8_t index; // = M_SLEEP;
  uint8_t state; // = S_NONE;
  bool sleep;    // = true;
  uint8_t fade;  // = 1;
  uint8_t param[UI_PARAM];
} M_UI;

typedef struct 
{
  float   title_y_calc; 		//= TILE_INDI_S + (TILE_INDI_H - TILE_B_TITLE_H) / 2 + TILE_B_TITLE_H * 2;
  float   title_y_trg_calc; //= TILE_INDI_S + (TILE_INDI_H - TILE_B_TITLE_H) / 2 + TILE_B_TITLE_H;
  int16_t temp;
  bool    select_flag;
  float   icon_x;
  float   icon_x_trg;
  float   icon_y;
  float   icon_y_trg;
  float   indi_x; 
  float   indi_x_trg;
  float   title_y;
  float   title_y_trg;
} M_TILE;

//��������
//���д���ҳ�涼ʹ��ͬһ�ײ���
#define   TILE_B_FONT         u8g2_font_helvB18_tr        //�������������
#define   TILE_B_TITLE_H      18                          //�������������߶�
#define   TILE_ICON_H         30                          //����ͼ��߶�
#define   TILE_ICON_W         30                          //����ͼ����
#define   TILE_ICON_S         36                          //����ͼ����
#define   TILE_INDI_H         27                          //���������ָʾ���߶�
#define   TILE_INDI_W         7                           //���������ָʾ�����
#define   TILE_INDI_S         36                          //���������ָʾ���ϱ߾�


typedef void (*fun_callback_t)(int number);//�����ص�����ָ��

extern M_BTN_INFO btn;
extern M_UI ui;

char *itoa(uint32_t num);
char *ftoa(float num);
//֪ͨ��Ҫ�洢����
void eeprom_notify_change(void);
// ��������
void animation(float *a, float *a_trg, uint8_t n);
// ��ʾ��ֵ�ĳ�ʼ��
void check_box_v_init(uint8_t *param);
// �б���ҳ��ͨ����ʾ����
void list_show(M_SELECT arr[], uint8_t ui_index);
// �б���ҳ����תʱ�ж�ͨ�ú���
void list_rotate_switch();
// ������ֵ��ʼ��
void window_value_init(char title[], uint8_t select, uint8_t *value, uint8_t max, uint8_t min, uint8_t step, M_SELECT *bg, uint8_t index);
// ȷ�ϵ�����ֵ��ʼ��
void confirm_window_value_init(char title[], uint8_t _select_btn, fun_callback_t _cb, M_SELECT *bg, uint8_t index);
void gui_btn_send_signal(uint8_t btn_id,uint8_t event);
void gui_task(void);
void gui_init(void);



#endif		/******		__GUI_MAIN_H		******/
