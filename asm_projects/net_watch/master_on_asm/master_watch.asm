	PROCESSOR P16F874		; Определяем тип процессора
	LIST N=75				; Количество строк на страницу
	TITLE "Nechaev Evgenij - for exhibition 2009"
	INCLUDE P16F874.INC		; подключаем файл с описанием констант процессора
	INCLUDE MACROSES.INC	; подключаем файл с макросами   
	__CONFIG _HS_OSC & _WDT_OFF & _PWRTE_OFF & _BODEN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _WRT_ENABLE_ON & _DEBUG_ON
		
		CBLOCK			0x20										; ? переменных
		COUNT1,COUNT2,COUNT3,DATACMDLCD
		ClearDisplay,CurAtHome,EntryModeSet,DisplControl
		FunctionSet,CurDispShift,CurDispShiftP,run
		WBuffer,StatBuffer,nine,five,three,two,one,zero
		T,i,m,e
		A,l,a,r,c,o,k
		n,f
		flag,clock,dec,siren,on_off
		Sec,TenSec,Min,TenMin,Hour,TenHour							; переменные для расчета времени
		Sc,TS,M,TM,H,TH
		colon,gap ; : & ' '
		
		ENDC

		ORG 		0x00					; установка счетчика команд на 0х00
		bcf     	INTCON,GIE      		; Запрет всех прерываний на время иниц. 
  		goto 		Start					; переходим на стартовую строчку программы

		ORG 		0x04 					; Переходим на адрес блока прерываний 0x04

		PUSH								; резервное копирование значений спец. регистров
		bcf 	INTCON,GIE					; запрещаем глобальные прерывания
		btfss	PIR1, TMR1IF				; проверяем, было ли переполнение TMR1
		goto	EndOfInterruption			; если нет, переходим на выход из блока обработки прерываний
;==================================================================================================================
; Начать основные функции
;==================================================================================================================

; Проверка запуска часов
		movf		run,0					; Временно сохранить W
		subwf		one,0					; сохраить результат в W
		btfss		STATUS,Z				; если Z==0, то результат ненулевой	
		goto		EndOfInterruption		; если Z==1, то результат нулевой
; Логика расчета времени для LCD
		call		Time_on_LCD				;
		call		LCD						;
		call		Al_Cl					;
		call		Alarmalarm				;
EndOfInterruption:
		bcf 	PIR1,TMR1IF					; сбрасываем флаг переполнения TMR1
		POP									; установка значений спец. регистров в начальное состояние
		bsf 	INTCON,GIE					; разрешаем глобальные прерывания
		movlw 	0xDC						; задержка на 850 mсек.
		movwf 	TMR1L						;
		movlw 	0x82						;
		movwf 	TMR1H						;
		return		

Table:	addwf		PCL						; 
		retlw		0x30					; 0
		retlw		0x31					; 1
		retlw		0x32					; 2
		retlw		0x33					; 3
		retlw		0x34					; 4
		retlw		0x35					; 5
		retlw		0x36					; 6
		retlw		0x37					; 7
		retlw		0x38					; 8
		retlw		0x39					; 9

;========================================================================
; Начало основной программы
;========================================================================
Start:
		call		InitPerem				;

		reciever_on							;

;Настройка Timer1
		movlw 		b'11000000'				; конфигурируем INTCON. Биты разрешений и флаги прерываний:
		movwf 		INTCON					;
		bsf 		STATUS,RP0				; устанавливаем 1-й банк памяти
		movlw 		b'00000001'				; конфигурируем PIE1 (Биты разрешения периферийных прерываний)
		movwf 		PIE1					; 
		bcf 		STATUS,RP0				; устанавливаем 0-й банк памяти
		clrf 		TMR1H					; очистили старший байт регистра TMR1
		clrf 		TMR1L					; очистили младший байт регистра TMR1
		movlw 		b'00001111'				; конфигурируем T1CON
		movwf 		T1CON					;

		call		LCD_Init				;
		call		Delay40mks				;

		call		LCD_CLEARDISP			;
		call		LCD						;

		call		Slave1					;
		call		Slave2					;
		call		Slave3					;
repeat:
		call		Key						;


		goto		repeat					;
;==================================================================================================================
; Функция, отображающая время на LCD
;==================================================================================================================
LCD_Time:

		movlw		b'10000000'				; 			Перевод строки
		call		LCD_sendCmd				; 
		call		Delay40mks				;
		movf		T,0						;
		call		LCD_sendData			;T
		call		Delay40mks				;
		movf		i,0						;
		call		LCD_sendData			;i
		call		Delay40mks				;
		movf		m,0						;
		call		LCD_sendData			;m
		call		Delay40mks				;
		movf		e,0						;
		call		LCD_sendData			;e
		call		Delay40mks				;
		movf		colon,0					;
		call		LCD_sendData			;:
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			; пробел
		call		Delay40mks				;
;==================================================================================================================
		movf		TenHour,0				;
		call		Table					;
		call		LCD_sendData			; TenHour
		call		Delay40mks				;
		movf		Hour,0					;
		call		Table					;
		call		LCD_sendData			; Hour
		call		Delay40mks				;
		movf		colon,0					;
		call		LCD_sendData			; :
		call		Delay40mks				;
		movf		TenMin,0				;
		call		Table					;
		call		LCD_sendData			; TenMin
		call		Delay40mks				;
		movf		Min,0					;
		call		Table					;
		call		LCD_sendData			; Min
		call		Delay40mks				;
		movf		colon,0					;
		call		LCD_sendData			; :
		call		Delay40mks				;
		movf		TenSec,0				;
		call		Table					;
		call		LCD_sendData			; TenSec
		call		Delay40mks				;
		movf		Sec,0					;
		call		Table					;
		call		LCD_sendData			; Sec
		call		Delay40mks				;

		movlw		b'11000000'				; 			Перевод строки
		call		LCD_sendCmd				; 
		call		Delay40mks				;
		movf		A,0						;
		call		LCD_sendData			;A
		call		Delay40mks				;
		movf		l,0						;
		call		LCD_sendData			;l
		call		Delay40mks				;
		movf		a,0						;
		call		LCD_sendData			;a
		call		Delay40mks				;
		movf		r,0						;
		call		LCD_sendData			;r
		call		Delay40mks				;
		movf		m,0						;
		call		LCD_sendData			;m
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			; пробел
		call		Delay40mks				;
		movf		c,0						;
		call		LCD_sendData			;c
		call		Delay40mks				;
		movf		l,0						;
		call		LCD_sendData			;l
		call		Delay40mks				;
		movf		o,0						;
		call		LCD_sendData			;o
		call		Delay40mks				;
		movf		c,0						;
		call		LCD_sendData			;c
		call		Delay40mks				;
		movf		k,0						;
		call		LCD_sendData			;k
		call		Delay40mks				;
		movf		colon,0					;
		call		LCD_sendData			;:
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			; пробел
		call		Delay40mks				;
;==================================================================================================================
		btfss		clock,0					; 
		goto		next1					;
		movf		o,0						;
		call		LCD_sendData			; o
		call		Delay40mks				;
		movf		n,0						;
		call		LCD_sendData			; n
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			; ' '
		call		Delay40mks				;
		goto		and2					;
next1:
		movf		o,0						;
		call		LCD_sendData			; o
		call		Delay40mks				;
		movf		f,0						;
		call		LCD_sendData			; f
		call		Delay40mks				;
		movf		f,0						;
		call		LCD_sendData			; f
		call		Delay40mks				;
and2:
		return								
;==================================================================================================================
; Функция, отображающая будильник на LCD
;==================================================================================================================
LCD_Alarm:

		movlw		b'10000000'				; 			Перевод строки
		call		LCD_sendCmd				; 
		call		Delay40mks				;
		movf		A,0						;
		call		LCD_sendData			;A
		call		Delay40mks				;
		movf		l,0						;
		call		LCD_sendData			;l
		call		Delay40mks				;
		movf		a,0						;
		call		LCD_sendData			;a
		call		Delay40mks				;
		movf		r,0						;
		call		LCD_sendData			;r
		call		Delay40mks				;
		movf		m,0						;
		call		LCD_sendData			;m
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			; пробел
		call		Delay40mks				;
		movf		c,0						;
		call		LCD_sendData			;c
		call		Delay40mks				;
		movf		l,0						;
		call		LCD_sendData			;l
		call		Delay40mks				;
		movf		o,0						;
		call		LCD_sendData			;o
		call		Delay40mks				;
		movf		c,0						;
		call		LCD_sendData			;c
		call		Delay40mks				;
		movf		k,0						;
		call		LCD_sendData			;k
		call		Delay40mks				;
		movf		colon,0					;
		call		LCD_sendData			;:
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			; пробел
		call		Delay40mks				;
;==================================================================================================================
		movlw		b'11000000'				; 			Перевод строки
		call		LCD_sendCmd				; 
		call		Delay40mks				;

		btfss		clock,0					; 
		goto		next					;
		movf		o,0						;
		call		LCD_sendData			; o
		call		Delay40mks				;
		movf		n,0						;
		call		LCD_sendData			; n
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			; ' '
		call		Delay40mks				;
		goto		and1					;
next:
		movf		o,0						;
		call		LCD_sendData			; o
		call		Delay40mks				;
		movf		f,0						;
		call		LCD_sendData			; f
		call		Delay40mks				;
		movf		f,0						;
		call		LCD_sendData			; f
		call		Delay40mks				;
and1:
		movlw		b'11000100'				; 			Перевод строки
		call		LCD_sendCmd				; 
		call		Delay40mks				;

		movf		TH,0					;
		call		Table					;
		call		LCD_sendData			; TenHour
		call		Delay40mks				;
		movf		H,0						;
		call		Table					;
		call		LCD_sendData			; Hour
		call		Delay40mks				;
		movf		colon,0					;
		call		LCD_sendData			; :
		call		Delay40mks				;
		movf		TM,0					;
		call		Table					;
		call		LCD_sendData			; TenMin
		call		Delay40mks				;
		movf		M,0						;
		call		Table					;
		call		LCD_sendData			; Min
		call		Delay40mks				;
		movf		colon,0					;
		call		LCD_sendData			; :
		call		Delay40mks				;
		movf		TS,0					;
		call		Table					;
		call		LCD_sendData			; TenSec
		call		Delay40mks				;
		movf		Sc,0					;
		call		Table					;
		call		LCD_sendData			; Sec
		call		Delay40mks				;

		return								

;==================================================================================================================
; Функция, стирающая экран LCD
;==================================================================================================================
LCD_CLEARDISP:

		movlw		b'10000000'				; 			Перевод строки
		call		LCD_sendCmd				; 
		call		Delay40mks				;

		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;

		movlw		b'11000000'				; 			Перевод строки
		call		LCD_sendCmd				;
		call		Delay40mks				;

		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;
		movf		gap,0					;
		call		LCD_sendData			;
		call		Delay40mks				;

		return								;
;==================================================================================================================

;==================================================================================================================
; Функции, необходимые для LCD
;==================================================================================================================
LCD_Init:
;  PORTA,2  LCD_RS     - Сигнал выбора режима LCD ("0" - команда, "1" - данные)
;  PORTA,1  LCD_RW     - Сигнал быбора операции LCD ("0" - запись, "1" - чтение)
;  PORTA,0  LCD_EN     - Сигнал стробирования\синхронизации LCD
		bcf			PORTA,2					;
		bcf			PORTA,1					;
		bcf			PORTA,0					;

		call 		Delay50ms				; Временная задержка на 50мс

		; Передаем команду в LCD (без анадлизи флага занятости "BF")

		movf		FunctionSet,0			; Команда "Function Set"
		call		LCD_sendCmd				; запись команды в LCD
						
		call		Delay5ms				; Временная задержка на 5мс
					
		; Передаем команду в LCD (без анадлизи флага занятости "BF")

		movf		FunctionSet,0			; Команда "Function Set"
		call		LCD_sendCmd				; запись команды в LCD
						
		call 		Delay100mks				; Временная задержка на 100мкс

		; Передаем команду в LCD (без анадлизи флага занятости "BF")

		movf		FunctionSet,0			; Команда "Function Set"
		call		LCD_sendCmd				; запись команды в LCD
						
		call		Delay50mks				; Временная задержка на 50мкс

		movf		FunctionSet,0			; Команда "Function Set"
		call		LCD_sendCmd				; запись команды в LCD

		call		Delay50mks				; Временная задержка на 50мкс

		call		LCD_DOOC				;

		call		LCD_Clear				; Очистка дисплея и установка курсора в адрес "0"

		call		LCD_Home				; Установка курсора в адрес "0"

		return								;

; Очистка дисплея и установка курсора в адрес "0"
LCD_Clear:
		movf		ClearDisplay,0			; Команда "Clear Display"
		call		LCD_sendCmd				; запись команды в LCD
		
		return								;

; Установка курсора в адрес "0"
LCD_Home:	
		movf		CurAtHome,0				; Команда "Cursor At Home"
		call		LCD_sendCmd				; запись команды в LCD
		
		return								;

; Дисплей отключен и сдвиг курсора вправо
LCD_EMS:	
		movf		EntryModeSet,0			; Команда "Entry Mode Set"
		call		LCD_sendCmd				; запись команды в LCD
		
		return								;

; Дисплей включен, курсор отключен и режим мигания отключен
LCD_DOOC:	
		movf		DisplControl,0			; Команда "Display On/Off Control"
		call		LCD_sendCmd				; запись команды в LCD
		
		return								;

LCD_CDSP:	
		movf		CurDispShiftP,0		; Команда "Display On/Off Control"
		call		LCD_sendCmd				; запись команды в LCD
		
		return								;
; Сдвиг дисплея влево
LCD_CDS:	
		movf		CurDispShift,0			; Команда "Cursor/Display Shift"
		call		LCD_sendCmd				; запись команды в LCD
		
		return								;

; Указание на запись команды в LCD
LCD_sendCmd:		
		movwf		DATACMDLCD				;
		bcf			PORTA,2					; Режим: "Команда"
		bcf			PORTA,1					; 
		call		LCD_send				; Запись команды в LCD
		return								;

; Указание на запись данных в LCD
LCD_sendData:		
		movwf		DATACMDLCD				;
		bsf			PORTA,2					; Режим: "Данные"
		bcf			PORTA,1					; 
		call		LCD_send				; Запись данных в LCD
		return								;

; Запись в LCD
LCD_send:	
		; Выставляем данные на шину LCD
		bsf			PORTA,0					; Взводим линию стробирования
		call		LCD_4bita				;

		bcf			PORTA,0					; 
		bsf			PORTA,0					;

		call		LCD_4bita				;

		bcf			PORTA,0					; Сбрасываем линию стробирования
		call 		Delay1mks				; Временная задержка на 1мкс

		bsf			PORTD,4					; Высокоимпендансное состояние линий данных LCD
		bsf			PORTD,5					;
		bsf			PORTD,6					;
		bsf			PORTD,7					;

		return								;

; Запись 4 бит в LCD
LCD_4bita:

		bcf			STATUS,C				;
		rlf			DATACMDLCD,1			;
		btfss		STATUS,C				;
		goto		PORT1D
		bsf			PORTD,7					;
		goto		PORT2D					;
PORT1D:
		bcf			PORTD,7					;		
PORT2D:
		bcf			STATUS,C				;
		rlf			DATACMDLCD,1			;
		btfss		STATUS,C				;
		goto		PORT3D
		bsf			PORTD,6					;
		goto		PORT4D					;
PORT3D:
		bcf			PORTD,6					;		
PORT4D:

		bcf			STATUS,C				;
		rlf			DATACMDLCD,1			;
		btfss		STATUS,C				;
		goto		PORT5D
		bsf			PORTD,5					;
		goto		PORT6D					;
PORT5D:
		bcf			PORTD,5					;		
PORT6D:
		bcf			STATUS,C				;
		rlf			DATACMDLCD,1			;
		btfss		STATUS,C				;
		goto		PORT7D
		bsf			PORTD,4					;
		goto		PORT8D					;
PORT7D:
		bcf			PORTD,4					;		
PORT8D:
		
		return
;==================================================================================================================

;==================================================================================================================
; Обработка клавиатуры
;==================================================================================================================
Key:
		call		Keyboard			; 
		return

Keyboard:
		bcf			PORTB,0				;
		bsf			PORTB,1				;
		bsf			PORTB,2				;

		btfss		PORTB,3				;
		goto		hourplus			;
		btfss		PORTB,4				;
		goto		minplus				;
		btfss		PORTB,5				;
		goto		secplus				;
;==================================================================================================================
		bsf			PORTB,0				;
		bcf			PORTB,1				;
		bsf			PORTB,2				;

		btfss		PORTB,3				;
		goto		hourminus			;
		btfss		PORTB,4				;
		goto		minminus			;
		btfss		PORTB,5				;
		goto		secminus			;
;==================================================================================================================
		bsf			PORTB,0				;
		bsf			PORTB,1				;
		bcf			PORTB,2				;

		btfss		PORTB,3				;
		goto		go					;
		btfss		PORTB,4				;
		goto		routing				;
		btfss		PORTB,5				;
		goto		alarm				;
		return
;==================================================================================================================
hourplus:
		call		Delay200ms			;

		btfsc		flag,0				; проверка режима отображения и контроль кнопок
		goto		clock1				;
;время===================================
		movf		Hour,0				; Временно сохранить W
		subwf		nine,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else0				; если Z==1, то результат нулевой

		movf		TenHour,0			; Временно сохранить W
		subwf		two,0				; сохраить результат в W
		btfss		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else1				; если Z==1, то результат нулевой

		movf		Hour,0				; Временно сохранить W
		subwf		three,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else2				; если Z==1, то результат нулевой
else1:
		incf		Hour,1				;
		call		LCD					;
		call		Slave3				;
		return							
else0:
		clrf		Hour				;
		incf		TenHour,1			;
		call		LCD					;
		call		Slave3				;
		return							
else2:
		clrf		Hour				;
		clrf		TenHour				;
		call		LCD					;
		call		Slave3				;
		return	
;будильник===============================
clock1:
		movf		H,0					; Временно сохранить W
		subwf		nine,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else01				; если Z==1, то результат нулевой

		movf		TH,0				; Временно сохранить W
		subwf		two,0				; сохраить результат в W
		btfss		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else11				; если Z==1, то результат нулевой

		movf		H,0					; Временно сохранить W
		subwf		three,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else21				; если Z==1, то результат нулевой
else11:
		incf		H,1					;
		call		LCD					;
		return							
else01:
		clrf		H					;
		incf		TH,1				;
		call		LCD					;
		return							
else21:
		clrf		H					;
		clrf		TH					;
		call		LCD					;
		return	
	
;==================================================================================================================
minplus:
		call		Delay200ms			;
		btfsc		flag,0				; проверка режима отображения и контроль кнопок
		goto		clock2				;
;время===================================
		movf		Min,0				; Временно сохранить W
		subwf		nine,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else3				; если Z==1, то результат нулевой
		incf		Min,1				;
		call		LCD					;
		call		Slave2				;
		return								
else3:
		movf		TenMin,0			; Временно сохранить W
		subwf		five,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		cleaning3			; если Z==1, то результат нулевой
		clrf		Min					;
		incf		TenMin,1			;
		call		LCD					;
		call		Slave2				;
		return		
cleaning3:
		clrf		Min					;
		clrf		TenMin				;
		call		LCD					;
		call		Slave2				;
		return		
;будильник===============================
clock2:
		movf		M,0					; Временно сохранить W
		subwf		nine,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else31				; если Z==1, то результат нулевой
		incf		M,1					;
		call		LCD					;
		return								
else31:
		movf		TM,0				; Временно сохранить W
		subwf		five,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		cleaning31			; если Z==1, то результат нулевой
		clrf		M					;
		incf		TM,1				;
		call		LCD					;
		return		
cleaning31:
		clrf		M					;
		clrf		TM					;
		call		LCD					;
		return		

;==================================================================================================================
secplus:
		call		Delay200ms			;
		btfsc		flag,0				; проверка режима отображения и контроль кнопок
		goto		clock3				;
;время===================================
		movf		Sec,0				; Временно сохранить W
		subwf		nine,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else4				; если Z==1, то результат нулевой
		incf		Sec,1				;
		call		LCD					;
		call		Slave1				;
		return								
else4:
		movf		TenSec,0			; Временно сохранить W
		subwf		five,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		cleaning4			; если Z==1, то результат нулевой
		clrf		Sec					;
		incf		TenSec,1			;
		call		LCD					;
		call		Slave1				;
		return		
cleaning4:
		clrf		Sec					;
		clrf		TenSec				;
		call		LCD					;
		call		Slave1				;	
		return
;будильник===============================
clock3:
		movf		Sc,0				; Временно сохранить W
		subwf		nine,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else41				; если Z==1, то результат нулевой
		incf		Sc,1				;
		call		LCD					;
		return								
else41:
		movf		TS,0				; Временно сохранить W
		subwf		five,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		cleaning41			; если Z==1, то результат нулевой
		clrf		Sc					;
		incf		TS,1				;
		call		LCD					;
		return		
cleaning41:
		clrf		Sc					;
		clrf		TS					;
		call		LCD					;
		return
		
;==================================================================================================================
hourminus:
		call		Delay200ms			;
		btfsc		flag,0				; проверка режима отображения и контроль кнопок
		goto		clock4				;
;время===================================
		movf		Hour,0				; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else7				; если Z==1, то результат нулевой
		decf		Hour,1				;
		call		LCD					;
		call		Slave3				;
		return								
else7:
		movf		TenHour,0			; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		restoration7		; если Z==1, то результат нулевой
		movf		nine,0				;
		movwf		Hour				;
		decf		TenHour,1			;
		call		LCD					;
		call		Slave3				;
		return		
restoration7:
		movf		three,0				;
		movwf		Hour				;
		movf		two,0				;
		movwf		TenHour				;
		call		LCD					;
		call		Slave3				;
		return	
;будильник===============================
clock4:
		movf		H,0					; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else71				; если Z==1, то результат нулевой
		decf		H,1					;
		call		LCD					;
		return								
else71:
		movf		TH,0				; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		restoration71		; если Z==1, то результат нулевой
		movf		nine,0				;
		movwf		H					;
		decf		TH,1				;
		call		LCD					;
		return		
restoration71:
		movf		three,0				;
		movwf		H					;
		movf		two,0				;
		movwf		TH					;
		call		LCD					;
		return	

;==================================================================================================================
minminus:
		call		Delay200ms			;
		btfsc		flag,0				; проверка режима отображения и контроль кнопок
		goto		clock5				;
;время===================================
		movf		Min,0				; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else6				; если Z==1, то результат нулевой
		decf		Min,1				;
		call		LCD					;
		call		Slave2				;
		return								
else6:
		movf		TenMin,0			; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		restoration6		; если Z==1, то результат нулевой
		movf		nine,0				;
		movwf		Min					;
		decf		TenMin,1			;
		call		LCD					;
		call		Slave2				;
		return		
restoration6:
		movf		nine,0				;
		movwf		Min					;
		movf		five,0				;
		movwf		TenMin				;
		call		LCD					;
		call		Slave2				;
		return	
;будильник===============================
clock5:	
		movf		M,0					; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else61				; если Z==1, то результат нулевой
		decf		M,1					;
		call		LCD					;
		return								
else61:
		movf		TM,0				; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		restoration61		; если Z==1, то результат нулевой
		movf		nine,0				;
		movwf		M					;
		decf		TM,1				;
		call		LCD					;
		return		
restoration61:
		movf		nine,0				;
		movwf		M					;
		movf		five,0				;
		movwf		TM					;
		call		LCD					;
		return	

;==================================================================================================================
secminus:
		call		Delay200ms			;
		btfsc		flag,0				; проверка режима отображения и контроль кнопок
		goto		clock6				;
;время===================================
		movf		Sec,0				; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else5				; если Z==1, то результат нулевой
		decf		Sec,1				;
		call		LCD					;
		call		Slave1				;
		return								
else5:
		movf		TenSec,0			; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		restoration5		; если Z==1, то результат нулевой
		movf		nine,0				;
		movwf		Sec					;
		decf		TenSec,1			;
		call		LCD					;
		call		Slave1				;
		return		
restoration5:
		movf		nine,0				;
		movwf		Sec					;
		movf		five,0				;
		movwf		TenSec				;
		call		LCD					;
		call		Slave1				;
		return		
;будильник===============================
clock6:	
		movf		Sc,0				; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else51				; если Z==1, то результат нулевой
		decf		Sc,1				;
		call		LCD					;
		return								
else51:
		movf		TS,0				; Временно сохранить W
		subwf		zero,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		restoration51		; если Z==1, то результат нулевой
		movf		nine,0				;
		movwf		Sc					;
		decf		TS,1				;
		call		LCD					;
		return		
restoration51:
		movf		nine,0				;
		movwf		Sc					;
		movf		five,0				;
		movwf		TS					;
		call		LCD					;
		return		

;==================================================================================================================
go:
		call		Delay200ms			;
		btfss		run,0				; 
		goto		runexit				;
		bcf			run,0				; стоп
		return
runexit:
		bsf			run,0				; старт
		return
;==================================================================================================================
routing:
		call		Delay200ms			;
		btfss		flag,0				; 
		goto		routingexit			;
		bcf			flag,0				; время
		call		LCD_CLEARDISP		;
		call		LCD					;
		return
routingexit:
		bsf			flag,0				; будильник
		call		LCD_CLEARDISP		;
		call		LCD					;
		return
;==================================================================================================================
alarm:
		call		Delay200ms			;
		btfss		clock,0				; 
		goto		clockexit			;
		bcf			clock,0				;
		call		LCD_CLEARDISP		;
		call		LCD					;
		return
clockexit:
		bsf			clock,0				;
		call		LCD_CLEARDISP		;
		call		LCD					;
		return

;==================================================================================================================
; Время или будильник на LCD
;==================================================================================================================
LCD:
		btfss		flag,0				; 
		goto		rexit				;
		call		LCD_Alarm			; будильник
		return
rexit:
		call		LCD_Time			; время
		return

;==================================================================================================================
; Проверка будильника
;==================================================================================================================
Al_Cl:
		btfss		clock,0				; 
		return

		movf		TenHour,0			; Временно сохранить W
		subwf		TH,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		al1					; если Z==1, то результат нулевой
		return
al1:
		movf		Hour,0				; Временно сохранить W
		subwf		H,0					; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		al2					; если Z==1, то результат нулевой
		return
al2:
		movf		TenMin,0			; Временно сохранить W
		subwf		TM,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		al3					; если Z==1, то результат нулевой
		return
al3:
		movf		Min,0				; Временно сохранить W
		subwf		M,0					; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		al4					; если Z==1, то результат нулевой
		return
al4:
		movf		TenSec,0			; Временно сохранить W
		subwf		TS,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		al5					; если Z==1, то результат нулевой
		return
al5:
		movf		Sec,0				; Временно сохранить W
		subwf		Sc,0				; сохраить результат в W
		btfss		STATUS,Z			; если Z==0, то результат ненулевой							
		return							; если Z==1, то результат нулевой
		
		bsf			siren,0				;

		return

;==================================================================================================================
; Функция сигнала будильника
;==================================================================================================================
Alarmalarm:

		btfss		siren,0				; проверка режима отображения и контроль кнопок
		return
		decfsz		dec,1				;
		goto		dalee				;
		goto		theend				;
dalee:
		btfsc		on_off,0			; проверка режима отображения и контроль кнопок
		goto		off
		bsf			PORTD,0				;
		bsf			PORTD,1				;
		bsf			PORTD,2				;
		bsf			PORTD,3				;
		bsf			on_off,0			;
		return
off:
		bcf			PORTD,0				;
		bcf			PORTD,1				;
		bcf			PORTD,2				;
		bcf			PORTD,3				;
		bcf			on_off,0			;
		return
theend:
		movlw		.10					;
		movwf		dec					;
		bcf			siren,0				;
		bcf			PORTD,0				;
		bcf			PORTD,1				;
		bcf			PORTD,2				;
		bcf			PORTD,3				;
		return

;==================================================================================================================
; Логика расчета времени для LCD
;==================================================================================================================
Time_on_LCD:
		movf		Sec,0				; Временно сохранить W
		subwf		nine,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else8				; если Z==1, то результат нулевой
		incf		Sec,1				;
		call		Slave1				;
		return									
else8:
		movf		TenSec,0			; Временно сохранить W
		subwf		five,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else9				; если Z==1, то результат нулевой
		clrf		Sec					;
		incf		TenSec,1			;
		call		Slave1				;
		return							
else9:
		movf		Min,0				; Временно сохранить W
		subwf		nine,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else10				; если Z==1, то результат нулевой
		clrf		Sec					;
		clrf		TenSec				;
		incf		Min,1				;
		call		Slave1				;
		call		Slave2				;
		return							
else10:
		movf		TenMin,0			; Временно сохранить W
		subwf		five,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else111				; если Z==1, то результат нулевой
		clrf		Sec					;
		clrf		TenSec				;
		clrf		Min					;
		incf		TenMin,1			;
		call		Slave1				;
		call		Slave2				;
		return							
else111:
		movf		Hour,0				; Временно сохранить W
		subwf		nine,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else12				; если Z==1, то результат нулевой

		movf		TenHour,0			; Временно сохранить W
		subwf		two,0				; сохраить результат в W
		btfss		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else13				; если Z==1, то результат нулевой

		movf		Hour,0				; Временно сохранить W
		subwf		three,0				; сохраить результат в W
		btfsc		STATUS,Z			; если Z==0, то результат ненулевой	
		goto		else14				; если Z==1, то результат нулевой
else13:
		clrf		Sec					;
		clrf		TenSec				;
		clrf		Min					;
		clrf		TenMin				;
		incf		Hour,1				;
		call		Slave1				;
		call		Slave2				;
		call		Slave3				;
		return							
else12:
		clrf		Sec					;
		clrf		TenSec				;
		clrf		Min					;
		clrf		TenMin				;
		clrf		Hour				;
		incf		TenHour,1			;
		call		Slave1				;
		call		Slave2				;
		call		Slave3				;
		return							
else14:
		clrf		Sec					;
		clrf		TenSec				;
		clrf		Min					;
		clrf		TenMin				;
		clrf		Hour				;
		clrf		TenHour				;
		call		Slave1				;
		call		Slave2				;
		call		Slave3				;
		return						
;==================================================================================================================
; Передача данных по USART ведомым
;==================================================================================================================
Slave1:
		movlw		B'10000000'			; передача данных 1-ому ведомому
		call		TX_BUF				; 
		call		Delay5ms			;
		call		Delay5ms			;

		movf		Sec,0				;
		call		TX_BUF				; 
		call		Delay5ms			;
		call		Delay5ms			;

		movf		TenSec,0			;
		call		TX_BUF				; 
		call		Delay5ms			;
		call		Delay5ms			;
		return
;==================================================================================================================
Slave2:
		movlw		B'11000000'			; передача данных 2-ому ведомому
		call		TX_BUF				; 
		call		Delay5ms			;
		call		Delay5ms			;

		movf		Min,0				;
		call		TX_BUF				; 
		call		Delay5ms			;
		call		Delay5ms			;

		movf		TenMin,0			;
		call		TX_BUF				; 
		call		Delay5ms			;
		call		Delay5ms			;
		return
;==================================================================================================================
Slave3:
		movlw		B'11100000'			; передача данных 3-ему ведомому
		call		TX_BUF				; 
		call		Delay5ms			;
		call		Delay5ms			;

		movf		Hour,0				;
		call		TX_BUF				; 
		call		Delay5ms			;
		call		Delay5ms			;

		movf		TenHour,0			;
		call		TX_BUF				; 
		call		Delay5ms			;
		call		Delay5ms			;
		return
;==================================================================================================================
; Передача по USART
;==================================================================================================================
TX_BUF:
		movwf		TXREG				;
		bsf			STATUS, RP0			;
		bsf			TXSTA, TXEN			;
		bcf			STATUS, RP0			;
		return
;==================================================================================================================
; Инициализация переменных
;==================================================================================================================
InitPerem:
		PROPERTIESPORTS						;
		CLEARPORTS							;

		movlw		0x54					; 
		movwf		T						;
		movlw		0x69					; 
		movwf		i						;
		movlw		0x6D					; 
		movwf		m						;
		movlw		0x65					; 
		movwf		e						;

		movlw		0x41					; 
		movwf		A						;
		movlw		0x6C					; 
		movwf		l						;
		movlw		0x61					; 
		movwf		a						;
		movlw		0x72					; 
		movwf		r						;
		movlw		0x63					; 
		movwf		c						;
		movlw		0x6F					; 
		movwf		o						;
		movlw		0x6B					; 
		movwf		k						;

		movlw		0x6E					; 
		movwf		n						;
		movlw		0x66					; 
		movwf		f						;

		movlw		0x3A					; 
		movwf		colon					;
		movlw		0x20					; 
		movwf		gap						;

		movlw	B'00000001'					; Команда "Clear Display"
		movwf	ClearDisplay				;
		movlw	B'00000010'					; Команда "Cursor At Home"
		movwf	CurAtHome					;
		movlw	B'00000110'					; Команда "Entry Mode Set":
;                    	||________________  Cдвиг дисплея "1" - включен, "0" - отключен
;                       |__________________ Сдвиг курсора "1" - вправо,  "0" - влево
		movwf	EntryModeSet				;
		movlw	B'00001100'					; Команда "Display On/Off Control":
;                      |||_________________ Режим мигания "1" - включен, "0" - отключен
;                      ||__________________ Курсор        "1" - включен, "0" - отключен
;                      |___________________ Дисплей       "1" - включен, "0" - отключен
		movwf	DisplControl				;
		movlw	B'00011000'					; Команда "Cursor/Display Shift":
;                     ||___________________ Сдвиг "1" - вправо,  "0" - влево 
;                     |____________________ Cдвиг "1" - дисплея, "0" - курсора 
		movwf	CurDispShift				;
		movlw	B'00011100'					; Команда "Cursor/Display Shift":
;                     ||___________________ Сдвиг "1" - вправо,  "0" - влево 
;                     |____________________ Cдвиг "1" - дисплея, "0" - курсора 
		movwf	CurDispShiftP				;
		movlw	B'00101000'					; Команда "Function Set": 
;                  	 |||___________________ Шрифт     "1" - 5х10 точек, "0" - 5х7 точек
;                    ||____________________ Строки    "1" - 2 строки, "0" - 1 строка
;                    |_____________________ Интерфейс "1" - 8 бит, "0" - 4 бит
		movwf	FunctionSet					;

		clrf	DATACMDLCD					;
		clrf	COUNT1						;
		clrf	COUNT2						;
		clrf	COUNT3						;
		clrf	Sec							;
		clrf	TenSec						;
		clrf	Min							;
		clrf	TenMin						;
		clrf	Hour						;
		clrf	TenHour						;
		clrf	Sc							;
		clrf	TS							;
		clrf	M							;
		clrf	TM							;
		clrf	H							;
		clrf	TH							;
		clrf	flag						;
		clrf	clock						;
		clrf	run							;
		clrf	siren						;
		clrf	zero						;
		clrf	on_off						;
		movlw	.1							;
		movwf	one							;
		movlw	.2							;
		movwf	two							;
		movlw	.3							;
		movwf	three						;
		movlw	.5							;
		movwf	five						;
		movlw	.9							;
		movwf	nine						;
		movlw	.10							;
		movwf	dec							;
		return								;

;==================================================================================================================
; Задержки 
;==================================================================================================================

Delay40mks:
		movlw		.46					; используется
		movwf		COUNT1				;
zad0:	decfsz		COUNT1,1			;
		goto		zad0				;
		return							;

Delay50mks:								; используется
		movlw		.58					;
		movwf		COUNT1				;
zad1:	decfsz		COUNT1,1			;
		goto		zad1				;
		return							;

Delay100mks:							; используется
		movlw		.117				;
		movwf		COUNT1				;
zad2:	decfsz		COUNT1,1			;
		goto		zad2				;
		return							;

Delay5ms:								; используется
		movlw		.250				;
		movwf		COUNT1				;
		movlw		.23					;
		movwf		COUNT2				;
zad3:	decfsz		COUNT1,1			;
		goto		zad3				;
		decfsz		COUNT2,1			;
		goto		zad3				;
		return							;

Delay50ms:								; используется
		movlw		.250				;
		movwf		COUNT1				;
		movlw		.228				;
		movwf		COUNT2				;
zad4:	decfsz		COUNT1,1			;
		goto		zad4				;
		decfsz		COUNT2,1			;
		goto		zad4				;
		return							;

Delay1mks:								; используется
		movlw		.1					;
		movwf		COUNT1				;
zad5:	decfsz		COUNT1,1			;
		goto		zad5				;
		return							;

Delay200ms:
		movlw		.255				; используется
		movwf		COUNT1				;
		movlw		.23					;
		movwf		COUNT2				;
		movlw		.6					;
		movwf		COUNT3				;
zad6	decfsz		COUNT1,1			;
		goto		zad6				;
		decfsz		COUNT2,1			;
		goto		zad6				;
		decfsz		COUNT3,1			;
		goto		zad6				;
		return
;==================================================================================================================

	END
