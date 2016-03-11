	PROCESSOR P16F877A		; ���������� ��� ����������
	LIST N=3000				; ���������� ����� �� ��������
	TITLE "Nechaev Evgenij"
	INCLUDE P16F877A.INC	; ���������� ���� � ��������� �������� ����������
	INCLUDE MACRO.INC		; ���������� ���� � ���������  

	__CONFIG _HS_OSC & _WDT_OFF & _PWRTE_OFF & _BODEN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _DEBUG_OFF ;115

	CBLOCK			0x20				; ��������� ������ ��� ����������										
	rc,address,Ten,del
	WBuffer,StatBuffer
										;| ���
	Sec,TenSec,Min,TenMin,Hour,TenHour	;| ����������

	Time,TimeSec,TimeMin,TimeHour		;| ��� �����������
	tx_temp
	ENDC		
												
		ORG 	0x00					; ��������� �������� ������ �� 0�00
		bcf     INTCON,GIE      		; ������ ���� ���������� �� ����� ����.        
  		goto 	Start					; ��������� �� ��������� ������� ���������

		ORG			0x04				; ��������� �� ����� ����� ���������� 0x04
		PUSH							; ��������� ����������� �������� ����. ���������
		bcf 	INTCON,GIE				; ��������� ���������� ����������
		btfsc	PIR1, RCIF				;
; ���� ���������� �� �������� USART
		goto	rc_isr					;
		goto	endint
rc_isr:
		bcf		PIR1, RCIF				;
		bsf		rc,0					;
; ��� ���������� ���������� �� �������� (receiver - reseption)
		movf	RCREG, 0				;
; ���������� �������� - �������� ������ ����������� � W
		movwf	tx_temp					;
		POP								;
		bsf 	INTCON,GIE				; ��������� ���������� ����������
		return
endint:
		POP								;
		bsf 	INTCON,GIE				; ��������� ���������� ����������
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
; ������ �������� ���������
;========================================================================
Start:		
		CLEARPORTS						; ������� ������
		PROPERTIESPORTS					; ��������� ������
		call 	Clrreg					; ������������� ��������
		reciever_on						;
		movlw 	b'11000000'				; ������������� INTCON. ���� ���������� � ����� ����������:
		movwf	INTCON					;

repeat:									; ����� ��� �������
rpt1:
		call	Sensor					;
		btfss	rc,0					; 
		goto 	rpt1					;
		clrf	rc						;

		movf	tx_temp,0				;
		subwf	address,0				; �������� ��������� � W
		btfss	STATUS,Z				; ���� Z==0, �� ��������� ���������
		goto	repeat					; ���� Z==1, �� ��������� �������

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

		goto 	repeat					; ������� �� ����� �������
;========================================================================
; ������������ ���������� ������� (1->10,2->20) + sec
;========================================================================
Updiodes:
		clrf		Time				;
;		movf		TenSec,0			; ����� ������ �����
;		movf		TenMin,0			; ����� ������ �����
		movf		TenHour,0			; ����� ������ �����
ADD1										
		addwf		Time,1				;
		decfsz		Ten,1				;
		goto		ADD1				;
;		movf		Sec,0				; ����� ������ �����
;		movf		Min,0				; ����� ������ �����
		movf		Hour,0				; ����� ������ �����
		addwf		Time,1				;

		movlw		.10					;
		movwf		Ten					;

		return							;
;========================================================================
; ������������ ���������� ���������� � ������
;========================================================================
Sensor:	
;		movlw		B'00000110'
;		movwf 		PORTA				; 
;		clrf		PORTB				; �������� PORTB
;		clrf		PORTD				; �������� PORTB

		bcf			PORTA,1				;
		bsf			PORTA,2				;
;		movf		Sec,0				; ����� ������ �����
;		movf		Min,0				; ����� ������ �����
		movf		Hour,0				; ����� ������ �����
		call		Table				;
		movwf		PORTB				;
		call		Delay100mksec		;
		call		Delay100mksec		;

;		movlw		B'00000110'
;		movwf 		PORTA				; 
;		clrf		PORTB				; �������� PORTD

		bsf			PORTA,1				;
		bcf			PORTA,2				;
;		movf		TenSec,0			; ����� ������ �����
;		movf		TenMin,0			; ����� ������ �����
		movf		TenHour,0			; ����� ������ �����
		call		Table				;
		movwf		PORTB				;
		call		Delay100mksec		;
		call		Delay100mksec		;

		movf		Time,0				;
		movwf		PORTD				;
		return

;========================================================================
; ������������ �������� 25 �����.
;========================================================================
Delay100mksec:
		movlw   .117					;
		movwf   del						;
zad1	decfsz  del,1					;
		goto    zad1					;
		return	
;========================================================================
; ������������� ��������
;========================================================================
Clrreg:
		bcf 	STATUS,RP0				; ��������� �������� �����
; �������� ��� ��������� ��������		
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
