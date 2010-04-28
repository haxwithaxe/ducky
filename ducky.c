/* Keyboard example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_keyboard.h"

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD &= ~(1<<6))
#define LED_OFF		(PORTD |= (1<<6))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

void LinuxCommandRun(char *SomeCommand){
   usb_keyboard_press(KEY_F2, KEY_ALT);
   _delay_ms(500);
   usb_keyboard_send_string(SomeCommand);
   _delay_ms(10);
   usb_keyboard_press(KEY_ENTER,0);
};

void WindowsCommandRun(char *SomeCommand){
   usb_keyboard_press(KEY_R, KEY_GUI);
   _delay_ms(500);
   usb_keyboard_send_string(SomeCommand);
   _delay_ms(10);
   usb_keyboard_press(KEY_ENTER,0);
};

void AppleCommandRun(char *SomeCommand){
   usb_keyboard_press(KEY_SPACE, KEY_GUI);
   _delay_ms(500);
   usb_keyboard_send_string("Terminal.app");
   _delay_ms(10);
   usb_keyboard_press(KEY_ENTER,0);
   _delay_ms(500);
   usb_keyboard_send_string(SomeCommand);
   usb_keyboard_press(KEY_ENTER,0);
};

void LinuxPayload(void){
   LinuxCommandRun("xterm -e \"echo kill all humans! ... 01010001 > pwnd && cat pwnd -\"");
};

void WindowsPayload(void){
   WindowsCommandRun("notepad.exe");
   _delay_ms(1000);
   usb_keyboard_send_string("hax was here!!! }:)");
};

void ApplePayload(void){
   AppleCommandRun("/Applications/TextEdit.app/Contents/MacOS/TextEdit"); // open text editor
   _delay_ms(1000);
   usb_keyboard_send_string("hax was here!!! }:)"); // type message
   // or
   AppleCommandRun("echo kill all humans! ... 01010001"); // print a message directly in the terminal
   // run drive finding command
   // i don't own a mac so please feel free to make something up for me
   // also waiting for Darren's USBDucky example code to see if he has gotten around doing this
};

void blinkLEDLoop(int ms_on,int ms_off){
   while (1)
   {
      LED_ON;
      _delay_ms(ms_on);
      LED_OFF;
      _delay_ms(ms_off);
   };
};

void main(void)
{
   LED_OFF; // make sure LED is off

   CPU_PRESCALE(0); // set for 16 MHz clock

   // Initialize the USB, and then wait for the host to set configuration.
   // If the Teensy is powered without a PC connected to the USB port,
   // this will wait forever.
   usb_init();
   while (!usb_configured()) /* wait */ ;

   // Wait an extra second for the PC's operating system to load drivers
   // and do whatever it does to actually be ready for input
   _delay_ms(1000);
// DO STUFF ------------------------------------------------------------
   switch (keyboard_leds)
   {
      case 1: // num lock
         LinuxPayload();
         blinkLEDLoop(1000,1000);
         break;
      case 2: // caps lock
         ApplePayload();
         blinkLEDLoop(2000,1000);
         break;
      case 4: // scroll lock
         WindowsPayload();
         blinkLEDLoop(4000,1000);
         break;
      default: // none match
         blinkLEDLoop(1000,4000);
   }
   
// blink light when done -----------------------------------------------
   blinkLEDLoop(500,500);
}
