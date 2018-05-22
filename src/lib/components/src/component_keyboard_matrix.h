#ifndef INCLUDED_COMPONENT_KEYBOARD_MATRIX_H
#define INCLUDED_COMPONENT_KEYBOARD_MATRIX_H

#include <component_base.h>

#include <Arduino.h>

#include <ArduinoJson.h>

class ComponentKeyboardMatrix : public ComponentBase
{
    /*
    * Create a keyboard matrix with a paricular number buttons
    * These button/switches will support isPresssed / unPressed semantics,
    * providing a state change in either direction
    *
    * @param matrixSize - The size of one side of the (square) matrix
    * @param columnPinStart - The digital I/O pin starting place for column reads
    * @param rowPinStart - The digital I/O pin starting place for row reads
    *
    * e.g.
    *     ComponentBase *matrix = new ComponentKeyboardMatrix(4, 5, 14);
    *
    * Would create a 4x4 matrix, which utilizes pins D5 - D8 and D14 - D17
    */
    ComponentKeyboardMatrix(int matrixSize,
                            int columnPinStart, int rowPinStart);

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
    int at(int x, int y, int *buttonArray);
    int at_old(int x, int y) { return at(x, y, m_buttonArrayStateOld); }
    int at_current(int x, int y) {return at(x, y, m_buttonArrayStateCurrent); }

    void setAt(int x, int y, int state, int*buttonArray);

    void readMatrix();

    int m_matrixSize;  // one side of the matrix
    int m_columnPinStart;
    int m_rowPinStart;

    //  Exposing the 'matrix' as an array will make memory management easier.
    //  We'll just build an ->at() accessor to the matrix

    //  We keep two arrays which will track the button array states, since
    //  we want to limit the amount of data that's sent out in a state change.
    int *m_buttonArrayStateOld;
    int *m_buttonArrayStateCurrent;

    // If we note that there is a state change during ->step(), this gets
    // set to true.  We will only attempt to build a state change JSON object
    // if this is true
    bool m_hasStateChange;
};



#endif
