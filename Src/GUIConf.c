#include "GUI.h"

// Define the available number of bytes available for the GUI
#define GUI_NUMBYTES  (1024) *  32   // x KByte

/* 32 bit aligned memory area */
static U32 extMem[GUI_NUMBYTES / 4];

void GUI_X_Config(void)
{
  GUI_ALLOC_AssignMemory(extMem, GUI_NUMBYTES);
}
