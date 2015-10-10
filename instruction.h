//
// Created by Peter on 10/10/2015.
//

#ifndef IFJ_INSTRUCTION_H
#define IFJ_INSTRUCTION_H

typedef enum {

} INSTRUCTION;

typedef int Symbol; // TODO: change symbol to specific struct from ast

typedef struct Instruction {
		INSTRUCTION type;
		Symbol *source1;
		Symbol *source2;
		void *destination;

		struct Instruction *next;
} Instruction;

typedef struct {
		Instruction *first;
		Instruction *last;
} InstructionSet;

#endif //IFJ_INSTRUCTION_H
