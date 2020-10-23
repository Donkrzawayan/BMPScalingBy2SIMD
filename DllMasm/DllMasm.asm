
.code

MyProc1 proc

		; RCX - dest, RDX - src, R8 - width, R9 - size
	mov RBX, RDX ; potrzebuje miejsce na idiv

	mov RAX, R9   ; skopuj size do RAXa (pierwsza liczba dzielenia)
	cdq           ; zwieksz RAX na RAX i RDX (RDX:RAX)
	idiv R8       ; dzielenie RDX:RAX przez R8 (width); wykonaj petle tyle co w RAX
	test RAX, RAX ; jesli jest 0 ustawia flage ZR na 1
	je koniec     ; ZR=1 konczy program

	mov R9, R8  ; o tyle przeskakuj w dest po kazdym wiwrszu src
	shl R9, 2   ; po kazdym przejsciu wiersza przeskocz dwa dalej (dest+=2*2*width)


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

	movups XMM3, [RBX]       ; wez 16 bajtow z src
	pshufb XMM3, XMM0        ; rozmiesc bajty w XMM3 wedlug maski w XMM0
	movups [RCX], XMM3       ; zapisz dane w dest
	
	movups XMM4, 7[RBX]       ; wez 16 bajtow z src
	pshufb XMM4, XMM1         ; rozmiesc bajty w XMM4 wedlug maski w XMM1
	movups 16[RCX], XMM4      ; zapisz dane w dest
	
	movups XMM5, 15[RBX]      ; wez 16 bajtow z src
	pshufb XMM5, XMM2         ; rozmiesc bajty w XMM5 wedlug maski w XMM2
	movups 32[RCX], XMM5      ; zapisz dane w dest

	movups [RCX+2*R8], XMM3   ; zapisz dane w dest+2*width (width jest z src)
	movups 16[RCX+2*R8], XMM4 ; zapisz dane w dest+2*width (width jest z src)
	movups 32[RCX+2*R8], XMM5 ; zapisz dane w dest+2*width (width jest z src)


	add RCX, R9  ; przeskocz dwa wiersze w dest
	add RBX, R8  ; przeskocz do kolejnego wiersza w src

	dec RAX           ; kolejne przejscie petli do 0
	test RAX, RAX     ; jesli jest 0 ustawia flage ZR na 1
	jne poczatekPetli ; ZR=0 wykonuje ponownie


koniec:


ret ; zwraca zawartosc RAX
MyProc1 endp
end
