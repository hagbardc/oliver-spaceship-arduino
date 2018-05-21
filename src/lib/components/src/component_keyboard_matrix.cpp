#include <component_keyboard_matrix.h>

#include <math.h>

ComponentKeyboardMatrix::ComponentKeyboardMatrix(
    int numberOfButtons,
    int pinStart
    ) :
    m_numberOfButtons(numberOfButtons)
{
    double sqrtButtons = sqrt(m_numberOfButtons);
    m_matrixSize = (int)(ceil(sqrtButtons)); // Make sure we have an integer matrix size

    m_buttonMatrix = new int[m_matrixSize];  // compile error?

}

ComponentKeyboardMatrix::ComponentKeyboardMatrix()
{
    delete[] m_buttonMatrix;
}


int ComponentKeyboardMatrix::at(int x, int y)
{
    // TODO:  Do some logging here
    if(! m_buttonMatrix ) {
        return -1;
    }

    return m_buttonMatrix[x + m_matrixSize*y];
}
