struct Citizen {
  const char * name;
  const char * surname;
  Citizen * next;

  ~Citizen () {
      free( (char*)name );
      free( (char*)surname );
  }
};

struct CitizenIndex {
    Citizen * citizens = nullptr;

    ~CitizenIndex () {
        Citizen * currCit = citizens;
        while ( currCit ) {
            Citizen * tmp = currCit->next;
            delete currCit;
            currCit = tmp;
        }
    }
};
CitizenIndex citizenIndex;

struct Owner {
    Citizen * citizen;
    Owner * next;
};

struct Car {
    const char * rz;
    int ownersCount;
    Owner * owners;
    Car * next;

    ~Car () {
        free( (char*)rz );

        Owner * currO = owners;
        while ( currO ) {
            Owner * tmp = currO->next;
            delete currO;
            currO = tmp;
        }
    }
};

// ********************************************************************************************** //
class CCarList {
  public:
    CCarList ( const Car * cars, const char * name, const char * surname ) {
        this->name = name;
        this->surname = surname;
        currC = cars;

        if ( strcmp( currC->owners->citizen->name, name ) || strcmp( currC->owners->citizen->surname, surname ) )
            Next();
    }
    
    const char   * RZ           () const { return currC->rz; }
    bool           AtEnd        () const { return ! currC; }
    void           Next         () {
        for ( currC = currC->next; currC; currC = currC->next )
            if ( ! strcmp( currC->owners->citizen->name, name ) && ! strcmp( currC->owners->citizen->surname, surname ) )
                break;
    }

  private:
    const Car * currC;
    const char * name;
    const char * surname;
};

class COwnerList {
  public:
    COwnerList ( const Owner * owners ) : currO ( owners ) {}

    const char   * Name         () const { return currO->citizen->name; }
    const char   * Surname      () const { return currO->citizen->surname; }
    bool           AtEnd        () const { return ! currO; }
    void           Next         () { currO = currO->next; }
  
  private:
    const Owner * currO;
};

class CRegister {
  public:
    CRegister ();
    ~CRegister ();
    CRegister ( const CRegister & other );
    CRegister & operator = ( const CRegister & other );

    bool           AddCar       ( const char      * rz,
                                  const char      * name,
                                  const char      * surname );
    bool           DelCar       ( const char      * rz );
    bool           Transfer     ( const char      * rz,
                                  const char      * nName,
                                  const char      * nSurname );
    CCarList       ListCars     ( const char      * name,
                                  const char      * surname ) const;
    int            CountCars    ( const char      * name,
                                  const char      * surname ) const;
    COwnerList     ListOwners   ( const char      * RZ ) const;
    int            CountOwners  ( const char      * RZ ) const;
  
  private:
    Car * carsHead;
    Car * carsTail;

    Citizen * findCitizen ( const char * name, const char * surname ) {
        Citizen * prevCit = citizenIndex.citizens;
        for ( Citizen * currCit = citizenIndex.citizens; currCit; currCit = currCit->next ) {
            if ( ! strcmp( currCit->name, name ) && ! strcmp( currCit->surname, surname ) )
                return currCit; // citizen found

            prevCit = currCit;
        }

        // not found -> add
        Citizen * cit = new Citizen();
        cit->name = strdup(name);
        cit->surname = strdup(surname);
        if (  ! citizenIndex.citizens )
            citizenIndex.citizens = cit;
        else
            prevCit->next = cit;

        return cit;
    }
};

// ********************************************************************************************** //
#include "CRegister.cpp"
