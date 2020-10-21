
.code

MyProc1 proc

		; RCX - dest, RDX - src, R8 - width, R9 - size
	mov R11, RCX ; skopiuj dest do zwrocenia na koncu
	mov RBX, RDX ; potrzebuje miejsce na idiv
	;mov R15, R8
	;shr R15, 1


		; for (tempSrc = src, tempDest = dest; tempSrc < src + size; tempDest += destWidth) {
	xor R10, R10 ; licznik petli
	cmp R10, R9  ; tempSrc < src + size
	jae koniecPetli

		; do {
petlaPrzechodzacaWierszIWysokosc:

		; memcpy(tempDest, tempSrc, 3);
		; memcpy(tempDest + 3, tempSrc, 3);
		; MoveSmall3
	movzx EAX, word ptr [RBX+R10]  ; get two bytes from source
	movzx EDX, byte ptr 2[RBX+R10] ; get last byte from source
	mov [RCX+2*R10], EAX           ; write two bytes to destination
	mov 2[RCX+2*R10], EDX          ; write last byte to destination
	mov 3[RCX+2*R10], EAX          ; write two bytes to destination
	mov 5[RCX+2*R10], EDX          ; write last byte to destination


		; tempSrc += 3;
		; tempDest += 2 * 3;
	add R10, 3 ; przejdz na kolejny pixel


		; } while ((tempSrc - src) % sourceWidth);
		; tempSrc - src to to samo co licznik
	mov RAX, R10  ; skopuj licznik do RAXa (pierwsza liczba dzielenia)
	cdq           ; zwieksz RAX na RAX i RDX (RDX:RAX)
	idiv R8       ; dzielenie RDX:RAX przez R8 (width)
	test RDX, RDX ; jesli jest 0 ustawia flage ZR na 1
	jne petlaPrzechodzacaWierszIWysokosc


		; memcpy(tempDest, tempDest - destWidth, destWidth);
		; w SSE2


		; tempDest += destWidth
	add RCX, R8
	add RCX, R8 ; pomijaj co drugi wiersz
	;add RCX, R15


	cmp R10, R9 ; tempSrc < src + size
	jb petlaPrzechodzacaWierszIWysokosc ; przesocz na poczatek petli

koniecPetli:
	mov RAX, R11 ; zwroc dest


ret ;zwraca zawartosc RAX
MyProc1 endp
end
