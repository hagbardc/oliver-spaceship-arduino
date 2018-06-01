#include <component_keyboard_matrix.h>

#include <math.h>

#include <Arduino.h>

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
    // Do a full scan of the buttons and set the state
        this->readMatrix();
}

void ComponentKeyboardMatrix::printMatrix()
{
    for(int y=0; y<m_matrixSize; ++y) {
        for(int x=0; x<m_matrixSize; ++x) {
            Serial.print(this->at_current(x, y));
            Serial.print(" ");
        }
        Serial.println("");
    }
}

int ComponentKeyboardMatrix::getStateChange(JsonObject &jsonState)
{
    if(! this->m_hasStateChange) {
        return 0;
    }

    // Loop over
    for(int ii=0; ii<(m_matrixSize*m_matrixSize); ++ii) {
        if(this->m_buttonArrayStateOld[ii] == this->m_buttonArrayStateCurrent[ii] ) {
            continue;
        }

        // TODO: Add the state transition to the JSON object

        this->m_buttonArrayStateOld[ii] = this->m_buttonArrayStateCurrent[ii];
    }

    this->m_hasStateChange = false;
    return 1;
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
            if(this->at_old(rowIndex, colIndex) != this->at_current(rowIndex, colIndex)) {
                this->m_hasStateChange = true;
            }

            pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);
    }
}
