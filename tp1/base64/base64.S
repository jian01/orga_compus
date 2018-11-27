#include <mips/regdef.h>
#include <sys/syscall.h>

.extern bytes_extract_n_bits
.extern byte_extract_n_bits

.text
.abicalls
.align 2
.globl base64_transform_3bytes
.globl base64_decode_4bytes
.globl decode_byte

#define CARACTER_A 65
#define CARACTER_a 97
#define CARACTER_Z 90
#define CARACTER_z 122
#define CARACTER_0 48
#define CARACTER_9 57
#define CARACTER_MAS 43
#define CARACTER_BARRA 47
#define CARACTER_IGUAL 61


#define STACKSIZE_decode_byte 16

#define STACKSIZE_base64_transform_3_bytes 40

#define STACKSIZE_base64_decode_4bytes 48

.ent decode_byte

decode_byte:
	.frame $fp, STACKSIZE_decode_byte, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_decode_byte
	.cprestore 8

	#Guardo calle-saved.
	sw $fp, 12(sp)

	move $fp, sp

	#Guardo args en ABA.
	sw a0, STACKSIZE_decode_byte(sp)


	#Guardamos en v0 el valor de retorno por default.
	li v0, -1

	#Restamos al byte el valor de A y nos fijamos si es mayor o igual al byte.
	subu t0, a0, CARACTER_A
	bltz t0, minusculas

	#Segunda condicion del if.
	li t8, CARACTER_Z
	subu t0, t8, a0
	bltz t0, minusculas

	#Asignamos v0, se cumplieron las dos condiciones del if.
	subu v0, a0, CARACTER_A
	j return_decode_byte


minusculas:
	#Restamos al byte el valor de 'a' y nos fijamos si es mayor o igual al byte.
	subu t0, a0, CARACTER_a
	bltz t0, numeros

	#Segunda condicion del if.
	li t8, CARACTER_z
	subu t0, t8, a0
	bltz t0, numeros

	#Asignamos v0, se cumplieron las dos condiciones del if.
	subu v0, a0, CARACTER_a
	addu v0, v0, 26
	j return_decode_byte

numeros:
	#Restamos al byte el valor de 0 y nos fijamos si es mayor o igual al byte.
	subu t0, a0, CARACTER_0
	bltz t0, mas

	#Segunda condicion del if.
	li t8, CARACTER_9
	subu t0, t8, a0
	bltz t0, mas

	#Asignamos v0, se cumplieron las dos condiciones del if.
	subu v0, a0, CARACTER_0
	addu v0, v0, 52
	j return_decode_byte

mas:
	#Me fijo si es el caracter "+". En caso de no serlo sigo.
	li t1, CARACTER_MAS
	bne a0, t1, barra

	#Devuelve 62
	li v0, 62
	j return_decode_byte

barra:
	#Me fijo si es el caracter "/". En caso de no serlo sigo.
	li t1, CARACTER_BARRA
	bne a0, t1, igual

	#Devuelve 63. No hace falta el jmp al return.
	li v0, 63

igual:
	li t1, CARACTER_IGUAL
	bne a0, t1, return_decode_byte
	li v0, 64


return_decode_byte:
	#El valor de retorno ya fue cargado en v0.
	lw gp, 8(sp)
	lw $fp, 12(sp)
	addu sp, sp, STACKSIZE_decode_byte
	jr ra

.end decode_byte


.align 2
.ent base64_transform_3bytes

base64_transform_3bytes:
	.frame $fp, STACKSIZE_base64_transform_3_bytes, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_base64_transform_3_bytes
	.cprestore 24

	#Guardo calle-saved.
	sw $fp, 28(sp)
	sw ra, 32(sp)

	move $fp, sp

	#Guardos args en ABA.
	sw a0, STACKSIZE_base64_transform_3_bytes(sp)

	#t0 -> indice
	li t0, 0

	sw t0, 16(sp)

loop1:
	#loop termina en 4 (no hace falta tenerlo en stack, es cte.)
	li t1, 4

	#Me fijo si llegue al final del loop.
	beq t1, t0, continuar1

	#Cargo argumentos para invocar funcion.
	lw a0, STACKSIZE_base64_transform_3_bytes(sp)
	mul a1, t0, 6
	addu a2, a1, 5

	#Guardo caller-saved.
	sw t0, 16(sp)

	#Llamo funcion externa.
	la t9, bytes_extract_n_bits
	jal t9

	#cargamos caller-save.
	lw t0, 16(sp)

	#Caso i == 0
	li t1, 0
	bne t0, t1, caso_1

	#Cargo primer byte del vector devuelto y shifteo su valor a derecha 2 bytes.
	lb t3, (v0)
	srl t2, t3, 2

	and t2,t2,0x3f

	j terminacion_del_loop1

caso_1:
	#Caso i == 1
	li t1, 1
	bne t0, t1, caso_2

	lb t3, (v0)
	lb t4, 1(v0)
	sll t3, t3, 4
	srl t4, t4, 4
	and	t3, t3, 0xf0
	and t4, t4, 0xf
	or t2, t3, t4	

	j terminacion_del_loop1

caso_2:
	#Caso i == 2
	li t1, 2
	bne t0, t1, caso_3

	lb t3, (v0)
	lb t4, 1(v0)
	sll t3, t3, 2
	srl t4, t4, 6
	and	t3, t3, 0xfc
	and t4, t4, 0x3
	or t2, t3, t4

	j terminacion_del_loop1

caso_3:
	#Caso i == 3
	li t1, 3
	bne t0, t1, terminacion_del_loop1

	lb t2, (v0)

terminacion_del_loop1:
	#t3 -> base64+i
	la t3, transform_3bytes_vector
	addu t3, t3, t0

	#guardo t2 en base64[i]
	sb t2, (t3)

	#t4 -> base64[i]
	lb t4, (t3)

	#t5 -> base64_table + base64[i]
	la t5, base64_table
	addu t5, t5, t4

	#t6 -> base64_table[base64[i]]
	lb t6, (t5)

	#Guardo.
	sb t6, (t3)

	addu t0, t0, 1
	j loop1

continuar1:
	#Extraigo valores del stack.
	lw gp, 24(sp)
	lw $fp, 28(sp)
	lw ra, 32(sp)

	addu sp, sp, STACKSIZE_base64_transform_3_bytes

	la v0, transform_3bytes_vector

	jr ra


.end base64_transform_3bytes



.align 2
.ent base64_decode_4bytes

base64_decode_4bytes:
	.frame $fp, STACKSIZE_base64_decode_4bytes, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_base64_decode_4bytes
	.cprestore 32

	#Guardo calle-saved.
	sw $fp, 36(sp)
	sw ra, 40(sp)

	move $fp, sp

	#Guardos args en ABA.
	sw a0, STACKSIZE_base64_decode_4bytes(sp)
	sw a1, STACKSIZE_base64_decode_4bytes+4(sp)

	#Calculamos cantidad de iguales.
	#t8 -> cantidad de iguales.
	li t8, 0
	lb t1, 2(a0)
	lb t2, 3(a0)

	#En caso de que el primero sea igual, el segundo tambien lo es.
	beq t1, CARACTER_IGUAL, incrementar_1
	beq t2, CARACTER_IGUAL, incrementar_2

	j salto_incrementos

incrementar_1:
	addu t8, t8, 1

incrementar_2:
	addu t8, t8, 1

salto_incrementos:

	#Guardo hasta finalizar la funcion.
	sw t8, 20(sp)

	#Inicializamos el vector con todo en 0.
	la t1, decode_4bytes_vector
	sb zero, (t1)
	sb zero, 1(t1)
	sb zero, 2(t1) 

	#memcpy.
	la t1, copy_4bytes_vector

	lb t2, (a0)
	sb t2, (t1)
	lb t2, 1(a0)
	sb t2, 1(t1)
	lb t2, 2(a0)
	sb t2, 2(t1)
	lb t2, 3(a0)
	sb t2, 3(t1)



	#llamadas para decodificacion.
	lb a0, (t1)
	la t9, decode_byte
	jal decode_byte
	li t8, -1
	beq v0, t8, fatal_error
	la t1, copy_4bytes_vector
	sb v0, (t1)

	lb a0, 1(t1)
	la t9, decode_byte
	jal decode_byte
	li t8, -1
	beq v0, t8, fatal_error
	la t1, copy_4bytes_vector
	sb v0, 1(t1)

	lb a0, 2(t1)
	la t9, decode_byte
	jal decode_byte
	li t8, -1
	beq v0, t8, fatal_error
	la t1, copy_4bytes_vector
	sb v0, 2(t1)

	lb a0, 3(t1)
	la t9, decode_byte
	jal decode_byte
	li t8, -1
	beq v0, t8, fatal_error
	la t1, copy_4bytes_vector
	sb v0, 3(t1)

	li t0, 0

loop2:
	#loop termina en 4 (no hace falta tenerlo en stack, es cte.)
	li t1, 4

	#Me fijo si llegue al final del loop.
	beq t1, t0, continuar2

	#Cargo argumentos para invocar funcion.
	la a0, copy_4bytes_vector
	mul a1, t0, 8
	addu a2, a1, 7

	#Guardo caller-saved.
	sw t0, 16(sp)

	#Llamo funcion externa.
	la t9, bytes_extract_n_bits
	jal t9

	#cargamos caller-save.
	lw t0, 16(sp)

	#Caso i == 0
	li t1, 0
	bne t0, t1, caso_5

	la t3, decode_4bytes_vector
	lb t4, (t3)
	lb a0, (v0)
	li a1, 2
	li a2, 7
	sw t0, 16(sp)
	la t9, byte_extract_n_bits
	jal t9
	lw t0, 16(sp)
	sll t8, v0, 2
	la t3, decode_4bytes_vector
	lb t4, (t3)
	or t4, t4, t8
	sb t4, (t3)

	j terminacion_del_loop2

caso_5:
	#Caso i == 1
	li t1, 1
	bne t0, t1, caso_6

	la t3, decode_4bytes_vector
	lb t4, (t3)
	lb a0, (v0)
	li a1, 2
	li a2, 4
	sw t0, 16(sp)
	sw v0, 24(sp)
	la t9, byte_extract_n_bits
	jal t9
	lw t0, 16(sp)
	srl t8, v0, 4
	la t3, decode_4bytes_vector
	lb t4, (t3)
	or t4, t4, t8
	sb t4, (t3)

	lw v0, 24(sp)
	la t3, decode_4bytes_vector
	lb t4, 1(t3)
	lb a0, (v0)
	li a1, 4
	li a2, 7
	sw t0, 16(sp)
	la t9, byte_extract_n_bits
	jal t9
	lw t0, 16(sp)
	sll t8, v0, 4
	la t3, decode_4bytes_vector
	lb t4, 1(t3)
	or t4, t4, t8
	sb t4, 1(t3)	
	

	j terminacion_del_loop2

caso_6:
	#Caso i == 2
	li t1, 2
	bne t0, t1, caso_7

	la t3, decode_4bytes_vector
	lb t4, 1(t3)
	lb a0, (v0)
	li a1, 2
	li a2, 5
	sw t0, 16(sp)
	sw v0, 24(sp)
	la t9, byte_extract_n_bits
	jal t9
	lw t0, 16(sp)
	srl t8, v0, 2
	la t3, decode_4bytes_vector
	lb t4, 1(t3)
	or t4, t4, t8
	sb t4, 1(t3)


	lw v0, 24(sp)
	la t3, decode_4bytes_vector
	lb t4, 2(t3)
	lb a0, (v0)
	li a1, 5
	li a2, 7
	sw t0, 16(sp)
	la t9, byte_extract_n_bits
	jal t9
	lw t0, 16(sp)
	sll t8, v0, 6
	la t3, decode_4bytes_vector
	lb t4, 2(t3)
	or t4, t4, t8
	sb t4, 2(t3)

	j terminacion_del_loop2

caso_7:
	#Caso i == 3
	li t1, 3
	bne t0, t1, terminacion_del_loop2

	la t3, decode_4bytes_vector
	lb t4, 2(t3)
	lb a0, (v0)
	li a1, 2
	li a2, 7
	sw t0, 16(sp)
	la t9, byte_extract_n_bits
	jal t9
	lw t0, 16(sp)
	la t3, decode_4bytes_vector
	lb t4, 2(t3)
	or t4, t4, v0
	sb t4, 2(t3)

terminacion_del_loop2:
	lw t0, 16(sp)
	addu t0, t0, 1
	j loop2

continuar2:
	lw a1, STACKSIZE_base64_decode_4bytes+4(sp)
	la t5, decode_4bytes_vector
	sw t5, (a1)

	#t8 -> cantidad de iguales.
	lw t8, 20(sp)
	
	#Devuelvo cantidad de bytes encodeados.
	li t7, 3
	subu v0, t7, t8


	#Extraigo valores del stack.
	lw gp, 32(sp)
	lw $fp, 36(sp)
	lw ra, 40(sp)

	addu sp, sp, STACKSIZE_base64_decode_4bytes

	jr ra

fatal_error:
	#Retorno error
	li v0, -1

	#Extraigo valores del stack.
	lw gp, 32(sp)
	lw $fp, 36(sp)
	lw ra, 40(sp)

	addu sp, sp, STACKSIZE_base64_decode_4bytes

	jr ra

.end base64_decode_4bytes

.data
	
	transform_3bytes_vector: .space 4
	decode_4bytes_vector: .space 3
	copy_4bytes_vector: .space 4

	base64_table: .byte 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M' ,'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm' ,'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z','0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'



