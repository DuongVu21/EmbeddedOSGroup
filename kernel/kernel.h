#include "mbox.h"
#include "framebf.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

#define EACH_SCROLL_DIST 30
#define NUM_OF_IMGS 4
#define TOTAL_VIDEO_FRAMES 62

void q1a(char c);
void scroll(int offY);
void addFramesToVideo();
void playVideo();
int setXForCenterAlign(int imgWidth);