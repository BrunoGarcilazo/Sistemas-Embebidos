#include <stdio.h>
#include <stdbool.h>

#define  BTN1Flag 0
#define  BTN2Flag 0


#define BTN1_Set1()      (BTN1Flag = 1)
#define BTN1_Reset1()    (BTN1Flag = 0)
#define BTN1_GetValue1() (BTN1Flag)

#define BTN2_Set2()      (BTN2Flag = 1)
#define BTN2_Reset2()    (BTN2Flag = 0)
#define BTN2_GetValue2() (BTN2Flag)
