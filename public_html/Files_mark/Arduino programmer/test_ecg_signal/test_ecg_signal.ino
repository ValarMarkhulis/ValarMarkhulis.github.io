//timer based on http://arduino.cc/forum/index.php/topic=130423.15.html
volatile unsigned int ADC_Value = 0;
boolean timer_fired=false;
// Black magic
void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)irq);
  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
  uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
  TC_SetRA(tc, channel, rc/2); //50% high, 50% low
  TC_SetRC(tc, channel, rc);
  TC_Start(tc, channel);
  tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
  tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
  NVIC_EnableIRQ(irq);
}

void setup(){
  // Start timer. Parameters are:

  // TC1 : timer counter. Can be TC0, TC1 or TC2
  // 0   : channel. Can be 0, 1 or 2
  // TC3_IRQn: irq number. 
  // 250 : frequency (in Hz)
  // The interrupt service routine is TC3_Handler. 
 Serial.begin(57600);
  startTimer(TC1, 0, TC3_IRQn, 250);
}

void loop(){
  if (timer_fired)
  {
     ADC_Value = analogRead(0);
     Serial.print("D!");
     Serial.println(ADC_Value);
     timer_fired=false;
  }
}

volatile boolean l;

// This function is called every 1/250 sec.
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  timer_fired=true;
}
