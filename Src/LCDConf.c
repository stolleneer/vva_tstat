#include "GUI.h"
#include "GUIDRV_FlexColor.h"

#define XSIZE_PHYS  240
#define YSIZE_PHYS  320

#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif

/*********************************************************************
*       FSCM 16 bit data - address pointer
*
**********************************************************************
*/
#define LCD_BASE           ((uint32_t)(0x60000000)
#define LCD_REG_ADDRESS   (*(volatile U16*)(0x60000000))
#define LCD_DATA_ADDRESS  (*(volatile U16*)(0x60020000))

/* Private Functions -----------------------------------------------------*/
static void LcdWriteReg(U16 Data) {
  LCD_REG_ADDRESS = Data;
}
static void LcdWriteData(U16 Data) {
  LCD_DATA_ADDRESS = Data;
}
static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    LCD_DATA_ADDRESS = *pData++;
  }
}
static void LcdReadDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    *pData++ = LCD_DATA_ADDRESS;
  }
}
/* Public Functions ------------------------------------------------------*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  // Set display driver and color conversion
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);
  // Display driver configuration, required for Lin-driver
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
  // Orientation
  Config.Orientation = GUI_SWAP_XY | GUI_MIRROR_Y; // Landscape
  GUIDRV_FlexColor_Config(pDevice, &Config);
  // Set controller and operation mode
  PortAPI.pfWrite16_A0  = LcdWriteReg;
  PortAPI.pfWrite16_A1  = LcdWriteData;
  PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM16_A1  = LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);
}

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  switch (Cmd) {
  	case LCD_X_INITCONTROLLER: {
  		return 0;
  	}
  	default:
  		r = -1;
  }
  return r;
}

