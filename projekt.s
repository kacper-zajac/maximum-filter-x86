SECTION .TEXT
	GLOBAL filtr

filtr:
	push rbp
	mov rbp, rsp
	push r12
	push r13
	push r14
	push r15

	;rdi -> output buffer
	;rsi -> input buffer
	;rdx -> rozmiar maski
	;rcx -> szerokość

	;r15 -> licznik zewnetrznej petli (robi petle na wysokosci)
	;r14 -> licznik wewnetrznej petli (robi petle na szerokosci)
	;r12 -> maska
	;r11 -> wysokosc
	;r9  -> szerokość x3 

	xor r11, r11
	lea r11, [r8]
	xor r12, r12
	lea r12, [rdx]
	mov r13, r12
	imul r13, 3

	xor r9, r9

	lea r9, [ecx + ecx*2] ; width
	


	xor r15, r15
	inc r15

outer_loop:
	xor r14, r14

inner_loop:

	inc r14
	cmp r14, r9
	jg inc_r15

	xor r10, r10  		; r10 wartosc maksymalna

	xor rcx, rcx
	mov rcx, r15
	sub rcx, r12		; odejmuje od biezacego wiersza - maske

	cmp rcx, 0
	jg next

check_rcx:
	inc rcx			; rcx licznik biezacego wiersza
	cmp rcx, 0
	jle check_rcx

next:
	xor rax, rax
	mov rax, r14		; rax -> od biezacej pozycji - na kolumnach ( poziomo ) odejmuje 3x maska	
	sub rax, r13		; rax -> iterator po masce

	cmp rax, 0
	jg loop

inc_rax:
	add rax, 3
	cmp rax, 0		; porownuje z zerem
	jl inc_rax

loop:

	xor rbx, rbx
	mov rbx, r14		; rbx -> biezaca pozycja (poziomo) dodaje maske *3
	add rbx, r13		; sprawdza czy nie wyszlismy poza obszar roboczy
	cmp rax, rbx
	jg inc_rcx

	xor rbx, rbx
	mov rbx, rcx		; mnozy licznik biezacego - licznik zewnetrzny (pozycji w pionie)
	sub rbx, r15		; mnozy razy dlugosc rzedu (rbx -> ilosc bitow)
	imul rbx, r9 		; rbx -> ilosc bitow do miejsca 

	xor rdx, rdx		
	mov rdx, rax		
	sub rdx, r14		; iterator - biezaca pozycja
	add rbx, rdx
	xor rdx, rdx
	neg rbx
	mov dl, [rsi + rbx]
	
	cmp edx, r10d
	jg change_max

next_loop:
	add rax, 3
	cmp rax, r9		; porownuje  z maks dlugoscia wersu
	jge inc_rcx
	jmp loop

change_max:
	xor r10, r10
	mov r10b, dl
	jmp next_loop

end:
	mov [rdi], r10b
	dec rdi
	dec rsi
	jmp inner_loop

inc_rcx:
	inc rcx
	xor rbx, rbx
	mov rbx, r15
	add rbx, r12
	cmp rcx, rbx		; porownuje z maksymalnym biezacym - wiersz + maska
	jg end
	cmp rcx, r11		; porownuje z wysokoscia
	jg end			; wydaje sie potrzebne ale nie ma znaczenia czy jest czy nie

	jmp next

inc_r15:
	inc r15
	cmp r15, r11
	jl outer_loop		; tu jle ew.


epilog:
	pop r15
	pop r14
	pop r13
	pop r12
	mov rsp, rbp
	pop rbp
	ret
	
