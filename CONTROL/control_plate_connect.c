/**
  ******************************************************************************
  * @file    control_plate.c
  * @author  
  * @version V1.0.0
  * @date    2020.1.5
  * @brief   ���� ���� �������
  *          ��Ҫ������ư�Ļ�����Ϣ
  *               ���������ݷ��͡���ȡ����
  ******************************************************************************
  *
  *
  *
  *
  *
  ******************************************************************************/


#include "control_plate_connect.h"


TYPE_PlateRxInfo_t g_ControlPlateRx_t = {0};

typedef enum {None, USE_CAN1, USE_CAN2}Tyep_Enum_CONNECT_CAN;

Tyep_Enum_CONNECT_CAN enum_CONNECT_CAN = DEFAULT_CONNECT_CAN;

uint8_t g_PLATE_ID = DEFAULT_PLATE_ID;


void ControlPlate_Info_Clear(void)
{
    g_ControlPlateRx_t.val1[0] = 0x00;
    g_ControlPlateRx_t.val1[1] = 0x00;
    g_ControlPlateRx_t.val1[2] = 0x00;
    g_ControlPlateRx_t.val1[3] = 0x00;
    
    g_ControlPlateRx_t.val2[0] = 0x00;
    g_ControlPlateRx_t.val2[1] = 0x00;
    g_ControlPlateRx_t.val2[2] = 0x00;
    g_ControlPlateRx_t.val2[3] = 0x00;
    
    g_ControlPlateRx_t.val3[0] = 0x00;
    g_ControlPlateRx_t.val3[1] = 0x00;
    g_ControlPlateRx_t.val3[2] = 0x00;
    g_ControlPlateRx_t.val3[3] = 0x00;
    
    g_ControlPlateRx_t.val4[0] = 0x00;
    g_ControlPlateRx_t.val4[1] = 0x00;
    g_ControlPlateRx_t.val4[2] = 0x00;
    g_ControlPlateRx_t.val4[3] = 0x00;
}

/**
  * @brief  ���ƽ�����CAN�ص�����,
  * @param  RxMsg:CAN�������ݵĽṹ��
  *         
  * @retval None
  */

uint8_t ControlPlate_GetFeedbackInfo(CanRxMsg* RxMsg)
{
    uint8_t  cmd_all = RxMsg -> StdId & PLATE_ALL_CMD;
    
    uint8_t  cmd  = cmd_all & PLATE_CMD_GET;
    uint16_t recive_id = cmd_all & PLATE_CMD_TO_WHERE;

    //���͸�����, ��㲥���� ��������Ч
    if((recive_id == g_PLATE_ID || recive_id == PLATE_ID_ALL))
      //  &&  cmd <= PLATE_CMD_NUM)
    {
        g_ControlPlateRx_t.cmd = 0;
        if(cmd == PLATE_CMD_SPEED || cmd == PLATE_CMD_CURRENT || cmd == PLATE_CMD_ANGLE)
        {
            g_ControlPlateRx_t.val1[0]  = RxMsg -> Data[0];
            g_ControlPlateRx_t.val1[1]  = RxMsg -> Data[1];
            g_ControlPlateRx_t.val2[0]  = RxMsg -> Data[2];
            g_ControlPlateRx_t.val2[1]  = RxMsg -> Data[3];
            g_ControlPlateRx_t.val3[0]  = RxMsg -> Data[4];
            g_ControlPlateRx_t.val3[1]  = RxMsg -> Data[5];
            g_ControlPlateRx_t.val4[0]  = RxMsg -> Data[6];
            g_ControlPlateRx_t.val4[1]  = RxMsg -> Data[7];
        }
        else if(cmd == PLATE_CMD_POSITION12)
        {
            g_ControlPlateRx_t.val1[0]  = RxMsg -> Data[0];
            g_ControlPlateRx_t.val1[1]  = RxMsg -> Data[1];
            g_ControlPlateRx_t.val1[2]  = RxMsg -> Data[2];
            g_ControlPlateRx_t.val1[3]  = RxMsg -> Data[3];
            g_ControlPlateRx_t.val2[0]  = RxMsg -> Data[4];
            g_ControlPlateRx_t.val2[1]  = RxMsg -> Data[5];
            g_ControlPlateRx_t.val2[2]  = RxMsg -> Data[6];
            g_ControlPlateRx_t.val2[3]  = RxMsg -> Data[7];            
        }
        else if(cmd == PLATE_CMD_POSITION34)
        {
            g_ControlPlateRx_t.val3[0]  = RxMsg -> Data[0];
            g_ControlPlateRx_t.val3[1]  = RxMsg -> Data[1];
            g_ControlPlateRx_t.val3[2]  = RxMsg -> Data[2];
            g_ControlPlateRx_t.val3[3]  = RxMsg -> Data[3];
            g_ControlPlateRx_t.val4[0]  = RxMsg -> Data[4];
            g_ControlPlateRx_t.val4[1]  = RxMsg -> Data[5];
            g_ControlPlateRx_t.val4[2]  = RxMsg -> Data[6];
            g_ControlPlateRx_t.val4[3]  = RxMsg -> Data[7];         
        }
        else if (cmd == PLATE_CMD_CONTROL)
        {
            g_ControlPlateRx_t.val1[0]  = RxMsg -> Data[0];
            g_ControlPlateRx_t.val1[1]  = RxMsg -> Data[1];
            g_ControlPlateRx_t.val1[2]  = RxMsg -> Data[2];
            g_ControlPlateRx_t.val1[3]  = RxMsg -> Data[3];
            g_ControlPlateRx_t.val2[0]  = RxMsg -> Data[4];
            g_ControlPlateRx_t.val2[1]  = RxMsg -> Data[5];
            g_ControlPlateRx_t.val2[2]  = RxMsg -> Data[6];
            g_ControlPlateRx_t.val2[3]  = RxMsg -> Data[7];
            ControlPlate_API_Control_Task();
            
        }
        
        else if(cmd == PLATE_CMD_NONE)
        {
            ControlPlate_Info_Clear();
        }
        g_ControlPlateRx_t.cmd = cmd_all;
        
    }
    else if(1)
    {
        ;
    } 
    return cmd_all;
}

/**
  * @brief  ���ƽ����ķ��ͺ���,
  * @param  toWhere:���͸�˭    g_PLATE_ID
  *         cmd    :���͵�����  PLATE_CMD
  *         txData :���͵�����, u8����ָ��,�����С����Ϊ8
  *         
  * @retval None
  */

uint8_t ControlPlate_SendData(uint16_t toWhere, uint8_t cmd, uint8_t *txData)
{
    uint8_t MailBox = 0;

    CanTxMsg TxMsg;    

    TxMsg.StdId  = PLATE_BASE_ID| toWhere |cmd;
    TxMsg.DLC    = 0x08;
    TxMsg.IDE    = CAN_Id_Standard;
    TxMsg.RTR    = CAN_RTR_Data;
    
        
    
    TxMsg.Data[0] = txData[0];
    TxMsg.Data[1] = txData[1];
    TxMsg.Data[2] = txData[2];
    TxMsg.Data[3] = txData[3];
    TxMsg.Data[4] = txData[4];
    TxMsg.Data[5] = txData[5];
    TxMsg.Data[6] = txData[6];
    TxMsg.Data[7] = txData[7];
    
    if(enum_CONNECT_CAN == None)
    {
        return CAN_TxStatus_Failed;
    }
    
    #if COULD_CONNECT_CAN_CHANGE
    else if(enum_CONNECT_CAN == USE_CAN2)
    {
        MailBox = CAN_Transmit(CAN2, &TxMsg);
        if(MailBox == CAN_TxStatus_NoMailBox)
            return CAN_TxStatus_Failed;
        return CAN_TxStatus_Ok;
    }
    else if(enum_CONNECT_CAN == USE_CAN1)
    {
        MailBox = CAN_Transmit(CAN1, &TxMsg);
        if(MailBox == CAN_TxStatus_NoMailBox)
            return CAN_TxStatus_Failed;
        return CAN_TxStatus_Ok;
    }
     else
    {
        return CAN_TxStatus_Failed;
    }

    #else
      #if enum_CONNECT_CAN == USE_CAN2
        MailBox = CAN_Transmit(CAN2, &TxMsg);
        if(MailBox == CAN_TxStatus_NoMailBox)
            return CAN_TxStatus_Failed;
        return CAN_TxStatus_Ok;
      #elif enum_CONNECT_CAN == USE_CAN1
        MailBox = CAN_Transmit(CAN1, &TxMsg);
        if(MailBox == CAN_TxStatus_NoMailBox)
            return CAN_TxStatus_Failed;
        return CAN_TxStatus_Ok;
      #endif
    #endif
   
}


/**
  * @brief  ����4��int16����������
  * @param  toWhere: ���յİ��ӵ�ID
  *         cmd    : ��������
  *         moto1-moto4:�ĸ������Ϣ
  *         
  *         
  * @retval CAN_TxStatus_Ok, CAN_TxStatus_Failed
  */

uint8_t ControlPlate_API_Send_int16(uint16_t toWhere, uint8_t cmd, int16_t data1, int16_t data2, int16_t data3, int16_t data4)
{
    int16_t txBuff[4];
    
    txBuff[0] = data1;
    txBuff[1] = data2;
    txBuff[2] = data3;
    txBuff[3] = data4;
    return ControlPlate_SendData(toWhere, cmd, (uint8_t*)txBuff);
    
}


/**
  * @brief  ���ͽǶȵ���������
  * @param  toWhere: ���յİ��ӵ�ID
  *         moto1-moto4:�ĸ������Ϣ
  *         
  *         
  * @retval None
  */

void ControlPlate_API_GetSpeed    (int16_t* v1, int16_t* v2, int16_t* v3, int16_t* v4)
{
    *v1 = *(int16_t*)g_ControlPlateRx_t.val1;
    *v2 = *(int16_t*)g_ControlPlateRx_t.val2;
    *v3 = *(int16_t*)g_ControlPlateRx_t.val3; 
    *v4 = *(int16_t*)g_ControlPlateRx_t.val4;
}
/**
  * @brief  ��ȡ����
  *
  * @param  �ĸ������ݵ�ָ��
  * @retval None
  */

void ControlPlate_API_GetCurrent  (int16_t* I1, int16_t* I2, int16_t* I3, int16_t* I4)
{
    *I1 = *(int16_t*)g_ControlPlateRx_t.val1;
    *I2 = *(int16_t*)g_ControlPlateRx_t.val2;
    *I3 = *(int16_t*)g_ControlPlateRx_t.val3; 
    *I4 = *(int16_t*)g_ControlPlateRx_t.val4;
}

/**
  * @brief  ��ȡλ��
  *
  * @param  �ĸ������ݵ�ָ��
  * @retval None
  */

void ControlPlate_API_GetPosition (int32_t* p1, int32_t* p2, int32_t* p3, int32_t* p4)
{
     *p1 = *(int32_t*)g_ControlPlateRx_t.val1;
     *p2 = *(int32_t*)g_ControlPlateRx_t.val2;
     *p3 = *(int32_t*)g_ControlPlateRx_t.val3;
     *p4 = *(int32_t*)g_ControlPlateRx_t.val4; 
} 

/**
  * @brief  ��ȡ�Ƕ�
  *
  * @param  �ĸ������ݵ�ָ��
  * @retval None
  */

void ControlPlate_API_GetAngle(int16_t* a1, int16_t* a2, int16_t* a3, int16_t* a4)
{
    *a1 = *(int16_t*)g_ControlPlateRx_t.val1;
    *a2 = *(int16_t*)g_ControlPlateRx_t.val2;
    *a3 = *(int16_t*)g_ControlPlateRx_t.val3; 
    *a4 = *(int16_t*)g_ControlPlateRx_t.val4;
}

/**
  * @brief  ��ȡuint16
  *
  * @param  �ĸ������ݵ�ָ��
  * @retval None
  */

void ControlPlate_API_Get_int16     (int16_t* data1, int16_t* data2, int16_t* data3, int16_t* data4)

{
    *data1 = *(int16_t*)g_ControlPlateRx_t.val1;
    *data2 = *(int16_t*)g_ControlPlateRx_t.val2;
    *data3 = *(int16_t*)g_ControlPlateRx_t.val3; 
    *data4 = *(int16_t*)g_ControlPlateRx_t.val4;
}

void ControlPlate_API_GetControlInfo(uint8_t* controlInfo)
{
    controlInfo[0] = g_ControlPlateRx_t.val1[0]  ;
    controlInfo[1] = g_ControlPlateRx_t.val1[1]  ;
    controlInfo[2] = g_ControlPlateRx_t.val1[2]  ;
    controlInfo[3] = g_ControlPlateRx_t.val1[3]  ;
    controlInfo[4] = g_ControlPlateRx_t.val2[0]  ;
    controlInfo[5] = g_ControlPlateRx_t.val2[1]  ;
    controlInfo[6] = g_ControlPlateRx_t.val2[2]  ;
    controlInfo[7] = g_ControlPlateRx_t.val2[3]  ;
}

uint16_t ControlPlate_API_GetAllCMD(void)
{
    return g_ControlPlateRx_t.cmd;
}


/**
  * @brief  �����Ƿ񷢸��Լ�
  * @param  
  * @retval 1:��, 0:��
  */
uint8_t ControlPlate_API_IsCmdToSelf(void)
{
    if(g_PLATE_ID == (g_ControlPlateRx_t.cmd & PLATE_ID_GET))
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}
uint8_t  ControlPlate_API_GetPlateID (void)
{
    
    return g_PLATE_ID;
}

pTYPE_PlateRxInfo_t ControlPlate_API_GetPlateRxInfo(void)
{    
    return &g_ControlPlateRx_t;
}



void ControlPlate_API_Change_ConnectCan(uint16_t i)
{
    #if COULD_CONNECT_CAN_CHANGE
    if (i < 3)
        enum_CONNECT_CAN = (Tyep_Enum_CONNECT_CAN) i;
    #endif
}

void ControlPlate_API_Change_PlateID(uint16_t id)
{
    #if COULD_PLATE_ID_CHANGE
    if (id&0x0F)
        return;
    id &= 0xF0;
    if(id > 0x0F && id < 0xF0)
       
        g_PLATE_ID = id;
    #endif
}









