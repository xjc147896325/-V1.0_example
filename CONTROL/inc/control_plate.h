#ifndef __CONTROL_PLATE_H
#define __CONTROL_PLATE_H








#include "control_plate_connect.h"
#include "control_plate_slave.h"
#include "control_plate_master.h"
#include "control_plate_control.h"


#define COULD_SALVE_CONTROL_MOTO 1         //�ܷ���Ƶ��
#define IS_USE_ORIGINAL_ESC      1         //ʹ��ԭ���ĵ���ļ�



#define IS_USE_ORIGINAL_JOYSTICK   0        //�ǻ�ʹ��ԭ�����ֱ��ṹ��
#define COULD_RUN_SELF_FUNCTION    0        //�ܷ���������Զ��庯������
#define COULD_PLATE_ID_CHANGE      1        //�ܷ�̬���Ŀ��ư�ID
#define COULD_CONNECT_CAN_CHANGE   1        //�ܷ�̬�������ӵ�CAN

#define PLATE_MASTER               1        //��Ϊ����
#define PLATE_SLAVE                1        //��Ϊ�ӻ�
#define DEFAULT_PLATE_SLAVE_MODE   PLATE_SLAVE_MODE_LOOPBACK   //��Ϊ����Ĭ��ģʽ
#define DEFAULT_PLATE_MASTER_MODE  PLATE_MASTER_MODE_LOOPBACK  //��Ϊ�ӻ�Ĭ��ģʽ
#define DEFAULT_MOTO_USE           C620                        //��Ϊ�ӻ�Ĭ�Ͽ��Ƶĵ��

#define DEFAULT_CONNECT_CAN  USE_CAN1       //ʹ�õķ�CAN   USE_CAN1 USE_CAN2  None     
#define DEFAULT_PLATE_ID     PLATE_ID_TEST  //����ID        PALTE_ID_... A) - F0
#define plate_printf         my_printf      //printf�ض���  ����Ҫ��һ��void fun(char*, ...)�ĺ���ʵ��
                                            //��: void none_printf(char* p, ...) {;};//һ���պ���

#define ANGLE_RATIO          1146880        //14 * 36 * 8192 / 360 / 100
                                            //�Ƕ�ģʽ�µĵ��ת0.01�ȵ�position
                       
                       
                       
//��ͷ���ⲿ����������������������warning, ���ö�
#if IS_USE_ORIGINAL_ESC && PLATE_SLAVE
extern void Plate_Set_Zore_Position(uint8_t mode, int32_t *pPosition);
#endif

#if COULD_SALVE_CONTROL_MOTO
extern void Plate_Slave_Moto_Use_Change(uint8_t moto_use);
extern void Plate_Set_Zore_Position(uint8_t mode, int32_t *pPosition);
#endif

#if COULD_RUN_SELF_FUNCTION
extern void *  g_p_Self_Funs[];
extern void    ControlPlate_Deal_Run_Self_Fun(uint8_t* controlInfo);
extern uint8_t ControlPlate_API_Plate_Run_Self_Function(uint8_t toWhere, uint8_t funNum, uint8_t* parameter);
#endif

#if PLATE_MASTER
extern void    Plate_Master_Mode_Change(uint8_t i) ;
extern uint8_t Plate_Master_Mode_Get(void)         ;
extern uint8_t ControlPlate_API_SendMotoSpeed    (uint16_t toWhere, int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4);
extern uint8_t ControlPlate_API_SendMotoCurrent  (uint16_t toWhere, int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4);
extern uint8_t ControlPlate_API_SendMotoPosition (uint16_t toWhere, int32_t moto1, int32_t moto2, int32_t moto3, int32_t moto4);
extern uint8_t ControlPlate_API_SendMotoAngle    (uint16_t toWhere, int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4);
#endif

#if PLATE_SLAVE
void    Plate_Slave_Mode_Change(uint8_t mode);
uint8_t Plate_Slave_Mode_Get(void);
#endif


#endif











