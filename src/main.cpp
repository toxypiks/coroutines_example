#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define BM_STACK_CAPACITY 1024

typedef enum {
    TRAP_OK = 0,
    TRAP_STACK_OVERFLOW,
    TRAP_STACK_UNDERFLOW,
    TRAP_ILLEGAL_INST,
} Trap;

typedef int64_t Word;

typedef struct {
    Word stack[BM_STACK_CAPACITY];
    size_t stack_size;
} Bm;

typedef enum {
    INST_PUSH,
    INST_PLUS,
} Inst_Type;

typedef struct {
    Inst_Type type;
    Word operand;
} Inst;

Inst inst_push(Word operand)
{
    return (Inst) {
        .type = INST_PUSH,
        .operand = operand,
    };
}

Inst inst_plus(void)
{
    return (Inst) {.type = INST_PLUS};
}

Trap bm_execute_inst(Bm *bm, Inst inst)
{
    switch(inst.type) {
    case INST_PUSH:
        if(bm->stack_size >= BM_STACK_CAPACITY) {
            return TRAP_STACK_OVERFLOW;
        }
        bm->stack[bm->stack_size++] = inst.operand;
        break;

    case INST_PLUS:
        if (bm->stack_size < 2) {
            return TRAP_STACK_UNDERFLOW;
        }
        bm->stack[bm->stack_size - 2] += bm->stack[bm->stack_size -1];
        bm->stack_size -= 1;
        break;

    default:
        return TRAP_ILLEGAL_INST;
    }
    return TRAP_OK;
}

void bm_dump(const Bm *bm)
{
    printf("Stack:\n");
    if (bm->stack_size > 0) {
        for (size_t i = 0; i < bm->stack_size; ++i) {
            printf("  %ld\n", bm->stack[i]);
        }
    } else {
        printf("  [empty]\n");
    }
}

Bm bm = {0};

int main()
{
    bm_dump(&bm);
    bm_execute_inst(&bm, inst_push(69));
    bm_dump(&bm);
    bm_execute_inst(&bm, inst_push(420));
    bm_dump(&bm);
    bm_execute_inst(&bm, inst_plus());
    bm_dump(&bm);
    return 0;
}
