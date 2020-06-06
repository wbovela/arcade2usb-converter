/*
   joystick.cpp

   Joystick routines to handle arcade to usb conversion

   Version 1.2 (See CHANGELOG)
   Release date: 2020-06-07

   Written & released by W.Bovelander (The Highlander)

    Licensed under the MIT License. See LICENSE file in the project root for full license information.
*/

#include "joystick.h"

#if defined(_USING_HID)

#define JOYSTICK_REPORT_ID 0x01

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  0x05, 0x01,        /* Usage Page (Generic Desktop)  */
  0x09, 0x04,        /* Usage (Joystick)      */
  0xA1, 0x01,        /* Collection (Application)    */
    0xA1, 0x02,        /*   Collection (Logical)    */
      0x85, JOYSTICK_REPORT_ID,
    
      0x75, 0x08,        /*     Report Size (8)   */
      0x95, 0x05,        /*     Report Count (5)    */
      0x15, 0x00,        /*     Logical Minimum (0)   */
      0x26, 0xFF, 0x00,  /*     Logical Maximum (255)   */
      0x35, 0x00,        /*     Physical Minimum (0)    */
      0x46, 0xFF, 0x00,  /*     Physical Maximum (255)  */
      0x09, 0x30,        /*     Usage (Direction-X)     */
      0x09, 0x30,        /*     Usage (Direction-X)      */
      0x09, 0x30,        /*     Usage (Direction-X)     */
      0x09, 0x30,        /*     Usage (Direction-X)     */      
      0x09, 0x31,        /*     Usage (Direction-Y)     */
      0x81, 0x02,        /*     Input (Variable)    */
    
      0x75, 0x04,        /*     Report Size (4)   */
      0x95, 0x01,        /*     Report Count (1)    */
      0x25, 0x07,        /*     Logical Maximum (7)   */
      0x46, 0x3B, 0x01,  /*     Physical Maximum (315)  */
      0x65, 0x14,        /*     Unit (Centimeter)   */
      0x09, 0x00,        /*     Usage (Hat switch)    */
      0x81, 0x42,        /*     Input (Variable)    */
      
      0x65, 0x00,        /*     Unit (None)     */
      0x75, 0x01,        /*     Report Size (1)   */
      0x95, 0x0A,        /*     Report Count (10)    */
      0x25, 0x01,        /*     Logical Maximum (1)   */
      0x46, 0x01,        /*     Physical Maximum (1)  */
      0x05, 0x09,         /*  Usage Page (Buttons) */
      0x09, 0x01,          /* Usage button 1 */
      0x09, 0x02,          /* Usage button 2 */
      0x09, 0x03,          /* Usage button 3 */
      0x09, 0x04,          /* Usage button 4 */
      0x09, 0x05,          /* Usage button 5 */
      0x09, 0x06,          /* Usage button 6 */
      0x09, 0x07,          /* Usage button 7 */
      0x09, 0x08,          /* Usage button 8 */
      0x09, 0x09,          /* Usage button 9 */
      0x09, 0x0A,          /* Usage button 10 */                              
      0x81, 0x02,          /* Input 2 (Data,Variable,Absolute,No Wrap,Linear,Preferred State,No Null Position) */
      
      0x06, 0x09,          /* usage page (vendor specific) */
      0x75, 0x01,        /*     Report Size (1)   */
      0x95, 0x0A,        /*     Report Count (10)    */
      0x25, 0x01,        /*     Logical Maximum (1)   */
      0x46, 0x01,        /*     Physical Maximum (1)  */
      0x09, 0x01,        /* Usage (vendor usage 1) */
      0x81, 0x02,         /* Input 2 (Data,Variable,Absolute,No Wrap,Linear,Preferred State,No Null Position) */

      0xC0,

      0xA1, 0x02,        /*   Collection (Logical)    */      
        0x75, 0x01,        /*     Report Size (8)   */
        0x95, 0x0A,        /*     Report Count (4)    */
        0x46, 0xFF, 0x00,        /*     Physical Maximum (1)  */
        0x26, 0xFF, 0x00,  /*     Logical Maximum (255)   */     
        0x09, 0x02,        /* Usage (vendor usage 2) */  
        0x91, 0x02,          /* Output 2  (Data,Variable,Absolute,No Wrap,Linear,Preferred State,No Null Position,Non Volatile) */               
    0xC0,              /*   End Collection      */
  0xC0               /* End Collection      */
};      

//================================================================================
//  Joystick
//================================================================================

Joystick_::Joystick_(void)
{
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
}

void Joystick_::reset(void)
{
  //reset button state..
  _buttons = 0x0F;
  _start_button = 0x00;

  //reset x & y axis
  _x_axis = 0x7f;
  _y_axis = 0x7f;
}

void Joystick_::up()
{
  _y_axis = 0x0;
}

void Joystick_::right()
{
  _x_axis = 0xff;
}

void Joystick_::down()
{
  _y_axis = 0xff;
}

void Joystick_::left()
{
  _x_axis = 0x00;
}

void Joystick_::button_press(uint8_t b)
{
  _buttons = _buttons | b;
}

void Joystick_::start_press(uint8_t b)
{
  _start_button = _start_button | b;
}

void Joystick_::usb_update()
{
  // only send usb data if needed..
  uint8_t m[7];
  // the report_id (0x01) is the first byte of 5
  m[0] = 0x7f;
  m[1] = 0x7f;
  m[2] = _x_axis;
  m[3] = _y_axis;
  // then follow 4 bits, 10 bits and 10 bits (24 bits = 3 bytes)
  m[4] = _buttons;
  m[5] = _start_button;
  m[6] = 0x00;

  HID().SendReport(JOYSTICK_REPORT_ID, m, sizeof(m));
}

Joystick_ Joystick;

#endif
