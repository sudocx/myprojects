	AREA demo, CODE, READONLY
	
	import mymain
	import dma_handler
	ENTRY
	
	b reset_handler
	nop
	nop
	nop
	nop
	nop
	b irq_handler
	nop
	
reset_handler	
	;change to irq mode
	mov r0, #0xD2
	msr cpsr_cxsf,r0 
	mov sp,#0xe00
	
	;old code
	mov r0,#0x53
	msr cpsr_cxsf,r0
	mov sp, #0x1000
	
	b mymain
	
	
irq_handler	
	;ldr r0,=0x20800000
	;mov r1, #0xff
	;strb r1,[r0]
	b dma_handler

	END
	