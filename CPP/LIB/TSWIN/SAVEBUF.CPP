#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#endif

#include <tswin.hpp>

SaveBuffer::SaveBuffer()
{
 ptr = NULL;
 xs = ys = 0;
}

SaveBuffer::~SaveBuffer()
{
 delete [] ptr;
}

void
SaveBuffer::copy(int x,int y,int xsize,int ysize)
{
 if(xs!=xsize || ys!=ysize)
   {
    delete [] ptr;
    ptr = NULL;
   }

 if(!ptr)
   {
    ptr = new word[xsize*ysize];
    xs = xsize;
    ys = ysize;
   }

 tsw_gettext(ptr,x,y,x+xs-1,y+ys-1);
}

void
SaveBuffer::paste(int x,int y)
{
 if(ptr) tsw_puttext(ptr,x,y,x+xs-1,y+ys-1);
}

void
SaveBuffer::paste(int xf,int yf,int xt,int yt,int xsize,int ysize)
{
 if(!ptr) return;

 word *pf = &ptr[xf-1+(yf-1)*xs];

 for(int i=0;i<ysize;i++)
   {
    tsw_puttext(pf,xt,yt+i,xt+xsize-1,yt+i);
    pf += xs;
   }
}

void
SaveBuffer::clear()
{
 if(ptr)
   {
    delete [] ptr;
    ptr = NULL;
   }
}
