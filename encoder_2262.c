//*******************************
#include <STC15Fxxxx.h>//头文件

#define BUFF_SIZE 16
u32 buff[BUFF_SIZE];

u32 *pHead = buff;
u32 *pTail = buff;

u8 idxHeadBit = 0;

sbit RF_TX P0^5;

void send(){
	if(pHead == pTail)
		return;
	
	if((*pHead) & (0x00000001 << idxHeadBit)){
		RF_TX = 1;
	}else{
		RF_TX = 0;
	}

	++idxHeadBit;
	if(idxHeadBit >= 8){
		pHead++;
		idxHeadBit = 0;
		if(pHead >= buff + BUFF_SIZE){
			pHead = buff;
		}
	}
}

void addBuff(u32 data){
	*pTail = data;
	pTail++;
	if(pTail >= buff + BUFF_SIZE){
		pTail = buff;
	}
}

/******* 发送数据“1”---12a 高+4a 低**************/
void sendbit_1()
{
	REM = 1;
	TH1 = TH1_12a; //12a
	TL1 = TL1_12a; //12a
	TR1 = 1;
	while (TR1); //高电平
	REM = 0;
	TH1 = TH1_4a; //4a
	TL1 = TL1_4a; //4a
	TR1 = 1;
	while (TR1); //低电平
}
/******* 发送数据“0”---4a 高+12a 低**************/
void sendbit_0()
{
	REM = 1;
	TH1 = TH1_4a; //4a
	TL1 = TL1_4a; //4a
	TR1 = 1;
	while (TR1); //高电平
	REM = 0;
	TH1 = TH1_12a; //12a
	TL1 = TL1_12a; //12a
	TR1 = 1;
	while (TR1); //低电平
}
/******* 发送数据“f”---4a 高+50ms 低**************/
//同步头
void sendRF_sof()
{
	REM = 1;
	TH1 = TH1_4a; //4a
	TL1 = TL1_4a; //4a
	TR1 = 1;
	while (TR1); //高电平
	REM = 0;
	TH1 = (65536 - 20000) / 256; //50ms
	TL1 = (65536 - 20000) % 256; //
	TR1 = 1;
	while (TR1); //低电平
}
// 发送一组编码，连续发送四次
void RFSendData(uchar ADDER, uchar Dat1)
www.lchstar.com
{
	sendRF_sof();
	SendINT(ADDER);
	SendINT(Dat1);
	sendRF_sof();
	SendINT(ADDER);
	SendINT(Dat1);
	sendRF_sof();
	SendINT(ADDER);
	SendINT(Dat1);
	sendRF_sof();
	SendINT(ADDER);
	SendINT(Dat1);
}