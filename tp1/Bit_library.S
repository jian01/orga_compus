#include <mips/regdef.h>
#include <sys/syscall.h>

.text
.abicalls
.align 2
.globl byte_nth_bit
.globl byte_mask_nth_bit
.globl byte_extract_n_bits
.globl bytes_nth_bit
.globl bytes_mask_nth_bit
.globl bytes_extract_n_bits

.ent byte_nth_bit

#define STACKSIZE_byte_nth_bit 16

#define STACKSIZE_byte_mask_nth_bit 32

#define STACKSIZE_byte_extract_n_bits 40

#define STACKSIZE_bytes_nth_bit 32

#define STACKSIZE_bytes_mask_nth_bit 32

#define STACKSIZE_bytes_extract_n_bits 48

#No tiene aba, es leaf.
byte_nth_bit:
	.frame $fp, STACKSIZE_byte_nth_bit, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_byte_nth_bit
	.cprestore 8

	#Guardo calle-saved.
	sw $fp, 12(sp)

	move $fp, sp

	#Guardo args en ABA.
	sw a0, STACKSIZE_byte_nth_bit(sp)
	sw a1, STACKSIZE_byte_nth_bit+4(sp)

	#Resto 7 al numero.
	move t1, a1
	subu t1, t1, 7

	#Abs del numero.
	sra t8, t1, 31
	xor t1, t1, t8
	sub t1, t1, t8

	#Shift a derecha del char.
	move t0, a0
	srlv t0, t0, t1

	#Mascara bit primera posicion.
	and v0, t0, 0x01

	lw gp, 8(sp)
	lw $fp, 12(sp)

	addu sp, sp, STACKSIZE_byte_nth_bit

	jr ra

.end byte_nth_bit

.align 2
.ent byte_mask_nth_bit

#Tiene ABA, no es leaf.
byte_mask_nth_bit:
	.frame $fp, STACKSIZE_byte_mask_nth_bit, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_byte_mask_nth_bit
	.cprestore 16

	#Guardo calle-saved.
	sw $fp, 20(sp)
	sw ra, 24(sp)

	move $fp, sp

	#Guardo args en ABA.
	sw a0, STACKSIZE_byte_mask_nth_bit(sp)
	sw a1, STACKSIZE_byte_mask_nth_bit+4(sp)

	#Call (son los mismos parametros que recibi).
	la t9, byte_nth_bit
	jal t9
	nop

	#Recupero a0, a1
	lw a0, STACKSIZE_byte_mask_nth_bit(sp)
	lw a1, STACKSIZE_byte_mask_nth_bit+4(sp)

	#Resto 7 al numero.
	move t1, a1
	subu t1, t1, 7

	#Abs del numero.
	sra t8, t1, 31
	xor t1, t1, t8
	sub t1, t1, t8

	#Shifteo el resultado devuelto.
	sllv v0, v0, t1

	#Extraigo valores del stack.
	lw gp, 16(sp)
	lw $fp, 20(sp)
	lw ra, 24(sp)

	addu sp, sp, STACKSIZE_byte_mask_nth_bit

	jr ra

.end byte_mask_nth_bit

.align 2
.ent byte_extract_n_bits

byte_extract_n_bits:
	.frame $fp, STACKSIZE_byte_extract_n_bits, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_byte_extract_n_bits
	.cprestore 24

	#Guardo calle-saved.
	sw $fp, 28(sp)
	sw ra, 32(sp)

	move $fp, sp

	#Guardos args en ABA.
	sw a0, STACKSIZE_byte_extract_n_bits(sp)
	sw a1, STACKSIZE_byte_extract_n_bits+4(sp)
	sw a2, STACKSIZE_byte_extract_n_bits+8(sp)

	#Inicio retval e index.
	and t0, zero, zero
	move t1, a1

loop:
	#Recupero variables de los caller-saved.
	lw a0, STACKSIZE_byte_extract_n_bits(sp)
	lw a1, STACKSIZE_byte_extract_n_bits+4(sp)
	lw a2, STACKSIZE_byte_extract_n_bits+8(sp)
	

	#Veo si termina loop.
	subu t8, t1, 1
	beq t8, a2, continuar

	#Guardo en la LTA.
	sw t0, 16(sp)
	sw t1, 20(sp)

	#Calleo.
	move a1, t1
	la t9, byte_mask_nth_bit
	jal t9
	nop

	#Recupero de la LTA.
	lw t0, 16(sp)
	lw t1, 20(sp)

	#Actualizo retval e indice.
	addu t1, t1, 1
	or t0, t0, v0
	
	#Reinicio loop
	j loop

continuar:
	#Extraigo valores del stack.
	lw gp, 24(sp)
	lw $fp, 28(sp)
	lw ra, 32(sp)

	addu sp, sp, STACKSIZE_byte_extract_n_bits

	move v0, t0

	jr ra

.end byte_extract_n_bits

.align 2
.ent bytes_nth_bit
bytes_nth_bit:
	.frame $fp, STACKSIZE_bytes_nth_bit, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_bytes_nth_bit
	.cprestore 16

	#Guardo calle-saved.
	sw $fp, 20(sp)
	sw ra, 24(sp)

	move $fp, sp

	#Guardo en la ABA de mi caller.
	sw a0, STACKSIZE_bytes_nth_bit(sp)
	sw a1, STACKSIZE_bytes_nth_bit+4(sp)

	#Divido el primer parametro por 8 y guardo en t0.
	div t0, a1, 8

	#Multiplico t0 por 8.
	mul t1, t0, 8
	subu t1, a1, t1

	#Parametros para invocar.
	addu a0, a0, t0
	lb a0, (a0) 
	move a1, t1

	la t9, byte_nth_bit
	jal t9
	nop

	#Recupero calle-saved.
	lw $fp, 20(sp)
	lw ra, 24(sp)

	#Vuelvo sp.
	addu sp, sp, STACKSIZE_bytes_nth_bit

	#Ya tengo el resultado en v0.
	jr ra

.end bytes_nth_bit

.align 2
.ent bytes_mask_nth_bit
bytes_mask_nth_bit:
	.frame $fp, STACKSIZE_bytes_mask_nth_bit, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_bytes_mask_nth_bit
	.cprestore 16

	#Guardo calle-saved.
	sw $fp, 20(sp)
	sw ra, 24(sp)

	move $fp, sp

	#Guardo en la ABA de mi caller.
	sw a0, STACKSIZE_bytes_mask_nth_bit(sp)
	sw a1, STACKSIZE_bytes_mask_nth_bit+4(sp)

	#Divido el primer parametro por 8 y guardo en t0.
	div t0, a1, 8

	#Multiplico t0 por 8.
	mul t1, t0, 8
	subu t1, a1, t1

	#Parametros para invocar.
	addu a0, a0, t0
	lb a0, (a0) 
	move a1, t1

	la t9, byte_mask_nth_bit
	jal t9
	nop

	#Recupero calle-saved.
	lw $fp, 20(sp)
	lw ra, 24(sp)

	#Vuelvo sp.
	addu sp, sp, STACKSIZE_bytes_mask_nth_bit

	#Ya tengo el resultado en v0.
	jr ra

.end bytes_mask_nth_bit

.align 2
.ent bytes_extract_n_bits

bytes_extract_n_bits:
	.frame $fp, STACKSIZE_bytes_extract_n_bits, ra
	.set noreorder
	.cpload t9
	.set reorder
	subu sp, sp, STACKSIZE_bytes_extract_n_bits
	.cprestore 32

	#Guardo calle-saved.
	sw $fp, 36(sp)
	sw ra, 40(sp)

	move $fp, sp

	#Guardos args en ABA.
	sw a0, STACKSIZE_bytes_extract_n_bits(sp)
	sw a1, STACKSIZE_bytes_extract_n_bits+4(sp)
	sw a2, STACKSIZE_bytes_extract_n_bits+8(sp)

	#Inicializacion de variables.
	div t0, a1, 8 #t0 -> initial_byte
	div t1, a2, 8 #t1 -> last_byte
	
	#Guardo en a0 cantidad bytes que debo alocar con syscall.
	subu a0, t1, t0 
	addu a0, a0, 1 #a0 -> cantidad_bytes
	#li a0, 128

	#Guardo variables temporales.
	sw t0, 16(sp)
	sw t1, 20(sp)

	#Syscall para alocar memoria en heap.
	#la t9, malloc
	#jal malloc
	la v0, nuevo_vector

	#Aca deberiamos verificar que nos dio memoria?
	sw v0, 24(sp)

	move t3, t0 # t3 -> iterador i.
	sb t3, 28(sp)

loop2:
	#Recupero variables temporales.
	lw t0, 16(sp)
	lw t1, 20(sp) 
	lw t2, 24(sp) #t2 -> puntero a cadena.
	lb t3, 28(sp)

	#Recupero argumentos.
	lw a0, STACKSIZE_bytes_extract_n_bits(sp)
	lw a1, STACKSIZE_bytes_extract_n_bits+4(sp)
	lw a2, STACKSIZE_bytes_extract_n_bits+8(sp)

	#Veo si termina loop.
	subu t8, t3, 1
	beq t8, t1, continuar2

	#Cargo variable.
	li t4, 0 # t4 -> initial_bit

	bne t0, t3, skip_mod8_initial_n

	#initial_n mod 8.
	div t8, a1, 8
	mul t8, t8, 8
	subu t4, a1, t8

skip_mod8_initial_n:
	li t5, 7 # t5 -> last_bit

	bne t1, t3, skip_mod8_last_n

	#last_n mod 8.
	div t8, a2, 8
	mul t8, t8, 8
	subu t5, a2, t8

skip_mod8_last_n:
	#Guardo caller-saved regs.
	sw t0, 16(sp)
	sw t1, 20(sp) 
	sw t2, 24(sp)
	sb t3, 28(sp)


	addu a0, a0, t3
	lb a0, (a0) # bytes[i]
	move a1, t4 # initial_bit
	move a2, t5 # last_bit

	la t9, byte_extract_n_bits
	jal byte_extract_n_bits
	nop

	lw t0, 16(sp)
	lw t1, 20(sp) 
	lw t2, 24(sp)
	lb t3, 28(sp)

	subu t8, t3, t0
	addu t2, t2, t8
	sb v0, (t2)

	addu t3, t3, 1
	sw t3, 28(sp)

	j loop2
	
continuar2:
	#Retval
	lw t2, 24(sp)
	move v0, t2

	#Recupero callee-saved.
	lw gp, 32(sp)
	lw $fp, 36(sp)
	lw ra, 40(sp)

	addu sp, sp, STACKSIZE_bytes_extract_n_bits

	jr ra

.end bytes_extract_n_bits

.data 
	nuevo_vector: .space 2048

.rdata
.align 2






