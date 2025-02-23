#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void bar()
{
    longjmp(env, 1);
    printf("bar\n");
}

void foo()
{
    printf("foo\n");
    bar();
}

int main()
{
    if (!setjmp(env)) {
        foo();
    } else {
        printf("Jumped back\n");
    }
    return 0;
}
