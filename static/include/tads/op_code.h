#ifndef OP_CODE_H_
#define OP_CODE_H_

typedef enum
{
	//Console - Kernel
	INSTRUCTIONS,
	//EXIT,
	UNDEFINED,

	//Kernel - CPU
	PCB,
	EXECUTE_PROCESS,
	INTERRUPT_PROCESS,
	END_PROCESS


}op_code;


#endif

