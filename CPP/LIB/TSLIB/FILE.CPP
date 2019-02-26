#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <stdio.h>
  #include <stdarg.h>
  #include <time.h>
  #include <string.h>
#endif

#include <tslib.hpp>
#include <msdos.hpp>

const byte action_none  = 0;
const byte action_read  = 1;
const byte action_write = 2;

void
File::init()
{
   fh         = -1;
   buffer     = NULL;
   bufPtr     = NULL;
   bufSize    = 0;
   bytesInBuf = 0;
   bufPos     = 0;
   mode       = 0;
   lastAction = action_none;
}

File::File(const char *fname,word m,word bufsize)
{
   init();
   open(fname,m,bufsize);
}

bool
File::open(const char *fname,word m,word buf)
{
   if(fh>0) return FALSE;

   init();

   clearError();

   mode = m;

   if(!(mode & 0x70)) mode |= fmode_shared;

   if(mode & fmode_create)
   {
      if((mode & 3) == fmode_read || (mode & 3) == fmode_write)
      {
         fh    = msdos_create(fname);
         mode &= 0xFF00;
         mode |= fmode_write;
      }
      else
      {
         mode |= fmode_copen;
         mode &= ~fmode_create;
      }
   }
   else
   {
      time_t t = time(NULL);

      while(time(NULL)-t < (time_t)10)
      {
         fh = msdos_open(fname,mode & 0xF3);

         if(fh>0 || msdos_error != MSDOS_ERR_NOACCESS) break;

         sleep_clock();
      }
   }

   if(fh<0 && (mode & fmode_copen))
   {
      fh = msdos_create(fname);

      if(fh<0)
      {
         errorCode = msdos_error;

         return FALSE;
      }

      msdos_close(fh);

      fh = msdos_open(fname,mode & 0xF3);
   }

   if(fh<0)
   {
      errorCode = msdos_error;

      return FALSE;
   }

   if(textMode() && !buf) buf = 512;

   if(buf)
   {
      buffer = new byte[buf];
      if(buffer != NULL)
      {
         bufPtr  = buffer;
         bufSize = buf;
         mode   |= fmode_buffered;
      }
   }

   if(mode & fmode_append) seek(len());

   return !error();
}

void
File::close()
{
   if(fh < 0) return;

   if(buffered())
   {
      flush();
      delete buffer;
   }

   msdos_close(fh);

   init();

   clearError();
}

void
File::flush()
{
   if(fh<0) return;

   if(!buffered())
   {
      seek(pos());
      lastAction = action_none;

      errorCode = msdos_error;

      return;
   }

   if(lastAction == action_write)
   {
      msdos_seek(fh,bufPos,seek_set);
      msdos_write(fh,buffer,bytesInBuf);
      bufPos += bytesInBuf;
   }
   else
   {
      bufPos += word(bufPtr-buffer);
   }

   msdos_seek(fh,bufPos,seek_set);

   lastAction = action_none;

   bufPtr     = buffer;
   bytesInBuf = 0;

   errorCode = msdos_error;
}

bool
File::fillBuffer()
{
   bufPos += bytesInBuf;

   bytesInBuf = msdos_read(fh,buffer,bufSize);

   errorCode = msdos_error;

   if(!error() && bytesInBuf<1) atEof = TRUE;

   bufPtr = buffer;

   return (bytesInBuf && !error());
}

word
File::read(void *p,word size)
{
   if(fh < 0) return 0;

   clearError();

   if(textMode())
   {
      for(word i=0;i<size;i++)
      {
         int c = readByte();

         if(c < 0) break;

         *((byte *)p)++ = byte(c);
      }

      return i;
   }

   if(lastAction == action_write) flush();

   lastAction = action_read;

   if(!buffered())
   {
      size = msdos_read(fh,p,size);

      errorCode = msdos_error;

      return error() ? 0 : size;
   }

   if(!bytesInBuf) if(!fillBuffer()) return 0;

   if(bufPtr+size <= buffer+bytesInBuf)
   {
      memcpy(p,bufPtr,size);
      bufPtr += size;
      return size;
   }

   word left = size;

   while(left)
   {
      word x = bytesInBuf-word(bufPtr-buffer);

      if(!x)
      {
         if(!fillBuffer()) return (size-left);
                     else continue;
      }

      if(x >= left)
      {
         memcpy(p,bufPtr,left);
         bufPtr += left;
         return size;
      }
      else
      {
         memcpy(p,bufPtr,x);
      }

      left      -= x;
      (byte *)p += x;
      bufPtr    += x;
   }

   return size;
}

int
File::readByte()
{
   if(fh<0) return -1;

   clearError();

   if(lastAction == action_write) flush();

   if(!buffered())
   {
      byte c;

      lastAction = action_read;

      int n = msdos_read(fh,&c,1);

      errorCode = msdos_error;

      if(n == 1) return int(c);

      if(n == 0) atEof = TRUE;

      return -1;
   }

   for(;;)
   {
      if(bufPtr+1 > buffer+bytesInBuf || !bytesInBuf)
         if(!fillBuffer())
            return -1;

      lastAction = action_read;

      byte c = *bufPtr++;

      if(c=='\r' && textMode()) continue;       // CR

      if(c==26 && textMode())
      {
         atEof = TRUE;

         return -1;        // EOF!
      }

      return int(c);
   }
}

word
File::write(const void *p,word size)
{
   if(fh<0) return 0;

   clearError();

   if(textMode())
   {
      for(word i=0;i<size;i++)
         if(!writeByte(*((byte *)p)++)) break;

      return i;
   }

   if(lastAction == action_read) flush();

   if(!size)
   {
      flush();

      if(!error())
      {
         msdos_write(fh,p,0);

         errorCode = msdos_error;
      }

      seek(pos());

      lastAction = action_none;

      return 0;
   }

   if(!buffered())
   {
      size = msdos_write(fh,p,size);
      lastAction = action_write;

      errorCode = msdos_error;

      return error() ? 0 : size;
   }

   if(size>bufSize)
   {
      flush();

      size = msdos_write(fh,p,size);

      errorCode = msdos_error;

      if(error()) return 0;

      seek(bufPos + size);

      return size;
   }

   if(bufSize-size < bytesInBuf) flush();

   lastAction = action_write;

   memcpy(bufPtr,p,size);

   bufPtr     += size;
   bytesInBuf += size;

   return error() ? 0 : size;
}

bool
File::writeByte(byte c)
{
   if(fh<0) return FALSE;

   clearError();

   if(lastAction == action_read) flush();

   lastAction = action_write;

   if(c=='\n' && textMode())
      if(!writeByte('\r'))
         return FALSE;

   if(!buffered())
   {
      msdos_write(fh,&c,1);

      errorCode = msdos_error;

      return error() ? FALSE:TRUE;
   }

   if(bytesInBuf == bufSize) flush();

   lastAction = action_write;

   *bufPtr++ = c;
   bytesInBuf++;

   return !error();
}


long
File::seek(long p,int mode)
{
   if(fh<0) return 0;

   clearError();

   if(buffered() && lastAction == action_read)
   {
      if(mode == seek_cur)
      {
         p   += pos();
         mode = seek_set;
      }

      if(mode == seek_set && p>=bufPos && p<bufPos+bytesInBuf)
      {
         bufPtr = buffer + unsigned(p-bufPos);

         return p;
      }
   }

   if(!buffered())
   {
      lastAction = action_none;

      p = msdos_seek(fh,p,mode);

      errorCode = msdos_error;

      return error() ? -1L : p;
   }

   flush();

   bufPos = msdos_seek(fh,p,mode);

   errorCode = msdos_error;

   return error() ? -1L : bufPos;
}

long
File::pos()
{
   if(fh<0) return 0;

   if(error()) return -1;

   long p;

   if(buffered())
   {
      p = bufPos + word(bufPtr-buffer);
   }
   else
   {
      p = msdos_seek(fh,0,seek_cur);

      errorCode = msdos_error;
   }

   return error() ? -1L : p;
}

bool
File::lock(long pos,long len)
{
   if(fh < 0) return FALSE;

   clearError();

   flush();

   time_t t = time(NULL);

   while((time(NULL)-t)<20)
   {
      if(msdos_lock(fh,pos,len)) break;

      if(msdos_error != 0x21) break;

      sleep_clock();
   }

   errorCode = msdos_error;

   return !error();
}

bool
File::unlock(long pos,long len)
{
   if(fh < 0) return FALSE;

   clearError();

   flush();

   time_t t = time(NULL);

   while((time(NULL)-t)<20)
   {
      if(msdos_unlock(fh,pos,len)) break;

      if(msdos_error != 0x21) break;

      sleep_clock();
   }

   errorCode = msdos_error;

   return !error();
}

void
File::cut()
{
   byte dummy;

   write(&dummy,0);
}

long
File::len()
{
   if(fh<0) return 0;

   long p = pos();
   long l = seek(0,seek_end);

   seek(p);

   return l;
}

void
File::enableBuffer()
{
   if(fh<0) return;

   if(buffered()) flush();

   if(bufSize) mode |= fmode_buffered;
}

void
File::disableBuffer()
{
   if(fh<0) return;

   if(buffered())
   {
      flush();
      mode ^= fmode_buffered;
   }
}

File&
File::operator<<(char c)
{
   write(&c,1);

   return *this;
}

File&
File::operator<<(int i)
{
   write(&i,sizeof(i));

   return *this;
}

File&
File::operator<<(word w)
{
   write(&w,sizeof(w));

   return *this;
}

File&
File::operator<<(long l)
{
   write(&l,sizeof(l));

   return *this;
}

File&
File::operator<<(dword d)
{
   write(&d,sizeof(d));

   return *this;
}

File&
File::operator<<(const char *s)
{
   write(s,strlen(s));

   return *this;
}

File&
File::operator>>(char& c)
{
   read(&c,1);

   return *this;
}

File&
File::operator>>(int& i)
{
   read(&i,2);

   return *this;
}

File&
File::operator>>(word& i)
{
   read(&i,2);

   return *this;
}

File&
File::operator>>(long& i)
{
   read(&i,4);

   return *this;
}

File&
File::operator>>(dword& i)
{
   read(&i,4);

   return *this;
}

bool
File::readLine(char *s,int max)
{
   if(!textMode()) return FALSE;

   for(int i=0;i<max;)
   {
      int c = readByte();

      if(c < 0) break;

      *s++ = char(c);
      i++;

      if(char(c) == '\n') break;
   }

   *s   = '\0';

   return !!i;
}

String
File::readLine()
{
   if(!textMode()) return "";

   String s;

   for(;;)
   {
      int c = readByte();

      if(c < 0) break;

      s << char(c);

      if(char(c) == '\n') break;
   }

   return s;
}


bool
File::printf(const char *fmt,...)
{
   bool ret = TRUE;
   va_list x;
   va_start(x,fmt);

   char *s = new char[ strlen(fmt) + 1024 ];

   int l = vsprintf(s,fmt,x);

   if(write(s,l) != l) ret = FALSE;

   delete s;
   return ret;
}
