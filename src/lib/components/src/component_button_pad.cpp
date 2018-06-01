#include <component_button_pad.h>


ComponentButtonPad::ComponentButtonPad( uint8_t buttonSelectPinStart,
                                        uint8_t buttonReadPinStart,
                                        uint8_t ledSelectPinStart,
                                        uint8_t colorPins[4][3]) : m_currentColumn(0)
{

    for(int ii=0; ii<ComponentButtonPad::NUM_LED_COLUMNS; ++ii) {
        this->m_btnSelectPins[ii] = buttonSelectPinStart+ii;
        this->m_btnReadPins[ii] = buttonReadPinStart+ii;
        this->m_ledSelectPins[ii] = ledSelectPinStart+ii;
    }

    for(int ii=0; ii<4; ++ii) {
        for(int jj=0; jj<3; ++jj) {
            m_colorPins[ii][jj] = colorPins[ii][jj];
        }
    }

    setupPins();

    for(uint8_t ii = 0; ii < NUM_LED_ROWS; ii++) {
        for(uint8_t jj = 0; jj < NUM_LED_COLUMNS; jj++) {
            m_ledOutputs[ii][jj] = 0;
        }
    }

    this->m_nextScan = millis();
}


void ComponentButtonPad::setupPins()
{
    uint8_t i;

    // initialize
    // select lines
    for(i = 0; i < NUM_LED_COLUMNS; i++)
    {
        pinMode(m_ledSelectPins[i], OUTPUT);

        // with nothing selected by default
        digitalWrite(m_ledSelectPins[i], HIGH);
    }

    for(i = 0; i < NUM_BTN_COLUMNS; i++)
    {
        pinMode(m_btnSelectPins[i], OUTPUT);

        // with nothing selected by default
        digitalWrite(m_btnSelectPins[i], HIGH);
    }

    // key return lines
    for(i = 0; i < 4; i++)
    {
        pinMode(m_btnReadPins[i], INPUT_PULLUP);
    }

    // LED drive lines
    for(i = 0; i < NUM_LED_ROWS; i++)
    {
        for(uint8_t j = 0; j < NUM_COLORS; j++)
        {
            pinMode(m_colorPins[i][j], OUTPUT);
            digitalWrite(m_colorPins[i][j], LOW);
        }
    }

    for(uint8_t i = 0; i < NUM_BTN_COLUMNS; i++)
    {
        for(uint8_t j = 0; j < NUM_BTN_ROWS; j++)
        {
            m_debounceCount[i][j] = 0;
        }
    }
}

void ComponentButtonPad::scan()
{

    //  Ensure that we don't scan more than once every millisecond
    if(millis() < this->m_nextScan) {
        //Serial.println("Returning from scan");
        return;
    }
    this->m_nextScan = millis()+1;

    uint8_t val;
    uint8_t i, j;

    //run
    digitalWrite(m_btnSelectPins[m_currentColumn], LOW);
    digitalWrite(m_ledSelectPins[m_currentColumn], LOW);

    for(i = 0; i < NUM_LED_ROWS; i++)
    {
        uint8_t val = (m_ledOutputs[m_currentColumn][i] & 0x03);

        if(val)
        {
            digitalWrite(m_colorPins[i][val-1], HIGH);
        }
  }

  delay(1);

  for( j = 0; j < NUM_BTN_ROWS; j++)
  {
    val = digitalRead(m_btnReadPins[j]);

    if(val == LOW)
    {
      // active low: val is low when btn is pressed
      if( m_debounceCount[m_currentColumn][j] < MAX_DEBOUNCE)
      {
        m_debounceCount[m_currentColumn][j]++;
        if( m_debounceCount[m_currentColumn][j] == MAX_DEBOUNCE )
        {
          Serial.print("Key Down ");
          Serial.println((m_currentColumn * NUM_BTN_ROWS) + j);

          m_ledOutputs[m_currentColumn][j]++;
        }
      }
    }
    else
    {
      // otherwise, button is released
      if( m_debounceCount[m_currentColumn][j] > 0)
      {
        m_debounceCount[m_currentColumn][j]--;
        if( m_debounceCount[m_currentColumn][j] == 0 )
        {
          Serial.print("Key Up ");
          Serial.println((m_currentColumn * NUM_BTN_ROWS) + j);
        }
      }
    }
  }// for j = 0 to 3;

  delay(1);

  digitalWrite(m_btnSelectPins[m_currentColumn], HIGH);
  digitalWrite(m_ledSelectPins[m_currentColumn], HIGH);

  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    for(j = 0; j < NUM_COLORS; j++)
    {
      digitalWrite(m_colorPins[i][j], LOW);
    }
  }

  m_currentColumn++;
  if (m_currentColumn >= NUM_BTN_COLUMNS)
  {
    m_currentColumn = 0;
  }
}
