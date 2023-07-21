;--------------- INIT ----------------------
.cseg ; nasledujici umistit do pameti programu (implicitni)
.include "m169def.inc"    ; definice typu procesoru
.org 0x1000               ; podprogramy pro praci s displejem na adresu Ox1000, aby se neprepsaly
.include "print.inc"

.org 0                    ; zacatek programu - po resetu
jmp start

.org 0x100                ; zacatek programu - hlavni program, preskoceni vektoru preruseni
str_play: .db "PLAY",0  ; polozky menu - maximalni delka textu je  5 !!!
str_score: .db "SCORE",0

start:
    ldi r16, 0xFF         ; inicializace zasobniku
    out SPL, r16
    ldi r16, 0x04
    out SPH, r16
    call init_disp        ; inicializace displeje
    call init_joy         ; inicializace joysticku

; pouzivane promenne
; Z pouzivam pro zobrazovane menu polozky
.def r_char     = r16                      ; promenna pro praci se znaky
.def r_charPos  = r17                      ; pozice na displeji - 2 az 7
.def r_cycle    = r18                      ; pomocna promenna pro cykly
.def r_menuItem = r19                      ; aktualne vybrana polozka v menu
.def r_joyState = r20                      ; stav joysticku


ldi r_charPos, 2           ; nastaveni pozice pro vypis stavu
ldi r_menuItem, 0

;--------------- VLASTNI KOD ---------------
main_loop:               ; hlavni smycka

    call read_joy        ; nacti stav joysticku do r_joyState
    cpi r_joyState, 1           ; podivej se, co je v nem za hodnotu
    brne show_result        ; neni enter -> preskoc

    inc r_menuItem

	cpi r_menuItem, 2
	brne show_result

	ldi r_menuItem, 0

show_result:
	call display_menu_item


jmp main_loop


end: jmp end             ; konec programu


;--------------- HELPER PODPROGRAMY --------
init_joy:
    in r17, DDRE         ; nastaveni portu E (smer vlevo a vpravo)
    andi r17, 0b11110011
    in r16, PORTE
    ori r16, 0b00001100
    out DDRE, r17
    out PORTE, r16
    ldi r16, 0b00000000
    sts DIDR1, r16

    in r17, DDRB        ; nastaveni portu B (smer dolu, nahoru a enter)
    andi r17, 0b00101111
    in r16, PORTB
    ori r16, 0b11010000
    out DDRB, r17
    out PORTB, r16
ret


read_joy:                     ; ulozi smer joysticku do registru r20
    push r16                  ; uklid r16 a r17
    push r17

    joy_reread:
        in r16, PINB          ; nacti hodnotu joysticku 1

        ldi r20, 255          ; chvili cekej 2
    joy_wait: dec r20
        brne joy_wait

        in r17, PINB          ; nacti jeste jednou

        andi r16, 0b00010000  ; vymaskuj ostatni bity 3
        andi r17, 0b00010000

        cp r16, r17
        brne joy_reread      ; hodnoty se nerovnaly -> nacti znovu

        ldi r_joyState, 0           ; vychozi hodnota - nic neni aktivni
        cpi r16, 0
        brne joy_no_enter    ; hodnota je inverzni -> neni 0 znamena neni aktivni 4
        ldi r_joyState, 1           ; r20 = 1, kdyz je enter aktivni
    joy_no_enter:

    pop r17                  ; obnoveni r16 a r17
    pop r16
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


;--------------- PODPROGRAMY ---------------
display_menu_item:
	ldi r_charPos, 2                   ; zobraz sipecku
	ldi r_char, '>'
	call show_char
	inc r_charPos

	; rozhodni, jaky text se bude vypisovat
	cpi r_menuItem, 0
		breq display_0
	cpi r_menuItem, 1
		breq display_1

	display_0:
		ldi r30, low(2*str_play)
	    ldi r31, high(2*str_play)
		jmp display_continue
	display_1:
		ldi r30, low(2*str_score)
	    ldi r31, high(2*str_score)
		jmp display_continue
	display_continue:

	; zobrazuj
    display_show:
        lpm r_char, Z+              ; nacti dalsi znak a zkontroluj, jestli uz nejsi na konci
		cpi r_char, 0
		breq display_show_end

        call show_char              ; zobraz znak a zvedni pozici pro priste
        inc r_charPos

		jmp display_show
	display_show_end:

	call wait                           ; chvili cekej a pak vsechno smaz

	ldi r_char, ' '
	display_clear:
		call show_char

		dec r_charPos
		cpi r_charPos, 2
		brne display_clear

	call wait                           ; zase chvili cekej (zpusobi blikani textu)


    ret



