// ----------------------------------- framebf.h -------------------------------------
void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawLineARGB32(int x1, int y1, int x2, int y2, unsigned int attr);
void drawImgARGB32(int startX, int startY, int imgWidth, int imgHeight, const unsigned long img[]);
void clearImg(int startX, int startY, int imgWidth, int imgHeight);


#define PHYSICAL_WIDTH 1024
#define PHYSICAL_HEIGHT 550
#define MAX_VIRT_WIDTH 1200
#define MAX_VIRT_HEIGHT 1200
