#ifdef PEX
   #include <pb_sdk.h>
#else
   #include <dos.h>
#endif

#include <tslib.hpp>

long
ts_CopyFile(char *from,char *to,int bufsize)
{
   FileName fni,fno;

   fni = from;

   fno = to;

   fno.delLast('\\');

   if(ts_DirExists(fno))
   {
      FileName tmp = fni;
      tmp.stripPath();

      fno << '\\' << tmp;
   }

   File fi,fo;

	byte *buf;
	long totalbytes = 0;

	buf = new byte[bufsize];

	if(buf == NULL) return -1;

	if(!fi.open(fni,fmode_read))
	{
		delete [] buf;

		return -1;
	}

	if(!fo.open(fno,fmode_create))
	{
		delete [] buf;

		return -1;
	}

	unsigned fdate,ftime;

	for(;;)
	{
		word bytesread = fi.read(buf,bufsize);

		if(bytesread < 1) break;

		totalbytes += bytesread;

		if(fo.write(buf,bytesread) != bytesread)
		{
			totalbytes = -1;
			break;
		}
	}

	delete [] buf;

	if(!_dos_getftime(fi.dosHandle(),&fdate,&ftime))
		_dos_setftime(fo.dosHandle(),fdate,ftime);

	return totalbytes;
}
