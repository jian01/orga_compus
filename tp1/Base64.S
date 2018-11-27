#include <mips/regdef.h>
#include <sys/syscall.h>

.globl base64_encode
.globl base64_decode
.globl errmsg
.extern base64_transform_3bytes
.extern base64_decode_4bytes

.text
.abicalls

#define CARACTER_IGUAL 61

#define STACKSIZE_base64_encode 48
#define STACKSIZE_base64_decode 48

#define CODIGO_ERROR_LECTURA 1
#define CODIGO_ERROR_ESCRITURA 2
#define GRUPOS_POR_SALTO_DE_LINEA 20

.align 2

.ent base64_encode

base64_encode:
	.frame $fp, STACKSIZE_base64_encode, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_base64_encode
	.cprestore 32

	#Guardo calle-saved.
	sw $fp, 36(sp)
	sw ra, 40(sp)

	move $fp, sp

	#Guardo parametros (file descriptors) en ABA de mi caller.
	sw a0, STACKSIZE_base64_encode(sp)
	sw a1, STACKSIZE_base64_encode+4(sp)

	#t8->Acumulador de grupos de 4 bytes encodeados
	li t8,0
	sw t8, 20(sp)

loop_encode:

	#recupero parametros.
	lw a0, STACKSIZE_base64_encode(sp)

	#Preparo args para syscall.
	#a0 -> lo que ya esta.
	la a1, three_bytes_vector
	sb zero, (a1)
	sb zero, 1(a1)
	sb zero, 2(a1)
	li a2, 3
	li v0, SYS_read
	syscall

	#v0 -> cantidad_bytes_leidos.
	move t8, v0
	li t0, -1
	li v0, CODIGO_ERROR_LECTURA
	beq t8, t0, fin_encode

	move v0, t8

	#Caso que no lee 3 enteros (2,1 0) -> termina el archivo.
	li t0, 3
	bne t0, v0, fin_loop

	la a0, three_bytes_vector
	la t9, base64_transform_3bytes
	jal t9

	#Cargo para syscall. a0 -> fd, a1 -> vector_to_write, a2 -> vector_size.
	lw a0, STACKSIZE_base64_encode+4(sp)
	move a1, v0
	li a2, 4
	li v0, SYS_write
	syscall

	#Aumentar el acumulador
	lw t8, 20(sp)
	addu t8,t8,1
	sw t8, 20(sp)

	bne t8, GRUPOS_POR_SALTO_DE_LINEA, no_imprimir_salto

	#Restar 80 al acumulador
	li t8,0
	sw t8, 20(sp)

	#Escribir un \n
	lw a0, STACKSIZE_base64_encode+4(sp)
	li v0, SYS_write
	li t8, '\n'
	sb t8, 0(a1)
	li a2, 1
	syscall

no_imprimir_salto:

	move t8, v0
	li t0, -1
	li v0, CODIGO_ERROR_ESCRITURA
	beq t8, t0, fin_encode

	move v0, t8

	j loop_encode

fin_loop:
	li t0, 0
	beq t0, v0, fin_encode

	#Guardo la cantidad de bytes que lei, la voy a necesitar.
	sw v0, 16(sp)

	la a0, three_bytes_vector
	la t9, base64_transform_3bytes
	jal t9

	#Recupero la cantidad de bytes leidos.
	lw t0, 16(sp)

	#A 3 le resto la cantidad de bytes que lei.
	li t1, 3
	subu t2, t1, t0

determinar_iguales:
	li t1, 4
	subu t1, t1, t2

	#Sumo t2.
	addu t3, v0, t1
	li t4, CARACTER_IGUAL
	sb t4, (t3)

	#Decremento a ver si puse todos los iguales.
	subu t2, t2, 1
	li t1, 0
	beq t2, t1, escribir_ultimos

	j determinar_iguales

escribir_ultimos:
	#Cargo para syscall. a0 -> fd, a1 -> vector_to_write, a2 -> vector_size.
	lw a0, STACKSIZE_base64_encode+4(sp)
	move a1, v0
	li a2, 4
	li v0, SYS_write
	syscall

	move t8, v0
	li t0, -1
	li v0, CODIGO_ERROR_ESCRITURA
	beq t8, t0, fin_encode

	move v0, t8

	li v0, 0

fin_encode:
	#Extraigo valores del stack.
	lw gp, 32(sp)
	lw $fp, 36(sp)
	lw ra, 40(sp)

	addu sp, sp, STACKSIZE_base64_encode

	jr ra

.end base64_encode

.align 2

.ent base64_decode
base64_decode:
	.frame $fp, STACKSIZE_base64_decode, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_base64_decode
	.cprestore 32

	#Guardo calle-saved.
	sw $fp, 36(sp)
	sw ra, 40(sp)

	move $fp, sp
	#Guardo parametros (file descriptors) en ABA de mi caller.
	sw a0, STACKSIZE_base64_decode(sp)
	sw a1, STACKSIZE_base64_decode+4(sp)

loop_decode:

	#recupero parametros.
	lw a0, STACKSIZE_base64_decode(sp)

	#Preparo args para syscall.
	la a1, four_bytes_vector

	li t7, 0

#Lee byte uno por uno para ignorar cualquier \n
read_byte1:
	sb zero, (a1)
	la a1, four_bytes_vector
	li a2, 1
	li v0, SYS_read
	sw t7, 16(sp)
	syscall
	lw t7, 16(sp)
	lb t8, 0(a1)
	beq t8, '\n', read_byte1
	addu t7, t7, v0
read_byte2:
	sb zero, 1(a1)
	la a1, four_bytes_vector+1
	li a2, 1
	li v0, SYS_read
	sw t7, 16(sp)
	syscall
	lw t7, 16(sp)
	lb t8, 0(a1)
	beq t8, '\n', read_byte2
	addu t7, t7, v0
read_byte3:
	sb zero, 2(a1)
	la a1, four_bytes_vector+2
	li a2, 1
	li v0, SYS_read
	sw t7, 16(sp)
	syscall
	lw t7, 16(sp)
	lb t8, 0(a1)
	beq t8, '\n', read_byte3
	addu t7, t7, v0
read_byte4:
	sb zero, 3(a1)
	la a1, four_bytes_vector+3
	li a2, 1
	li v0, SYS_read
	sw t7, 16(sp)
	syscall
	lw t7, 16(sp)
	lb t8, 0(a1)
	beq t8, '\n', read_byte4
	addu t7, t7, v0

	#v0 -> cantidad_bytes_leidos.
	li t0, 0
	beq t7, t0, fin_decode

	move t8, t7
	li t0, 4
	li t7, CODIGO_ERROR_LECTURA
	bne t8, t0, fin_encode

	move v0, t8


	# char* bytes
	la a0, four_bytes_vector

	# char** dest
	la a1, pointer

	la t8, three_bytes_vector
	sw t8, (a1)

	la t9, base64_decode_4bytes
	jal t9

	#Cargo para syscall. a0 -> fd, a1 -> vector_to_write, a2 -> vector_size.
	lw a0, STACKSIZE_base64_decode+4(sp)
	la a1, pointer
	lw a1, (a1)
	move a2, v0
	li v0, SYS_write
	syscall

	move t8, v0
	li t0, -1
	li v0, CODIGO_ERROR_ESCRITURA
	beq t8, t0, fin_decode

	move v0, t8

	j loop_decode

fin_decode:
	#Extraigo valores del stack.
	lw gp, 32(sp)
	lw $fp, 36(sp)
	lw ra, 40(sp)

	addu sp, sp, STACKSIZE_base64_decode

	jr ra

.end base64_decode


.data
	

	three_bytes_vector: .space 3
	four_bytes_vector: .space 4
	pointer: .word
	error_lectura: .ascii "Error: File reading failed."
	error_escritura: .ascii "Error: File writing failed."
	#0 no es un error.
	errmsg: .word 0x00, error_lectura, error_escritura


