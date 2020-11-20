
/**
  ******************************************************************************
  * @file    control_plate_control.c
  * @author  
  * @version V1.0.0
  * @date    2020.2.2
  * @brief   ���� ���� һЩ��������
  *            ��ҪΪһЩ������� 
  *                
  ******************************************************************************
  *
  *
  *
  *
  *
  ******************************************************************************/


#include "control_plate_control.h"
#define pass ;

/**
  * @brief  ��can�Ļص������е���, ControlPlate_GetFeedbackInfo��
  *
  * @param  
  *         
  * @retval None
  */

void ControlPlate_API_Control_Task(void)
{
    uint8_t controlInfo[9];
    uint8_t id, controlCMD;
    ControlPlate_API_GetControlInfo(controlInfo);
    id         = controlInfo[0] & 0xF0;
    controlCMD = controlInfo[0] & 0x0F;
    
    switch (controlCMD)
    {
        case CONTROL_ONLINE:
            ControlPlate_Deal_Online(id);
            break;
        case CONTROL_REQ_DATA:            
            ControlPlate_Deal_ReqData(id, controlInfo[1] & 0xF0, controlInfo[1] & 0x0F);
            break;
        case CONTROL_RESP_DATA:
            ControlPlate_Deal_RespData(controlInfo);
            break;
        #if COULD_RUN_SELF_FUNCTION
        case CONTROL_RUN_SELF_FUNCTION:
            //�����Զ��庯��
            //(*(void(*)(uint8_t*))g_p_Self_Funs[controlInfo[1]])(controlInfo + 2);
            ControlPlate_Deal_Run_Self_Fun(controlInfo);
        #endif
        
        #if  IS_USE_ORIGINAL_ESC && PLATE_SLAVE
        case CONTROL_SET_610_P_0:
            ControlPlate_Deal_Set_610_P_0(controlInfo);
            break;
        #endif
        
        #if COULD_SLAVE_CONTROL_MOTO
        case CONTROL_STOP_MOTO:
            ControlPlate_Deal_Stop_Moto(controlInfo[1]);
            break;
        #endif
        
        case CONTROL_CHANGE_CONFIG:
            ControlPlate_Deal_Plate_Config(controlInfo[1],controlInfo[2]);
            break;        
        default:
            pass;
    }
}
/**
  * @brief  ������������, 
  *
  * @param  onlineID:���ߵİ���ID
  *         
  * @retval None
  */
void ControlPlate_Deal_Online(uint8_t onlineID)
{
    switch (onlineID)
    {
        case PLATE_ID_MASTER:
            
            break;  
  
        case PLATE_ID_C620:
       
            break;
       
        case PLATE_ID_C610:
       
             break;
       
        case PALTE_ID_MECHANISM:
       
             break;
            
        case PLATE_ID_TEST:
             plate_printf("  PLATE_ID_TEST online\r\n");
             break;
        
        case PLATE_ID_ALL:
             break;
        
        default:
            ;

    }
}

/**
  * @brief  ������������, 
  *
  * @param      reqId : �������ݵİ���ID
  *           dataType: �������ݵ�����
  *             num   : ���������ݺ�
  * @retval CAN_TxStatus_Ok
  *         CAN_TxStatus_Fail
  */

uint8_t ControlPlate_Deal_ReqData(uint8_t reqId, uint8_t dataType, uint8_t num)
{
    uint8_t s[9]={0};
    s[0] = g_PLATE_ID | CONTROL_RESP_DATA;
    s[1] = dataType   | num;
  
    
    if (dataType == REQ_DATA_JOYSTICK)
    {
        #if IS_USE_ORIGINAL_JOYSTICK
        if (g_Joystick_Online_Flag == 0x80 || g_Joystick_Online_Flag == 0x40)       
        {
            if (num == JOYSTICK_DATA_LXY_RX)
            {
                s[2] = g_Joystick_t.Lx;
                s[3] = g_Joystick_t.Lx >> 8;
                s[4] = g_Joystick_t.Ly;
                s[5] = g_Joystick_t.Ly >> 8;
                s[6] = g_Joystick_t.Rx;
                s[7] = g_Joystick_t.Rx >> 8;            
            }
            else if (num == JOYSTICK_DATA_RXY_LX)
            {
                 s[2] = g_Joystick_t.Rx;
                 s[3] = g_Joystick_t.Rx >> 8;
                 s[4] = g_Joystick_t.Ry;
                 s[5] = g_Joystick_t.Ry >> 8;
                 s[6] = g_Joystick_t.Lx;
                 s[7] = g_Joystick_t.Lx >> 8; 
            }
            else if (num == JOYSTICK_DATA_LXY_KEY)
            {
                s[2] = g_Joystick_t.Lx;
                s[3] = g_Joystick_t.Lx >> 8;
                s[4] = g_Joystick_t.Ly;
                s[5] = g_Joystick_t.Ly >> 8;
                s[6] = g_Joystick_t.Key1;
                s[7] = g_Joystick_t.Key2;
            }
            else if (num == JOYSTICK_DATA_RXY_KEY)
            {
                s[2] = g_Joystick_t.Rx;
                s[3] = g_Joystick_t.Rx >> 8;
                s[4] = g_Joystick_t.Ry;
                s[5] = g_Joystick_t.Ry >> 8;
                s[6] = g_Joystick_t.Key1;
                s[7] = g_Joystick_t.Key2;
            }
        }
        #else
        
        if(0)
        {
            
        }
        
        #endif
        else
        {
            s[1] = dataType | JOYSTICK_DATA_NONE;
        }
        
        
    }
    else if(dataType == REQ_DATA_PLATE_ID)
    {
        if(num == 0x02)
            reqId = PLATE_ID_ALL;
    }
    #if IS_USE_ORIGINAL_ESC
    else if(dataType == REQ_DATA_MOTO_P_C610)
    {
        // *(int32_t*) (s + 4) = g_MotorInfo610_t[num].position;
    }
    #endif
    return ControlPlate_SendData(reqId, PLATE_CMD_CONTROL, s);
}

#if IS_USE_ORIGINAL_ESC
/**
  * @brief  ������λ����, 
  *          
  * @param   controlInfo:��������������
  *          
  *          
  * @retval 
  *
  */
#if PLATE_SLAVE
void ControlPlate_Deal_Set_610_P_0(uint8_t* controlInfo)
{
    static int32_t positions[4] = {0};
    if(controlInfo[1] == 0)
    {
        //�Ե�ǰ���λ��Ϊ0λ
        Plate_Set_Zore_Position(1, (int32_t *) NULL);
    }
    else if(controlInfo[1] == 1)
    {
        //����λ����Ϣ
        positions[controlInfo[2]] = * (int32_t*) (controlInfo+4);        
    }
    else if(controlInfo[2] == 2)
    {
        //�Է��͵�λ��Ϊ0λ
        Plate_Set_Zore_Position(0, positions);
    }
}
#endif

/**
  * @brief  ����ֹͣ���, 
  *          
  * @param   num:ֹͣ�ĵ����,��Ӧ���ܻ�δʵ��,
  *               
  *          
  * @retval 
  *
  */
void ControlPlate_Deal_Stop_Moto(uint8_t num)
{
    #if COULD_SALVE_CONTROL_MOTO && PALTE_SLAVE
    Plate_Slave_SetRunFlag(0);  
    #endif
}

#endif



/**
  * @brief ����������� 
  * @param   controlInfo:��������������
  *          
  *          
  * @retval None
  */
void ControlPlate_Deal_Plate_Config(uint8_t changeWhat, uint16_t changeResult)
{
    switch(changeWhat)
    {
        case CONTROL_CHANGE_PLATE_ID:
            ControlPlate_API_Change_PlateID(changeResult);
            break;
        case CONTROL_CHANGE_CONNECT_CAN:
            ControlPlate_API_Change_ConnectCan(changeResult);
            break;
        
        #if PLATE_MASTER
        case CONTROL_CHANGE_MASTER_MODE:
            Plate_Master_Mode_Change(changeResult);
            break;
        #endif
        
        #if PALTE_SLAVE
        case CONTROL_CHANGE_SLAVE_MODE:
            Plate_Slave_Mode_Change(changeResult);
            break;
           #if COULD_SALVE_CONTROL_MOTO
           case CONTROL_CHANGE_SLAVE_RUN_FLAG:
               Plate_Slave_SetRunFlag(changeResult);
               break;
            #if IS_USE_ORIGINAL_ESC
            case CONTROL_CHANGE_SLAVE_MOTO_USE:
                Plate_Slave_Moto_Use_Change(changeResult);
                break;
            #endif
           #endif
        #endif 
           
        #if IS_USE_ORIGINAL_ESC
        case CONTROL_CHANGE_CAN1_USE:
          //  CAN1_Use_Change(changeResult);
            break;
        case CONTROL_CHANGE_CAN2_USE:
         //   CAN2_Use_Change(changeResult);
            break;
        case CONTROL_CHANGE_C610_CAN_USE:
         //   C610_Can_Use_Change(changeResult);
            break;
        case CONTROL_CHANGE_C620_CAN_USE:
         //   C620_Can_Use_Change(changeResult);
            break;
        #endif
        
        default:
            pass;
    } 
}


/**
  * @brief  ��Ӧ��������, 
  * @param   controlInfo:��������������
  *          
  *          
  * @retval None
  */
void ControlPlate_Deal_RespData(uint8_t* controlInfo)
{
    uint8_t dataType = controlInfo[1] & 0xF0;
//    uint8_t num      = controlInfo[1] & 0x0F;
    
    if (dataType == REQ_DATA_JOYSTICK)
    {
        #if IS_USE_ORIGINAL_JOYSTICK
        g_Joystick_Frame_Num++; 
        
        if (num == JOYSTICK_DATA_LXY_RX)
        {
            g_Joystick_t.Lx = (int16_t) (controlInfo[3] << 8 | controlInfo[2]) ;
            g_Joystick_t.Ly = (int16_t) (controlInfo[5] << 8 | controlInfo[4]) ;
            g_Joystick_t.Rx = (int16_t) (controlInfo[7] << 8 | controlInfo[6]) ;
                                        
        }
        else if (num == JOYSTICK_DATA_RXY_LX)
        {
            g_Joystick_t.Rx = (int16_t) (controlInfo[3] << 8 | controlInfo[2]);
            g_Joystick_t.Ry = (int16_t) (controlInfo[5] << 8 | controlInfo[4]);
            g_Joystick_t.Lx = (int16_t) (controlInfo[7] << 8 | controlInfo[6]);
        }                                                                    
        else if (num == JOYSTICK_DATA_LXY_KEY)
        {
           g_Joystick_t.Lx = ((int16_t) (controlInfo[3] << 8 | controlInfo[2])) + 1;
           g_Joystick_t.Ly = (int16_t) (controlInfo[5] << 8 | controlInfo[4]);
            
           g_Joystick_t.Key1 = controlInfo[6];
           g_Joystick_t.Key2 = controlInfo[7];
            
        }
        else if (num == JOYSTICK_DATA_RXY_KEY)
        {
           g_Joystick_t.Rx = (int16_t) (controlInfo[3] << 8 | controlInfo[2]);
           g_Joystick_t.Ry = (int16_t) (controlInfo[5] << 8 | controlInfo[4]);
            
           g_Joystick_t.Key1 = controlInfo[6];
           g_Joystick_t.Key2 = controlInfo[7];
        }
        #else
        if(0)
        {
            ;
        }
        /******��ֲ��Ҫ�����*********
        if (num == JOYSTICK_DATA_LXY_RX)
        {
            g_Joystick_t.Lx = (int16_t) (controlInfo[3] << 8 | controlInfo[2]) ;
            g_Joystick_t.Ly = (int16_t) (controlInfo[5] << 8 | controlInfo[4]) ;
            g_Joystick_t.Rx = (int16_t) (controlInfo[7] << 8 | controlInfo[6]) ;
                                        
        }
        else if (num == JOYSTICK_DATA_RXY_LX)
        {
            g_Joystick_t.Rx = (int16_t) (controlInfo[3] << 8 | controlInfo[2]);
            g_Joystick_t.Ry = (int16_t) (controlInfo[5] << 8 | controlInfo[4]);
            g_Joystick_t.Lx = (int16_t) (controlInfo[7] << 8 | controlInfo[6]);
        }                                                                    
        else if (num == JOYSTICK_DATA_LXY_KEY)
        {
           g_Joystick_t.Lx = ((int16_t) (controlInfo[3] << 8 | controlInfo[2])) + 1;
           g_Joystick_t.Ly = (int16_t) (controlInfo[5] << 8 | controlInfo[4]);
            
           g_Joystick_t.Key1 = controlInfo[6];
           g_Joystick_t.Key2 = controlInfo[7];
            
        }
        else if (num == JOYSTICK_DATA_RXY_KEY)
        {
           g_Joystick_t.Rx = (int16_t) (controlInfo[3] << 8 | controlInfo[2]);
           g_Joystick_t.Ry = (int16_t) (controlInfo[5] << 8 | controlInfo[4]);
            
           g_Joystick_t.Key1 = controlInfo[6];
           g_Joystick_t.Key2 = controlInfo[7];
        }
        
        *************************/
        #endif
        
        /*else if (num == JOYSTICK_DATA_NONE)    
        {
            g_Joystick_Frame_Num--;
        }   */ 
        else        
        {
            // g_Joystick_Frame_Num--;
        }
    }
    else if(dataType == REQ_DATA_PLATE_ID)
    {
        //plate_printf("  PALTE_ID %d Online\r\n", controlInfo[0]&PLATE_ID_GET);
        ControlPlate_Deal_Online(controlInfo[0] & 0xF0);
    }
    #if IS_USE_ORIGINAL_ESC
    else if(dataType == REQ_DATA_MOTO_P_C610)
    {
       ;
    } 
    #endif
}

/**
  * @brief  ����״̬���� 
  *
  * @param   toWhere:���͵İ���ID
  *          
  *          
  * @retval None
  */

uint8_t ControlPlate_API_OnlineSend(uint16_t toWhere)
{
    uint8_t s[9]={0};
    s[0] = g_PLATE_ID | CONTROL_ONLINE;
    return ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
}

/**
  * @brief  ������������
  *
  * @param   toWhere:���Ŀ������������
  *          dataType: �������ݵ�����
  *            num   : ���������ݺ�
  *
  * @retval None
  */
uint8_t ControlPlate_API_Request_Data(uint16_t toWhere, uint8_t dataType, uint8_t num)
{
    uint8_t s[9]={0};
    s[0] = g_PLATE_ID | CONTROL_REQ_DATA;
    s[1] = dataType   | num;     
    return ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
}

#if IS_USE_ORIGINAL_ESC
/**
  * @brief  ����c610����λ
  *
  * @param   toWhere:���͵��Ŀ����
  *          flag     : 0:�Ե�ǰ���λ��Ϊ0λ
  *                     1:�Լ����õ���λ
  *          position : λ������ͷָ��ݺ�,��flagΪ0ʱ��Ч
  *
  * @retval None
  */
uint8_t ControlPlate_API_Set_610_P_0(uint16_t toWhere, uint8_t flag, int32_t* position)
{
    uint8_t s[9]={0};
    s[0] = g_PLATE_ID | CONTROL_SET_610_P_0;
    if (flag == 0)
    {
        //�Ե�ǰ���λ��Ϊ0λ
        s[1] = 0;
        return ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
    }
    else if(flag == 1)
    {
        //�Լ����õ���λ
        s[1] = 1;
        s[2] = 0;
        *(int32_t*) (s + 4) = *(position + 0);
        ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
        delay_ms(1);
        s[2] = 1;
        *(int32_t*) (s + 4) = *(position + 1);
        ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
        delay_ms(1);
        s[2] = 2;
        *(int32_t*) (s + 4) = *(position + 2);
        ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
        delay_ms(1);
        s[2] = 3;
        *(int32_t*) (s + 4) = *(position + 3);
        ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
        delay_ms(1);
        
        s[1] = 2;
        return ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
        
    }
    else 
    {
        return 0xff;
    }
}
uint8_t ControlPlate_API_Stop_Moto(uint8_t toWhere, uint8_t num)
{
    uint8_t s[9]={0};
    s[0] = g_PLATE_ID | CONTROL_STOP_MOTO;
    s[1] = num;

    return ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
    
}
#endif

/**
  * @brief   ���ÿ��ư����
  *
  * @param   toWhere:���͵��Ŀ����,����㲥����
  *          changeWhat Ҫ�ı����Ϣ,��ͷ�ļ�
  *          changeResult:�ı�Ľ��,����ο�����ļ���ö�ٶ���
  *
  * @retval None
  */

uint8_t ControlPlate_API_Plate_Config(uint8_t toWhere, uint8_t changeWhat, uint16_t changeResult)
{
    uint8_t s[9]={0};
     delay_ms(1);
    if (toWhere == PLATE_ID_ALL)
        return 0x00;
    else
    {
        s[0] = g_PLATE_ID | CONTROL_CHANGE_CONFIG;
        s[1] = changeWhat;
        s[2] = changeResult;
        return ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
    }
}

void Plate_Control_Test(void)
{
    static uint8_t i = 0, j = 0;
    delay_ms(1);

    ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_PLATE_ID        , i);delay_ms(1);
    
   // ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_CONNECT_CAN     , j);delay_ms(1);
   // ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_SLAVE_MODE      , j);delay_ms(1);
   ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_SLAVE_MOTO_USE , j);delay_ms(1);
   ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_CAN1_USE       , j);delay_ms(1);
   ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_CAN2_USE       , j);delay_ms(1);
   ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_C610_CAN_USE   , j);delay_ms(1);
   ControlPlate_API_Plate_Config(g_PLATE_ID,CONTROL_CHANGE_C620_CAN_USE   , j);delay_ms(1);
    i += 0x10;
    j ++;
    if (j > 4)
        j = 0;
}

#if COULD_RUN_SELF_FUNCTION
/**
  * @brief   ������д���庯��
  *            !!!!!!!!!!!!!ע��!!!!!!!!!!!!!!!!!!!!!!
  *             ������Ҫʹ�ô˺���
  *          ��˹��ܶ��ʹ��ָ������������,����ָ��ǿ��ת��,��ǣ����������
  *            �����ض��岻��ȷ,����Ƿ����ʶ�ʹ��������
  *
  *
  * @param   toWhere:���͵��Ŀ����
  *          funNum ����ĵڸ�����, 
  *                     
  *          parameter:����ָ��
  *                    ��Ҫ�Ĳ���תΪuint8_t ����, �����������Ϊ6���ֽ�,
  *                    ʹ��ʱҪ����һ������Ϊ6���ֽڵĿռ�,��Ȼ��Ƿ�����
  *
  * @retval None
  */


uint8_t ControlPlate_API_Plate_Run_Self_Function(uint8_t toWhere, uint8_t funNum, uint8_t* parameter)
{
    uint8_t s[9]={0};
    s[0] = g_PLATE_ID | CONTROL_RUN_SELF_FUNCTION;
    s[1] = funNum;
    
    s[2] = parameter[0];
    s[3] = parameter[1];
    s[4] = parameter[2];
    s[5] = parameter[3];
    s[6] = parameter[4];
    s[7] = parameter[5];
   return ControlPlate_SendData(toWhere, PLATE_CMD_CONTROL, s);
   
}
/**
  * @brief ���������Զ��庯�� 
  * @param   controlInfo:��������������
  *          
  *          
  * @retval None
  */
void    ControlPlate_Deal_Run_Self_Fun(uint8_t* controlInfo)
{
   
    //uint8_t run_num = controlInfo[1];
    (*(void(*)(uint8_t*))g_p_Self_Funs[controlInfo[1]])(controlInfo + 2);   
}
#endif





















