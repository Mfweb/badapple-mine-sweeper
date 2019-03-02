#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <tlhelp32.h>
#include <time.h>
#include "process.h"
//http://mfweb.top
unsigned char get_all(HWND WindowList[4]);
void init(void);
void Deinit(void);
void Change_Window(unsigned char buff[2][30],unsigned char select);
void Window_Clear(void);
void Update_Window(void);
void Out_Page(unsigned char buff[4][60]);

HWND window_list[4];
DWORD porcess_list[4];
HANDLE porcess_handle_list[4];
		// Address = 1005340h + 高度*32 + 宽度
		// 0x1005140  鼠标按下
		// 0x100511C  鼠标按下时候选中的X方格
		// 0x1005118  鼠标按下时候选中的Y方格
		// 0x1005164  是否开始计时
		// 0x100579C  时间
		// 
unsigned char t1[][60]=
{
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x10,0xD0,0xF0,0xD0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x70,0xF0,0xF0,0xF0,0xD0,0xD0,0xD0,0xD0,0xD0,0x90,0x90,0x90,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x07,0xFF,0xFF,0xFE,0x80,0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xE0,0xE1,0xE1,0xE1,0xE1,0xF1,0x73,0x7B,0x7F,0x3F,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x1F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x30,0x78,0x78,0xF8,0xFC,0xFC,0xFC,0xFE,0xFE,0xEE,0xEF,0xEF,0xE7,0xE1,0xE0,0xE0,0xE0,0xC0,0x00,0x00,0x00,0x01,0x01,0x01,0x03,0x03,0x0F,0x9F,0xFE,0xF8,0xF0,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x0A,0x1E,0x1E,0x1E,0x1E,0x1E,0x0E,0x0E,0x0E,0x0F,0x07,0x07,0x07,0x03,0x03,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};

void buff_clear(unsigned char buff2[][60])
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<60;j++)
			buff2[i][j]=0x00;
	}
}

void read_once(unsigned char *buff,int ct)
{
	FILE *pFile=fopen("2.bin","r");
	fseek(pFile,ct*(4*60),SEEK_SET);
	printf("read:%d \r\n",fread(buff,1,4*60,pFile));
	fclose(pFile);
}
int main(int argc, char* argv[])
{
	int i = 0;
	int j = 0;
	unsigned char temp[4][60];
	printf("   By  Mfweb\r\n\r\n");
	if(!get_all(window_list))
	{
		printf("Can not found");
		system("PAUSE");
		return 1;
	}

	printf("hwnd:");
	for(i=0;i<4;i++)
		printf("%x ",window_list[i]);

	printf("\r\nPID:");
	for(i=0;i<4;i++)
	{
		GetWindowThreadProcessId(window_list[i],porcess_list+i);
		printf("%u ",porcess_list[i]);
	}
	printf("\r\n");
	
	buff_clear(temp);

	init();
	Window_Clear();
	printf("Clear windows.\r\n");
	Out_Page(t1);
	printf("按顺序摆好窗口\r\n");
	system("PAUSE");

	int count = 0;
	int index = 0;
	int delay_time;
	//PlaySound((LPCSTR)"1.wav",0,SND_FILENAME|SND_SYNC);
	while(1)
	{
		delay_time = clock();
		unsigned char pBuf[4*60];
		read_once(pBuf,count);
		for(i=0;i<4;i++)
		{
			for(j=0;j<60;j++)
			{
				temp[i][j] = pBuf[i*60+j];
			}
		}
		count ++;
		Out_Page(temp);
		printf("%06d",count);
		if(count>=6523)break;
		while(clock()-delay_time<33);		//控制到30帧每秒
	}
	printf("END\r\n");
	Deinit();
	system("PAUSE");
	return 1;
}

void Out_Page(unsigned char buff[4][60])
{
	unsigned char temp0[2][30];
	unsigned char temp1[2][30];
	unsigned char temp2[2][30];
	unsigned char temp3[2][30];
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<60;j++)
		{
			if(i<=1 && j<=29)
			{
				temp1[i][j] = buff[i][j];
			}
			else if(i<=1 && j>29)
			{
				temp0[i][j-30] = buff[i][j];
			}
			else if(i>1 && j<=29)
			{
				temp2[i-2][j] = buff[i][j];
			}
			else if(i>1 && j>29)
			{
				temp3[i-2][j-30] = buff[i][j];
			}
		}
	}
	Change_Window(temp0,0);
	Change_Window(temp1,1);
	Change_Window(temp2,2);
	Change_Window(temp3,3);
	Update_Window();
}

unsigned char get_all(HWND WindowList[4])
{
	int i,count = 0;
	HWND hWnd;
	char txt_temp[10]={'\0'};
	for(i=0;i<100;i++)
	{
		if(i == 0)
			hWnd = FindWindow(NULL,"扫雷");
		else
			hWnd = GetNextWindow(hWnd,GW_HWNDNEXT);
		int bk = GetWindowText(hWnd,txt_temp,10);
		if(bk == 4 && !strcmp(txt_temp,"扫雷"))
			WindowList[count++] = hWnd;

		if(count >= 4)break;
	}
	if(count !=4 )return 0;
	else return 1;
}

void init(void)
{
	int starta=0x0;
	int i;
	DWORD temp;
	for(i = 0; i < 4; i++)
	{
		porcess_handle_list[i] = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,0,porcess_list[i]);
		if(porcess_handle_list[i] == NULL)
		{
			printf("open error!\r\n");
			break;
		}
		starta = 30*16;
		WriteProcessMemory(porcess_handle_list[i],(LPVOID)0x01005194,&starta,4,&temp);//修改插旗数
		starta = 1;
		WriteProcessMemory(porcess_handle_list[i],(LPVOID)0x01005164,&starta,4,&temp);//开始计时
	}
}

void Deinit(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		CloseHandle(porcess_handle_list[i]);
	}
}

void Window_Clear(void)
{
	unsigned char temp1[2][30];
	int i,j;

	for(i=0;i<2;i++)
		for(j=0;j<30;j++)
			temp1[i][j] = 0x00;
	for(i=0;i<4;i++)
	{
		Change_Window(temp1,i);
	}
	Update_Window();
}

void Change_Window(unsigned char buff[2][30],unsigned char select)
{
	int j,i;
	unsigned int read_l = 0x00;
	DWORD temp;
	for(i=0;i<16;i++)
	{
		for(j=0;j<30;j++)
		{
			read_l = (((buff[i/8][j]>>(i/8?i-8:i))&0x01)?0x0e:0x0f);
			WriteProcessMemory(porcess_handle_list[select],(LPVOID)(0x1005340 +(i+1)*32+j+1),&read_l,1,&temp);
		}
	}
}

void Update_Window(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		InvalidateRect(window_list[i],NULL,0);
		//UpdateWindow(window_list[i]);		
	}
}

