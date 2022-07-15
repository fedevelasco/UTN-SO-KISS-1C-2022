#ifndef OP_CODE_H_
#define OP_CODE_H_

typedef enum
{
	INSTRUCTIONS,
	EXIT,
	UNDEFINED,
	PROCESS_INIT,
	PROCESS_SUSPEND,
	PROCESS_KILL,
	GET_SECOND_LEVEL_TABLE,
	GET_FRAME,
	READ_MEMORY,
	WRITE_MEMORY,
	DEBUG
}op_code;


#endif

