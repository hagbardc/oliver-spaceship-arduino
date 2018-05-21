#ifndef INCLUDED_COMPONENT_KEYBOARD_MATRIX_H
#define INCLUDED_COMPONENT_KEYBOARD_MATRIX_H

#include <component_base.h>

#include <ArduinoJson.h>

class ComponentKeyboardMatrix : public ComponentBase
{
    /*
    * Create a keyboard matrix with a paricular number buttons
    * These button/switches will support isPresssed / unPressed semantics,
    * providing a state change in either direction
    *
    * @param numberOfButtons - The number of buttons/switches we're expected to support
    * @param pinStart - The digital I/O pin starting place for the matrix
    *
    * e.g.
    *     ComponentBase *matrix = new ComponentKeyboardMatrix(10, 5);
    *
    * Would create a 4x4 matrix (since 4x4 is the smallest required to support 10
    * switches) which utilizes pins D5 - D20
    */
    ComponentKeyboardMatrix(int numberOfButtons, int pinStart);
    ~ComponentKeyboardMatrix();

    virtual int getStateChange(JsonObject &jsonState);

    virtual void getCurrentState(JsonObject &jsonState);

    // The ->step functionality for the keyboard matrix is an interesting
    // one.  We need to decide if this will be implemented as a blocking step
    // or something that respects the tick...
    virtual void step();

private:

    /*  Returns value in button 'Matrix' for (x,y) (zero-indexed)
        m_matrixSize = 3
        0,0     0,1     0,2
        1,0     1,1     1,2
    */
    int at(int x, int y);

    int m_numberOfButtons;
    int m_matrixSize;  // one side of the matrix

    //  Exposing the 'matrix' as an array will make memory management easier.
    //  We'll just build an ->at() accessor to the matrix
    int *m_buttonArray;
};



#endif
