	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 12, 3
	.globl	_c4_impl_exchangeSpace          ; -- Begin function c4_impl_exchangeSpace
	.p2align	2
_c4_impl_exchangeSpace:                 ; @c4_impl_exchangeSpace
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	.cfi_def_cfa_offset 64
	str	wzr, [sp, #60]
	b	LBB0_1
LBB0_1:                                 ; =>This Inner Loop Header: Depth=1
	mov	x8, #-1
	str	x8, [sp, #48]
	ldur	x9, [sp, #60]
	ldr	x10, [sp, #48]
	adrp	x11, _lock@PAGE
	add	x11, x11, _lock@PAGEOFF
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp]                        ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #12]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB0_3
	b	LBB0_2
LBB0_2:                                 ;   in Loop: Header=BB0_1 Depth=1
	ldr	x8, [sp]                        ; 8-byte Folded Reload
	stur	x8, [sp, #60]
	b	LBB0_3
LBB0_3:                                 ;   in Loop: Header=BB0_1 Depth=1
	ldr	w8, [sp, #12]                   ; 4-byte Folded Reload
	mov	w9, #1
	and	w8, w8, w9
	strb	w8, [sp, #47]
	ldrb	w8, [sp, #47]
	tbnz	w8, #0, LBB0_5
	b	LBB0_4
LBB0_4:                                 ;   in Loop: Header=BB0_1 Depth=1
	b	LBB0_1
LBB0_5:
	adrp	x10, _brk@PAGE
	add	x10, x10, _brk@PAGEOFF
	ldar	x8, [x10]
	adrp	x11, _toSpace@PAGE
	ldr	x9, [x11, _toSpace@PAGEOFF]
	subs	x8, x8, x9
	str	x8, [sp, #32]
	ldr	x8, [x11, _toSpace@PAGEOFF]
	str	x8, [sp, #24]
	adrp	x9, _fromSpace@PAGE
	ldr	x8, [x9, _fromSpace@PAGEOFF]
	str	x8, [x11, _toSpace@PAGEOFF]
	stlr	x8, [x10]
	ldr	x8, [sp, #24]
	str	x8, [x9, _fromSpace@PAGEOFF]
	str	xzr, [sp, #16]
	ldr	x8, [sp, #16]
	adrp	x9, _lock@PAGE
	add	x9, x9, _lock@PAGEOFF
	stlr	x8, [x9]
	ldr	x0, [sp, #32]
	add	sp, sp, #64
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_impl_sbrk                   ; -- Begin function c4_impl_sbrk
	.p2align	2
_c4_impl_sbrk:                          ; @c4_impl_sbrk
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #128
	.cfi_def_cfa_offset 128
	str	x0, [sp, #112]
	str	x1, [sp, #104]
	str	w2, [sp, #100]
	ldr	x8, [sp, #112]
	add	x8, x8, #40
	str	x8, [sp, #88]
	b	LBB1_1
LBB1_1:                                 ; =>This Inner Loop Header: Depth=1
	adrp	x8, _lock@PAGE
	add	x8, x8, _lock@PAGEOFF
	ldar	x8, [x8]
	str	x8, [sp, #80]
	ldr	x8, [sp, #80]
	adds	x8, x8, #1
	b.ne	LBB1_3
	b	LBB1_2
LBB1_2:                                 ;   in Loop: Header=BB1_1 Depth=1
	b	LBB1_1
LBB1_3:
	mov	x8, #1
	str	x8, [sp, #72]
	ldr	x8, [sp, #72]
	adrp	x9, _lock@PAGE
	add	x9, x9, _lock@PAGEOFF
	ldaddal	x8, x8, [x9]
	str	x8, [sp, #64]
	ldr	x8, [sp, #88]
	lsr	x8, x8, #0
	str	x8, [sp, #48]
	ldr	x8, [sp, #48]
	adrp	x10, _brk@PAGE
	add	x10, x10, _brk@PAGEOFF
	ldaddal	x8, x8, [x10]
	str	x8, [sp, #40]
	ldr	x8, [sp, #40]
	str	x8, [sp, #56]
	ldr	x8, [sp, #56]
	ldr	x10, [sp, #88]
	add	x8, x8, x10
	adrp	x10, _toSpace@PAGE
	ldr	x10, [x10, _toSpace@PAGEOFF]
	adrp	x11, _spaceSize@PAGE
	ldr	x11, [x11, _spaceSize@PAGEOFF]
	add	x10, x10, x11
	subs	x8, x8, x10
	cset	w8, hs
	and	w8, w8, #0x1
	str	w8, [sp, #36]
	mov	x8, #-1
	str	x8, [sp, #24]
	ldr	x8, [sp, #24]
	ldaddal	x8, x8, [x9]
	str	x8, [sp, #16]
	ldr	w8, [sp, #36]
	cbz	w8, LBB1_5
	b	LBB1_4
LBB1_4:
                                        ; kill: def $x8 killed $xzr
	str	xzr, [sp, #120]
	b	LBB1_8
LBB1_5:
	ldr	w8, [sp, #100]
	cbz	w8, LBB1_7
	b	LBB1_6
LBB1_6:
	ldr	x8, [sp, #88]
	str	x8, [sp, #8]
	ldr	x8, [sp, #8]
	adrp	x9, _heapAllocated@PAGE
	add	x9, x9, _heapAllocated@PAGEOFF
	ldaddal	x8, x8, [x9]
	str	x8, [sp]
	b	LBB1_7
LBB1_7:
	ldr	x8, [sp, #56]
	add	x9, x8, #8
	mov	x8, #0
	stlr	x8, [x9]
	ldr	x9, [sp, #56]
	add	x9, x9, #16
	stlr	x8, [x9]
	ldr	x8, [sp, #104]
	ldr	x9, [sp, #56]
	str	x8, [x9, #32]
	ldr	x8, [sp, #112]
	ldr	x9, [sp, #56]
	str	x8, [x9, #24]
	ldr	x9, [sp, #56]
	mov	w8, #0
	stlrb	w8, [x9]
	ldr	x8, [sp, #56]
	str	x8, [sp, #120]
	b	LBB1_8
LBB1_8:
	ldr	x0, [sp, #120]
	add	sp, sp, #128
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_malloc                      ; -- Begin function c4_malloc
	.p2align	2
_c4_malloc:                             ; @c4_malloc
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	x0, [x29, #-16]
	str	x1, [sp, #24]
	str	wzr, [sp, #20]
	ldur	x0, [x29, #-16]
	ldr	x1, [sp, #24]
	mov	w2, #1
	bl	_c4_impl_sbrk
	str	x0, [sp, #8]
	ldr	x8, [sp, #8]
	cbnz	x8, LBB2_7
	b	LBB2_1
LBB2_1:
	bl	_c4_gcRoutine
	mov	x8, x0
	str	w8, [sp, #20]
	b	LBB2_2
LBB2_2:                                 ; =>This Inner Loop Header: Depth=1
	bl	_c4_gcRoutine
	ldrsw	x8, [sp, #20]
	subs	x8, x0, x8
	b.ne	LBB2_4
	b	LBB2_3
LBB2_3:                                 ;   in Loop: Header=BB2_2 Depth=1
	b	LBB2_2
LBB2_4:
	ldur	x0, [x29, #-16]
	ldr	x1, [sp, #24]
	mov	w2, #1
	bl	_c4_impl_sbrk
	str	x0, [sp, #8]
	cbnz	x0, LBB2_6
	b	LBB2_5
LBB2_5:
                                        ; kill: def $x8 killed $xzr
	stur	xzr, [x29, #-8]
	b	LBB2_10
LBB2_6:
	b	LBB2_7
LBB2_7:
	adrp	x8, _gcCopyFlag@GOTPAGE
	ldr	x8, [x8, _gcCopyFlag@GOTPAGEOFF]
	ldarb	w8, [x8]
	strb	w8, [sp, #7]
	ldrb	w8, [sp, #7]
	tbnz	w8, #0, LBB2_9
	b	LBB2_8
LBB2_8:
	ldr	x0, [sp, #8]
	bl	_c4_impl_pushWorkList
	b	LBB2_9
LBB2_9:
	ldr	x8, [sp, #8]
	stur	x8, [x29, #-8]
	b	LBB2_10
LBB2_10:
	ldur	x0, [x29, #-8]
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_spaceSize                      ; @spaceSize
.zerofill __DATA,__common,_spaceSize,8,3
	.globl	_fromSpace                      ; @fromSpace
.zerofill __DATA,__common,_fromSpace,8,3
	.globl	_toSpace                        ; @toSpace
.zerofill __DATA,__common,_toSpace,8,3
	.globl	_brk                            ; @brk
.zerofill __DATA,__common,_brk,8,3
	.globl	_heapAllocated                  ; @heapAllocated
.zerofill __DATA,__common,_heapAllocated,8,3
.zerofill __DATA,__bss,_lock,8,3        ; @lock
.subsections_via_symbols
