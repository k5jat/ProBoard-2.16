extern "C"
   {
      /*** Do NOT use these functions. Use the "File" class instead! ***/

      bool msdos_lock(int fh,long pos,long len);
      bool msdos_unlock(int fh,long pos,long len);
      int  msdos_open(const char *fname,int mode);
      word msdos_read(int fh,void *buf,word size);
      word msdos_write(int fh,const void *buf,word size);
      int  msdos_create(const char *fname);
      void msdos_close(int fh);
      long msdos_seek(int fh,long pos,byte mode);

      extern int msdos_error;

      /*** Constants for msdos_error ***/

      const MSDOS_ERR_OK       = 0;    // No error
      const MSDOS_ERR_BADFUNC  = 1;    // Bad DOS function
      const MSDOS_ERR_NOFILE   = 2;    // No such file
      const MSDOS_ERR_NOPATH   = 3;    // No such directory
      const MSDOS_ERR_NUMFILES = 4;    // No more file handles
      const MSDOS_ERR_NOACCESS = 5;    // Access denied
      const MSDOS_ERR_BADFH    = 6;    // Bad file handle
   }

