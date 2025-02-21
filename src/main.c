#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define BM_STACK_CAPACITY 1

typedef enum {
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_ILLEGAL_INST,
} Err;

const char *err_as_cstr(Err err)
{
    switch (err) {
    case ERR_OK:
        return "ERR_OK";
    case ERR_STACK_OVERFLOW:
        return "ERR_STACK_OVERFLOW";
    case ERR_STACK_UNDERFLOW:
        return "ERR_STACK_UNDERFLOW";
    case ERR_ILLEGAL_INST:
        return "ERR_ILLEGAL_INST";
    default:
        assert(0 && "err_as_cstr: Unreachable");
    }
}

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

#define MAKE_INST_PUSH(value) ((Inst) {.type = INST_PUSH, .operand = (value)})
#define MAKE_INST_PLUS (Inst) {.type = INST_PLUS}

Err bm_execute_inst(Bm *bm, Inst inst)
{
    switch(inst.type) {
    case INST_PUSH:
        if(bm->stack_size >= BM_STACK_CAPACITY) {
            return ERR_STACK_OVERFLOW;
        }
        bm->stack[bm->stack_size++] = inst.operand;
        break;

    case INST_PLUS:
        if (bm->stack_size < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        bm->stack[bm->stack_size - 2] += bm->stack[bm->stack_size -1];
        bm->stack_size -= 1;
        break;

    default:
        return ERR_ILLEGAL_INST;
    }
    return ERR_OK;
}

void bm_dump(FILE *stream, const Bm *bm)
{
    fprintf(stream, "Stack:\n");
    if (bm->stack_size > 0) {
        for (size_t i = 0; i < bm->stack_size; ++i) {
            fprintf(stream, "  %ld\n", bm->stack[i]);
        }
    } else {
        fprintf(stream, "  [empty]\n");
    }
}

#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs[0])))

Bm bm = {0};

Inst program[] = {
    //MAKE_INST_PUSH(69),
    //MAKE_INST_PUSH(420),
    MAKE_INST_PLUS,
};

int main()
{
    bm_dump(stdout, &bm);
    for (size_t i = 0; i < ARRAY_SIZE(program); ++i) {
        Err err = bm_execute_inst(&bm, program[i]);
        if (err != ERR_OK) {
            fprintf(stderr, "Error activated: %s\n", err_as_cstr(err));
            bm_dump(stderr, &bm);
            exit(1);
        }
    }
    bm_dump(stdout, &bm);
    return 0;
}
