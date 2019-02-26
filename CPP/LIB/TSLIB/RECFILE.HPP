template <class T>
  class RecordFile : public File
    {
        public:
            bool read(T& , long rec = -1);
            bool write(T& , long rec = -1);

            bool seek(long rec);

            RecordFile& operator>>(T&);
            RecordFile& operator<<(T&);

            void operator+=(long rec) { seek(pos() + rec * sizeof(T); }
            void operator-=(long rec) { seek(pos() - rec * sizeof(T); }
    };

template <class T>
bool
RecordFile<T>::read( T& obj )
{
   return (((rec < 0 || seek(rec)) && File::read(&obj,sizeof(T)) == sizeof(T)) ? TRUE:FALSE;
}

template <class T>
bool
RecordFile<T>::write( T& obj , long rec)
{
   return (((rec < 0 || seek(rec)) && File::write(&obj,sizeof(T)) == sizeof(T)) ? TRUE:FALSE;
}

template <class T>
RecordFile<T>&
RecordFile<T>::operator<<(T& obj)
{
    write(obj);

    return *this;
}

template <class T>
RecordFile<T>&
RecordFile<T>::operator>>(T& obj)
{
    read(obj);

    return *this;
}

template <class T>
bool
RecordFile<T>::seek( long rec )
{
    long pos = rec * sizeof(T);

    return (pos == File::seek(pos)) ? TRUE : FALSE;
}

