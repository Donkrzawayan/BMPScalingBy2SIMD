
.code

MyProc1 proc

;RCX - dest, RDX - src, R8 - width, R9 - size
mov RBX, RDX ; potrzebuje miejsce na idiv
;mov RSI, RDX ; przenies src do RSI
;mov RAX, RCX ; przenies RCX (pierwszy parametr) do RAX
;mov RBX, R8 ; przenies width do RBX


		; for (tempSrc = src, tempDest = dest; tempSrc < src + size; tempDest += destWidth) {
	;add R9, RDX ; src + size
	xor R12, R12 ; licznik petli
poczatekPetli:
	cmp R12, R9 ; tempSrc < src + size
	jae koniecPetli

		; do {
petlaPrzechodzacaWiersz:

		; memcpy(tempDest, tempSrc, 3);
		; memcpy(tempDest, tempSrc, 3);
		; memcpy(tempDest + 3, tempSrc, 3);
		; MoveSmall3
	movzx R10D, word ptr [RBX+R12]  ; get two bytes from source
	movzx R11D, byte ptr 2[RBX+R12] ; get two bytes from source
	mov [RCX+2*R12], R10D           ; write two bytes to destination
	mov 2[RCX+2*R12], R11D          ; write two bytes to destination
	mov 3[RCX+2*R12], R10D          ; write two bytes to destination
	mov 5[RCX+2*R12], R11D          ; write two bytes to destination


		; tempSrc += 3;
		; tempDest += 2 * 3;
	add R12, 3 ; przejdz na kolejny pixel


		; } while ((tempSrc - src) % sourceWidth);
		; tempSrc - src == licznik
	mov RAX, R12  ; skopuj licznik do RAXa (pierwsza liczba dzielenia)
	cdq           ; zwieksz RAX na RAX i RDX
	idiv R8       ; dzielenie RDX:RAX przez R8 (width)
	test RDX, RDX ; jesli jest 0 ustawia flage ZF
	jne petlaPrzechodzacaWiersz


		; memcpy(tempDest, tempDest - destWidth, destWidth);
		; w SSE2


	jmp poczatekPetli ; przesocz na poczatek petli

koniecPetli:
	mov RAX, RCX ; zwroc dest


; for (i = 0; i < size; i += 3)
;mov RCX, 0 ;licznik petli
;poczatekPetli:
;cmp RCX, R8 ; porownaj size z liczikiem
;jae koniecPetli


; uint8_t *tempSource = Source + i;
; uint8_t *tempDest = Dest + 2 * i;
; memcpy(tempDest, tempSource, 3 * sizeof(uint8_t));
; memcpy(tempDest + 3, tempSource, 3 * sizeof(uint8_t));
; MoveSmall3
;movzx   R9d, word ptr [RDX+RCX]     ; get two bytes from source
;movzx   R10d, byte ptr 2[RDX+RCX]   ; get last byte from source
;mov     [rax+2*RCX], R9d            ; write two bytes to destination
;mov     2[rax+2*RCX], r10b          ; write last byte to destination
;mov     3[rax+2*RCX], R9d           ; write two bytes to destination
;mov     5[rax+2*RCX], r10b          ; write last byte to destination


;i += 3
;add RCX, 3 ; dodaj do licznika 3

;}
;jmp poczatekPetli


;koniecPetli:
; memcpy

ret ;zwraca zawartosc RAX
MyProc1 endp
end
