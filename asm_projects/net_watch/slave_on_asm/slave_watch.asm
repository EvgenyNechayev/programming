	PROCESSOR P16F877A		; Определяем тип процессора
	LIST N=3000				; Количество строк на страницу
	TITLE "Nechaev Evgenij"
	INCLUDE P16F877A.INC	; подключаем файл с описанием констант процессора
	INCLUDE MACRO.INC		; подключаем файл с макросами  

	__CONFIG _HS_OSC & _WDT_OFF & _PWRTE_OFF & _BODEN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _DEBUG_OFF ;115

	CBLOCK			0x20				; выделение памяти для переменных										
	rc,address,Ten,del
	WBuffer,StatBuffer
										;| для
	Sec,TenSec,Min,TenMin,Hour,TenHour	;| индикатора

	Time,TimeSec,TimeMin,TimeHour		;| для светодиодов
	tx_temp
	ENDC		
												
		ORG 	0x00					; установка счетчика команд на 0х00
		bcf     INTCON,GIE      		; запрет всех прерываний на время иниц.        
  		goto 	Start					; переходим на стартовую строчку программы

		ORG			0x04				; переходим на адрес блока прерываний 0x04
		PUSH							; резервное копирование значений спец. регистров
		bcf 	INTCON,GIE				; запрещаем глобальные прерывания
		btfsc	PIR1, RCIF				;
; флаг прерывания от приёмника USART
		goto	rc_isr					;
		goto	endint
rc_isr:
		bcf		PIR1, RCIF				;
		bsf		rc,0					;
; тут обработчик прерывания от приёмника (receiver - reseption)
		movf	RCREG, 0				;
; содержимое регистра - приёмника данных перноситься в W
		movwf	tx_temp					;
		POP								;
		bsf 	INTCON,GIE				; разрешаем глобальные прерывания
		return
endint:
		POP								;
		bsf 	INTCON,GIE				; разрешаем глобальные прерывания
		return

Table:	addwf		PCL					; 
		retlw		B'00111111'			; 0
		retlw		B'00000110'			; 1
		retlw		B'01011011'			; 2
		retlw		B'01001111'			; 3
		retlw		B'01100110'			; 4
		retlw		B'01101101'			; 5
		retlw		B'01111101'			; 6
		retlw		B'00100111'			; 7
		retlw		B'01111111'			; 8
		retlw		B'01101111'			; 9

;========================================================================
; Начало основной программы
;========================================================================
Start:		
		CLEARPORTS						; очистка портов
		PROPERTIESPORTS					; настройка портов
		call 	Clrreg					; инициализация значений
		reciever_on						;
		movlw 	b'11000000'				; конфигурируем INTCON. Биты разрешений и флаги прерываний:
		movwf	INTCON					;

repeat:									; метка для повтора
rpt1:
		call	Sensor					;
		btfss	rc,0					; 
		goto 	rpt1					;
		clrf	rc						;

		movf	tx_temp,0				;
		subwf	address,0				; сохраить результат в W
		btfss	STATUS,Z				; если Z==0, то результат ненулевой
		goto	repeat					; если Z==1, то результат нулевой

rpt2:
		call	Sensor					;
		btfss	rc,0					; 
		goto 	rpt2					;
		clrf	rc						;

		movf	tx_temp,0				;
;		movwf	Sec						;
;		movwf	Min						;
		movwf	Hour					;
rpt3:
		call	Sensor					;
		btfss	rc,0					; 
		goto 	rpt3					;
		clrf	rc						;

		movf	tx_temp,0				;
;		movwf	TenSec					;
;		movwf	TenMin					;
		movwf	TenHour					;

		call	Updiodes				;

		goto 	repeat					; переход на метку повтора
;========================================================================
; Подпрограмма увеличения порядка (1->10,2->20) + sec
;========================================================================
Updiodes:
		clrf		Time				;
;		movf		TenSec,0			; вывод второй цифры
;		movf		TenMin,0			; вывод первой цифры
		movf		TenHour,0			; вывод первой цифры
ADD1										
		addwf		Time,1				;
		decfsz		Ten,1				;
		goto		ADD1				;
;		movf		Sec,0				; вывод первой цифры
;		movf		Min,0				; вывод первой цифры
		movf		Hour,0				; вывод первой цифры
		addwf		Time,1				;

		movlw		.10					;
		movwf		Ten					;

		return							;
;========================================================================
; Подпрограмма обновления индикатора и диодов
;========================================================================
Sensor:	
;		movlw		B'00000110'
;		movwf 		PORTA				; 
;		clrf		PORTB				; очистить PORTB
;		clrf		PORTD				; очистить PORTB

		bcf			PORTA,1				;
		bsf			PORTA,2				;
;		movf		Sec,0				; вывод первой цифры
;		movf		Min,0				; вывод первой цифры
		movf		Hour,0				; вывод первой цифры
		call		Table				;
		movwf		PORTB				;
		call		Delay100mksec		;
		call		Delay100mksec		;

;		movlw		B'00000110'
;		movwf 		PORTA				; 
;		clrf		PORTB				; очистить PORTD

		bsf			PORTA,1				;
		bcf			PORTA,2				;
;		movf		TenSec,0			; вывод второй цифры
;		movf		TenMin,0			; вывод первой цифры
		movf		TenHour,0			; вывод первой цифры
		call		Table				;
		movwf		PORTB				;
		call		Delay100mksec		;
		call		Delay100mksec		;

		movf		Time,0				;
		movwf		PORTD				;
		return

;========================================================================
; Подпрограмма задержки 25 мксек.
;========================================================================
Delay100mksec:
		movlw   .117					;
		movwf   del						;
zad1	decfsz  del,1					;
		goto    zad1					;
		return	
;========================================================================
; Инициализация значений
;========================================================================
Clrreg:
		bcf 	STATUS,RP0				; установка нулевого банка
; Очистить все временные регистры		
		clrf	WBuffer					;
		clrf	StatBuffer				;

		clrf	Sec						;
		clrf	TenSec					;
		clrf	Min						;
		clrf	TenMin					;
		clrf	Hour					;
		clrf	TenHour					;
										;
		clrf	Time					;

		clrf	rc						;
		clrf	tx_temp					;
		clrf	del						;

;		movlw	B'10000000'				;
;		movlw	B'11000000'				;
		movlw	B'11100000'				;
		movwf	address					;

		movlw	.10						;
		movwf	Ten						;
		return
	END								
