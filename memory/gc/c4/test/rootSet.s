	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 12, 3
	.globl	_c4_getRootSet                  ; -- Begin function c4_getRootSet
	.p2align	2
_c4_getRootSet:                         ; @c4_getRootSet
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	x0, [sp, #8]
	adrp	x8, _basicrs@PAGE
	ldr	x8, [x8, _basicrs@PAGEOFF]
	ldr	x9, [sp, #8]
	add	x0, x8, x9, lsl #5
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_pushRootSet                 ; -- Begin function c4_pushRootSet
	.p2align	2
_c4_pushRootSet:                        ; @c4_pushRootSet
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	str	x0, [sp, #40]
	str	x1, [sp, #32]
	str	x2, [sp, #24]
	str	x3, [sp, #16]
	ldr	x8, [sp, #40]
	str	x8, [sp, #8]
	ldr	x9, [sp, #8]
	mov	w8, #0
	stlrb	w8, [x9]
	ldr	x8, [sp, #16]
	ldr	x9, [sp, #8]
	str	x8, [x9, #16]
	ldr	x8, [sp, #24]
	ldr	x9, [sp, #8]
	str	x8, [x9, #8]
	ldr	x8, [sp, #32]
	add	x8, x8, #24
	ldar	x8, [x8]
	ldr	x9, [sp, #8]
	add	x9, x9, #24
	stlr	x8, [x9]
	ldr	x8, [sp, #32]
	add	x9, x8, #24
	ldr	x8, [sp, #8]
	str	x8, [sp]
	ldr	x8, [sp]
	stlr	x8, [x9]
	ldr	x0, [sp, #8]
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_popNextRootSet              ; -- Begin function c4_popNextRootSet
	.p2align	2
_c4_popNextRootSet:                     ; @c4_popNextRootSet
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	x0, [x29, #-8]
	stur	x1, [x29, #-16]
	ldur	x8, [x29, #-8]
	add	x9, x8, #24
                                        ; kill: def $x8 killed $xzr
	str	xzr, [sp, #16]
	ldr	x8, [sp, #16]
	swpal	x8, x8, [x9]
	str	x8, [sp, #8]
	ldr	x8, [sp, #8]
	str	x8, [sp, #24]
	ldr	x8, [sp, #24]
	ldr	x8, [x8, #8]
	cbnz	x8, LBB2_2
	b	LBB2_1
LBB2_1:
	b	LBB2_7
LBB2_2:
	b	LBB2_3
LBB2_3:                                 ; =>This Inner Loop Header: Depth=1
	ldr	x8, [sp, #24]
	ldarb	w8, [x8]
	strb	w8, [sp, #7]
	ldrb	w8, [sp, #7]
	tbz	w8, #0, LBB2_5
	b	LBB2_4
LBB2_4:                                 ;   in Loop: Header=BB2_3 Depth=1
	b	LBB2_3
LBB2_5:
	ldur	x8, [x29, #-16]
	cbz	x8, LBB2_7
	b	LBB2_6
LBB2_6:
	ldur	x8, [x29, #-16]
	ldr	x0, [sp, #24]
	blr	x8
	b	LBB2_7
LBB2_7:
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_loadRefAddr                 ; -- Begin function c4_loadRefAddr
	.p2align	2
_c4_loadRefAddr:                        ; @c4_loadRefAddr
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	str	x1, [sp]
	ldr	x0, [sp, #8]
	bl	_c4_impl_loadValueBarrier
	add	x8, x0, #40
	ldr	x9, [sp]
	add	x0, x8, x9, lsl #3
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_storeRef                    ; -- Begin function c4_storeRef
	.p2align	2
_c4_storeRef:                           ; @c4_storeRef
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	x0, [x29, #-8]
	stur	x1, [x29, #-16]
	str	x2, [sp, #24]
	ldur	x0, [x29, #-8]
	ldur	x1, [x29, #-16]
	bl	_c4_loadRefAddr
	str	x0, [sp, #8]                    ; 8-byte Folded Spill
	ldr	x0, [sp, #24]
	bl	_c4_impl_loadValueBarrier
	mov	x8, x0
	ldr	x0, [sp, #8]                    ; 8-byte Folded Reload
	str	x8, [sp, #16]
	ldr	x8, [sp, #16]
	stlr	x8, [x0]
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_load                        ; -- Begin function c4_load
	.p2align	2
_c4_load:                               ; @c4_load
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	str	x1, [sp]
	ldr	x0, [sp, #8]
	bl	_c4_impl_loadValueBarrier
	add	x8, x0, #40
	ldr	x9, [sp]
	ldr	x0, [x8, x9]
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_store                       ; -- Begin function c4_store
	.p2align	2
_c4_store:                              ; @c4_store
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	x0, [x29, #-8]
	str	x1, [sp, #16]
	str	x2, [sp, #8]
	ldr	x8, [sp, #8]
	str	x8, [sp]                        ; 8-byte Folded Spill
	ldur	x0, [x29, #-8]
	bl	_c4_impl_loadValueBarrier
	ldr	x8, [sp]                        ; 8-byte Folded Reload
	add	x9, x0, #40
	ldr	x10, [sp, #16]
	str	x8, [x9, x10]
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_basicrs                        ; @basicrs
.zerofill __DATA,__common,_basicrs,8,3
.subsections_via_symbols
