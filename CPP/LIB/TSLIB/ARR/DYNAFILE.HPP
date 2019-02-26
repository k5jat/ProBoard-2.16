template <class T>
class DynamicArrayFile
   {
      T (*emptyfunc)(long rec);
      void (*movefunc) (long recfrom,long recto,T& obj);

      File& f;
      long  numrecs;

   public:
      DynamicArrayFile(File&);
     ~DynamicArrayFile() {}

      long numRecords() { return numrecs; }

      bool read  (long rec,T&);
      bool write (long rec,T&);

      bool insert(long rec,T&);
      bool insert(long rec);

      bool remove(long rec);

      void setEmptyFunc(T (*f)(long))            { emptyfunc = f; }
      void setMoveFunc (void (*f)(long,long,T&)) { movefunc = f;  }
   };


template <class T>
DynamicArrayFile<T>::DynamicArrayFile(File& pFile) : f(pFile)
{
   numrecs = f.len() / sizeof(T);

   if(f.len() != numrecs * sizeof(T))
   {
      f.seek(numrecs * sizeof(T));
      f.cut();
   }

   emptyfunc = 0;
   movefunc  = 0;
}

template <class T>
bool
DynamicArrayFile<T>::read(long rec,T& obj)
{
   if(rec >= numrecs)
      return FALSE;

   f.seek(rec * sizeof(T));
   if(f.read(&obj,sizeof(T)) != sizeof(T))
      return FALSE;

   return TRUE;
}

template <class T>
bool
DynamicArrayFile<T>::write(long rec,T& obj)
{
   if(rec >= numrecs)
   {
      for(long r = numrecs;r<rec;r++)
      {
         f.seek(r * sizeof(T));

         if(emptyfunc)
         {
            T obj = emptyfunc(r);
            if(f.write(&obj,sizeof(T)) != sizeof(T))
               return FALSE;
         }
         else
         {
            T obj;
            if(f.write(&obj,sizeof(T)) != sizeof(T))
               return FALSE;
         }

         numrecs = r+1;
      }
   }

   f.seek(rec * sizeof(T));

   if(f.write(&obj,sizeof(T)) != sizeof(T))
      return FALSE;

   if(rec >= numrecs)
      numrecs = rec+1;

   return TRUE;
}

template <class T>
bool
DynamicArrayFile<T>::insert(long rec,T& obj)
{
   if(movefunc)
   {
      T obj;

      for(long r=numrecs-1;r>=rec;r--)
      {
         f.seek(r * sizeof(T));
         f.read(&obj,sizeof(T));

         movefunc(r,r+1,obj);

         f.seek((r+1) * sizeof(T));
         f.write(&obj,sizeof(T));
      }
   }
   else
   {
      byte *buf = new byte[sizeof(T)];

      for(long r=numrecs-1;r>=rec;r--)
      {
         f.seek(r * sizeof(T));
         f.read(buf,sizeof(T));
         f.seek((r+1) * sizeof(T));
         f.write(buf,sizeof(T));
      }

      delete [] buf;
   }

   numrecs++;

   f.seek(rec * sizeof(T));
   f.write(&obj,sizeof(T));

   return TRUE;
}

template <class T>
bool
DynamicArrayFile<T>::insert(long rec)
{
   if(movefunc)
   {
      T obj;

      for(long r=numrecs-1;r>=rec;r--)
      {
         f.seek(r * sizeof(T));
         f.read(&obj,sizeof(T));

         movefunc(r,r+1,obj);

         f.seek((r+1) * sizeof(T));
         f.write(&obj,sizeof(T));
      }
   }
   else
   {
      byte *buf = new byte[sizeof(T)];

      for(long r=numrecs-1;r>=rec;r--)
      {
         f.seek(r * sizeof(T));
         f.read(buf,sizeof(T));
         f.seek((r+1) * sizeof(T));
         f.write(buf,sizeof(T));
      }

      delete [] buf;
   }

   numrecs++;

   if(emptyfunc)
   {
      T obj = emptyfunc(rec);

      f.seek(rec * sizeof(T));
      f.write(&obj,sizeof(T));
   }
   else
   {
      T obj;

      f.seek(rec * sizeof(T));
      f.write(&obj,sizeof(T));
   }

   return TRUE;
}

template <class T>
bool
DynamicArrayFile<T>::remove(long rec)
{
   if(movefunc)
   {
      T obj;

      for(long r=rec+1;r<numrecs;r++)
      {
         f.seek(r * sizeof(T));
         f.read(&obj,sizeof(T));

         movefunc(r,r-1,obj);

         f.seek((r-1) * sizeof(T));
         f.write(&obj,sizeof(T));
      }
   }
   else
   {
      byte *buf = new byte[sizeof(T)];

      for(long r=rec+1;r < numrecs;r++)
      {
         f.seek(r * sizeof(T));
         f.read(buf,sizeof(T));
         f.seek((r-1) * sizeof(T));
         f.write(buf,sizeof(T));
      }

      delete [] buf;
   }

   numrecs--;

   f.seek(numrecs * sizeof(T));
   f.cut();

   return TRUE;
}
