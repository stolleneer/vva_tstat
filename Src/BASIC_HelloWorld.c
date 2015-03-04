#include "GUI.h"

void MainTask(void) {
  GUI_Clear();
  GUI_SetFont(&GUI_Font20_1);
  GUI_DispStringAt("Hello world!", (LCD_GetXSize()-100)/2, (LCD_GetYSize()-20)/2);

  GUI_DispStringAt("Verventa is live!", (LCD_GetXSize()-150)/2, (LCD_GetYSize()-60)/2);
  while(1);
}
