#include "stm32f10x.h"                  // Device header
#include "leoSPI.h"
#include "W25Q64_Ins.h"

/*ʹ��W25Q64ģ�飬ֻ֧��ģʽ0��3����ģʽ0*/
/*������ַ��xxx000-xxxFFF*/
/*ҳ��ַ��xxxx00-xxxxFF*/

void W25Q64_Init(void)	//W25Q64��ʼ��
{
		leoSPI_Init();
}

void W25Q64_WaitBuzy(void)	//�ȴ�����Buzyģʽ
{
		uint32_t Timeout = 10000;	//����ѭ��
		
		leoSPI_Start();
		leoSPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);	//���Ͷ�״̬�Ĵ���1ָ��
		while ((leoSPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
		{
				Timeout --;
				if (Timeout == 0) break;
		}
		leoSPI_Stop();
}

void W25Q64_WriteEnable(void)	//дʹ��
{
		W25Q64_WaitBuzy();
		
		leoSPI_Start();	//��ʼ
		leoSPI_SwapByte(W25Q64_WRITE_ENABLE);	//����дʹ��ָ�����ֵ������
		leoSPI_Stop();	//��ֹ
}

void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)	//ָ���ȡ����ID���豸ID
{
		W25Q64_WaitBuzy();
		
		leoSPI_Start();	//��ʼ
		leoSPI_SwapByte(W25Q64_JEDEC_ID);	//���Ͷ�IDָ�����ֵ������
		*MID = leoSPI_SwapByte(W25Q64_DUMMY_BYTE);	//����MID������ֵ������
		*DID = (leoSPI_SwapByte(W25Q64_DUMMY_BYTE) << 8);	//����DID�߰�λ
		*DID |= leoSPI_SwapByte(W25Q64_DUMMY_BYTE);	//����DID�Ͱ�λ
		leoSPI_Stop();	//��ֹ
}

void W25Q64_PageProgram(uint32_t Address, uint8_t *OutDataArray, uint16_t CountMax256)	//ָ��ҳ���д�����ݣ���ַ�����顢������
{
		W25Q64_WriteEnable();	//дʹ��;
		
		leoSPI_Start();
		leoSPI_SwapByte(W25Q64_PAGE_PROGRAM);	//����ҳ���ָ��
		
		//��������������ַ
		leoSPI_SwapByte(Address >> 16);
		leoSPI_SwapByte(Address >> 8);
		leoSPI_SwapByte(Address);		
		
		//���η���д������
		for (uint16_t i = 0; i < CountMax256; i ++)
		{
				leoSPI_SwapByte(OutDataArray[i]);	
		}
		
		leoSPI_Stop();
}

void W25Q64_SectorErase(uint32_t Address)	//ָ����������
{
		W25Q64_WriteEnable();	//дʹ��
	
		leoSPI_Start();
		leoSPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);	
		
		//��������������ַ
		leoSPI_SwapByte(Address >> 16);
		leoSPI_SwapByte(Address >> 8);
		leoSPI_SwapByte(Address);		
		
		leoSPI_Stop();
}

void W25Q64_ReadData(uint32_t Address, uint8_t *InDataArray, uint32_t Count)	//ָ����ַ��ȡ����
{
		W25Q64_WaitBuzy();
	
		leoSPI_Start();
		leoSPI_SwapByte(W25Q64_READ_DATA);	//���Ͷ�����ָ��
		
		//��������������ַ
		leoSPI_SwapByte(Address >> 16);
		leoSPI_SwapByte(Address >> 8);
		leoSPI_SwapByte(Address);		
	
		//��������
		for (uint32_t i = 0; i < Count; i ++)
		{
				InDataArray[i] = leoSPI_SwapByte(W25Q64_DUMMY_BYTE);
		}	
		
		leoSPI_Stop();
}
