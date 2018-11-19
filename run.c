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
			// printf("Im addu\n");
			//R[rd] = R[rs] + R[rt]
			cpu->REGS[rd] = cpu->REGS[rs] + cpu->REGS[rt];
		}
		else if(funct==0x24){	//and
			// printf("Im and\n");
			//R[rd] = R[rs] & R[rt]
			cpu->REGS[rd] = cpu->REGS[rs] & cpu->REGS[rt];
		}
		else if(funct==0x8){	//jr
			// printf("Im jr\n");
			//PC=R[rs]
			CURRENT_STATE.PC = cpu->REGS[rs];
		}
		else if(funct==0x27){	//nor
			// printf("Im nor\n");
			//R[rd] = ~ (R[rs] | R[rt])
			cpu->REGS[rd] = ~(cpu->REGS[rs] | cpu->REGS[rt]);
		}
		else if(funct==0x25){	//or
			// printf("Im or\n");
			//R[rd] = R[rs] | R[rt]
			cpu->REGS[rd] = cpu->REGS[rs] | cpu->REGS[rt];
		}
		else if(funct==0x2b){	//sltu
			// printf("Im sltu\n");
			//R[rd] = (R[rs] < R[rt]) ? 1 : 0
			cpu->REGS[rd] = (cpu->REGS[rs] < cpu->REGS[rt]) ? 1:0;
		}
		else if(funct==0x0){	//sll
			// printf("Im sll\n");
			//R[rd] = R[rt] << shamt
			cpu->REGS[rd] = cpu->REGS[rt] << shamt;
		}
		else if(funct==0x2){	//srl
			// printf("Im srl\n");
			//R[rd] = R[rt] >> shamt
			cpu->REGS[rd] = cpu->REGS[rt] >> shamt;
		}
		else if(funct==0x23){	//subu
			// printf("Im subu\n");
			//R[rd] = R[rs] - R[rt]
			cpu->REGS[rd] = cpu->REGS[rs] - cpu->REGS[rt];
		}
		else{
			printf("R type: exception occured.\n");
		}

	}
	else if(opcode==2 || opcode==3){ // J type
		target = 4*TARGET(instr);
		if(opcode==2){		//	j
			// printf("Im j\n");
			//PC=JumpAddr
			CURRENT_STATE.PC = target;
		}
		else if(opcode==3){	// jal
			// printf("Im jal\n");
			//R[31]=PC+8;PC=JumpAddr
			cpu->REGS[31] = CURRENT_STATE.PC + 4;
			CURRENT_STATE.PC = target;
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
			// printf("Im addiu\n");
			//R[rt] = R[rs] + SignExtImm
			cpu->REGS[rt] = cpu->REGS[rs] + SIGN_EX(imm);
		}			
		else if(opcode==0xc){	//	andi
			// printf("Im andi\n");
			//R[rt] = R[rs] & ZeroExtImm
			cpu->REGS[rt] = cpu->REGS[rs] & (imm | 0x00000000);
		}
		else if(opcode==0x4){	//	beq
			// printf("Im beq\n");
			//if(R[rs]==R[rt]) PC=PC+4+BranchAddr    //BranchAddr: { 14{immediate[15]}, immediate, 2’b0 }
			if(cpu->REGS[rs] == cpu->REGS[rt]){
				CURRENT_STATE.PC += (SIGN_EX(imm)<<2);
			}
			// printf("0x%08x\n", CURRENT_STATE.PC);
		}
		else if(opcode==0x5){	//	bne
			// printf("Im bne\n");
			//if(R[rs]!=R[rt]) PC=PC+4+BranchAddr
			if(cpu->REGS[rs] != cpu->REGS[rt]){
				CURRENT_STATE.PC += (SIGN_EX(imm)<<2);
			}
		}
		else if(opcode==0xf){	//	lui
			// printf("Im lui\n");
			//R[rt] = {imm, 16’b0}
			cpu->REGS[rt] = imm<<16;
		}
		else if(opcode==0x23){	//	lw
			// printf("Im lw\n");
			//R[rt] = M[R[rs]+SignExtImm]

			//uint32_t mem_read_32(uint32_t address)

			cpu->REGS[rt] = mem_read_32(cpu->REGS[rs] + SIGN_EX(imm));
		}
		else if(opcode==0xd){	//	ori
			// printf("Im ori\n");
			//R[rt] = R[rs] | ZeroExtImm
			cpu->REGS[rt] = cpu->REGS[rs] | (imm | 0x00000000);
		}
		else if(opcode==0xb){	//	sltiu
			// printf("Im sltiu\n");
			//R[rt] = (R[rs] < SignExtImm) ? 1 : 0
			cpu->REGS[rt] = (cpu->REGS[rs] < SIGN_EX(imm)) ? 1:0;
		}
		else if(opcode==0x2b){	//	sw
			// printf("Im sw\n");
			// M[R[rs]+SignExtImm] = R[rt]
			// void mem_write_32(uint32_t address, uint32_t value)
			mem_write_32(cpu->REGS[rs] + SIGN_EX(imm), cpu->REGS[rt]);
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
