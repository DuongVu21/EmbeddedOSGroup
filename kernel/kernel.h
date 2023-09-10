#include "mbox.h"
#include "framebf.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

#define EACH_SCROLL_DIST 30
#define NUM_OF_IMGS 3

void q1a(char c);
void scroll(int offY);
int setXForCenterAlign(int imgWidth);