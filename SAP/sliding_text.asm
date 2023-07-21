;--------------- INIT ----------------------
.dseg
.org 0x100
charBuffer: .byte 10                      ; maximalni mozna delka

.cseg                                     ; nasledujici umistit do pameti programu (implicitni)
.include "m169def.inc"                    ; definice typu procesoru
.org 0x1000                               ; podprogramy pro praci s displejem na adresu Ox1000, aby se neprepsaly
.include "print.inc"

.org 0                                    ; zacatek programu - po resetu
jmp start

.org 0x100                                ; zacatek programu - hlavni program, preskoceni vektoru preruseni

length: .db 4                             ; definice read-only konstanty v pameti programu - 4 az 10
string: .db "SEN O JEZDICIM TEXTU JSEM SI JIZ SPLNIL",0 ; retezec zakonceny nulou

start:
    ldi r16, 0xFF                         ; inicializace zasobniku
    out SPL, r16
    ldi r16, 0x04
    out SPH, r16
    
    call init_disp                        ; inicializace displeje

    ldi r28, low(charBuffer)              ; nacteni adresy charBuffer do registru Y
    ldi r29, high(charBuffer)

    ldi r27, 10                           ; naplneni charBufferu mezerami
    ldi r26, ' '
    fillSpace:
        st Y+, r26
        dec r27
        brne fillSpace

; Z pouzivam pro nacitani dalsiho znaku z pameti a pro pro delku displeje
; Y pouzivam pro zobrazovane znaky
.def r_char    = r16                      ; promenna pro praci se znaky
.def r_charPos = r17                      ; pozice na displeji - 2 az 7
.def r_next    = r18                      ; dalsi znak z pameti dat
.def r_cycle   = r19                      ; pomocna promenna pro cykly
.def r_endFlag = r20                      ; priznak konce vstupu
.def r_dirFlag = r21                      ; priznak smeru jizdy textu
.def r_textLen = r22                      ; celkova delka textu
.def r_length  = r24                      ; registr pro delku zobrazovaneho textu

ldi r_dirFlag, 0                          ; 0 = zleva doprava, 1 = zprava doleva


;--------------- VLASTNI KOD ---------------
cpi r_dirFlag, 0
brne left                                ; rozdohni se, jestli text pojede doleva, nebo doprava


; VARIANTA ZLEVA DOPRAVA
right:
	ldi r30, low(2*string)                ; reset hodnot
    ldi r31, high(2*string)
	ldi r_textLen, 0
	ldi r_endFlag, 0

	prep:
		lpm r_next, Z+                    ; nacti dalsi znak

		cpi r_next, 0                     ; cti dokud nenarazis na nulu aka konec vstupu
		breq read_right

		inc r_textLen                     ; pocitej si delku textu a davej znaky na zasobnik
		push r_next

		jmp prep

	read_right:
		cpi r_textLen, 0
		breq dontRead                     ; jakmile se precte vse, tak dalsi znaky necti, ale nastav flag a znak na mezeru

		pop r_next

		dec r_textLen

	dontRead_right:
		call update_right
		call display
		call wait

		cpi r_endFlag, 1
		breq afterEnd_right               ; preskoceni cteni vstupu, kdyz uz je docteny
		jmp read_right


; VARIANTA ZPRAVA DOLEVA
left:
	ldi r30, low(2*string)                ; reset hodnot
    ldi r31, high(2*string)
	ldi r_textLen, 0
	ldi r_endFlag, 0

	read_left:
		lpm r_next, Z+                    ; nacti dalsi znak

		cpi r_next, 0
		breq dontRead                     ; jakmile se precte vse, tak dalsi znaky necti, ale nastav flag a znak na mezeru

	dontRead_left:
		call update_left
		call display
		call wait

		cpi r_endFlag, 1
		breq afterEnd_left                ; preskoceni cteni vstupu, kdyz uz je docteny
		jmp read_left


;--------------- HELPER PODPROGRAMY --------
dontRead:
	ldi r_endFlag, 1
	ldi r_next, ' '

	push r30                              ; uloz si Z na zasobnik, nacti si a precti z nej delku a vrat zpet
	push r31
	ldi r30, low(2*length)
    ldi r31, high(2*length)
    lpm r_length, Z
	inc r_length                          ; zvys delku o nekolik hodnot, aby po prejeti textu byla kratka pauza
	inc r_length
	inc r_length
	inc r_length
	pop r31
	pop r30

	cpi r_dirFlag, 0
	breq dontRead_right                   ; rozdohni se kam se vratit
	jmp dontRead_left


afterEnd_right:
	dec r_length
	cpi r_length, 0
	breq right
	jmp dontRead_right


afterEnd_left:
	dec r_length
	cpi r_length, 0
	breq left
	jmp dontRead_left


pre_update:
	ldi r28, low(charBuffer)              ; resetuj stav registru se zobrazovanymi znaky
    ldi r29, high(charBuffer)

	push r30                              ; uloz si Z na zasobnik, nacti si a precti z nej delku a vrat zpet
	push r31
	ldi r30, low(2*length)
    ldi r31, high(2*length)
    lpm r_cycle, Z
	pop r31
	pop r30

    dec r_cycle
	ret


;--------------- PODPROGRAMY ---------------
update_left:
    call pre_update

    update_cycle:                         ; posun vsechny znaky doleva
        inc r28
        ld r_char, Y                      ; nacti znak na pozici Y+1
        dec r28
        st Y+, r_char                     ; uloz ho na pozici Y a zvys pozici

        dec r_cycle
        brne update_cycle

    st Y, r_next                          ; do posledniho registru uloz nove nacteny znak
    ret


update_right:
	call pre_update

	.def r_prev = r23
	push r_prev

	ld r_prev, Y
	st Y+, r_next                          ; do prvniho registru uloz nove nacteny znak
	
    update_cycle2:                         ; posun vsechny znaky doprava
		ld r_char, Y
		st Y+, r_prev
		mov r_prev, r_char

		dec r_cycle
        brne update_cycle2

	pop r_prev
    ret


display:
    ldi r28, low(charBuffer)              ; resetuj stav registru se zobrazovanymi znaky
    ldi r29, high(charBuffer)
    ldi r_charPos, 2

	push r30                              ; uloz si Z na zasobnik, nacti si a precti z nej delku a vrat zpet
	push r31
	ldi r30, low(2*length)
    ldi r31, high(2*length)
    lpm r_cycle, Z
	pop r31
	pop r30    

    display_cycle:
        ld r_char, Y+
        call show_char
        inc r_charPos

        dec r_cycle
        brne display_cycle
    ret


wait:
    push r20
    push r21
	push r22

	ldi r22, 2
	cek3:
	    ldi r21, 0xFF
	    cek2:
			ldi r20, 0xFF
		    cek:
				dec r20
		        brne cek
		    dec r21
		    brne cek2
		dec r22
		brne cek3

	pop r22
    pop r21
    pop r20
    ret
