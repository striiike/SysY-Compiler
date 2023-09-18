//
// Created by hiccup on 2023/9/13.
//
#include <stdio.h>
#include "libsysy/libsysy.h"


// ConstDef, ConstExp
const int constA = 1;
const int constC = 3, constD2 = 5;
const int constE[1 * 1 + 1] = {234, 8746};
const int constF[1] = {constA};
const int constG[1 * 2 - 0] = {constA + constD2 * constC, 3};
const int constH[1 * 2 - 0][2] = {{0, 24},
                                  {1, constC}};
const int constB = constA + 234;

// VarDef
int var;
int varArray[2];
int varArrr[234][43];
int varC = 3, var2 = (5 + 5);
int varE[1 * 1 + 1] = {234, 8746};
int varF[1 + 1] = {constA,
                   24 * constA};
int varG[1 * 2 - 0] = {43 - constD2 / constC, 3};
int varH[1 * 2 - 0][2] = {{0, 24},
                          {1, constC}};

void printInfo() {
    int a;

    a = getint();
    printf("21373275\n");
    printf("genshin\n");
    printf("genshin\n");
    printf("genshin\n");
    printf("genshin\n");
    printf("genshin\n");
    printf("genshin\n");
    printf("genshin\n");
    printf("genshin\n");
    return;
}

int get_num(int a) {
    return a;
}

// FuncFParam
int func(int a, int b[], int c[][2], int d[][2], int e[], int f) {
    int tmp;
    tmp = a;
    if (tmp > 1 && tmp < 2 && tmp >= a + 1 && tmp <= a && tmp == a + 23) {
        tmp = func(a, b, c, d, e, f) + 1;

    } else { ;
    }
    if (tmp > 1 && tmp < 2 || tmp >= 0 && tmp <= 4 && tmp != 123) { ;
    } else {
    }
    if (tmp > 1 && tmp < 2 || tmp >= 0 && tmp <= 4 && tmp != 123) { ;

    } else if (!tmp && - +tmp || !!tmp) { ;
    }
    if (tmp == 1) { ;
    }
    int i = 0;
    for (i = 0; i <= 342; i = i + 1) {
        if (i) {
            continue;
        } else {
            continue;
        }
        continue;
        i;


    }
    for (i = i + 1; i || !i; i = i % i) {
        break;
    }
    for (i = i + 1;;) break;
    for (; i || !i; i = i + 1) break;
    for (i = i + 1; i || !i;) break;
    for (i = i + 1;; i = i + 1) break;
    for (;; i = i + 1) break;
    for (;;) break;
    for (; i || !i;) break;
    return 234;
}

int main() {
    const int constMainA[2] = {2, 5};
    int w = constMainA[0] + 23;
    int array_b[2] = {1, 1};
    int array_c[2][2] = {{1, 1},
                         {1, 1}};
    int array_d[2][2] = {{1, 1},
                         {1, 1}};
    printInfo();
    int a;
    a = 0 * 0;
    printf("a=%d", a + 0 * 0);
    int varA = - + -1321;
    {{}}
    {
        { ;;;
        }
    };
    varA = get_num(varA);
    func(array_c[0][0], array_b, array_c, array_d, array_d[1], - +varA);
    return 0;
}


