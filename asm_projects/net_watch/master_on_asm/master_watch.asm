	PROCESSOR P16F874		; ���������� ��� ����������
	LIST N=75				; ���������� ����� �� ��������
	TITLE "Nechaev Evgenij - for exhibition 2009"
	INCLUDE P16F874.INC		; ���������� ���� � ��������� �������� ����������
	INCLUDE MACROSES.INC	; ���������� ���� � ���������   
	__CONFIG _HS_OSC & _WDT_OFF & _PWRTE_OFF & _BODEN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _WRT_ENABLE_ON & _DEBUG_ON
		
		CBLOCK			0x20										; ? ����������
		COUNT1,COUNT2,COUNT3,DATACMDLCD
		ClearDisplay,CurAtHome,EntryModeSet,DisplControl
		FunctionSet,CurDispShift,CurDispShiftP,run
		WBuffer,StatBuffer,nine,five,three,two,one,zero
		T,i,m,e
		A,l,a,r,c,o,k
		n,f
		flag,clock,dec,siren,on_off
		Sec,TenSec,Min,TenMin,Hour,TenHour							; ���������� ��� ������� �������
		Sc,TS,M,TM,H,TH
		colon,gap ; : & ' '
		
		ENDC

		ORG 		0x00					; ��������� �������� ������ �� 0�00
		bcf     	INTCON,GIE      		; ������ ���� ���������� �� ����� ����. 
  		goto 		Start					; ��������� �� ��������� ������� ���������

		ORG 		0x04 					; ��������� �� ����� ����� ���������� 0x04

		PUSH								; ��������� ����������� �������� ����. ���������
		bcf 	INTCON,GIE					; ��������� ���������� ����������
		btfss	PIR1, TMR1IF				; ���������, ���� �� ������������ TMR1
		goto	EndOfInterruption			; ���� ���, ��������� �� ����� �� ����� ��������� ����������
;==================================================================================================================
; ������ �������� �������
;==================================================================================================================

; �������� ������� �����
		movf		run,0					; �������� ��������� W
		subwf		one,0					; �������� ��������� � W
		btfss		STATUS,Z				; ���� Z==0, �� ��������� ���������	
		goto		EndOfInterruption		; ���� Z==1, �� ��������� �������
; ������ ������� ������� ��� LCD
		call		Time_on_LCD				;
		call		LCD						;
		call		Al_Cl					;
		call		Alarmalarm				;
EndOfInterruption:
		bcf 	PIR1,TMR1IF					; ���������� ���� ������������ TMR1
		POP									; ��������� �������� ����. ��������� � ��������� ���������
		bsf 	INTCON,GIE					; ��������� ���������� ����������
		movlw 	0xDC						; �������� �� 850 m���.
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
; ������ �������� ���������
;========================================================================
Start:
		call		InitPerem				;

		reciever_on							;

;��������� Timer1
		movlw 		b'11000000'				; ������������� INTCON. ���� ���������� � ����� ����������:
		movwf 		INTCON					;
		bsf 		STATUS,RP0				; ������������� 1-� ���� ������
		movlw 		b'00000001'				; ������������� PIE1 (���� ���������� ������������ ����������)
		movwf 		PIE1					; 
		bcf 		STATUS,RP0				; ������������� 0-� ���� ������
		clrf 		TMR1H					; �������� ������� ���� �������� TMR1
		clrf 		TMR1L					; �������� ������� ���� �������� TMR1
		movlw 		b'00001111'				; ������������� T1CON
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
; �������, ������������ ����� �� LCD
;==================================================================================================================
LCD_Time:

		movlw		b'10000000'				; 			������� ������
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
		call		LCD_sendData			; ������
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

		movlw		b'11000000'				; 			������� ������
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
		call		LCD_sendData			; ������
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
		call		LCD_sendData			; ������
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
; �������, ������������ ��������� �� LCD
;==================================================================================================================
LCD_Alarm:

		movlw		b'10000000'				; 			������� ������
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
		call		LCD_sendData			; ������
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
		call		LCD_sendData			; ������
		call		Delay40mks				;
;==================================================================================================================
		movlw		b'11000000'				; 			������� ������
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
		movlw		b'11000100'				; 			������� ������
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
; �������, ��������� ����� LCD
;==================================================================================================================
LCD_CLEARDISP:

		movlw		b'10000000'				; 			������� ������
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

		movlw		b'11000000'				; 			������� ������
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
; �������, ����������� ��� LCD
;==================================================================================================================
LCD_Init:
;  PORTA,2  LCD_RS     - ������ ������ ������ LCD ("0" - �������, "1" - ������)
;  PORTA,1  LCD_RW     - ������ ������ �������� LCD ("0" - ������, "1" - ������)
;  PORTA,0  LCD_EN     - ������ �������������\������������� LCD
		bcf			PORTA,2					;
		bcf			PORTA,1					;
		bcf			PORTA,0					;

		call 		Delay50ms				; ��������� �������� �� 50��

		; �������� ������� � LCD (��� �������� ����� ��������� "BF")

		movf		FunctionSet,0			; ������� "Function Set"
		call		LCD_sendCmd				; ������ ������� � LCD
						
		call		Delay5ms				; ��������� �������� �� 5��
					
		; �������� ������� � LCD (��� �������� ����� ��������� "BF")

		movf		FunctionSet,0			; ������� "Function Set"
		call		LCD_sendCmd				; ������ ������� � LCD
						
		call 		Delay100mks				; ��������� �������� �� 100���

		; �������� ������� � LCD (��� �������� ����� ��������� "BF")

		movf		FunctionSet,0			; ������� "Function Set"
		call		LCD_sendCmd				; ������ ������� � LCD
						
		call		Delay50mks				; ��������� �������� �� 50���

		movf		FunctionSet,0			; ������� "Function Set"
		call		LCD_sendCmd				; ������ ������� � LCD

		call		Delay50mks				; ��������� �������� �� 50���

		call		LCD_DOOC				;

		call		LCD_Clear				; ������� ������� � ��������� ������� � ����� "0"

		call		LCD_Home				; ��������� ������� � ����� "0"

		return								;

; ������� ������� � ��������� ������� � ����� "0"
LCD_Clear:
		movf		ClearDisplay,0			; ������� "Clear Display"
		call		LCD_sendCmd				; ������ ������� � LCD
		
		return								;

; ��������� ������� � ����� "0"
LCD_Home:	
		movf		CurAtHome,0				; ������� "Cursor At Home"
		call		LCD_sendCmd				; ������ ������� � LCD
		
		return								;

; ������� �������� � ����� ������� ������
LCD_EMS:	
		movf		EntryModeSet,0			; ������� "Entry Mode Set"
		call		LCD_sendCmd				; ������ ������� � LCD
		
		return								;

; ������� �������, ������ �������� � ����� ������� ��������
LCD_DOOC:	
		movf		DisplControl,0			; ������� "Display On/Off Control"
		call		LCD_sendCmd				; ������ ������� � LCD
		
		return								;

LCD_CDSP:	
		movf		CurDispShiftP,0		; ������� "Display On/Off Control"
		call		LCD_sendCmd				; ������ ������� � LCD
		
		return								;
; ����� ������� �����
LCD_CDS:	
		movf		CurDispShift,0			; ������� "Cursor/Display Shift"
		call		LCD_sendCmd				; ������ ������� � LCD
		
		return								;

; �������� �� ������ ������� � LCD
LCD_sendCmd:		
		movwf		DATACMDLCD				;
		bcf			PORTA,2					; �����: "�������"
		bcf			PORTA,1					; 
		call		LCD_send				; ������ ������� � LCD
		return								;

; �������� �� ������ ������ � LCD
LCD_sendData:		
		movwf		DATACMDLCD				;
		bsf			PORTA,2					; �����: "������"
		bcf			PORTA,1					; 
		call		LCD_send				; ������ ������ � LCD
		return								;

; ������ � LCD
LCD_send:	
		; ���������� ������ �� ���� LCD
		bsf			PORTA,0					; ������� ����� �������������
		call		LCD_4bita				;

		bcf			PORTA,0					; 
		bsf			PORTA,0					;

		call		LCD_4bita				;

		bcf			PORTA,0					; ���������� ����� �������������
		call 		Delay1mks				; ��������� �������� �� 1���

		bsf			PORTD,4					; ������������������ ��������� ����� ������ LCD
		bsf			PORTD,5					;
		bsf			PORTD,6					;
		bsf			PORTD,7					;

		return								;

; ������ 4 ��� � LCD
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
; ��������� ����������
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

		btfsc		flag,0				; �������� ������ ����������� � �������� ������
		goto		clock1				;
;�����===================================
		movf		Hour,0				; �������� ��������� W
		subwf		nine,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else0				; ���� Z==1, �� ��������� �������

		movf		TenHour,0			; �������� ��������� W
		subwf		two,0				; �������� ��������� � W
		btfss		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else1				; ���� Z==1, �� ��������� �������

		movf		Hour,0				; �������� ��������� W
		subwf		three,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else2				; ���� Z==1, �� ��������� �������
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
;���������===============================
clock1:
		movf		H,0					; �������� ��������� W
		subwf		nine,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else01				; ���� Z==1, �� ��������� �������

		movf		TH,0				; �������� ��������� W
		subwf		two,0				; �������� ��������� � W
		btfss		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else11				; ���� Z==1, �� ��������� �������

		movf		H,0					; �������� ��������� W
		subwf		three,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else21				; ���� Z==1, �� ��������� �������
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
		btfsc		flag,0				; �������� ������ ����������� � �������� ������
		goto		clock2				;
;�����===================================
		movf		Min,0				; �������� ��������� W
		subwf		nine,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else3				; ���� Z==1, �� ��������� �������
		incf		Min,1				;
		call		LCD					;
		call		Slave2				;
		return								
else3:
		movf		TenMin,0			; �������� ��������� W
		subwf		five,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		cleaning3			; ���� Z==1, �� ��������� �������
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
;���������===============================
clock2:
		movf		M,0					; �������� ��������� W
		subwf		nine,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else31				; ���� Z==1, �� ��������� �������
		incf		M,1					;
		call		LCD					;
		return								
else31:
		movf		TM,0				; �������� ��������� W
		subwf		five,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		cleaning31			; ���� Z==1, �� ��������� �������
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
		btfsc		flag,0				; �������� ������ ����������� � �������� ������
		goto		clock3				;
;�����===================================
		movf		Sec,0				; �������� ��������� W
		subwf		nine,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else4				; ���� Z==1, �� ��������� �������
		incf		Sec,1				;
		call		LCD					;
		call		Slave1				;
		return								
else4:
		movf		TenSec,0			; �������� ��������� W
		subwf		five,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		cleaning4			; ���� Z==1, �� ��������� �������
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
;���������===============================
clock3:
		movf		Sc,0				; �������� ��������� W
		subwf		nine,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else41				; ���� Z==1, �� ��������� �������
		incf		Sc,1				;
		call		LCD					;
		return								
else41:
		movf		TS,0				; �������� ��������� W
		subwf		five,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		cleaning41			; ���� Z==1, �� ��������� �������
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
		btfsc		flag,0				; �������� ������ ����������� � �������� ������
		goto		clock4				;
;�����===================================
		movf		Hour,0				; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else7				; ���� Z==1, �� ��������� �������
		decf		Hour,1				;
		call		LCD					;
		call		Slave3				;
		return								
else7:
		movf		TenHour,0			; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		restoration7		; ���� Z==1, �� ��������� �������
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
;���������===============================
clock4:
		movf		H,0					; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else71				; ���� Z==1, �� ��������� �������
		decf		H,1					;
		call		LCD					;
		return								
else71:
		movf		TH,0				; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		restoration71		; ���� Z==1, �� ��������� �������
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
		btfsc		flag,0				; �������� ������ ����������� � �������� ������
		goto		clock5				;
;�����===================================
		movf		Min,0				; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else6				; ���� Z==1, �� ��������� �������
		decf		Min,1				;
		call		LCD					;
		call		Slave2				;
		return								
else6:
		movf		TenMin,0			; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		restoration6		; ���� Z==1, �� ��������� �������
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
;���������===============================
clock5:	
		movf		M,0					; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else61				; ���� Z==1, �� ��������� �������
		decf		M,1					;
		call		LCD					;
		return								
else61:
		movf		TM,0				; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		restoration61		; ���� Z==1, �� ��������� �������
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
		btfsc		flag,0				; �������� ������ ����������� � �������� ������
		goto		clock6				;
;�����===================================
		movf		Sec,0				; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else5				; ���� Z==1, �� ��������� �������
		decf		Sec,1				;
		call		LCD					;
		call		Slave1				;
		return								
else5:
		movf		TenSec,0			; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		restoration5		; ���� Z==1, �� ��������� �������
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
;���������===============================
clock6:	
		movf		Sc,0				; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else51				; ���� Z==1, �� ��������� �������
		decf		Sc,1				;
		call		LCD					;
		return								
else51:
		movf		TS,0				; �������� ��������� W
		subwf		zero,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		restoration51		; ���� Z==1, �� ��������� �������
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
		bcf			run,0				; ����
		return
runexit:
		bsf			run,0				; �����
		return
;==================================================================================================================
routing:
		call		Delay200ms			;
		btfss		flag,0				; 
		goto		routingexit			;
		bcf			flag,0				; �����
		call		LCD_CLEARDISP		;
		call		LCD					;
		return
routingexit:
		bsf			flag,0				; ���������
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
; ����� ��� ��������� �� LCD
;==================================================================================================================
LCD:
		btfss		flag,0				; 
		goto		rexit				;
		call		LCD_Alarm			; ���������
		return
rexit:
		call		LCD_Time			; �����
		return

;==================================================================================================================
; �������� ����������
;==================================================================================================================
Al_Cl:
		btfss		clock,0				; 
		return

		movf		TenHour,0			; �������� ��������� W
		subwf		TH,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		al1					; ���� Z==1, �� ��������� �������
		return
al1:
		movf		Hour,0				; �������� ��������� W
		subwf		H,0					; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		al2					; ���� Z==1, �� ��������� �������
		return
al2:
		movf		TenMin,0			; �������� ��������� W
		subwf		TM,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		al3					; ���� Z==1, �� ��������� �������
		return
al3:
		movf		Min,0				; �������� ��������� W
		subwf		M,0					; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		al4					; ���� Z==1, �� ��������� �������
		return
al4:
		movf		TenSec,0			; �������� ��������� W
		subwf		TS,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		al5					; ���� Z==1, �� ��������� �������
		return
al5:
		movf		Sec,0				; �������� ��������� W
		subwf		Sc,0				; �������� ��������� � W
		btfss		STATUS,Z			; ���� Z==0, �� ��������� ���������							
		return							; ���� Z==1, �� ��������� �������
		
		bsf			siren,0				;

		return

;==================================================================================================================
; ������� ������� ����������
;==================================================================================================================
Alarmalarm:

		btfss		siren,0				; �������� ������ ����������� � �������� ������
		return
		decfsz		dec,1				;
		goto		dalee				;
		goto		theend				;
dalee:
		btfsc		on_off,0			; �������� ������ ����������� � �������� ������
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
; ������ ������� ������� ��� LCD
;==================================================================================================================
Time_on_LCD:
		movf		Sec,0				; �������� ��������� W
		subwf		nine,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else8				; ���� Z==1, �� ��������� �������
		incf		Sec,1				;
		call		Slave1				;
		return									
else8:
		movf		TenSec,0			; �������� ��������� W
		subwf		five,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else9				; ���� Z==1, �� ��������� �������
		clrf		Sec					;
		incf		TenSec,1			;
		call		Slave1				;
		return							
else9:
		movf		Min,0				; �������� ��������� W
		subwf		nine,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else10				; ���� Z==1, �� ��������� �������
		clrf		Sec					;
		clrf		TenSec				;
		incf		Min,1				;
		call		Slave1				;
		call		Slave2				;
		return							
else10:
		movf		TenMin,0			; �������� ��������� W
		subwf		five,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else111				; ���� Z==1, �� ��������� �������
		clrf		Sec					;
		clrf		TenSec				;
		clrf		Min					;
		incf		TenMin,1			;
		call		Slave1				;
		call		Slave2				;
		return							
else111:
		movf		Hour,0				; �������� ��������� W
		subwf		nine,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else12				; ���� Z==1, �� ��������� �������

		movf		TenHour,0			; �������� ��������� W
		subwf		two,0				; �������� ��������� � W
		btfss		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else13				; ���� Z==1, �� ��������� �������

		movf		Hour,0				; �������� ��������� W
		subwf		three,0				; �������� ��������� � W
		btfsc		STATUS,Z			; ���� Z==0, �� ��������� ���������	
		goto		else14				; ���� Z==1, �� ��������� �������
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
; �������� ������ �� USART �������
;==================================================================================================================
Slave1:
		movlw		B'10000000'			; �������� ������ 1-��� ��������
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
		movlw		B'11000000'			; �������� ������ 2-��� ��������
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
		movlw		B'11100000'			; �������� ������ 3-��� ��������
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
; �������� �� USART
;==================================================================================================================
TX_BUF:
		movwf		TXREG				;
		bsf			STATUS, RP0			;
		bsf			TXSTA, TXEN			;
		bcf			STATUS, RP0			;
		return
;==================================================================================================================
; ������������� ����������
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

		movlw	B'00000001'					; ������� "Clear Display"
		movwf	ClearDisplay				;
		movlw	B'00000010'					; ������� "Cursor At Home"
		movwf	CurAtHome					;
		movlw	B'00000110'					; ������� "Entry Mode Set":
;                    	||________________  C���� ������� "1" - �������, "0" - ��������
;                       |__________________ ����� ������� "1" - ������,  "0" - �����
		movwf	EntryModeSet				;
		movlw	B'00001100'					; ������� "Display On/Off Control":
;                      |||_________________ ����� ������� "1" - �������, "0" - ��������
;                      ||__________________ ������        "1" - �������, "0" - ��������
;                      |___________________ �������       "1" - �������, "0" - ��������
		movwf	DisplControl				;
		movlw	B'00011000'					; ������� "Cursor/Display Shift":
;                     ||___________________ ����� "1" - ������,  "0" - ����� 
;                     |____________________ C���� "1" - �������, "0" - ������� 
		movwf	CurDispShift				;
		movlw	B'00011100'					; ������� "Cursor/Display Shift":
;                     ||___________________ ����� "1" - ������,  "0" - ����� 
;                     |____________________ C���� "1" - �������, "0" - ������� 
		movwf	CurDispShiftP				;
		movlw	B'00101000'					; ������� "Function Set": 
;                  	 |||___________________ �����     "1" - 5�10 �����, "0" - 5�7 �����
;                    ||____________________ ������    "1" - 2 ������, "0" - 1 ������
;                    |_____________________ ��������� "1" - 8 ���, "0" - 4 ���
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
; �������� 
;==================================================================================================================

Delay40mks:
		movlw		.46					; ������������
		movwf		COUNT1				;
zad0:	decfsz		COUNT1,1			;
		goto		zad0				;
		return							;

Delay50mks:								; ������������
		movlw		.58					;
		movwf		COUNT1				;
zad1:	decfsz		COUNT1,1			;
		goto		zad1				;
		return							;

Delay100mks:							; ������������
		movlw		.117				;
		movwf		COUNT1				;
zad2:	decfsz		COUNT1,1			;
		goto		zad2				;
		return							;

Delay5ms:								; ������������
		movlw		.250				;
		movwf		COUNT1				;
		movlw		.23					;
		movwf		COUNT2				;
zad3:	decfsz		COUNT1,1			;
		goto		zad3				;
		decfsz		COUNT2,1			;
		goto		zad3				;
		return							;

Delay50ms:								; ������������
		movlw		.250				;
		movwf		COUNT1				;
		movlw		.228				;
		movwf		COUNT2				;
zad4:	decfsz		COUNT1,1			;
		goto		zad4				;
		decfsz		COUNT2,1			;
		goto		zad4				;
		return							;

Delay1mks:								; ������������
		movlw		.1					;
		movwf		COUNT1				;
zad5:	decfsz		COUNT1,1			;
		goto		zad5				;
		return							;

Delay200ms:
		movlw		.255				; ������������
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
