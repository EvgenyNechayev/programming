#include	"pic.h"
	__CONFIG (HS & WDTDIS & PWRTDIS & BORDIS & LVPDIS & UNPROTECT & DUNPROT & DEBUGDIS); //0x3F3A
//==================================================================================================================
void Clearports(void);			// ������� ������
void Propertiesports(void); 	// ��������� ������
void Clrreg(void);				// ������������� ��������
//==================================================================================================================
void LCD_Init(void);			// ������������� ������� LCD
void LCD_Clear(void);			// ������� ������� � ��������� ������� � ����� "0"
void LCD_Home(void);			// ��������� ������� � ����� "0"
void LCD_Ems(void);				// ������� �������� � ����� ������� ������
void LCD_Dooc(void);			// ������� �������, ������ �������� � ����� ������� ��������
void LCD_Cdcp(void);			// ����� ������� ������
void LCD_Cdsl(void);			// ����� ������� �����
void LCD_sendCmd(char Cmd);		// �������� �� ������ ������� � LCD
void LCD_sendData(char Data);	// �������� �� ������ ������ � LCD
void LCD_send(void);			// ������ � LCD
void LCD_4Hbita(void);			// ������ ������� ������� ����� � LCD
void LCD_4Lbita(void);			// ������ ������� ������� ����� � LCD
void LCD_CLEARDISP(void);		// ������� �������
void LCD_Time(void);			// �������, ������������ ����� �� LCD
void LCD_Alarm(void);			// �������, ������������ ��������� �� LCD
void LCD(void);					// ����� ��� ��������� �� LCD
//==================================================================================================================
void Tmr1config(void); 			// ��������� Timer1
//==================================================================================================================
void Delay200ms(void);			// �������� � 200 ����.
void Delay50ms(void);			// �������� � 50 ����.
void Delay5ms(void);			// �������� � 5 ����.
void Delay100mks(void);			// �������� � 100 �����.
void Delay50mks(void);			// �������� � 50 �����.
void Delay40mks(void);			// �������� � 40 �����.
void Delay4mks(void);			// �������� � 4 �����.
//==================================================================================================================
void Keyboard(void);			// ��������� ����������
//==================================================================================================================
void hourplus(void);
void minplus(void);
void secplus(void);
void hourminus(void);
void minminus(void);			//������� � ������������ � �������� ����������
void secminus(void);
void go(void);
void routing(void);
void alarm(void);
//==================================================================================================================
void Time_on_LCD(void);			// ������ ������� ������� ��� LCD
void Al_Cl(void);				// �������� ����������
void Alarmalarm(void);			// ������� ������� ����������
//==================================================================================================================
void test(void);				// ����
//==================================================================================================================
void TX_BUF(char buf);			// �������� �� USART
void Reciever_on(void);			// ��������� ����������������� USART
//==================================================================================================================
volatile unsigned char DATACMDLCD @ 0x20;
/*      DATACMDLCD bits      */
static volatile bit	DC7	@ (unsigned)&DATACMDLCD*8+7;
static volatile bit	DC6	@ (unsigned)&DATACMDLCD*8+6;
static volatile bit	DC5	@ (unsigned)&DATACMDLCD*8+5;
static volatile bit	DC4	@ (unsigned)&DATACMDLCD*8+4;
static volatile bit	DC3	@ (unsigned)&DATACMDLCD*8+3;
static volatile bit	DC2	@ (unsigned)&DATACMDLCD*8+2;
static volatile bit	DC1	@ (unsigned)&DATACMDLCD*8+1;
static volatile bit	DC0	@ (unsigned)&DATACMDLCD*8+0;
//==================================================================================================================
char Sc=0,TS=0,M=0,TM=0;
char Ho=0,TH=0;
char Sec=0,TenSec=0,Min=0,TenMin=0,Hour=0,TenHour=0;
char flag=0,clock=0,run=0;

//==================================================================================================================
char ClearDisplay = 0b00000001; // ������� "Clear Display"
char CurAtHome = 0b00000010;	// ������� "Cursor At Home"
char EntryModeSet = 0b00000110; // ������� "Entry Mode Set"
//                        	||________________  C���� ������� "1" - �������, "0" - ��������
//                          |__________________ ����� ������� "1" - ������,  "0" - �����
char DisplControl = 0b00001100; // ������� "Display On/Off Control"
//                         |||_________________ ����� ������� "1" - �������, "0" - ��������
//                         ||__________________ ������        "1" - �������, "0" - ��������
//                         |___________________ �������       "1" - �������, "0" - ��������
char FunctionSet = 0b00101000;  // ������� "Function Set"
//                      |||___________________ �����     "1" - 5�10 �����, "0" - 5�7 �����
//                      ||____________________ ������    "1" - 2 ������, "0" - 1 ������
//                      |_____________________ ��������� "1" - 8 ���, "0" - 4 ���
char CurDispShiftL = 0b00011000;// ������� "Cursor/Display Shift"
//                         ||___________________ ����� "1" - ������,  "0" - ����� 
//                         |____________________ C���� "1" - �������, "0" - ������� 
char CurDispShiftP = 0b00011100;// ������� "Cursor/Display Shift"
//                         ||___________________ ����� "1" - ������,  "0" - ����� 
//                         |____________________ C���� "1" - �������, "0" - ������� 
//==================================================================================================================
char T = 0x54;
char i = 0x69;
char m = 0x6D;
char e = 0x65;
char A = 0x41;
char l = 0x6C;
char a = 0x61;
char r = 0x72;
char c = 0x63;
char o = 0x6F;
char k = 0x6B;
char n = 0x6E;
char f = 0x66;
char gap = 0x20;				// ������
char colon = 0x3A;				// ���������
char Table[] = { 
	0x30,						// 0
	0x31,						// 1
	0x32,						// 2
	0x33,						// 3
	0x34,						// 4
	0x35,						// 5
	0x36,						// 6
	0x37,						// 7
	0x38,						// 8
	0x39						// 9	
};
/*
char abc[] = {0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,
			  0x6E,0x6F,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A};
*/
//==================================================================================================================
// ��������� ���������� �� �������
//==================================================================================================================
void interrupt tmr1(void) @ 0x04 {
	GIE = 0;
	if (TMR1IF==1) { 			// �������� TMR1F(PIR1)
		if(run == 1) {
		Time_on_LCD();
		goto	mic;
		

		Al_Cl();
		Alarmalarm();
		}

		TMR1IF = 0;
		GIE = 1; 				//��������� ��� ����������(INTCON)
		TMR1L = 0xff;			//�������� �� 1 ���.
		TMR1H = 0x7f;
	return;
	} 

return;
}
//==================================================================================================================
// ������ �������� ���������
//==================================================================================================================
void main(void) {

	Propertiesports();
	Clearports();
	Clrreg();

	Reciever_on();
	Tmr1config();

	LCD_Init();
	Delay40mks();

	LCD_CLEARDISP();
	LCD();

	for(;;){ Keyboard(); }
}
//==================================================================================================================
void Clearports(void) {
	RP0 = 0;
	PORTA = 0b00000000;			
	PORTB = 0b00000000;			// ������� ����������� ����� B
	PORTC = 0b00000000;			// ������� ����������� ����� C
	PORTD = 0b00000000;			// ������� ����������� ����� D
	PORTE = 0b00000000;			// ������� ����������� ����� E	

return;
}
//==================================================================================================================
void Propertiesports(void) {
   	RP0 = 1;
	TRISA = 0b00000000;			// ��������� ����� A �� �����
	TRISB = 0b00111000;			// ����������
	TRISC = 0b11000000;			// USART
	TRISD = 0b00000000;			// ��������� ����� D �� �����
	TRISE = 0b00000000;			// ��������� ����� E �� ����� 
	ADCON1 = 0x06;				// PORTA0-PORTA3 - �������� �����.
	RBPU = 0;					// ������������� ��������� ��������
	RP0 = 0;

return;
}
//==================================================================================================================
// ��������� Timer1
//==================================================================================================================
void Tmr1config(void) {
	INTCON = 0b11000000;		// ������������� INTCON. ���� ���������� � ����� ����������:
	RP0 = 1;					// ������������� 1-� ���� ������
	PIE1 = 0b00000001;			// ������������� PIE1 (���� ���������� ������������ ����������)
	RP0 = 0;					// �������� ������� ���� �������� TMR1
	TMR1H = 0;					// �������� ������� ���� �������� TMR1
	TMR1L = 0;
	T1CON = 0b00001111;			// ������������� T1CON
	RP0 = 0;

return;					
}
//==================================================================================================================
// �������, ����������� ��� LCD
//==================================================================================================================
//  PORTA,2  LCD_RS     - ������ ������ ������ LCD ("0" - �������, "1" - ������)
//  PORTA,1  LCD_RW     - ������ ������ �������� LCD ("0" - ������, "1" - ������)
//  PORTA,0  LCD_EN     - ������ �������������\������������� LCD
void LCD_Init(void) {
	RA0 = 0;
	RA1 = 0;
	RA2 = 0;

	Delay50ms();
	LCD_sendCmd(FunctionSet);
	Delay5ms();
	LCD_sendCmd(FunctionSet);
	Delay100mks();
	LCD_sendCmd(FunctionSet);
	Delay50mks();
	LCD_sendCmd(FunctionSet);
	Delay50mks();
	LCD_Dooc();
	LCD_Clear();
	LCD_Home();

return;
}
void LCD_Clear(void){
	LCD_sendCmd(ClearDisplay);

return;
}
void LCD_Home(void){
	LCD_sendCmd(CurAtHome);

return;
}
void LCD_Ems(void){
	LCD_sendCmd(EntryModeSet);

return;
}
void LCD_Dooc(void){
	LCD_sendCmd(DisplControl);

return;
}
void LCD_Cdsp(void){
	LCD_sendCmd(CurDispShiftP);

return;
}
void LCD_Cdsl(void){
	LCD_sendCmd(CurDispShiftL);

return;
}
void LCD_CLEARDISP(void) {
	int i=0;
	LCD_sendCmd(0b10000000);
	Delay40mks();

	for(i = 0; i < 16; i++) {
		LCD_sendData(gap);
		Delay40mks();
	}

	LCD_sendCmd(0b11000000);
	Delay40mks();

	for(i = 0; i < 16; i++) {
		LCD_sendData(gap);
		Delay40mks();
	}
	
return;
}
void LCD_sendData(char Data) {
	DATACMDLCD = Data;
	RA2 = 1;
	RA1 = 0;
	LCD_send();

return;
}
void LCD_sendCmd(char Cmd) {
	DATACMDLCD = Cmd;
	RA2 = 0;
	RA1 = 0;
	LCD_send();

return;
}
void LCD_send(void) {

	LCD_4Hbita();
	RA0 = 1;
	RA0 = 0;

	LCD_4Lbita();
	RA0 = 1;
	RA0 = 0;
	Delay4mks();

	RD4 = 1;
	RD5 = 1;
	RD6 = 1;
	RD7 = 1;

return;
}
void LCD_4Hbita(void) {
		(DC4) ? RD4 = 1 : RD4 = 0;
		(DC5) ? RD5 = 1 : RD5 = 0;
		(DC6) ? RD6 = 1 : RD6 = 0;
		(DC7) ? RD7 = 1 : RD7 = 0;

return;
}
void LCD_4Lbita(void) {
		(DC0) ? RD4 = 1 : RD4 = 0;
		(DC1) ? RD5 = 1 : RD5 = 0;
		(DC2) ? RD6 = 1 : RD6 = 0;
		(DC3) ? RD7 = 1 : RD7 = 0;

return;
}
//==================================================================================================================
// �������, ������������ ����� �� LCD
//==================================================================================================================
void LCD_Time(void) {
		LCD_sendCmd(0b10000000); 			// ������� ������
		Delay40mks();						//

		LCD_sendData(T);					// T
		Delay40mks();						//
		LCD_sendData(i);					// i
		Delay40mks();						//
		LCD_sendData(m);					// m
		Delay40mks();						//
		LCD_sendData(e);					// e
		Delay40mks();						//
		LCD_sendData(colon);				// :
		Delay40mks();						//
		LCD_sendData(gap);					// ������
		Delay40mks();						//
//==================================================================================================================
		LCD_sendData(Table[TenHour]);		// TenHour
		Delay40mks();						//
		LCD_sendData(Table[Hour]);			// Hour
		Delay40mks();						//
		LCD_sendData(colon);				// :
		Delay40mks();						//
		LCD_sendData(Table[TenMin]);		// TenMin
		Delay40mks();						//
		LCD_sendData(Table[Min]);			// Min
		Delay40mks();						//
		LCD_sendData(colon);				// :
		Delay40mks();						//
		LCD_sendData(Table[TenSec]);		// TenSec
		Delay40mks();						//
		LCD_sendData(Table[Sec]);			// Sec
		Delay40mks();						//
//==================================================================================================================
		LCD_sendCmd(0b11000000); 			// ������� ������
		Delay40mks();						//

		LCD_sendData(A);					// A
		Delay40mks();						//
		LCD_sendData(l);					// l
		Delay40mks();						//
		LCD_sendData(a);					// a
		Delay40mks();						//
		LCD_sendData(r);					// r
		Delay40mks();						//
		LCD_sendData(m);					// r
		Delay40mks();						//
		LCD_sendData(gap);					// ������
		Delay40mks();						//
		LCD_sendData(c);					// c
		Delay40mks();						//
		LCD_sendData(l);					// l
		Delay40mks();						//
		LCD_sendData(o);					// o
		Delay40mks();						//
		LCD_sendData(c);					// c
		Delay40mks();						//
		LCD_sendData(k);					// k
		Delay40mks();						//
		LCD_sendData(colon);				// :
		Delay40mks();						//
		LCD_sendData(gap);					// ������
		Delay40mks();						//
//==================================================================================================================
		if(clock) {
		LCD_sendData(o);					// o
		Delay40mks();						//
		LCD_sendData(n);					// n
		Delay40mks();						//
		LCD_sendData(gap);					// ������
		Delay40mks();						//
		}
		else {
		LCD_sendData(o);					// o
		Delay40mks();						//
		LCD_sendData(f);					// f
		Delay40mks();						//
		LCD_sendData(f);					// f
		Delay40mks();						//
		}

return;
}		
//==================================================================================================================
// �������, ������������ ��������� �� LCD
//==================================================================================================================
void LCD_Alarm(void) {
		LCD_sendCmd(0b10000000); 			// ������� ������
		Delay40mks();						//

		LCD_sendData(A);					// A
		Delay40mks();						//
		LCD_sendData(l);					// l
		Delay40mks();						//
		LCD_sendData(a);					// a
		Delay40mks();						//
		LCD_sendData(r);					// r
		Delay40mks();						//
		LCD_sendData(m);					// r
		Delay40mks();						//
		LCD_sendData(gap);					// ������
		Delay40mks();						//
		LCD_sendData(c);					// c
		Delay40mks();						//
		LCD_sendData(l);					// l
		Delay40mks();						//
		LCD_sendData(o);					// o
		Delay40mks();						//
		LCD_sendData(c);					// c
		Delay40mks();						//
		LCD_sendData(k);					// k
		Delay40mks();						//
		LCD_sendData(colon);				// :
		Delay40mks();						//
		LCD_sendData(gap);					// :
		Delay40mks();						//
//==================================================================================================================
		if(clock) {
		LCD_sendData(o);					// o
		Delay40mks();						//
		LCD_sendData(n);					// n
		Delay40mks();						//
		LCD_sendData(gap);					// ������
		Delay40mks();						//
		}
		else {
		LCD_sendData(o);					// o
		Delay40mks();						//
		LCD_sendData(f);					// f
		Delay40mks();						//
		LCD_sendData(f);					// f
		Delay40mks();						//
		}
		LCD_sendCmd(0b11000100); 			// ������� ������
		Delay40mks();						//

		LCD_sendData(Table[M]);			// TenHour
		Delay40mks();						//
		LCD_sendData(Table[TM]);			// Hour
		Delay40mks();						//
		LCD_sendData(colon);				// :
		Delay40mks();						//
		LCD_sendData(Table[TM]);			// TenMin
		Delay40mks();						//
		LCD_sendData(Table[M]);				// Min
		Delay40mks();						//
		LCD_sendData(colon);				// :
		Delay40mks();						//
		LCD_sendData(Table[TS]);			// TenSec
		Delay40mks();						//
		LCD_sendData(Table[Sc]);			// Sec
		Delay40mks();						//
return;
}
//==================================================================================================================
// ����� ��� ��������� �� LCD
//==================================================================================================================
mic:void LCD(void) {
		if(!flag) { LCD_Time(); }
		else { LCD_Alarm(); }

return;
}
//==================================================================================================================
// ��������� ����������������� USART
//==================================================================================================================
void Reciever_on(void) {		
	RP0 =1;				
	SPBRG = 0x81;				// �������� �������� SPBRG ��� 2400 ��� �� 20 ����������
 
	BRGH = 0;					// ��������� ������������ ������
	TXEN = 0;					// ���������� �������� �� �����������
	RCIE = 1;					// ���������� ���������� �� �������� USART
	RP0 = 0;
	SPEN = 1;					// ���������� ����������������� �����
	CREN = 1;					// ���������� ������������ �����

return;
}
//==================================================================================================================
// ��������
//==================================================================================================================
void Delay200ms(void) {
	int i;
	char j;
	for(i = 0; i < 17550; ++i) {
		for(j = 0; j < 7; ++j) {}
	}

return;
}
void Delay50ms(void) {
	int i;
	for(i = 0; i < 16700; ++i) {}

return;
}
void Delay5ms(void) {
	int i;
	for(i = 0; i < 1667; ++i) {}

return;
}
void Delay100mks(void) {
	int i;
	for(i = 0; i < 33; ++i) {}

return;
}
void Delay50mks(void) {
	int i;
	for(i = 0; i < 17; ++i) {}

return;
}
void Delay40mks(void) {
	int i;
	for(i = 0; i < 13; ++i) {}

return;
}
void Delay4mks(void) {
	int i;
	for(i = 0; i < 1; ++i) {}

return;
}
//==================================================================================================================
// ������������� ����������
//==================================================================================================================
void Clrreg(void) {				// ������������� ��������	
	RP0 = 0;
	
return;						
}
//==================================================================================================================
// �������� �� USART
//==================================================================================================================
void TX_BUF(char buf) {			// �������� �� USART
	TXREG = buf;
	RP0	= 1;	
	TXEN = 1;
	RP0 = 0;
	Delay5ms();

return;
}
//==================================================================================================================
// ��������� ����������
//==================================================================================================================
void Keyboard(void) {
	RB0 = 0;
	RB1 = 1;
	RB2 = 1;

	if(RB3 == 0)		{ hourplus(); return; }
	else if(RB4 == 0)	{ minplus(); return; }
	else if(RB5 == 0)	{ secplus(); return; }

	RB0 = 1;
	RB1 = 0;
	RB2 = 1;

	if(RB3 == 0)		{ hourminus(); return; }
	else if(RB4 == 0)	{ minminus(); return; }
	else if(RB5 == 0)	{ secminus(); return; }

	RB0 = 1;
	RB1 = 1;
	RB2 = 0;

	if(RB3 == 0)		{ go(); return; }
	else if(RB4 == 0)	{ routing(); return; }
	else if(RB5 == 0)	{ alarm(); return; }

return;
}
//==================================================================================================================
// ������� � ������������ � �������� ����������
//==================================================================================================================
void hourplus(void) {
	Delay200ms();

	if(!flag) {
//�����===================================
	if(Hour != 9 && !(TenHour == 2 && Hour == 3)) { Hour++; LCD(); }
	else if( TenHour != 2) {Hour = 0; TenHour++; LCD(); }
	else   { Hour = TenHour = 0; LCD(); }
	
return;
	}
	else {
//���������===============================
	if(Ho != 9 && !(TH == 2 && Ho == 3)) { Ho++; LCD(); }
	else if( TH != 2) {Ho = 0; TH++; LCD(); }
	else   { Ho = TH = 0; LCD(); }
	}

return;
}
//==================================================================================================================
void minplus(void) {
	Delay200ms();

	if(!flag) {
//�����===================================
	if(Min != 9) { Min++; LCD(); }
	else if(TenMin != 5) { Min = 0; TenMin++; LCD(); }
	else   { Min = TenMin = 0; LCD(); }

return;
	}
	else {
//���������===============================
	if(M != 9) { M++; LCD(); }
	else if(TM != 5) { M = 0; TM++; LCD(); }
	else   { M = TM = 0; LCD(); }
	}

return;
}
//==================================================================================================================
void secplus(void) {
	Delay200ms();

	if(!flag) {
//�����===================================
	if(Sec != 9) { Sec++; LCD();}
	else if(TenSec != 5) {Sec = 0; TenSec++; LCD(); }
	else   { Sec = TenSec = 0; LCD(); }

return;
	}
	else {
//���������===============================
	if(Sc != 9) {Sc++; LCD(); }
	else if(TS != 5) {Sc = 0; TS++; LCD(); }
	else   { Sc = TS = 0; LCD(); }
	}

return;
}
//==================================================================================================================
void hourminus(void) {
	Delay200ms();

	if(!flag) {
//�����===================================
	if(Hour != 0) { Hour--; LCD(); }
	else if( TenHour != 0) {Hour = 9; TenHour--; LCD(); }
	else   { Hour = 3; TenHour = 2; LCD(); }
	
return;
	}
	else {
//���������===============================
	if(Ho != 0) { Ho--; LCD(); }
	else if( TH != 0) {Ho = 9; TH--; LCD(); }
	else   { Ho = 3; TH = 2; LCD(); }
	}

return;
}
//==================================================================================================================
void minminus(void) {
	Delay200ms();

	if(!flag) {
//�����===================================
	if(Min != 0) { Min--; LCD(); }
	else if(TenMin != 0) {Min = 9; TenMin--; LCD(); }
	else   { Min = 9; TenMin = 5; LCD(); }

return;
	}
	else {
//���������===============================
	if(M != 0) { M--; LCD(); }
	else if(TM != 0) {M = 9; TM--; LCD(); }
	else   { M = 9; TM = 5; LCD(); }
	}

return;
}
//==================================================================================================================
void secminus(void) {
	Delay200ms();

	if(!flag) {
//�����===================================
	if(Sec != 0) { Sec--; LCD();}
	else if(TenSec != 0) {Sec = 9; TenSec--; LCD(); }
	else   { Sec = 9; TenSec = 5; LCD(); }

return;
	}
	else {
//���������===============================
	if(Sc != 0) { Sc--; LCD();}
	else if(TS != 0) {Sc = 9; TS--; LCD(); }
	else   { Sc = 9; TS = 5; LCD(); }
	}

return;
}
//==================================================================================================================
void go(void) {
	Delay200ms();

	if(run) run = 0;
	else run = 1;

return;
}
//==================================================================================================================
void routing(void) {
	Delay200ms();

	if(flag) {
		flag = 0;
		LCD_CLEARDISP();
		LCD();
	}
	else {
		flag = 1;
		LCD_CLEARDISP();
		LCD();
	}	

return;
}
//==================================================================================================================
void alarm(void) {
	Delay200ms();

	if(clock) {
		clock = 0;
		LCD_CLEARDISP();
		LCD();
	}
	else {
		clock = 1;
		LCD_CLEARDISP();
		LCD();
	}	

return;
}
//==================================================================================================================
// ������ ������� ������� ��� LCD
//==================================================================================================================
void Time_on_LCD(void) {
	if(Sec != 9) {Sec++;}
	else if(TenSec != 5) {Sec = 0; TenSec++;}
	else if(Min != 9) {Sec = TenSec = 0; Min++;}
	else if(TenMin != 5) {Sec = TenSec = Min = 0; TenMin++;}
	else if(Hour != 9 && !(TenHour == 2 && Hour == 3))
       		{Sec = TenSec = Min = TenMin = 0; Hour++;}
	else if(TenHour != 2) {Sec = TenSec = Min = TenMin = Hour = 0; TenHour++;}
	else   {Sec = TenSec = Min = TenMin = Hour = TenHour = 0;}

return;
}

void Al_Cl(void) {
	
return;
}

void Alarmalarm(void) {

return;
}
//==================================================================================================================
// �������� ������ �� USART �������
//==================================================================================================================
