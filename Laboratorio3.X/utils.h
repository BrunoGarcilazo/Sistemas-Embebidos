#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H

#define UT_DELAY_CYCLES 50000





/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define EXAMPLE_CONSTANT 0


    int ExampleFunction(int param1, int param2);
    /*
     * Esta funcion decrementa un contador de interrupciones del timer dos.
     * De esta forma contamos milisegundos.
     */
    void DecrementarContador(void);

    void UT_delayms(int multiplo);




    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
