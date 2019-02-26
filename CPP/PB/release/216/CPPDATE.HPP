#define TODAY (-1L)

class Date
   {
      char _d ,
           _m ,
           _y ;

      int compare(Date&)        const;

      static char *weekdays[7];
      static char *s_weekdays[7];
      static char *l_weekdays[7];
      static char *months[12];
      static char *l_months[12];

   public:
      Date();                   // Initializes date to nothing
      Date(long);               // Initializes date to julian/today
      Date(int,int,int);        // Initializes date with specific values

      void operator()(int day,int month,int year) // Equal to set()
         {
            set(day,month,year);
         }

      char& operator[](int i);  // 0 = day , 1 = month , 2 = year

      int day() const           // Returns day (1-31)
         {
            return _d;
         }
      int month() const         // Returns month (1-12)
         {
            return _m;
         }
      int year() const          // Returns year (0-127)
         {
            return _y;
         }

      bool ok() const;           // Returns !=0 if date is valid

      int dayNum() const;       // Returns the number of the day in the year (1-366)
      int weekDay() const;      // Returns the day of week (0=sunday)
      int weekNum() const;      // Returns the week number (1-53)
      int daysInMonth(int = 0) const; // Returns # days in given month
      int daysInYear(int = 0) const;  // Returns # days in given year
      int weeksInYear(int = 0) const; // Returns # weeks in given year

      operator long() const;    // Converts to julian date
      Date& operator=(long);    // Converts julian date to normal date

      void operator++()
         {
            (*this) = long(*this) + 1;
         }
      void operator++(int)
         {
            operator++();
         }
      void operator--()
         {
            (*this) = long(*this) - 1;
         }
      void operator--(int)
         {
            operator--();
         }
      void operator+=(int n)
         {
            (*this) = long(*this) + n;
         }
      void operator-=(int n)
         {
            (*this) = long(*this) - n;
         }

      void set(int,int,int);    // Set date to a specific date
      void today();             // Set date to today

      int operator<(Date&) const;     //  ]
      int operator>(Date&) const;     //  ]
      int operator==(Date&) const;    //  ] Compare
      int operator!=(Date&) const;    //  ]      functions
      int operator<=(Date&) const;    //  ]
      int operator>=(Date&) const;    //  ]

      int operator-(Date&) const;     // Returns difference in number of days

      bool leap()               const;

      void fileDate(word);  // Sets date to packed date
   };
