; Skalowanie obrazu BMP przez 2
; 5 sem. 2020/2021
.code

MyProc1 proc
	push R12
	push R15

		; RCX - dest, RDX - src, R8 - width, R9 - size

	test R9, R9  ; jesli size rowne 0 to skoncz program
	je koniec
	lea RAX, [RDX+R9] ; kopiuj az do src+size

		; padding
	mov R9, R8                 ; srcPadding bedzie w R9
	imul R9, 55555556H         ; zamiast dzielenia i modulo pomnoz 0101...
	shr	R9, 32                 ; przesun na prawidlowa pozycje
	and	R9, 0ffffffff80000003H ; tylko liczby mniejsze niz 4 sa wazne

	mov R12, R9                ; padding dla dest
	and R12, 1                 ; moze byc tylko 2 albo 0
	shl R12, 1                 ; jak jest 1 to zrob 2

	lea R15, [2*R8+R12] ; po kazdym przejsciu wiersza przeskocz jeden dalej (dest+=2*width+padding)

		; ustawienie maski (wykorzystam XMM0-XMM2)
	mov R10, 0608070605040305h
	mov RBX, 0403020100020100h ; 0 - 5 i 1/3 pixela w dest
	movq XMM0, RBX
	pinsrq XMM0, R10, 1        ; przenies z normalnych rejestrow do XMM0
	mov R10, 0908070605070605h
	mov RBX, 0403020403020100h ; 5 i 1/3 - 10 i 2/3 pixela w dest
	movq XMM1, RBX
	pinsrq XMM1, R10, 1        ; przenies z normalnych rejestrow do XMM1
	mov R10, 0807060807060504h
	mov RBX, 0305040302010002h ; 10 i 2/3 - 16 pixel w dest
	movq XMM2, RBX
	pinsrq XMM2, R10, 1        ; przenies z normalnych rejestrow do XMM2


poczatekPetli:
	lea R11, [RDX+R8] ; miejsce konca wiersza (do tego pixela przetwarzaj)
	cmp R8, 24   ; jeli rowne lub wieksze od 8 pizeli/24subpixei
	jb mniejszeNiz24

nadal24LubWieksze:
	movups XMM3, [RDX]        ; wez 16 bajtow z src
	movups XMM4, 7[RDX]       ; wez 16 bajtow z src[7]
	movups XMM5, 15[RDX]      ; wez 16 bajtow z src[15]

	pshufb XMM3, XMM0         ; rozmiesc bajty w XMM3 wedlug maski w XMM0
	movups [RCX], XMM3        ; zapisz dane w dest
	pshufb XMM4, XMM1         ; rozmiesc bajty w XMM4 wedlug maski w XMM1
	movups 16[RCX], XMM4      ; zapisz dane w dest
	pshufb XMM5, XMM2         ; rozmiesc bajty w XMM5 wedlug maski w XMM2
	movups 32[RCX], XMM5      ; zapisz dane w dest

	movups [RCX+R15], XMM3    ; zapisz dane w dest+2*width (width jest z src)
	movups 16[RCX+R15], XMM4  ; zapisz dane w dest+2*width (width jest z src)
	movups 32[RCX+R15], XMM5  ; zapisz dane w dest+2*width (width jest z src)

	add RCX, 48  ; zwieksz dest o pixele wlasnie przetworzone
	add RDX, 48  ; dodaj 24 i drugi raz do porównania czy wykonac jeszcze raz
	cmp R11, RDX ; porownaj zwiekszony o 24 src
	sub RDX, 24  ; src w prawidlowym stanie do dalszych obliczen
	jbe nadal24LubWieksze

	cmp R11, RDX ; jesli rowne skoncz ten wiersz
	je nastepnyWiersz

mniejszeNiz24:

	mov R10D, [RDX]            ; get 4 bytes from source
	movzx EBX, byte ptr 2[RDX] ; get one byte from source
	mov [RCX], R10D            ; write 4 bytes to dest
	mov 3[RCX], R10W           ; write two bytes to dest
	mov 5[RCX], BL             ; write one byte to dest
	mov [RCX+R15], R10D        ; write 4 bytes to dest
	mov 3[RCX+R15], R10W       ; write two bytes to dest
	mov 5[RCX+R15], BL         ; write one byte to dest

	add RDX, 3   ; kolejny pixel w src
	add RCX, 6   ; kolejny pixel w dest
	cmp RDX, R11 ; porownaj czy juz nie sa rowne
	jb mniejszeNiz24

	
nastepnyWiersz:
	add RCX, R15 ; pomin wiersz w dest
	add RCX, R12 ; razem z paddingiem
	add RDX, R9

	cmp RDX, RAX ; czy obecny wskaznik na src jest mniejszy od zaostatniego elementu tablicy
	jne poczatekPetli


koniec:

	pop R15
	pop R12

ret ; zwraca zawartosc RAX
MyProc1 endp
end
