	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 12, 3
	.globl	_c4_init                        ; -- Begin function c4_init
	.p2align	2
_c4_init:                               ; @c4_init
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #96
	stp	x29, x30, [sp, #80]             ; 16-byte Folded Spill
	add	x29, sp, #80
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	adrp	x8, _basicrs@GOTPAGE
	ldr	x8, [x8, _basicrs@GOTPAGEOFF]
	str	x8, [sp]                        ; 8-byte Folded Spill
	stur	x0, [x29, #-16]
	stur	x1, [x29, #-24]
	stur	x2, [x29, #-32]
	ldur	x8, [x29, #-16]
	adrp	x9, _gcThrdCount@PAGE
	str	x8, [x9, _gcThrdCount@PAGEOFF]
	ldur	x8, [x29, #-32]
	mov	x9, #2
	udiv	x8, x8, x9
	adrp	x9, _spaceSize@GOTPAGE
	ldr	x9, [x9, _spaceSize@GOTPAGEOFF]
	str	x8, [x9]
	ldur	x10, [x29, #-24]
	adrp	x8, _fromSpace@GOTPAGE
	ldr	x8, [x8, _fromSpace@GOTPAGEOFF]
	str	x10, [x8]
	ldr	x8, [x8]
	ldr	x9, [x9]
	add	x8, x8, x9
	adrp	x9, _toSpace@GOTPAGE
	ldr	x9, [x9, _toSpace@GOTPAGEOFF]
	str	x8, [x9]
	adrp	x9, _brk@GOTPAGE
	ldr	x9, [x9, _brk@GOTPAGEOFF]
	stlr	x8, [x9]
	bl	_getMaxThrdCount
	str	x0, [sp, #40]
	ldr	x9, [sp, #40]
	mov	x8, #32
	mul	x8, x8, x9
	str	x8, [sp, #32]
	ldr	x0, [sp, #32]
	bl	_malloc
	ldr	x8, [sp]                        ; 8-byte Folded Reload
	str	x0, [x8]
	ldr	x8, [x8]
	cbnz	x8, LBB0_2
	b	LBB0_1
LBB0_1:
	mov	w8, #1
	stur	w8, [x29, #-4]
	b	LBB0_19
LBB0_2:
	str	wzr, [sp, #28]
	b	LBB0_3
LBB0_3:                                 ; =>This Inner Loop Header: Depth=1
	ldrsw	x8, [sp, #28]
	ldr	x9, [sp, #40]
	subs	x9, x9, #1
	subs	x8, x8, x9
	b.hs	LBB0_6
	b	LBB0_4
LBB0_4:                                 ;   in Loop: Header=BB0_3 Depth=1
	ldr	x9, [sp]                        ; 8-byte Folded Reload
	ldr	x8, [x9]
	ldr	w10, [sp, #28]
	add	w11, w10, #1
                                        ; implicit-def: $x10
	mov	x10, x11
	sxtw	x10, w10
	add	x8, x8, x10, lsl #5
	ldr	x9, [x9]
	ldrsw	x10, [sp, #28]
	add	x9, x9, x10, lsl #5
	add	x9, x9, #24
	stlr	x8, [x9]
	b	LBB0_5
LBB0_5:                                 ;   in Loop: Header=BB0_3 Depth=1
	ldr	w8, [sp, #28]
	add	w8, w8, #1
	str	w8, [sp, #28]
	b	LBB0_3
LBB0_6:
	ldr	x8, [sp]                        ; 8-byte Folded Reload
	ldr	x8, [x8]
	ldr	x9, [sp, #40]
	subs	x9, x9, #1
	add	x8, x8, x9, lsl #5
	add	x9, x8, #24
	mov	x8, #0
	stlr	x8, [x9]
	add	x0, sp, #16
	mov	x3, #0
	str	xzr, [sp, #16]
	mov	x1, x3
	adrp	x2, _c4_impl_phaseManager@GOTPAGE
	ldr	x2, [x2, _c4_impl_phaseManager@GOTPAGEOFF]
	bl	_pthread_create
	cbz	w0, LBB0_8
	b	LBB0_7
LBB0_7:
	mov	w8, #2
	stur	w8, [x29, #-4]
	b	LBB0_19
LBB0_8:
	ldr	x0, [sp, #16]
	bl	_pthread_detach
	cbz	w0, LBB0_10
	b	LBB0_9
LBB0_9:
	mov	w8, #3
	stur	w8, [x29, #-4]
	b	LBB0_19
LBB0_10:
	str	xzr, [sp, #8]
	b	LBB0_11
LBB0_11:                                ; =>This Inner Loop Header: Depth=1
	ldr	x8, [sp, #8]
	ldur	x9, [x29, #-16]
	subs	x8, x8, x9
	b.hs	LBB0_18
	b	LBB0_12
LBB0_12:                                ;   in Loop: Header=BB0_11 Depth=1
	add	x0, sp, #16
	mov	x3, #0
	mov	x1, x3
	adrp	x2, _c4_impl_gcThrdFunc@GOTPAGE
	ldr	x2, [x2, _c4_impl_gcThrdFunc@GOTPAGEOFF]
	bl	_pthread_create
	cbz	w0, LBB0_14
	b	LBB0_13
LBB0_13:
	mov	w8, #2
	stur	w8, [x29, #-4]
	b	LBB0_19
LBB0_14:                                ;   in Loop: Header=BB0_11 Depth=1
	ldr	x0, [sp, #16]
	bl	_pthread_detach
	cbz	w0, LBB0_16
	b	LBB0_15
LBB0_15:
	mov	w8, #3
	stur	w8, [x29, #-4]
	b	LBB0_19
LBB0_16:                                ;   in Loop: Header=BB0_11 Depth=1
	b	LBB0_17
LBB0_17:                                ;   in Loop: Header=BB0_11 Depth=1
	ldr	x8, [sp, #8]
	add	x8, x8, #1
	str	x8, [sp, #8]
	b	LBB0_11
LBB0_18:
	stur	wzr, [x29, #-4]
	b	LBB0_19
LBB0_19:
	ldur	w0, [x29, #-4]
	ldp	x29, x30, [sp, #80]             ; 16-byte Folded Reload
	add	sp, sp, #96
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_gcThrdCount                 ; -- Begin function c4_gcThrdCount
	.p2align	2
_c4_gcThrdCount:                        ; @c4_gcThrdCount
	.cfi_startproc
; %bb.0:
	adrp	x8, _gcThrdCount@PAGE
	ldr	x0, [x8, _gcThrdCount@PAGEOFF]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_allocated                   ; -- Begin function c4_allocated
	.p2align	2
_c4_allocated:                          ; @c4_allocated
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	adrp	x8, _heapAllocated@GOTPAGE
	ldr	x8, [x8, _heapAllocated@GOTPAGEOFF]
	ldar	x8, [x8]
	str	x8, [sp, #8]
	ldr	x0, [sp, #8]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_collected                   ; -- Begin function c4_collected
	.p2align	2
_c4_collected:                          ; @c4_collected
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	adrp	x8, _gcCollected@PAGE
	add	x8, x8, _gcCollected@PAGEOFF
	ldar	x8, [x8]
	str	x8, [sp, #8]
	ldr	x0, [sp, #8]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_gcThrdCount                    ; @gcThrdCount
.zerofill __DATA,__common,_gcThrdCount,8,3
	.globl	_gcCollected                    ; @gcCollected
.zerofill __DATA,__common,_gcCollected,8,3
.subsections_via_symbols
