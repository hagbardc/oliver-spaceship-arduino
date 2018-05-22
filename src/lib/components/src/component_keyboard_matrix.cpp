#include <component_keyboard_matrix.h>

#include <math.h>

ComponentKeyboardMatrix::ComponentKeyboardMatrix(
    int matrixSize,
    int columnPinStart,
    int rowPinStart
    ) :
    m_matrixSize(matrixSize),
    m_columnPinStart(columnPinStart),
    m_rowPinStart(rowPinStart)
{
    m_buttonArrayStateOld = new int[matrixSize*matrixSize];
    m_buttonArrayStateCurrent = new int[matrixSize*matrixSize];
}

ComponentKeyboardMatrix::~ComponentKeyboardMatrix()
{
    delete[] m_buttonArrayStateOld;
    delete[] m_buttonArrayStateCurrent;
}


int ComponentKeyboardMatrix::at(int x, int y, int *buttonArray)
{
    // TODO:  Do some logging here
    if(! buttonArray ) {
        return -1;
    }

    return buttonArray[x + m_matrixSize*y];
}

void ComponentKeyboardMatrix::setAt(int x, int y, int state, int*buttonArray)
{
    if(! buttonArray ) {
        return;
    }

    buttonArray[x + m_matrixSize*y] = state;
}


void ComponentKeyboardMatrix::step()
{
    // Do a full scan of the buttons

        // Put the button state into m_buttonArrayStateCurrent

        // Check the button state against the m_buttonArrayStateOld Matrix
        // if there is a change, set m_hasStateChange to true

    // Actually querying for and publishing a state change should be the
    // responsibility of another component (the main loop, probably)
}


void ComponentKeyboardMatrix::readMatrix() {
    // iterate the columns
    for (int colIndex=0; colIndex < m_matrixSize; colIndex++) {
        // col: set to output to low
        int curCol = m_columnPinStart + colIndex;
        pinMode(curCol, OUTPUT);
        digitalWrite(curCol, LOW);

        // row: interate through the rows
        for (int rowIndex=0; rowIndex < m_matrixSize; rowIndex++) {
            int rowCol = m_rowPinStart + rowIndex;
            pinMode(rowCol, INPUT_PULLUP);

            setAt(rowIndex, colIndex,  digitalRead(rowCol), m_buttonArrayStateCurrent);

            pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);
    }
}
