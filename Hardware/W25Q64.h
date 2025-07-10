#ifndef __W25Q64_H
#define __W25Q64_H

/*�ڲ�����*/
//void W25Q64_WriteEnable(void);	//дʹ��
//void W25Q64_WaitBuzy(void)	//�ȴ�����Buzyģʽ

void W25Q64_Init(void);	//W25Q64��ʼ��
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);	//ָ���ȡ����ID���豸ID
void W25Q64_PageProgram(uint32_t Address, uint8_t *OutDataArray, uint16_t CountMax256);	//ָ��ҳ���д�����ݣ���ַ�����顢������
void W25Q64_SectorErase(uint32_t Address);	//ָ����������
void W25Q64_ReadData(uint32_t Address, uint8_t *InDataArray, uint32_t Count);	//ָ����ַ��ȡ����

#endif
