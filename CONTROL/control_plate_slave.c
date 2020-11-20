
/**
  ******************************************************************************
  * @file    control_plate_slave.c
  * @author  
  * @version V1.0.0
  * @date    2020.1.5
  * @brief   �ӻ������ļ�
  *          
  ******************************************************************************
  *
  *
  *
  *
  *
  *****************************************************************************
***/



//�ĸ����

#include "control_plate_slave.h"
#include "control_plate_master.h"


#if IS_USE_ORIGINAL_ESC
TYPE_Moto_t    g_Moto_Salve_t[4] = {0};
C620_MotorInfomation_t* p_MotorInfo_ts    ;
C610_MotorInfomation_t* p_MotorInfo610_ts ;
#endif

#if COULD_SALVE_CONTROL_MOTO
int32_t        g_Zero_Position[4] = {0};
#endif

#if COULD_SALVE_CONTROL_MOTO
uint8_t g_slave_run_flag = 1;
#endif

#if PLATE_SLAVE


typedef enum{ None, C610, C620}TYPE_Enum_MOTO_USE;
typedef enum { PLATE_SLAVE_MODE_NONE      ,
       PLATE_SLAVE_MODE_SPEED     ,
       PLATE_SLAVE_MODE_CURRENT   ,
       PLATE_SLAVE_MODE_POSITION  ,
       PLATE_SLAVE_MODE_ANGLE     ,
       PLATE_SLAVE_MODE_SPCA      ,
       PLATE_SLAVE_MODE_LOOPBACK  ,    
     } TYPE_Enum_PSLAVE_MODE;
TYPE_Enum_PSLAVE_MODE enum_PLATE_SLAVE_MODE = DEFAULT_PLATE_SLAVE_MODE;
TYPE_Enum_MOTO_USE    enum_PLATE_MOTO_USE   = DEFAULT_MOTO_USE;
     

void Plate_Moto_Init(void)
{

    #if IS_USE_ORIGINAL_ESC
    p_MotorInfo_ts    = C620_MotorInfo;
    p_MotorInfo610_ts = C610_MotorInfo;

    if (enum_PLATE_MOTO_USE == C620)
    {
        Moto_API_PID_Init_C620(g_Moto_Salve_t + 0);
        Moto_API_PID_Init_C620(g_Moto_Salve_t + 1);
        Moto_API_PID_Init_C620(g_Moto_Salve_t + 2);
        Moto_API_PID_Init_C620(g_Moto_Salve_t + 3);
    }
    else if( enum_PLATE_MOTO_USE == C610)
    {
        Moto_API_PID_Init_C610(g_Moto_Salve_t + 0);
        Moto_API_PID_Init_C610(g_Moto_Salve_t + 1);
        Moto_API_PID_Init_C610(g_Moto_Salve_t + 2);
        Moto_API_PID_Init_C610(g_Moto_Salve_t + 3);
    }
    #else
    /*��ֲʱ��*/

    
    
    
    
    #endif
}
void Plate_Slave_Init(void)
{
    Plate_Moto_Init();
    
    #if COULD_SALVE_CONTROL_MOTO
    Plate_Set_Zore_Position(1, (int32_t *) NULL);
    #endif
}

uint8_t Plate_Slave_Mode_Get(void)
{
    return (uint8_t) enum_PLATE_SLAVE_MODE;
}
uint8_t Plate_Slave_Moto_Use_Get(void)
{
    return (uint8_t) enum_PLATE_MOTO_USE;
}
#if COULD_SALVE_CONTROL_MOTO

void Plate_LoseFoce(void)
{
    #if IS_USE_ORIGINAL_ESC
     if(enum_PLATE_MOTO_USE == C620)
    {
        C620_API_SendCurrentVal(0, 0, 0, 0);
    }
    else if(enum_PLATE_MOTO_USE == C610)
    {
        C610_API_SendCurrentVal(0, 0, 0, 0);
    }
    #else
    /*��ֲʱ��*/

    
    
    #endif
}


void Plate_SpeedMode(void)
{
    int16_t v[4] = {0,0,0,0};
    int16_t i[4] = {0,0,0,0};
    if((g_ControlPlateRx_t.cmd & 0x0F) == PLATE_CMD_SPEED)
    {
        ControlPlate_API_GetSpeed(v, v+1, v+2, v+3);
        
        #if IS_USE_ORIGINAL_ESC
        if(enum_PLATE_MOTO_USE == C620)
        {
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 0, p_MotorInfo_ts[0].Velocity, 0);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 1, p_MotorInfo_ts[1].Velocity, 0);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 2, p_MotorInfo_ts[2].Velocity, 0);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 3, p_MotorInfo_ts[3].Velocity, 0);
        }
        else if(enum_PLATE_MOTO_USE == C610)
        {
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 0, p_MotorInfo610_ts[0].Velocity, 0);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 1, p_MotorInfo610_ts[1].Velocity, 0);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 2, p_MotorInfo610_ts[2].Velocity, 0);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 3, p_MotorInfo610_ts[3].Velocity, 0);

        }
        else {}
        
        //  PID����Ӧ�÷��͵ĵ���, ��ȡҪ���͵ĵ���
        i[0] = Moto_API_Cal_GetCurrentVelocityMode(g_Moto_Salve_t + 0, v[0]);
        i[1] = Moto_API_Cal_GetCurrentVelocityMode(g_Moto_Salve_t + 1, v[1]);
        i[2] = Moto_API_Cal_GetCurrentVelocityMode(g_Moto_Salve_t + 2, v[2]);
        i[3] = Moto_API_Cal_GetCurrentVelocityMode(g_Moto_Salve_t + 3, v[3]);

        if(enum_PLATE_MOTO_USE == C620)
        {
            C620_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
        }
        else if(enum_PLATE_MOTO_USE == C610)
        {
            C610_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
        }
        #else  
        /*��ֲʱ��*/

        #endif
    }    
}

void Plate_PositionMode(void)
{
    int32_t p[4] = {0};
    int16_t i[4] = {0};
    if((g_ControlPlateRx_t.cmd & 0x0F) == PLATE_CMD_POSITION34)
    {
        ControlPlate_API_GetPosition(p, p+1, p+2, p+3);
        #if IS_USE_ORIGINAL_ESC
        if(enum_PLATE_MOTO_USE == C620)
        {
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 0, p_MotorInfo_ts[0].Velocity, p_MotorInfo_ts[0].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 1, p_MotorInfo_ts[1].Velocity, p_MotorInfo_ts[1].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 2, p_MotorInfo_ts[2].Velocity, p_MotorInfo_ts[2].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 3, p_MotorInfo_ts[3].Velocity, p_MotorInfo_ts[3].Position);
        }
        else if(enum_PLATE_MOTO_USE == C610)
        {
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 0, p_MotorInfo610_ts[0].Velocity, p_MotorInfo610_ts[0].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 1, p_MotorInfo610_ts[1].Velocity, p_MotorInfo610_ts[1].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 2, p_MotorInfo610_ts[2].Velocity, p_MotorInfo610_ts[2].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 3, p_MotorInfo610_ts[3].Velocity, p_MotorInfo610_ts[3].Position);

        }
       
        else {}
         //  PID����Ӧ�÷��͵ĵ���, ��ȡҪ���͵ĵ���
        if(0)
        {
            //��λ�ɱ�
            i[0] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 0, g_Zero_Position[0] + p[0], 20000);
            i[1] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 1, g_Zero_Position[1] + p[1], 20000);
            i[2] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 2, g_Zero_Position[2] + p[2], 20000);
            i[3] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 3, g_Zero_Position[3] + p[3], 20000);
        }
        else
        {
            //��λȷ��
            i[0] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 0, p[0], 20000);
            i[1] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 1, p[1], 20000);
            i[2] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 2, p[2], 20000);
            i[3] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 3, p[3], 20000);
        }
        if(enum_PLATE_MOTO_USE == C620)
        {
            C620_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
        }
        else if(enum_PLATE_MOTO_USE == C610)
        {
            C610_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
        }
        #else 
        /*��ֲʱ��*/

        
        #endif
    }
}

void Plate_CurrentMode(void)
{
    int16_t i[4] = {0, 0, 0, 0};
    if((g_ControlPlateRx_t.cmd & 0x0F) == PLATE_CMD_CURRENT)
    {
        ControlPlate_API_GetCurrent(i, i+1, i+2, i+3);
        
        #if IS_USE_ORIGINAL_ESC
        if(enum_PLATE_MOTO_USE == C620)
        {
            C620_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
        }
        else if(enum_PLATE_MOTO_USE == C610)
        {
            C610_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
        }
        #else
        /*��ֲʱ��*/

        
        #endif
    }
}
//����0λ, mode == 0, ���òο�Ϊ0
//         mode == 1, ����Ӧ����������ݵĽṹ��Ϊ0
void Plate_Set_Zore_Position(uint8_t mode, int32_t *pPosition)
{
    if (mode == 0)
    {
        g_Zero_Position[0] = * (pPosition + 0) ;
        g_Zero_Position[1] = * (pPosition + 1) ;
        g_Zero_Position[2] = * (pPosition + 2) ;
        g_Zero_Position[3] = * (pPosition + 3) ;
    }
    else if(mode == 1)
    {
        #if IS_USE_ORIGINAL_ESC
        if(enum_PLATE_MOTO_USE == C620)
        {
            p_MotorInfo_ts[0].Position = 0;
            p_MotorInfo_ts[1].Position = 0;
            p_MotorInfo_ts[2].Position = 0;
            p_MotorInfo_ts[3].Position = 0;
        }
        else if(enum_PLATE_MOTO_USE == C610)
        {
            p_MotorInfo610_ts[0].Position = 0;
            p_MotorInfo610_ts[1].Position = 0;
            p_MotorInfo610_ts[2].Position = 0;
            p_MotorInfo610_ts[3].Position = 0;
        }
        #else
        /*��ֲʱ��*/

        
        #endif
    }
}

void Plate_AngleMode(void)
{
    int32_t angle[4] = {0};
    int16_t i[4] = {0};
    if((g_ControlPlateRx_t.cmd & 0x0F) == PLATE_CMD_POSITION34)
    {
        ControlPlate_API_GetPosition(angle, angle+1, angle+2, angle+3);
        
        #if IS_USE_ORIGINAL_ESC
        if(enum_PLATE_MOTO_USE == C620)
        {
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 0, p_MotorInfo_ts[0].Velocity, p_MotorInfo_ts[0].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 1, p_MotorInfo_ts[1].Velocity, p_MotorInfo_ts[1].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 2, p_MotorInfo_ts[2].Velocity, p_MotorInfo_ts[2].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 3, p_MotorInfo_ts[3].Velocity, p_MotorInfo_ts[3].Position);
        }                                                                                         
        else if(enum_PLATE_MOTO_USE == C610)
        {
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 0, p_MotorInfo610_ts[0].Velocity, p_MotorInfo610_ts[0].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 1, p_MotorInfo610_ts[1].Velocity, p_MotorInfo610_ts[1].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 2, p_MotorInfo610_ts[2].Velocity, p_MotorInfo610_ts[2].Position);
            Moto_API_SetMotoInfo(g_Moto_Salve_t + 3, p_MotorInfo610_ts[3].Velocity, p_MotorInfo610_ts[3].Position);

        }

        else {}
         //  PID����Ӧ�÷��͵ĵ���, ��ȡҪ���͵ĵ���
        
        i[0] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 0, angle[0]*ANGLE_RATIO, 20000);
        i[1] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 1, angle[1]*ANGLE_RATIO, 20000);
        i[2] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 2, angle[2]*ANGLE_RATIO, 20000);
        i[3] = Moto_API_Cal_GetCurrentVelocityPositionMode(g_Moto_Salve_t + 3, angle[3]*ANGLE_RATIO, 20000);
        
        
        if(enum_PLATE_MOTO_USE == C620)
        {
            C620_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
        }
        else if(enum_PLATE_MOTO_USE == C610)
        {
            C610_API_SendCurrentVal(i[0], i[1], i[2], i[3]);
        }
        #else
        /*��ֲʱ��*/

        #endif
    }
}
void Plate_SCPA_Mode(void)
{
    Plate_SpeedMode();
    Plate_CurrentMode();
    Plate_PositionMode();
    Plate_AngleMode();
}

#endif


void Plate_Slave_NoneMode(void)
{
    ;
}

void Plate_Slave_Mode_Change(uint8_t mode)
{
    
    enum_PLATE_SLAVE_MODE = (TYPE_Enum_PSLAVE_MODE) mode;
    Plate_Slave_Init();
}

void Plate_Slave_Moto_Use_Change(uint8_t moto_use)
{
    enum_PLATE_MOTO_USE = (TYPE_Enum_MOTO_USE) moto_use;
    Plate_Slave_Init();
}
#if COULD_SALVE_CONTROL_MOTO
void Plate_Slave_SetRunFlag(uint8_t flag)
{
    g_slave_run_flag = flag;
}
#endif 
void Plate_Slave_LoopBackMode(void)
{
    plate_printf("    �յ�����ϢΪ:\r\n");
    if((g_ControlPlateRx_t.cmd & 0x0F) != PLATE_CMD_CONTROL)
    {
        plate_printf("     cmd: %X\r\n", g_ControlPlateRx_t.cmd&0x0f);
        plate_printf("    data: %d, %d, %d, %d\r\n",
                         *(int16_t*)g_ControlPlateRx_t.val1,
                         *(int16_t*)g_ControlPlateRx_t.val2,
                         *(int16_t*)g_ControlPlateRx_t.val3,
                         *(int16_t*)g_ControlPlateRx_t.val4);
    }
    else
    {
       plate_printf("  control cmd %X\r\n", g_ControlPlateRx_t.val1[0]);
    }
}
void Plate_Slave_Can_Control_Deal(void)
{
    /*
    uint8_t controlInfo[8] = {0,0,0,0,0,0,0,0};
    if((ControlPlate_API_GetAllCMD() & PLATE_CMD_GET) == PLATE_CMD_CONTROL)
    {
        
        ControlPlate_API_GetControlInfo(controlInfo);
        #if IS_USE_ORIGINAL_ESC
            ;
        #else
            ;
        #endif
    }
    */
    //pass;
}



void Plate_Slave_Task(void)
{
    
    
    
    #if COULD_SALVE_CONTROL_MOTO       
        if(g_slave_run_flag == 0)
        {
            Plate_LoseFoce();
            return;
        }
        
        if     (enum_PLATE_SLAVE_MODE == PLATE_SLAVE_MODE_NONE    ) Plate_LoseFoce();
        else if(enum_PLATE_SLAVE_MODE == PLATE_SLAVE_MODE_SPEED   ) Plate_SpeedMode();
        else if(enum_PLATE_SLAVE_MODE == PLATE_SLAVE_MODE_CURRENT ) Plate_CurrentMode();
        else if(enum_PLATE_SLAVE_MODE == PLATE_SLAVE_MODE_POSITION) Plate_PositionMode();
        else if(enum_PLATE_SLAVE_MODE == PLATE_SLAVE_MODE_SPCA    ) Plate_SCPA_Mode(); 
    #endif                                               
        
    else if(enum_PLATE_SLAVE_MODE == PLATE_SLAVE_MODE_LOOPBACK) Plate_Slave_LoopBackMode();
    if     (enum_PLATE_SLAVE_MODE == PLATE_SLAVE_MODE_NONE    ) Plate_Slave_NoneMode();
    
    else;
} 


void Plate_Slave_Test(void)
{
    static uint8_t i = 0, j = 0;
//    uint8_t s[9] = {0};
    delay_ms(1);
   
    ControlPlate_API_SendMotoSpeed   (DEFAULT_PLATE_ID, i, i, i, i);
                                    delay_us(50);
    ControlPlate_API_SendMotoCurrent (DEFAULT_PLATE_ID, i+1, i, i, i);
                                    delay_us(50);
  //  ControlPlate_API_SendMotoPosition(DEFAULT_PLATE_ID, i+2, i, i, i);
                                    delay_us(50);
    
  // ControlPlate_API_SendMotoAngle   (DEFAULT_PLATE_ID, i+3, i, i, i);
                                    delay_us(50);
  // ControlPlate_API_OnlineSend      (PLATE_ID_ALL);
                                    delay_us(50);
  //
   ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_PLATE_ID        , i);delay_ms(1);
    
  // ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_CONNECT_CAN     , j);delay_ms(1);
  // ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_SLAVE_MODE      , j);delay_ms(1);
   
  // ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_SLAVE_MOTO_USE  , j);delay_ms(1);
  // ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_CAN1_USE        , j);delay_ms(1);
  // ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_CAN2_USE        , j);delay_ms(1);
  // ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_C610_CAN_USE    , j);delay_ms(1);
  // ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_C620_CAN_USE    , j);delay_ms(1);
  // ControlPlate_API_Plate_Run_Self_Function(g_PLATE_ID, j, s);
    i += 0x01;
    j ++;
    if (j > 3)
        j = 0;
}





#endif


















