#include	"pic.h"
	__CONFIG (HS & WDTDIS & PWRTDIS & BORDIS & LVPDIS & UNPROTECT & DUNPROT & DEBUGDIS); //0x3F3A
//==================================================================================================================
void Clearports(void);			// Очистка портов
void Propertiesports(void); 	// Настройка портов
void Clrreg(void);				// Инициализация значений
//==================================================================================================================
void LCD_Init(void);			// Инициализация дисплея LCD
void LCD_Clear(void);			// Очистка дисплея и установка курсора в адрес "0"
void LCD_Home(void);			// Установка курсора в адрес "0"
void LCD_Ems(void);				// Дисплей отключен и сдвиг курсора вправо
void LCD_Dooc(void);			// Дисплей включен, курсор отключен и режим мигания отключен
void LCD_Cdcp(void);			// Сдвиг дисплея вправо
void LCD_Cdsl(void);			// Сдвиг дисплея влево
void LCD_sendCmd(char Cmd);		// Указание на запись команды в LCD
void LCD_sendData(char Data);	// Указание на запись данных в LCD
void LCD_send(void);			// Запись в LCD
void LCD_4Hbita(void);			// Запись старшей тетрады байта в LCD
void LCD_4Lbita(void);			// Запись младшей тетрады байта в LCD
void LCD_CLEARDISP(void);		// Очистка дисплея
void LCD_Time(void);			// Функция, отображающая время на LCD
void LCD_Alarm(void);			// Функция, отображающая будильник на LCD
void LCD(void);					// Время или будильник на LCD
//==================================================================================================================
void Tmr1config(void); 			// Настройка Timer1
//==================================================================================================================
void Delay200ms(void);			// Задержка в 200 мсек.
void Delay50ms(void);			// Задержка в 50 мсек.
void Delay5ms(void);			// Задержка в 5 мсек.
void Delay100mks(void);			// Задержка в 100 мксек.
void Delay50mks(void);			// Задержка в 50 мксек.
void Delay40mks(void);			// Задержка в 40 мксек.
void Delay4mks(void);			// Задержка в 4 мксек.
//==================================================================================================================
void Keyboard(void);			// Обработка клавиатуры
//==================================================================================================================
void hourplus(void);
void minplus(void);
void secplus(void);
void hourminus(void);
void minminus(void);			//События в соответствии с кнопками клавиатуры
void secminus(void);
void go(void);
void routing(void);
void alarm(void);
//==================================================================================================================
void Time_on_LCD(void);			// Логика расчета времени для LCD
void Al_Cl(void);				// Проверка будильника
void Alarmalarm(void);			// Функция сигнала будильника
//==================================================================================================================
void test(void);				// тест
//==================================================================================================================
void TX_BUF(char buf);			// Передача по USART
void Reciever_on(void);			// Настройка приемопередатчика USART
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
char ClearDisplay = 0b00000001; // Команда "Clear Display"
char CurAtHome = 0b00000010;	// Команда "Cursor At Home"
char EntryModeSet = 0b00000110; // Команда "Entry Mode Set"
//                        	||________________  Cдвиг дисплея "1" - включен, "0" - отключен
//                          |__________________ Сдвиг курсора "1" - вправо,  "0" - влево
char DisplControl = 0b00001100; // Команда "Display On/Off Control"
//                         |||_________________ Режим мигания "1" - включен, "0" - отключен
//                         ||__________________ Курсор        "1" - включен, "0" - отключен
//                         |___________________ Дисплей       "1" - включен, "0" - отключен
char FunctionSet = 0b00101000;  // Команда "Function Set"
//                      |||___________________ Шрифт     "1" - 5х10 точек, "0" - 5х7 точек
//                      ||____________________ Строки    "1" - 2 строки, "0" - 1 строка
//                      |_____________________ Интерфейс "1" - 8 бит, "0" - 4 бит
char CurDispShiftL = 0b00011000;// Команда "Cursor/Display Shift"
//                         ||___________________ Сдвиг "1" - вправо,  "0" - влево 
//                         |____________________ Cдвиг "1" - дисплея, "0" - курсора 
char CurDispShiftP = 0b00011100;// Команда "Cursor/Display Shift"
//                         ||___________________ Сдвиг "1" - вправо,  "0" - влево 
//                         |____________________ Cдвиг "1" - дисплея, "0" - курсора 
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
char gap = 0x20;				// пробел
char colon = 0x3A;				// двоеточие
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
// Обработка прерывания по таймеру
//==================================================================================================================
void interrupt tmr1(void) @ 0x04 {
	GIE = 0;
	if (TMR1IF==1) { 			// проверка TMR1F(PIR1)
		if(run == 1) {
		Time_on_LCD();
		goto	mic;
		

		Al_Cl();
		Alarmalarm();
		}

		TMR1IF = 0;
		GIE = 1; 				//разрешаем все прерывания(INTCON)
		TMR1L = 0xff;			//задержка на 1 сек.
		TMR1H = 0x7f;
	return;
	} 

return;
}
//==================================================================================================================
// Начало основной программы
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
	PORTB = 0b00000000;			// очистка содержимого порта B
	PORTC = 0b00000000;			// очистка содержимого порта C
	PORTD = 0b00000000;			// очистка содержимого порта D
	PORTE = 0b00000000;			// очистка содержимого порта E	

return;
}
//==================================================================================================================
void Propertiesports(void) {
   	RP0 = 1;
	TRISA = 0b00000000;			// установка порта A на выход
	TRISB = 0b00111000;			// клавиатура
	TRISC = 0b11000000;			// USART
	TRISD = 0b00000000;			// установка порта D на выход
	TRISE = 0b00000000;			// установка порта E на выход 
	ADCON1 = 0x06;				// PORTA0-PORTA3 - цифровые порты.
	RBPU = 0;					// подтягивающие резисторы включены
	RP0 = 0;

return;
}
//==================================================================================================================
// Настройка Timer1
//==================================================================================================================
void Tmr1config(void) {
	INTCON = 0b11000000;		// конфигурируем INTCON. Биты разрешений и флаги прерываний:
	RP0 = 1;					// устанавливаем 1-й банк памяти
	PIE1 = 0b00000001;			// конфигурируем PIE1 (Биты разрешения периферийных прерываний)
	RP0 = 0;					// очистили старший байт регистра TMR1
	TMR1H = 0;					// очистили младший байт регистра TMR1
	TMR1L = 0;
	T1CON = 0b00001111;			// конфигурируем T1CON
	RP0 = 0;

return;					
}
//==================================================================================================================
// Функции, необходимые для LCD
//==================================================================================================================
//  PORTA,2  LCD_RS     - Сигнал выбора режима LCD ("0" - команда, "1" - данные)
//  PORTA,1  LCD_RW     - Сигнал быбора операции LCD ("0" - запись, "1" - чтение)
//  PORTA,0  LCD_EN     - Сигнал стробирования\синхронизации LCD
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
// Функция, отображающая время на LCD
//==================================================================================================================
void LCD_Time(void) {
		LCD_sendCmd(0b10000000); 			// Перевод строки
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
		LCD_sendData(gap);					// пробел
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
		LCD_sendCmd(0b11000000); 			// Перевод строки
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
		LCD_sendData(gap);					// пробел
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
		LCD_sendData(gap);					// пробел
		Delay40mks();						//
//==================================================================================================================
		if(clock) {
		LCD_sendData(o);					// o
		Delay40mks();						//
		LCD_sendData(n);					// n
		Delay40mks();						//
		LCD_sendData(gap);					// пробел
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
// Функция, отображающая будильник на LCD
//==================================================================================================================
void LCD_Alarm(void) {
		LCD_sendCmd(0b10000000); 			// Перевод строки
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
		LCD_sendData(gap);					// пробел
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
		LCD_sendData(gap);					// пробел
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
		LCD_sendCmd(0b11000100); 			// Перевод строки
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
// Время или будильник на LCD
//==================================================================================================================
mic:void LCD(void) {
		if(!flag) { LCD_Time(); }
		else { LCD_Alarm(); }

return;
}
//==================================================================================================================
// Настройка приемопередатчика USART
//==================================================================================================================
void Reciever_on(void) {		
	RP0 =1;				
	SPBRG = 0x81;				// значение регистра SPBRG для 2400 бод на 20 мегагерцах
 
	BRGH = 0;					// установка асинхронного режима
	TXEN = 0;					// разрешение передачи от контроллера
	RCIE = 1;					// разрешение прерывания от приёмника USART
	RP0 = 0;
	SPEN = 1;					// разрешение последовательного порта
	CREN = 1;					// разрешение непрерывного приёма

return;
}
//==================================================================================================================
// Задержки
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
// Инициализация переменных
//==================================================================================================================
void Clrreg(void) {				// Инициализация значений	
	RP0 = 0;
	
return;						
}
//==================================================================================================================
// Передача по USART
//==================================================================================================================
void TX_BUF(char buf) {			// Передача по USART
	TXREG = buf;
	RP0	= 1;	
	TXEN = 1;
	RP0 = 0;
	Delay5ms();

return;
}
//==================================================================================================================
// Обработка клавиатуры
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
// События в соответствии с кнопками клавиатуры
//==================================================================================================================
void hourplus(void) {
	Delay200ms();

	if(!flag) {
//время===================================
	if(Hour != 9 && !(TenHour == 2 && Hour == 3)) { Hour++; LCD(); }
	else if( TenHour != 2) {Hour = 0; TenHour++; LCD(); }
	else   { Hour = TenHour = 0; LCD(); }
	
return;
	}
	else {
//будильник===============================
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
//время===================================
	if(Min != 9) { Min++; LCD(); }
	else if(TenMin != 5) { Min = 0; TenMin++; LCD(); }
	else   { Min = TenMin = 0; LCD(); }

return;
	}
	else {
//будильник===============================
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
//время===================================
	if(Sec != 9) { Sec++; LCD();}
	else if(TenSec != 5) {Sec = 0; TenSec++; LCD(); }
	else   { Sec = TenSec = 0; LCD(); }

return;
	}
	else {
//будильник===============================
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
//время===================================
	if(Hour != 0) { Hour--; LCD(); }
	else if( TenHour != 0) {Hour = 9; TenHour--; LCD(); }
	else   { Hour = 3; TenHour = 2; LCD(); }
	
return;
	}
	else {
//будильник===============================
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
//время===================================
	if(Min != 0) { Min--; LCD(); }
	else if(TenMin != 0) {Min = 9; TenMin--; LCD(); }
	else   { Min = 9; TenMin = 5; LCD(); }

return;
	}
	else {
//будильник===============================
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
//время===================================
	if(Sec != 0) { Sec--; LCD();}
	else if(TenSec != 0) {Sec = 9; TenSec--; LCD(); }
	else   { Sec = 9; TenSec = 5; LCD(); }

return;
	}
	else {
//будильник===============================
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
// Логика расчета времени для LCD
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
// Передача данных по USART ведомым
//==================================================================================================================
