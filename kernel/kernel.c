#include "kernel.h"
#include "data.h"

// SLIDESHOW VARIABLES
struct Image
{
    const unsigned long *data;
    const int width;
    const int height;
};
// 3 imgs showed on slideshow 
struct Image imgs[] = {
    {data1, 600, 900},
    {data2, 590, 796},
    {data3, 698, 886}
};

// variable used for scroll up & down to view img
int virtY = 0;  //keep track of virtual height offset
// variable used for tracking current img showing on the slideshow
int curImgIndex = 0;

void main()
{
    // set up serial console
    uart_init();
    // say hello
    uart_puts("\n\nHello World!\n");
    // Initialize frame buffer
    framebf_init();

    // Draw first img on the screen
    drawImgARGB32(setXForCenterAlign(imgs[0].width), EACH_SCROLL_DIST, imgs[0].width, imgs[0].height, imgs[0].data);

    // echo everything back
    while (1)
    {
        char c = getUart();
        q1a(c);
    }
}

void q1a(char c)
{
    if (c)
    {
        if (c == 'w')
        {                                  // scroll up -> move virtual screen up
            if (virtY >= EACH_SCROLL_DIST) // check for virtual screen limit exceed
            {
                scroll(-EACH_SCROLL_DIST);
            }
        }
        else if (c == 's')
        { // scroll down -> move virtual screen down
            if (virtY <= MAX_VIRT_HEIGHT - PHYSICAL_HEIGHT - EACH_SCROLL_DIST)
            {
                scroll(EACH_SCROLL_DIST);
            }
        }
        else if (c == 'a')
        {
            if (curImgIndex > 0)
            {
                // clear previous img
                clearImg(setXForCenterAlign(imgs[curImgIndex].width), EACH_SCROLL_DIST, imgs[curImgIndex].width, imgs[curImgIndex].height);
                curImgIndex--;
                // draw current img
                drawImgARGB32(setXForCenterAlign(imgs[curImgIndex].width), EACH_SCROLL_DIST, imgs[curImgIndex].width, imgs[curImgIndex].height, imgs[curImgIndex].data);
            }
        }
        else if (c == 'd')
        {
            if (curImgIndex < NUM_OF_IMGS - 1)
            {
                // clear previous img
                clearImg(setXForCenterAlign(imgs[curImgIndex].width), EACH_SCROLL_DIST, imgs[curImgIndex].width, imgs[curImgIndex].height);
                curImgIndex++;
                // draw current img
                drawImgARGB32(setXForCenterAlign(imgs[curImgIndex].width), EACH_SCROLL_DIST, imgs[curImgIndex].width, imgs[curImgIndex].height, imgs[curImgIndex].data);
            }
        }
        else if (c == ' ')
        {
        }
    }
}

// change virtual screen offset
void scroll(int offY)
{
    virtY += offY;

    mBuf[0] = 8 * 4; // Length of message in bytes
    mBuf[1] = MBOX_REQUEST;
    mBuf[2] = MBOX_TAG_SETVIRTOFF; // Set virtual offset
    mBuf[3] = 8;
    mBuf[4] = 0;
    mBuf[5] = 0;     // x offset
    mBuf[6] = virtY; // y offset
    mBuf[7] = MBOX_TAG_LAST;
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) && mBuf[6] == virtY)
    {
        uart_puts("New offset height: ");
        uart_dec(mBuf[6]);
        uart_sendc('\n');
    }
}

// return start X of img for it to be center aligned
int setXForCenterAlign(int imgWidth)
{
    return PHYSICAL_WIDTH / 2 - imgWidth / 2;
}