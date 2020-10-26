
.code

MyProc1 proc
	push R12
	push R15

		; RCX - dest, RDX - src, R8 - width, R9 - size
	mov RBX, RDX ; potrzebuje miejsce na idiv

	mov RAX, R9   ; skopuj size do RAXa (pierwsza liczba dzielenia)
	cdq           ; zwieksz RAX na RAX i RDX (RDX:RAX)
	idiv R8       ; dzielenie RDX:RAX przez R8 (width); wykonaj petle tyle co w RAX
	test RAX, RAX ; jesli jest 0 ustawia flage ZR na 1
	je koniec     ; ZR=1 konczy program

		; padding
	;mov	R9, 1431655766				; 55555556H
	;mov	rbx, rcx
	mov R9, R8
	imul R9, 1431655766				; 55555556H
	;mov	eax, edx
	shr	R9, 32
	;add	edx, eax
	and	R9, -2147483645			; ffffffff80000003H


	;mov R10, R8 ; int32_t srcPadding = 4 - sourceWidth % 4;
	;and R10, 0FFFFFFFF80000003h
	;jge	SHORT nieJestUjemna
	;dec	R10
	;or	R10, -4
	;inc	R10
;nieJestUjemna:
	mov R12, R9
	and R12, 1
	lea R12, DWORD PTR [R12+R12]
	;mov R9, 4
	;sub R9, R10
	;mov R12, R9
	;and R12, 1
	;lea R12, DWORD PTR [R12+R12]

	mov R15, R8 ; o tyle przeskakuj w dest po kazdym wiwrszu src
	shl R15, 1  ; po kazdym przejsciu wiersza przeskocz jeden dalej (dest+=2*width)
	add R15, R12


		; ustawienie maski (wykorzystam XMM0-XMM2)
	mov R10, 0608070605040305h
	mov RDX, 0403020100020100h ; 0 - 5 i 1/3 pixela w dest
	movq XMM0, RDX
	pinsrq XMM0, R10, 1        ; przenies z normalnych rejestrow do XMM0
	mov R10, 0908070605070605h
	mov RDX, 0403020403020100h ; 5 i 1/3 - 10 i 2/3 pixela w dest
	movq XMM1, RDX
	pinsrq XMM1, R10, 1        ; przenies z normalnych rejestrow do XMM1
	mov R10, 0807060807060504h
	mov RDX, 0305040302010002h ; 10 i 2/3 - 16 pixel w dest
	movq XMM2, RDX
	pinsrq XMM2, R10, 1        ; przenies z normalnych rejestrow do XMM2


poczatekPetli:

	xor R11, R11 ; licznik w wierszu
	cmp R8, 24   ; jeli rowne lub wieksze od 8 pizeli/24subpixei
	jb mniejszeNiz24

nadal24LubWieksze:
	
	movups XMM3, [RBX]        ; wez 16 bajtow z src
	movups XMM4, 7[RBX]       ; wez 16 bajtow z src[7]
	movups XMM5, 15[RBX]      ; wez 16 bajtow z src[15]

	pshufb XMM3, XMM0         ; rozmiesc bajty w XMM3 wedlug maski w XMM0
	movups [RCX], XMM3        ; zapisz dane w dest
	pshufb XMM4, XMM1         ; rozmiesc bajty w XMM4 wedlug maski w XMM1
	movups 16[RCX], XMM4      ; zapisz dane w dest
	pshufb XMM5, XMM2         ; rozmiesc bajty w XMM5 wedlug maski w XMM2
	movups 32[RCX], XMM5      ; zapisz dane w dest

	movups [RCX+R15], XMM3   ; zapisz dane w dest+2*width (width jest z src)
	movups 16[RCX+R15], XMM4 ; zapisz dane w dest+2*width (width jest z src)
	movups 32[RCX+R15], XMM5 ; zapisz dane w dest+2*width (width jest z src)

	add RBX, 24 ; zwieksz src o pixele wlasnie przetworzone
	add RCX, 48 ; zwieksz dest o pixele wlasnie przetworzone

	add R11, 48 ; dodaj 24 i drugi raz do porównania czy wykonac jeszcze raz
	cmp R8, R11 ; porownaj zwiekszony o 24 licznik
	sub R11, 24 ; liczznik w prawidlowym stanie do dalszych obliczen
	jbe nadal24LubWieksze

	cmp R8, R11 ; jesli rowne skoncz ten wiersz
	je nastepnyWiersz

mniejszeNiz24:

	mov R10D, [RBX]            ; get 4 bytes from source
	movzx EDX, byte ptr 2[RBX] ; get one byte from source
	mov [RCX], R10D            ; write 4 bytes to destination
	mov 3[RCX], R10W           ; write two bytes to destination
	mov 5[RCX], DL             ; write one byte to destination
	mov [RCX+R15], R10D       ; write 4 bytes to destination
	mov 3[RCX+R15], R10W      ; write two bytes to destination
	mov 5[RCX+R15], DL        ; write one byte to destination

	add RBX, 3  ; kolejny pixel w src
	add RCX, 6  ; kolejny pixel w src
	add R11, 3  ; kolejna iteracja
	cmp R11, R8 ; porownaj czy juz nie sa rowne
	jb mniejszeNiz24

	
nastepnyWiersz:
	add RCX, R15  ; pomin wiersz w dest
	add RCX, R12
	;add RCX, R8
	;add RCX, R8
	add RBX, R9

	dec RAX           ; kolejne przejscie petli do 0
	test RAX, RAX     ; jesli jest 0 ustawia flage ZR na 1
	jne poczatekPetli ; ZR=0 wykonuje ponownie


koniec:

	pop R15
	pop R12

ret ; zwraca zawartosc RAX
MyProc1 endp
end
