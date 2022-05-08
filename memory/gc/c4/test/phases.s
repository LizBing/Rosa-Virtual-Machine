	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 12, 3
	.globl	_c4_gcRoutine                   ; -- Begin function c4_gcRoutine
	.p2align	2
_c4_gcRoutine:                          ; @c4_gcRoutine
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	adrp	x8, _routine@PAGE
	add	x8, x8, _routine@PAGEOFF
	ldar	x8, [x8]
	str	x8, [sp, #8]
	ldr	x0, [sp, #8]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_impl_phaseManager           ; -- Begin function c4_impl_phaseManager
	.p2align	2
_c4_impl_phaseManager:                  ; @c4_impl_phaseManager
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #176
	stp	x29, x30, [sp, #160]            ; 16-byte Folded Spill
	add	x29, sp, #160
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	adrp	x8, _gcThrdCount@GOTPAGE
	ldr	x8, [x8, _gcThrdCount@GOTPAGEOFF]
	str	x8, [sp, #48]                   ; 8-byte Folded Spill
	b	LBB1_1
LBB1_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB1_2 Depth 2
                                        ;     Child Loop BB1_7 Depth 2
                                        ;     Child Loop BB1_12 Depth 2
	b	LBB1_2
LBB1_2:                                 ;   Parent Loop BB1_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	x8, [sp, #48]                   ; 8-byte Folded Reload
	stur	xzr, [x29, #-8]
	ldr	x9, [x8]
	ldur	x10, [x29, #-8]
	adrp	x11, _completedCount@PAGE
	add	x11, x11, _completedCount@PAGEOFF
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp, #32]                   ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #44]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB1_4
	b	LBB1_3
LBB1_3:                                 ;   in Loop: Header=BB1_2 Depth=2
	ldr	x8, [sp, #32]                   ; 8-byte Folded Reload
	ldr	x9, [sp, #48]                   ; 8-byte Folded Reload
	str	x8, [x9]
	b	LBB1_4
LBB1_4:                                 ;   in Loop: Header=BB1_2 Depth=2
	ldr	w8, [sp, #44]                   ; 4-byte Folded Reload
	mov	w9, #1
	and	w8, w8, w9
	sturb	w8, [x29, #-9]
	ldurb	w8, [x29, #-9]
	tbnz	w8, #0, LBB1_6
	b	LBB1_5
LBB1_5:                                 ;   in Loop: Header=BB1_2 Depth=2
	b	LBB1_2
LBB1_6:                                 ;   in Loop: Header=BB1_1 Depth=1
	bl	_c4_impl_exchangeSpace
	stur	x0, [x29, #-24]
	mov	x8, #1
	stur	x8, [x29, #-32]
	ldur	x8, [x29, #-32]
	adrp	x9, _routine@PAGE
	add	x9, x9, _routine@PAGEOFF
	ldaddal	x8, x8, [x9]
	stur	x8, [x29, #-40]
	adrp	x8, _workListBegin@PAGE
	ldr	x8, [x8, _workListBegin@PAGEOFF]
	adrp	x9, _workListIter@PAGE
	add	x9, x9, _workListIter@PAGEOFF
	stlr	x8, [x9]
	adrp	x8, _basicrs@GOTPAGE
	ldr	x8, [x8, _basicrs@GOTPAGEOFF]
	ldr	x8, [x8]
	adrp	x9, _rsIter@PAGE
	add	x9, x9, _rsIter@PAGEOFF
	stlr	x8, [x9]
	mov	w8, #1
	sturb	w8, [x29, #-41]
	ldurb	w8, [x29, #-41]
	adrp	x9, _gcCopyFlag@PAGE
	add	x9, x9, _gcCopyFlag@PAGEOFF
	stlrb	w8, [x9]
	b	LBB1_7
LBB1_7:                                 ;   Parent Loop BB1_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	x8, [sp, #48]                   ; 8-byte Folded Reload
	stur	xzr, [x29, #-56]
	ldr	x9, [x8]
	ldur	x10, [x29, #-56]
	adrp	x11, _completedCount@PAGE
	add	x11, x11, _completedCount@PAGEOFF
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp, #16]                   ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #28]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB1_9
	b	LBB1_8
LBB1_8:                                 ;   in Loop: Header=BB1_7 Depth=2
	ldr	x8, [sp, #16]                   ; 8-byte Folded Reload
	ldr	x9, [sp, #48]                   ; 8-byte Folded Reload
	str	x8, [x9]
	b	LBB1_9
LBB1_9:                                 ;   in Loop: Header=BB1_7 Depth=2
	ldr	w8, [sp, #28]                   ; 4-byte Folded Reload
	mov	w9, #1
	and	w8, w8, w9
	sturb	w8, [x29, #-57]
	ldurb	w8, [x29, #-57]
	tbnz	w8, #0, LBB1_11
	b	LBB1_10
LBB1_10:                                ;   in Loop: Header=BB1_7 Depth=2
	b	LBB1_7
LBB1_11:                                ;   in Loop: Header=BB1_1 Depth=1
	sturb	wzr, [x29, #-58]
	ldurb	w8, [x29, #-58]
	adrp	x9, _gcCopyFlag@PAGE
	add	x9, x9, _gcCopyFlag@PAGEOFF
	stlrb	w8, [x9]
	ldur	x8, [x29, #-24]
	adrp	x9, _copied@PAGE
	add	x9, x9, _copied@PAGEOFF
	ldar	x10, [x9]
	subs	x8, x8, x10
	stur	x8, [x29, #-72]
	ldur	x8, [x29, #-72]
	adrp	x10, _gcCollected@GOTPAGE
	ldr	x10, [x10, _gcCollected@GOTPAGEOFF]
	ldaddal	x8, x8, [x10]
	str	x8, [sp, #80]
	mov	x8, #0
	stlr	x8, [x9]
	adrp	x9, _remapListEnd@PAGE
	add	x9, x9, _remapListEnd@PAGEOFF
	stlr	x8, [x9]
	adrp	x9, _workListEnd@PAGE
	add	x9, x9, _workListEnd@PAGEOFF
	stlr	x8, [x9]
	mov	w8, #1
	strb	w8, [sp, #79]
	ldrb	w8, [sp, #79]
	adrp	x9, _remapFlag@PAGE
	add	x9, x9, _remapFlag@PAGEOFF
	stlrb	w8, [x9]
	b	LBB1_12
LBB1_12:                                ;   Parent Loop BB1_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	x8, [sp, #48]                   ; 8-byte Folded Reload
	str	xzr, [sp, #64]
	ldr	x9, [x8]
	ldr	x10, [sp, #64]
	adrp	x11, _completedCount@PAGE
	add	x11, x11, _completedCount@PAGEOFF
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp]                        ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #12]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB1_14
	b	LBB1_13
LBB1_13:                                ;   in Loop: Header=BB1_12 Depth=2
	ldr	x8, [sp]                        ; 8-byte Folded Reload
	ldr	x9, [sp, #48]                   ; 8-byte Folded Reload
	str	x8, [x9]
	b	LBB1_14
LBB1_14:                                ;   in Loop: Header=BB1_12 Depth=2
	ldr	w8, [sp, #12]                   ; 4-byte Folded Reload
	mov	w9, #1
	and	w8, w8, w9
	strb	w8, [sp, #63]
	ldrb	w8, [sp, #63]
	tbnz	w8, #0, LBB1_16
	b	LBB1_15
LBB1_15:                                ;   in Loop: Header=BB1_12 Depth=2
	b	LBB1_12
LBB1_16:                                ;   in Loop: Header=BB1_1 Depth=1
	strb	wzr, [sp, #62]
	ldrb	w8, [sp, #62]
	adrp	x9, _remapFlag@PAGE
	add	x9, x9, _remapFlag@PAGEOFF
	stlrb	w8, [x9]
	b	LBB1_1
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_impl_pushWorkList           ; -- Begin function c4_impl_pushWorkList
	.p2align	2
_c4_impl_pushWorkList:                  ; @c4_impl_pushWorkList
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	str	x0, [sp, #40]
	ldr	x9, [sp, #40]
	mov	w8, #1
	strb	w8, [sp, #39]
	ldrb	w8, [sp, #39]
	ldsetalb	w8, w8, [x9]
	strb	w8, [sp, #38]
	ldrb	w8, [sp, #38]
	tbz	w8, #0, LBB2_2
	b	LBB2_1
LBB2_1:
	b	LBB2_5
LBB2_2:
	ldr	x8, [sp, #40]
	str	x8, [sp, #16]
	ldr	x8, [sp, #16]
	adrp	x9, _workListEnd@PAGE
	add	x9, x9, _workListEnd@PAGEOFF
	swpal	x8, x8, [x9]
	str	x8, [sp, #8]
	ldr	x8, [sp, #8]
	str	x8, [sp, #24]
	ldr	x8, [sp, #24]
	cbnz	x8, LBB2_4
	b	LBB2_3
LBB2_3:
	ldr	x8, [sp, #40]
	adrp	x9, _workListBegin@PAGE
	str	x8, [x9, _workListBegin@PAGEOFF]
	adrp	x9, _workListIter@PAGE
	add	x9, x9, _workListIter@PAGEOFF
	stlr	x8, [x9]
	b	LBB2_5
LBB2_4:
	ldr	x8, [sp, #40]
	ldr	x9, [sp, #24]
	add	x9, x9, #16
	stlr	x8, [x9]
	ldr	x8, [sp, #40]
	add	x9, x8, #16
	mov	x8, #0
	stlr	x8, [x9]
	b	LBB2_5
LBB2_5:
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_impl_loadValueBarrier       ; -- Begin function c4_impl_loadValueBarrier
	.p2align	2
_c4_impl_loadValueBarrier:              ; @c4_impl_loadValueBarrier
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #144
	stp	x29, x30, [sp, #128]            ; 16-byte Folded Spill
	add	x29, sp, #128
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	x0, [x29, #-16]
                                        ; kill: def $x8 killed $xzr
	stur	xzr, [x29, #-24]
	stur	xzr, [x29, #-32]
	stur	wzr, [x29, #-36]
	b	LBB3_1
LBB3_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB3_2 Depth 2
                                        ;     Child Loop BB3_7 Depth 2
	b	LBB3_2
LBB3_2:                                 ;   Parent Loop BB3_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldur	x8, [x29, #-16]
	ldar	x8, [x8]
	stur	x8, [x29, #-48]
	ldur	x8, [x29, #-48]
	stur	x8, [x29, #-24]
	cbnz	x8, LBB3_4
	b	LBB3_3
LBB3_3:
                                        ; kill: def $x8 killed $xzr
	stur	xzr, [x29, #-8]
	b	LBB3_26
LBB3_4:                                 ;   in Loop: Header=BB3_2 Depth=2
	adrp	x8, _gcCopyFlag@PAGE
	add	x8, x8, _gcCopyFlag@PAGEOFF
	ldarb	w8, [x8]
	sturb	w8, [x29, #-49]
	ldurb	w8, [x29, #-49]
	tbz	w8, #0, LBB3_13
	b	LBB3_5
LBB3_5:                                 ;   in Loop: Header=BB3_1 Depth=1
	ldur	x9, [x29, #-24]
	sturb	wzr, [x29, #-50]
	ldurb	w8, [x29, #-50]
	mvn	w8, w8
	ldclralb	w8, w8, [x9]
	sturb	w8, [x29, #-51]
	ldurb	w8, [x29, #-51]
	tbnz	w8, #0, LBB3_10
	b	LBB3_6
LBB3_6:                                 ;   in Loop: Header=BB3_1 Depth=1
	b	LBB3_7
LBB3_7:                                 ;   Parent Loop BB3_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldur	x8, [x29, #-24]
	add	x8, x8, #8
	ldar	x8, [x8]
	str	x8, [sp, #64]
	ldr	x8, [sp, #64]
	stur	x8, [x29, #-32]
	cbnz	x8, LBB3_9
	b	LBB3_8
LBB3_8:                                 ;   in Loop: Header=BB3_7 Depth=2
	b	LBB3_7
LBB3_9:                                 ;   in Loop: Header=BB3_1 Depth=1
	b	LBB3_20
LBB3_10:                                ;   in Loop: Header=BB3_1 Depth=1
	ldur	x8, [x29, #-24]
	ldr	x0, [x8, #24]
	ldur	x8, [x29, #-24]
	ldr	x1, [x8, #32]
	mov	w2, #0
	bl	_c4_impl_sbrk
	stur	x0, [x29, #-32]
	ldur	x8, [x29, #-32]
	cbnz	x8, LBB3_12
	b	LBB3_11
LBB3_11:
	mov	w0, #1
	bl	_exit
LBB3_12:                                ;   in Loop: Header=BB3_1 Depth=1
	ldur	x8, [x29, #-32]
	add	x0, x8, #40
	ldur	x8, [x29, #-24]
	add	x1, x8, #40
	ldur	x8, [x29, #-24]
	ldr	x2, [x8, #24]
	mov	x3, #-1
	bl	___memcpy_chk
	ldur	x8, [x29, #-24]
	ldr	x8, [x8, #24]
	add	x8, x8, #40
	str	x8, [sp, #56]
	ldr	x8, [sp, #56]
	adrp	x9, _copied@PAGE
	add	x9, x9, _copied@PAGEOFF
	ldaddal	x8, x8, [x9]
	str	x8, [sp, #48]
	ldur	x8, [x29, #-24]
	add	x9, x8, #8
	ldur	x8, [x29, #-32]
	str	x8, [sp, #40]
	ldr	x8, [sp, #40]
	stlr	x8, [x9]
	b	LBB3_19
LBB3_13:                                ;   in Loop: Header=BB3_2 Depth=2
	adrp	x8, _remapFlag@PAGE
	add	x8, x8, _remapFlag@PAGEOFF
	ldarb	w8, [x8]
	strb	w8, [sp, #39]
	ldrb	w8, [sp, #39]
	tbz	w8, #0, LBB3_17
	b	LBB3_14
LBB3_14:                                ;   in Loop: Header=BB3_1 Depth=1
	ldur	x8, [x29, #-24]
	add	x8, x8, #8
	ldar	x8, [x8]
	cbnz	x8, LBB3_16
	b	LBB3_15
LBB3_15:
	ldur	x8, [x29, #-24]
	stur	x8, [x29, #-8]
	b	LBB3_26
LBB3_16:                                ;   in Loop: Header=BB3_1 Depth=1
	ldur	x8, [x29, #-24]
	add	x8, x8, #8
	ldar	x8, [x8]
	stur	x8, [x29, #-32]
	mov	w8, #1
	stur	w8, [x29, #-36]
	b	LBB3_18
LBB3_17:                                ;   in Loop: Header=BB3_2 Depth=2
	b	LBB3_2
LBB3_18:                                ;   in Loop: Header=BB3_1 Depth=1
	b	LBB3_19
LBB3_19:                                ;   in Loop: Header=BB3_1 Depth=1
	b	LBB3_20
LBB3_20:                                ;   in Loop: Header=BB3_1 Depth=1
	ldur	x11, [x29, #-16]
	ldur	x8, [x29, #-32]
	str	x8, [sp, #24]
	ldur	x9, [x29, #-24]
	ldr	x10, [sp, #24]
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp, #8]                    ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #16]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB3_22
	b	LBB3_21
LBB3_21:                                ;   in Loop: Header=BB3_1 Depth=1
	ldr	x8, [sp, #8]                    ; 8-byte Folded Reload
	stur	x8, [x29, #-24]
	b	LBB3_22
LBB3_22:                                ;   in Loop: Header=BB3_1 Depth=1
	ldr	w8, [sp, #16]                   ; 4-byte Folded Reload
	mov	w9, #1
	and	w8, w8, w9
	strb	w8, [sp, #23]
	ldrb	w8, [sp, #23]
	tbz	w8, #0, LBB3_1
	b	LBB3_23
LBB3_23:
	ldur	w8, [x29, #-36]
	cbz	w8, LBB3_25
	b	LBB3_24
LBB3_24:
	ldur	x0, [x29, #-32]
	bl	_pushRemapList
	b	LBB3_25
LBB3_25:
	ldur	x8, [x29, #-32]
	stur	x8, [x29, #-8]
	b	LBB3_26
LBB3_26:
	ldur	x0, [x29, #-8]
	ldp	x29, x30, [sp, #128]            ; 16-byte Folded Reload
	add	sp, sp, #144
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function pushRemapList
_pushRemapList:                         ; @pushRemapList
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	str	x0, [sp, #40]
	ldr	x9, [sp, #40]
	mov	w8, #1
	strb	w8, [sp, #39]
	ldrb	w8, [sp, #39]
	ldsetalb	w8, w8, [x9]
	strb	w8, [sp, #38]
	ldrb	w8, [sp, #38]
	tbz	w8, #0, LBB4_2
	b	LBB4_1
LBB4_1:
	b	LBB4_5
LBB4_2:
	ldr	x8, [sp, #40]
	str	x8, [sp, #16]
	ldr	x8, [sp, #16]
	adrp	x9, _remapListEnd@PAGE
	add	x9, x9, _remapListEnd@PAGEOFF
	swpal	x8, x8, [x9]
	str	x8, [sp, #8]
	ldr	x8, [sp, #8]
	str	x8, [sp, #24]
	ldr	x8, [sp, #24]
	cbnz	x8, LBB4_4
	b	LBB4_3
LBB4_3:
	ldr	x8, [sp, #40]
	adrp	x9, _remapListIter@PAGE
	add	x9, x9, _remapListIter@PAGEOFF
	stlr	x8, [x9]
	b	LBB4_5
LBB4_4:
	ldr	x8, [sp, #40]
	ldr	x9, [sp, #24]
	add	x9, x9, #16
	stlr	x8, [x9]
	ldr	x8, [sp, #40]
	add	x9, x8, #16
	mov	x8, #0
	stlr	x8, [x9]
	b	LBB4_5
LBB4_5:
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_mark                           ; -- Begin function mark
	.p2align	2
_mark:                                  ; @mark
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #144
	stp	x29, x30, [sp, #128]            ; 16-byte Folded Spill
	add	x29, sp, #128
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
                                        ; kill: def $x8 killed $xzr
	stur	xzr, [x29, #-8]
	stur	xzr, [x29, #-16]
	b	LBB5_1
LBB5_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB5_6 Depth 2
	adrp	x8, _rsIter@PAGE
	add	x8, x8, _rsIter@PAGEOFF
	ldar	x8, [x8]
	stur	x8, [x29, #-24]
	ldur	x8, [x29, #-24]
	stur	x8, [x29, #-8]
	cbz	x8, LBB5_13
	b	LBB5_2
LBB5_2:                                 ;   in Loop: Header=BB5_1 Depth=1
	ldur	x9, [x29, #-8]
	mov	w8, #1
	sturb	w8, [x29, #-25]
	ldurb	w8, [x29, #-25]
	stlrb	w8, [x9]
	ldur	x8, [x29, #-8]
	add	x8, x8, #24
	ldar	x8, [x8]
	stur	x8, [x29, #-40]
	ldur	x8, [x29, #-40]
	stur	x8, [x29, #-16]
	ldur	x8, [x29, #-16]
	stur	x8, [x29, #-48]
	ldur	x9, [x29, #-8]
	ldur	x10, [x29, #-48]
	adrp	x11, _rsIter@PAGE
	add	x11, x11, _rsIter@PAGEOFF
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp, #8]                    ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #20]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB5_4
	b	LBB5_3
LBB5_3:                                 ;   in Loop: Header=BB5_1 Depth=1
	ldr	x8, [sp, #8]                    ; 8-byte Folded Reload
	stur	x8, [x29, #-8]
	b	LBB5_4
LBB5_4:                                 ;   in Loop: Header=BB5_1 Depth=1
	ldr	w8, [sp, #20]                   ; 4-byte Folded Reload
	mov	w9, #1
	and	w8, w8, w9
	sturb	w8, [x29, #-49]
	ldurb	w8, [x29, #-49]
	tbz	w8, #0, LBB5_12
	b	LBB5_5
LBB5_5:                                 ;   in Loop: Header=BB5_1 Depth=1
	str	xzr, [sp, #64]
	b	LBB5_6
LBB5_6:                                 ;   Parent Loop BB5_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	x8, [sp, #64]
	ldur	x9, [x29, #-8]
	ldr	x9, [x9, #16]
	subs	x8, x8, x9
	b.hs	LBB5_11
	b	LBB5_7
LBB5_7:                                 ;   in Loop: Header=BB5_6 Depth=2
	ldur	x8, [x29, #-8]
	ldr	x8, [x8, #8]
	ldr	x9, [sp, #64]
	add	x8, x8, x9, lsl #3
	ldar	x8, [x8]
	str	x8, [sp, #48]
	ldr	x8, [sp, #48]
	str	x8, [sp, #56]
	ldr	x8, [sp, #56]
	cbz	x8, LBB5_9
	b	LBB5_8
LBB5_8:                                 ;   in Loop: Header=BB5_6 Depth=2
	ldr	x0, [sp, #56]
	bl	_c4_impl_pushWorkList
	b	LBB5_9
LBB5_9:                                 ;   in Loop: Header=BB5_6 Depth=2
	b	LBB5_10
LBB5_10:                                ;   in Loop: Header=BB5_6 Depth=2
	ldr	x8, [sp, #64]
	add	x8, x8, #1
	str	x8, [sp, #64]
	b	LBB5_6
LBB5_11:                                ;   in Loop: Header=BB5_1 Depth=1
	b	LBB5_12
LBB5_12:                                ;   in Loop: Header=BB5_1 Depth=1
	ldur	x9, [x29, #-8]
	strb	wzr, [sp, #47]
	ldrb	w8, [sp, #47]
	stlrb	w8, [x9]
	b	LBB5_1
LBB5_13:
	bl	_markWorkList
	mov	x8, #1
	str	x8, [sp, #32]
	ldr	x8, [sp, #32]
	adrp	x9, _completedCount@PAGE
	add	x9, x9, _completedCount@PAGEOFF
	ldaddal	x8, x8, [x9]
	str	x8, [sp, #24]
	ldp	x29, x30, [sp, #128]            ; 16-byte Folded Reload
	add	sp, sp, #144
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_c4_impl_gcThrdFunc             ; -- Begin function c4_impl_gcThrdFunc
	.p2align	2
_c4_impl_gcThrdFunc:                    ; @c4_impl_gcThrdFunc
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	b	LBB6_1
LBB6_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB6_2 Depth 2
                                        ;     Child Loop BB6_5 Depth 2
	bl	_mark
	b	LBB6_2
LBB6_2:                                 ;   Parent Loop BB6_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	adrp	x8, _gcCopyFlag@PAGE
	add	x8, x8, _gcCopyFlag@PAGEOFF
	ldarb	w8, [x8]
	sturb	w8, [x29, #-1]
	ldurb	w8, [x29, #-1]
	tbnz	w8, #0, LBB6_4
	b	LBB6_3
LBB6_3:                                 ;   in Loop: Header=BB6_2 Depth=2
	b	LBB6_2
LBB6_4:                                 ;   in Loop: Header=BB6_1 Depth=1
	bl	_copy
	b	LBB6_5
LBB6_5:                                 ;   Parent Loop BB6_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	adrp	x8, _remapFlag@PAGE
	add	x8, x8, _remapFlag@PAGEOFF
	ldarb	w8, [x8]
	sturb	w8, [x29, #-2]
	ldurb	w8, [x29, #-2]
	tbnz	w8, #0, LBB6_7
	b	LBB6_6
LBB6_6:                                 ;   in Loop: Header=BB6_5 Depth=2
	b	LBB6_5
LBB6_7:                                 ;   in Loop: Header=BB6_1 Depth=1
	bl	_remap
	b	LBB6_1
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function markWorkList
_markWorkList:                          ; @markWorkList
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #112
	stp	x29, x30, [sp, #96]             ; 16-byte Folded Spill
	add	x29, sp, #96
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
                                        ; kill: def $x8 killed $xzr
	stur	xzr, [x29, #-8]
	stur	xzr, [x29, #-16]
	b	LBB7_1
LBB7_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB7_6 Depth 2
	adrp	x8, _workListIter@PAGE
	add	x8, x8, _workListIter@PAGEOFF
	ldar	x8, [x8]
	stur	x8, [x29, #-24]
	ldur	x8, [x29, #-24]
	stur	x8, [x29, #-8]
	cbz	x8, LBB7_13
	b	LBB7_2
LBB7_2:                                 ;   in Loop: Header=BB7_1 Depth=1
	ldur	x8, [x29, #-8]
	add	x8, x8, #16
	ldar	x8, [x8]
	stur	x8, [x29, #-32]
	ldur	x8, [x29, #-32]
	stur	x8, [x29, #-16]
	ldur	x8, [x29, #-16]
	stur	x8, [x29, #-40]
	ldur	x9, [x29, #-8]
	ldur	x10, [x29, #-40]
	adrp	x11, _workListIter@PAGE
	add	x11, x11, _workListIter@PAGEOFF
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp, #8]                    ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #20]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB7_4
	b	LBB7_3
LBB7_3:                                 ;   in Loop: Header=BB7_1 Depth=1
	ldr	x8, [sp, #8]                    ; 8-byte Folded Reload
	stur	x8, [x29, #-8]
	b	LBB7_4
LBB7_4:                                 ;   in Loop: Header=BB7_1 Depth=1
	ldr	w8, [sp, #20]                   ; 4-byte Folded Reload
	mov	w9, #1
	and	w8, w8, w9
	sturb	w8, [x29, #-41]
	ldurb	w8, [x29, #-41]
	tbz	w8, #0, LBB7_12
	b	LBB7_5
LBB7_5:                                 ;   in Loop: Header=BB7_1 Depth=1
	str	xzr, [sp, #40]
	b	LBB7_6
LBB7_6:                                 ;   Parent Loop BB7_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	x8, [sp, #40]
	ldur	x9, [x29, #-8]
	ldr	x9, [x9, #32]
	subs	x8, x8, x9
	b.hs	LBB7_11
	b	LBB7_7
LBB7_7:                                 ;   in Loop: Header=BB7_6 Depth=2
	ldur	x8, [x29, #-8]
	add	x8, x8, #40
	ldr	x9, [sp, #40]
	add	x8, x8, x9, lsl #3
	ldar	x8, [x8]
	str	x8, [sp, #24]
	ldr	x8, [sp, #24]
	str	x8, [sp, #32]
	ldr	x8, [sp, #32]
	cbz	x8, LBB7_9
	b	LBB7_8
LBB7_8:                                 ;   in Loop: Header=BB7_6 Depth=2
	ldr	x0, [sp, #32]
	bl	_c4_impl_pushWorkList
	b	LBB7_9
LBB7_9:                                 ;   in Loop: Header=BB7_6 Depth=2
	b	LBB7_10
LBB7_10:                                ;   in Loop: Header=BB7_6 Depth=2
	ldr	x8, [sp, #40]
	add	x8, x8, #1
	str	x8, [sp, #40]
	b	LBB7_6
LBB7_11:                                ;   in Loop: Header=BB7_1 Depth=1
	b	LBB7_12
LBB7_12:                                ;   in Loop: Header=BB7_1 Depth=1
	b	LBB7_1
LBB7_13:
	ldp	x29, x30, [sp, #96]             ; 16-byte Folded Reload
	add	sp, sp, #112
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function copy
_copy:                                  ; @copy
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #128
	stp	x29, x30, [sp, #112]            ; 16-byte Folded Spill
	add	x29, sp, #112
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
                                        ; kill: def $x8 killed $xzr
	stur	xzr, [x29, #-8]
	stur	xzr, [x29, #-16]
	b	LBB8_1
LBB8_1:                                 ; =>This Inner Loop Header: Depth=1
	adrp	x8, _workListIter@PAGE
	add	x8, x8, _workListIter@PAGEOFF
	ldar	x8, [x8]
	stur	x8, [x29, #-24]
	ldur	x8, [x29, #-24]
	stur	x8, [x29, #-8]
	cbz	x8, LBB8_11
	b	LBB8_2
LBB8_2:                                 ;   in Loop: Header=BB8_1 Depth=1
	ldur	x8, [x29, #-8]
	add	x8, x8, #16
	ldar	x8, [x8]
	stur	x8, [x29, #-16]
	ldur	x8, [x29, #-16]
	stur	x8, [x29, #-32]
	ldur	x9, [x29, #-8]
	ldur	x10, [x29, #-32]
	adrp	x11, _workListIter@PAGE
	add	x11, x11, _workListIter@PAGEOFF
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp, #8]                    ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #20]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB8_4
	b	LBB8_3
LBB8_3:                                 ;   in Loop: Header=BB8_1 Depth=1
	ldr	x8, [sp, #8]                    ; 8-byte Folded Reload
	stur	x8, [x29, #-8]
	b	LBB8_4
LBB8_4:                                 ;   in Loop: Header=BB8_1 Depth=1
	ldr	w8, [sp, #20]                   ; 4-byte Folded Reload
	and	w8, w8, #0x1
	sturb	w8, [x29, #-33]
	ldurb	w8, [x29, #-33]
	tbnz	w8, #0, LBB8_6
	b	LBB8_5
LBB8_5:                                 ;   in Loop: Header=BB8_1 Depth=1
	b	LBB8_1
LBB8_6:                                 ;   in Loop: Header=BB8_1 Depth=1
	ldur	x9, [x29, #-8]
	sturb	wzr, [x29, #-34]
	ldurb	w8, [x29, #-34]
	mvn	w8, w8
	ldclralb	w8, w8, [x9]
	sturb	w8, [x29, #-35]
	ldurb	w8, [x29, #-35]
	tbnz	w8, #0, LBB8_8
	b	LBB8_7
LBB8_7:                                 ;   in Loop: Header=BB8_1 Depth=1
	b	LBB8_1
LBB8_8:                                 ;   in Loop: Header=BB8_1 Depth=1
	ldur	x8, [x29, #-8]
	ldr	x0, [x8, #24]
	ldur	x8, [x29, #-8]
	ldr	x1, [x8, #32]
	mov	w2, #0
	bl	_c4_impl_sbrk
	stur	x0, [x29, #-48]
	ldur	x8, [x29, #-48]
	cbnz	x8, LBB8_10
	b	LBB8_9
LBB8_9:
	mov	w0, #1
	bl	_exit
LBB8_10:                                ;   in Loop: Header=BB8_1 Depth=1
	ldur	x8, [x29, #-48]
	add	x0, x8, #40
	ldur	x8, [x29, #-8]
	add	x1, x8, #40
	ldur	x8, [x29, #-8]
	ldr	x2, [x8, #24]
	mov	x3, #-1
	bl	___memcpy_chk
	ldur	x8, [x29, #-8]
	ldr	x8, [x8, #24]
	add	x8, x8, #40
	str	x8, [sp, #56]
	ldr	x8, [sp, #56]
	adrp	x9, _copied@PAGE
	add	x9, x9, _copied@PAGEOFF
	ldaddal	x8, x8, [x9]
	str	x8, [sp, #48]
	ldur	x8, [x29, #-8]
	add	x9, x8, #8
	ldur	x8, [x29, #-48]
	str	x8, [sp, #40]
	ldr	x8, [sp, #40]
	stlr	x8, [x9]
	b	LBB8_1
LBB8_11:
	mov	x8, #1
	str	x8, [sp, #32]
	ldr	x8, [sp, #32]
	adrp	x9, _completedCount@PAGE
	add	x9, x9, _completedCount@PAGEOFF
	ldaddal	x8, x8, [x9]
	str	x8, [sp, #24]
	ldp	x29, x30, [sp, #112]            ; 16-byte Folded Reload
	add	sp, sp, #128
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function remap
_remap:                                 ; @remap
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #272
	stp	x28, x27, [sp, #240]            ; 16-byte Folded Spill
	stp	x29, x30, [sp, #256]            ; 16-byte Folded Spill
	add	x29, sp, #256
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w27, -24
	.cfi_offset w28, -32
                                        ; kill: def $x8 killed $xzr
	stur	xzr, [x29, #-24]
	stur	xzr, [x29, #-32]
	b	LBB9_1
LBB9_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB9_6 Depth 2
	adrp	x8, _rsIter@PAGE
	add	x8, x8, _rsIter@PAGEOFF
	ldar	x8, [x8]
	stur	x8, [x29, #-40]
	ldur	x8, [x29, #-40]
	stur	x8, [x29, #-24]
	cbz	x8, LBB9_19
	b	LBB9_2
LBB9_2:                                 ;   in Loop: Header=BB9_1 Depth=1
	ldur	x9, [x29, #-24]
	mov	w8, #1
	sturb	w8, [x29, #-41]
	ldurb	w8, [x29, #-41]
	stlrb	w8, [x9]
	ldur	x8, [x29, #-24]
	add	x8, x8, #24
	ldar	x8, [x8]
	stur	x8, [x29, #-56]
	ldur	x8, [x29, #-56]
	stur	x8, [x29, #-32]
	ldur	x8, [x29, #-32]
	stur	x8, [x29, #-64]
	ldur	x9, [x29, #-24]
	ldur	x10, [x29, #-64]
	adrp	x11, _rsIter@PAGE
	add	x11, x11, _rsIter@PAGEOFF
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp, #40]                   ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #52]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB9_4
	b	LBB9_3
LBB9_3:                                 ;   in Loop: Header=BB9_1 Depth=1
	ldr	x8, [sp, #40]                   ; 8-byte Folded Reload
	stur	x8, [x29, #-24]
	b	LBB9_4
LBB9_4:                                 ;   in Loop: Header=BB9_1 Depth=1
	ldr	w8, [sp, #52]                   ; 4-byte Folded Reload
	mov	w9, #1
	and	w8, w8, w9
	sturb	w8, [x29, #-65]
	ldurb	w8, [x29, #-65]
	tbz	w8, #0, LBB9_18
	b	LBB9_5
LBB9_5:                                 ;   in Loop: Header=BB9_1 Depth=1
	stur	xzr, [x29, #-80]
	b	LBB9_6
LBB9_6:                                 ;   Parent Loop BB9_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldur	x8, [x29, #-80]
	ldur	x9, [x29, #-24]
	ldr	x9, [x9, #16]
	subs	x8, x8, x9
	b.hs	LBB9_17
	b	LBB9_7
LBB9_7:                                 ;   in Loop: Header=BB9_6 Depth=2
	ldur	x8, [x29, #-24]
	ldr	x8, [x8, #8]
	ldur	x9, [x29, #-80]
	add	x8, x8, x9, lsl #3
	ldar	x8, [x8]
	stur	x8, [x29, #-96]
	ldur	x8, [x29, #-96]
	stur	x8, [x29, #-88]
	ldur	x8, [x29, #-88]
	cbz	x8, LBB9_15
	b	LBB9_8
LBB9_8:                                 ;   in Loop: Header=BB9_6 Depth=2
	ldur	x8, [x29, #-88]
	add	x8, x8, #8
	ldar	x8, [x8]
	cbnz	x8, LBB9_10
	b	LBB9_9
LBB9_9:                                 ;   in Loop: Header=BB9_6 Depth=2
	b	LBB9_16
LBB9_10:                                ;   in Loop: Header=BB9_6 Depth=2
	ldur	x8, [x29, #-24]
	ldr	x8, [x8, #8]
	ldur	x9, [x29, #-80]
	add	x11, x8, x9, lsl #3
	ldur	x8, [x29, #-88]
	add	x8, x8, #8
	ldar	x8, [x8]
	stur	x8, [x29, #-104]
	ldur	x9, [x29, #-88]
	ldur	x10, [x29, #-104]
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp, #24]                   ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #36]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB9_12
	b	LBB9_11
LBB9_11:                                ;   in Loop: Header=BB9_6 Depth=2
	ldr	x8, [sp, #24]                   ; 8-byte Folded Reload
	stur	x8, [x29, #-88]
	b	LBB9_12
LBB9_12:                                ;   in Loop: Header=BB9_6 Depth=2
	ldr	w8, [sp, #36]                   ; 4-byte Folded Reload
	and	w8, w8, #0x1
	sturb	w8, [x29, #-105]
	ldurb	w8, [x29, #-105]
	tbnz	w8, #0, LBB9_14
	b	LBB9_13
LBB9_13:                                ;   in Loop: Header=BB9_6 Depth=2
	ldur	x8, [x29, #-80]
	subs	x8, x8, #1
	stur	x8, [x29, #-80]
	b	LBB9_16
LBB9_14:                                ;   in Loop: Header=BB9_6 Depth=2
	ldur	x0, [x29, #-88]
	bl	_pushRemapList
	b	LBB9_15
LBB9_15:                                ;   in Loop: Header=BB9_6 Depth=2
	b	LBB9_16
LBB9_16:                                ;   in Loop: Header=BB9_6 Depth=2
	ldur	x8, [x29, #-80]
	add	x8, x8, #1
	stur	x8, [x29, #-80]
	b	LBB9_6
LBB9_17:                                ;   in Loop: Header=BB9_1 Depth=1
	b	LBB9_18
LBB9_18:                                ;   in Loop: Header=BB9_1 Depth=1
	ldur	x9, [x29, #-24]
	sturb	wzr, [x29, #-106]
	ldurb	w8, [x29, #-106]
	stlrb	w8, [x9]
	b	LBB9_1
LBB9_19:
	mov	x8, #1
	stur	x8, [x29, #-120]
	ldur	x8, [x29, #-120]
	adrp	x9, _completedCount@PAGE
	add	x9, x9, _completedCount@PAGEOFF
	ldaddal	x8, x8, [x9]
	str	x8, [sp, #128]
                                        ; kill: def $x8 killed $xzr
	str	xzr, [sp, #120]
	str	xzr, [sp, #112]
	b	LBB9_20
LBB9_20:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB9_25 Depth 2
	adrp	x8, _remapListIter@PAGE
	add	x8, x8, _remapListIter@PAGEOFF
	ldar	x8, [x8]
	str	x8, [sp, #104]
	ldr	x8, [sp, #104]
	str	x8, [sp, #120]
	cbz	x8, LBB9_34
	b	LBB9_21
LBB9_21:                                ;   in Loop: Header=BB9_20 Depth=1
	ldr	x8, [sp, #120]
	add	x8, x8, #16
	ldar	x8, [x8]
	str	x8, [sp, #112]
	ldr	x8, [sp, #112]
	str	x8, [sp, #96]
	ldr	x9, [sp, #120]
	ldr	x10, [sp, #96]
	adrp	x11, _remapListIter@PAGE
	add	x11, x11, _remapListIter@PAGEOFF
	mov	x8, x9
	casal	x8, x10, [x11]
	str	x8, [sp, #8]                    ; 8-byte Folded Spill
	subs	x10, x8, x9
	cset	w10, eq
	str	w10, [sp, #20]                  ; 4-byte Folded Spill
	subs	x8, x8, x9
	b.eq	LBB9_23
	b	LBB9_22
LBB9_22:                                ;   in Loop: Header=BB9_20 Depth=1
	ldr	x8, [sp, #8]                    ; 8-byte Folded Reload
	str	x8, [sp, #120]
	b	LBB9_23
LBB9_23:                                ;   in Loop: Header=BB9_20 Depth=1
	ldr	w8, [sp, #20]                   ; 4-byte Folded Reload
	mov	w9, #1
	and	w8, w8, w9
	strb	w8, [sp, #95]
	ldrb	w8, [sp, #95]
	tbz	w8, #0, LBB9_33
	b	LBB9_24
LBB9_24:                                ;   in Loop: Header=BB9_20 Depth=1
	str	xzr, [sp, #80]
	b	LBB9_25
LBB9_25:                                ;   Parent Loop BB9_20 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	x8, [sp, #80]
	ldr	x9, [sp, #120]
	ldr	x9, [x9, #32]
	subs	x8, x8, x9
	b.hs	LBB9_32
	b	LBB9_26
LBB9_26:                                ;   in Loop: Header=BB9_25 Depth=2
	ldr	x8, [sp, #120]
	add	x8, x8, #40
	ldr	x9, [sp, #80]
	add	x8, x8, x9, lsl #3
	ldar	x8, [x8]
	str	x8, [sp, #64]
	ldr	x8, [sp, #64]
	str	x8, [sp, #72]
	ldr	x8, [sp, #72]
	cbz	x8, LBB9_30
	b	LBB9_27
LBB9_27:                                ;   in Loop: Header=BB9_25 Depth=2
	ldr	x8, [sp, #72]
	add	x8, x8, #8
	ldar	x8, [x8]
	cbnz	x8, LBB9_29
	b	LBB9_28
LBB9_28:                                ;   in Loop: Header=BB9_25 Depth=2
	b	LBB9_31
LBB9_29:                                ;   in Loop: Header=BB9_25 Depth=2
	ldr	x8, [sp, #120]
	add	x8, x8, #40
	ldr	x9, [sp, #80]
	add	x9, x8, x9, lsl #3
	ldr	x8, [sp, #72]
	add	x8, x8, #8
	ldar	x8, [x8]
	str	x8, [sp, #56]
	ldr	x8, [sp, #56]
	stlr	x8, [x9]
	b	LBB9_30
LBB9_30:                                ;   in Loop: Header=BB9_25 Depth=2
	b	LBB9_31
LBB9_31:                                ;   in Loop: Header=BB9_25 Depth=2
	ldr	x8, [sp, #80]
	add	x8, x8, #1
	str	x8, [sp, #80]
	b	LBB9_25
LBB9_32:                                ;   in Loop: Header=BB9_20 Depth=1
	b	LBB9_33
LBB9_33:                                ;   in Loop: Header=BB9_20 Depth=1
	b	LBB9_20
LBB9_34:
	ldp	x29, x30, [sp, #256]            ; 16-byte Folded Reload
	ldp	x28, x27, [sp, #240]            ; 16-byte Folded Reload
	add	sp, sp, #272
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_gcCopyFlag                     ; @gcCopyFlag
.zerofill __DATA,__common,_gcCopyFlag,1,0
.zerofill __DATA,__bss,_routine,8,3     ; @routine
.zerofill __DATA,__bss,_completedCount,8,3 ; @completedCount
.zerofill __DATA,__bss,_workListBegin,8,3 ; @workListBegin
.zerofill __DATA,__bss,_workListIter,8,3 ; @workListIter
.zerofill __DATA,__bss,_rsIter,8,3      ; @rsIter
.zerofill __DATA,__bss,_copied,8,3      ; @copied
.zerofill __DATA,__bss,_remapListEnd,8,3 ; @remapListEnd
.zerofill __DATA,__bss,_workListEnd,8,3 ; @workListEnd
.zerofill __DATA,__bss,_remapFlag,1,0   ; @remapFlag
.zerofill __DATA,__bss,_remapListIter,8,3 ; @remapListIter
.subsections_via_symbols
