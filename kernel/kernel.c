#include "kernel.h"
#include "data.h"


struct Image
{
    const unsigned long *data;
    int width;
    int height;
};

// SLIDESHOW VARIABLES
// 3 imgs showed on slideshow, 4th img is for playing video
struct Image imgs[NUM_OF_IMGS] = {
    {data1, 600, 900},
    {data2, 590, 796},
    {data3, 698, 886},
    {frame01, VIDEO_WIDTH, VIDEO_HEIGHT}
};
// variable used for scroll up & down to view img
int virtY = 0;  //keep track of virtual height offset
// variable used for tracking current img showing on the slideshow
int curImgIndex = 0;

// VIDEO VARIABLES
const unsigned long *framesData[TOTAL_VIDEO_FRAMES] = {
	frame01, frame02, frame03, frame04, frame05, frame06, 
    frame07, frame08, frame09, frame10, frame11, frame12,
	frame13, frame14, frame15, frame16,	frame17, frame18,
	frame19, frame20, frame21, frame22, frame23, frame24,
	frame25, frame26, frame27, frame28, frame29, frame30,
	frame31, frame32, frame33, frame34, frame35, frame36,
	frame37, frame38, frame39, frame40, frame41, frame42,
	frame43, frame44, frame45, frame46,	frame47, frame48,
	frame49, frame50, frame51, frame52, frame53, frame54,
	frame55, frame56, frame57, frame58,	frame59, frame60,
	frame61, frame62
};

void main()
{
    // set up serial console
    uart_init();
    // say hello
    uart_puts("\n\nHello World!\n");
    // Initialize frame buffer
    framebf_init();

    // //initalize frame data in video array
    // addFramesToVideo();
    // Draw first img on the screen
    drawImgARGB32(setXForCenterAlign(imgs[0].width), EACH_SCROLL_DIST, imgs[0].width, imgs[0].height, imgs[0].data);

    // echo everything back
    while (1)
    {
        char c = getUart();
        q1a(c);
    }
}

// void addFramesToVideo(){
//     for (int i = 0; i< TOTAL_VIDEO_FRAMES; i++){
//         video[i].data = framesData[i];
//         video[i].width = VIDEO_WIDTH;
//         video[i].height = VIDEO_HEIGHT;
//     }
// }

void q1a(char c)
{
    if (c)  //getUart() returns something??
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
        {   //press space when at the 4th img to play video
            if (curImgIndex == 3){  //user at 4th img??
                playVideo();
            }
        }
    }
}

void playVideo(){
    //clear the 4th img showed on the slideshow
    clearImg(setXForCenterAlign(imgs[3].width), EACH_SCROLL_DIST, imgs[3].width, imgs[3].height);

    // for (int i = 0; i< NUM_OF_IMGS; i++){
    //     drawImgARGB32(setXForCenterAlign(imgs[i].width), EACH_SCROLL_DIST, imgs[i].width, imgs[i].height, imgs[i].data);
    //     wait_msec(5000);
    // }
    //start the video (20 fps)
    for(int i =0; i< TOTAL_VIDEO_FRAMES; i++){
        set_wait_timer(1, 5);
        //draw current frame
        drawImgARGB32(setXForCenterAlign(VIDEO_WIDTH), EACH_SCROLL_DIST, VIDEO_WIDTH, VIDEO_HEIGHT, framesData[i]);
        set_wait_timer(0, 5);
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