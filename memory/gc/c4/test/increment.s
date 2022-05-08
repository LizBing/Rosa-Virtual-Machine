	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 12, 3
	.globl	_test01                         ; -- Begin function test01
	.p2align	2
_test01:                                ; @test01
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	x0, #1
	bl	_thrdPoolInit
	mov	x0, #16777216
	str	x0, [sp, #16]                   ; 8-byte Folded Spill
	bl	_malloc
	ldr	x2, [sp, #16]                   ; 8-byte Folded Reload
	mov	x1, x0
	mov	x0, #2
	bl	_c4_init
	cbz	w0, LBB0_2
	b	LBB0_1
LBB0_1:
	mov	w0, #2
	bl	_exit
LBB0_2:
	stur	wzr, [x29, #-4]
	b	LBB0_3
LBB0_3:                                 ; =>This Inner Loop Header: Depth=1
	ldur	w8, [x29, #-4]
	mov	w9, #10000
	subs	w8, w8, w9
	b.ge	LBB0_6
	b	LBB0_4
LBB0_4:                                 ;   in Loop: Header=BB0_3 Depth=1
	mov	x0, #128
	mov	x1, #0
	bl	_c4_malloc
	mov	x8, x0
	adrp	x0, l_.str@PAGE
	add	x0, x0, l_.str@PAGEOFF
	mov	x9, sp
	str	x8, [x9]
	bl	_printf
	b	LBB0_5
LBB0_5:                                 ;   in Loop: Header=BB0_3 Depth=1
	ldur	w8, [x29, #-4]
	add	w8, w8, #1
	stur	w8, [x29, #-4]
	b	LBB0_3
LBB0_6:
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_test02                         ; -- Begin function test02
	.p2align	2
_test02:                                ; @test02
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #192
	stp	x29, x30, [sp, #176]            ; 16-byte Folded Spill
	add	x29, sp, #176
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	mov	x0, #1
	bl	_thrdPoolInit
	mov	x0, #16777216
	str	x0, [sp, #24]                   ; 8-byte Folded Spill
	bl	_malloc
	ldr	x2, [sp, #24]                   ; 8-byte Folded Reload
	mov	x1, x0
	mov	x0, #2
	bl	_c4_init
	cbz	w0, LBB1_2
	b	LBB1_1
LBB1_1:
	mov	w0, #2
	bl	_exit
LBB1_2:
	mov	x0, #0
	bl	_c4_getRootSet
	str	x0, [sp, #80]
	add	x0, sp, #88
	str	x0, [sp, #16]                   ; 8-byte Folded Spill
	mov	w1, #0
	mov	x2, #80
	bl	_memset
	mov	x0, #32
	bl	_malloc
	ldr	x2, [sp, #16]                   ; 8-byte Folded Reload
	ldr	x1, [sp, #80]
	mov	x3, #10
	bl	_c4_pushRootSet
	str	x0, [sp, #72]
	str	wzr, [sp, #68]
	b	LBB1_3
LBB1_3:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB1_5 Depth 2
                                        ;       Child Loop BB1_7 Depth 3
	ldr	w8, [sp, #68]
	subs	w8, w8, #10
	b.ge	LBB1_14
	b	LBB1_4
LBB1_4:                                 ;   in Loop: Header=BB1_3 Depth=1
	str	wzr, [sp, #64]
	b	LBB1_5
LBB1_5:                                 ;   Parent Loop BB1_3 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB1_7 Depth 3
	ldr	w8, [sp, #64]
	subs	w8, w8, #10
	b.ge	LBB1_12
	b	LBB1_6
LBB1_6:                                 ;   in Loop: Header=BB1_5 Depth=2
	ldrsw	x9, [sp, #64]
	add	x8, sp, #88
	add	x8, x8, x9, lsl #3
	str	x8, [sp, #56]
	ldr	x8, [sp, #56]
	str	x8, [sp, #8]                    ; 8-byte Folded Spill
	mov	x0, #128
	mov	x1, #10
	bl	_c4_malloc
	ldr	x9, [sp, #8]                    ; 8-byte Folded Reload
	str	x0, [sp, #48]
	ldr	x8, [sp, #48]
	stlr	x8, [x9]
	str	wzr, [sp, #44]
	b	LBB1_7
LBB1_7:                                 ;   Parent Loop BB1_3 Depth=1
                                        ;     Parent Loop BB1_5 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldr	w8, [sp, #44]
	subs	w8, w8, #10
	b.ge	LBB1_10
	b	LBB1_8
LBB1_8:                                 ;   in Loop: Header=BB1_7 Depth=3
	ldr	x0, [sp, #56]
	ldrsw	x1, [sp, #44]
	bl	_c4_loadRefAddr
	str	x0, [sp]                        ; 8-byte Folded Spill
	mov	x0, #128
	mov	x1, #0
	bl	_c4_malloc
	mov	x8, x0
	ldr	x0, [sp]                        ; 8-byte Folded Reload
	str	x8, [sp, #32]
	ldr	x8, [sp, #32]
	stlr	x8, [x0]
	b	LBB1_9
LBB1_9:                                 ;   in Loop: Header=BB1_7 Depth=3
	ldr	w8, [sp, #44]
	add	w8, w8, #1
	str	w8, [sp, #44]
	b	LBB1_7
LBB1_10:                                ;   in Loop: Header=BB1_5 Depth=2
	b	LBB1_11
LBB1_11:                                ;   in Loop: Header=BB1_5 Depth=2
	ldr	w8, [sp, #64]
	add	w8, w8, #1
	str	w8, [sp, #64]
	b	LBB1_5
LBB1_12:                                ;   in Loop: Header=BB1_3 Depth=1
	b	LBB1_13
LBB1_13:                                ;   in Loop: Header=BB1_3 Depth=1
	ldr	w8, [sp, #68]
	add	w8, w8, #1
	str	w8, [sp, #68]
	b	LBB1_3
LBB1_14:
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	ldur	x9, [x29, #-8]
	subs	x8, x8, x9
	b.ne	LBB1_16
	b	LBB1_15
LBB1_15:
	ldp	x29, x30, [sp, #176]            ; 16-byte Folded Reload
	add	sp, sp, #192
	ret
LBB1_16:
	bl	___stack_chk_fail
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, #0
	stur	w8, [x29, #-8]                  ; 4-byte Folded Spill
	stur	wzr, [x29, #-4]
	bl	_test02
	mov	w0, #500
	bl	_usleep
	bl	_c4_allocated
	str	x0, [sp, #24]                   ; 8-byte Folded Spill
	bl	_c4_collected
	stur	x0, [x29, #-16]                 ; 8-byte Folded Spill
	bl	_c4_gcRoutine
	ldr	x11, [sp, #24]                  ; 8-byte Folded Reload
	ldur	x10, [x29, #-16]                ; 8-byte Folded Reload
	mov	x8, x0
	adrp	x0, l_.str.1@PAGE
	add	x0, x0, l_.str.1@PAGEOFF
	mov	x9, sp
	str	x11, [x9]
	str	x10, [x9, #8]
	str	x8, [x9, #16]
	bl	_printf
	ldur	w0, [x29, #-8]                  ; 4-byte Folded Reload
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"%p\n"

l_.str.1:                               ; @.str.1
	.asciz	"\nallocated: %zu, collected: %zu\ncompleted routine: %zu\n"

.subsections_via_symbols
