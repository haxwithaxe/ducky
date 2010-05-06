#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_keyboard.h"

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD &= ~(1<<6))
#define LED_OFF		(PORTD |= (1<<6))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

void LinuxCommandRun_s(const char *sc);
void WindowsCommandRun_s(const char *sc);
void AppleCommandRun_s(const char *sc);
#define LinuxCommandRun(sc) LinuxCommandRun_s(PSTR(sc))
#define WindowsCommandRun(sc) WindowsCommandRun_s(PSTR(sc))
#define AppleCommandRun(sc) AppleCommandRun_s(PSTR(sc))

void blinkLEDLoop(int ms_on,int ms_off){
   while (1)
   {
      LED_ON;
      _delay_ms(ms_on);
      LED_OFF;
      _delay_ms(ms_off);
   };
   LED_OFF; // this should never happen
};

void blinkLED(int count){
   int i;
   for (i=0;i<count;i++)
   {
      LED_ON;
      _delay_ms(1000);
      LED_OFF;
      _delay_ms(1000);
   };
   LED_OFF;
};

void blinkLEDAlt(int count,int ms_on,int ms_off){
   int i;
   for (i=0;i<count;i++)
   {
      LED_ON;
      _delay_ms(ms_on);
      LED_OFF;
      _delay_ms(ms_off);
   };
   LED_OFF;
};

void LinuxCommandRun_s(const char *sc){
   usb_keyboard_press(KEY_F2, KEY_ALT);
   _delay_ms(500);
   usb_keyboard_send_string(sc);
   _delay_ms(10);
   usb_keyboard_press(KEY_ENTER,0);
};

void WindowsCommandRun_s(const char *sc){
   usb_keyboard_press(KEY_R, KEY_GUI);
   _delay_ms(500);
   usb_keyboard_send_string(sc);
   _delay_ms(10);
   usb_keyboard_press(KEY_ENTER,0);
};

void AppleCommandRun_s(const char *sc){
   usb_keyboard_press(KEY_SPACE, KEY_GUI);
   _delay_ms(500);
   usb_keyboard_send_string("Terminal.app");
   _delay_ms(10);
   usb_keyboard_press(KEY_ENTER,0);
   _delay_ms(500);
   usb_keyboard_send_string(sc);
   usb_keyboard_press(KEY_ENTER,0);
};
