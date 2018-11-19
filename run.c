/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   run.c                                                     */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "run.h"

/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc)
{
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}

/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/
void process_instruction()
{	
	instruction *instr = get_inst_info(CURRENT_STATE.PC);
	CPU_State *cpu = &CURRENT_STATE;
	CURRENT_STATE.PC += 4;	


	// cpu->REGS[0]++;
	// printf("%d\n", cpu->REGS[0]);

    short opcode = OPCODE(instr);
	unsigned char rs;
	unsigned char rt;
	short imm;
	unsigned char rd;
	unsigned char shamt;
    uint32_t target;
    short funct;
    uint32_t value; // ?????

    /*{"addiu","001001",'i'},{"addu","000000",'r',"100001"},{"and","000000",'r',"100100"},{"andi","001100",'i'},{"beq","000100",'i'},{"bne","000101",'i'},{"j","000010",'j'},
        {"jal","000011",'j'},{"jr","000000",'r',"001000"},{"lui","001111",'i'},{"lw","100011",'i'},{"la","111111",'i',"001101"},{"nor","000000",'r',"100111"},{"or","000000",'r',"100101"},
        {"ori","001101",'i'},{"sltiu","001011",'i'},{"sltu","000000",'r',"101011"},{"sll","000000",'r',"000000"},{"srl","000000",'r',"000010"},{"sw","101011",'i'},{"subu","000000",'r',"100011"}
	*/
	if(opcode == 0){ 	 // R type	
		rs=RS(instr);
		rt=RT(instr);
		rd=RD(instr);
		shamt=SHAMT(instr);
		funct=FUNC(instr);

		if(funct==0x21){	// addu
			printf("Im addu\n");
		}
		else if(funct==0x24){	//and
			printf("Im and\n");
		}
		else if(funct==0x8){	//jr
			printf("Im jr\n");
		}
		else if(funct==0x27){	//nor
			printf("Im nor\n");
		}
		else if(funct==0x25){	//or
			printf("Im or\n");
		}
		else if(funct==0x2b){	//sltu
			printf("Im sltu\n");
		}
		else if(funct==0x0){	//sll
			printf("Im sll\n");
		}
		else if(funct==0x2){	//srl
			printf("Im srl\n");
		}
		else if(funct==0x23){	//subu
			printf("Im subu\n");
		}
		else{
			printf("R type: exception occured.\n");
		}

	}
	else if(opcode==2 || opcode==3){ // J type
		target = TARGET(instr);
		
		if(opcode==2){		//	j
			printf("Im j\n");
		}
		else if(opcode==3){	// jal
			printf("Im jal\n");
		}
		else{
			printf("J type: exception occured.\n");
		}
	}
	else{				// I Type
		rs=RS(instr);
		rt=RT(instr);
		imm=IMM(instr);	

		if(opcode==0x9){	//	addiu
			printf("Im addiu\n");
		}			
		else if(opcode==0xc){	//	andi
			printf("Im andi\n");
		}
		else if(opcode==0x4){	//	beq
			printf("Im beq\n");
		}
		else if(opcode==0x5){	//	bne
			printf("Im bne\n");
		}
		else if(opcode==0xf){	//	lui
			printf("Im lui\n");
		}
		else if(opcode==0x23){	//	lw
			printf("Im lw\n");
		}
		else if(opcode==0xd){	//	ori
			printf("Im ori\n");
		}
		else if(opcode==0xb){	//	sltiu
			printf("Im sltiu\n");
		}
		else if(opcode==0x2b){	//	sw
			printf("Im sw\n");
		}
		else{
			printf("I type: exception occured.\n");
		}
	}




// ===================do not touch the below=========================


	if((CURRENT_STATE.PC - MEM_TEXT_START == 4*NUM_INST)){ 
		RUN_BIT=FALSE;
	}
}
