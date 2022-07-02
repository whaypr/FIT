CRegister::CRegister () {
    carsHead = carsTail = nullptr;
}

// ----------------------------------------------------------------------------------------------------- //
CRegister::~CRegister () {
    Car * currC = carsHead;
    while ( currC ) {
        Car * tmp = currC->next;
        delete currC;
        currC = tmp;
    }
}

// ----------------------------------------------------------------------------------------------------- //
CRegister::CRegister ( const CRegister & other ) {
    // cars
    Car * prevC = nullptr;
    for ( Car * otherC = other.carsHead; otherC; otherC = otherC->next ) {
        Car * c = new Car();
        c->rz = strdup(otherC->rz);
        c->ownersCount = otherC->ownersCount;

        if ( prevC )
            prevC->next = c;
        else 
            carsHead = c;

        prevC = c;

        // owners
        Owner * prevO = nullptr;
        for ( Owner * otherO = otherC->owners; otherO; otherO = otherO->next ) {
            Owner * o = new Owner();
            o->citizen = otherO->citizen;
            
            if ( prevO )
                prevO->next = o;
            else
                c->owners = o;

            prevO = o;
        }

        if ( ! otherC->next ) // carsTail
            carsTail = c;
    }
}

// ----------------------------------------------------------------------------------------------------- //
CRegister & CRegister::operator = ( const CRegister & other ) {
    if ( &other == this )
        return *this;

    // DESCTRUCTOR
    Car * currC = carsHead;
    while ( currC ) {
        Car * tmp = currC->next;
        delete currC;
        currC = tmp;
    }

    // CONSTRUCTOR
    // cars
    Car * prevC = nullptr;
    for ( Car * otherC = other.carsHead; otherC; otherC = otherC->next ) {
        Car * c = new Car();
        c->rz = strdup(otherC->rz);
        c->ownersCount = otherC->ownersCount;

        if ( prevC )
            prevC->next = c;
        else 
            carsHead = c;

        prevC = c;

        // owners
        Owner * prevO = nullptr;
        for ( Owner * otherO = otherC->owners; otherO; otherO = otherO->next ) {
            Owner * o = new Owner();
            o->citizen = otherO->citizen;
            
            if ( prevO )
                prevO->next = o;
            else
                c->owners = o;

            prevO = o;
        }

        if ( ! otherC->next ) // carsTail
            carsTail = c;
    }

    return *this;
}

// ----------------------------------------------------------------------------------------------------- //
bool CRegister::AddCar ( const char * rz, const char * name, const char * surname ) {
    Citizen * currCit = findCitizen( name, surname );

    // check car duplicate
    for ( Car * currC = carsHead; currC; currC = currC->next)
        if ( ! strcmp( currC->rz, rz ) )
            return false; // car is already in the list

    // add car to the cars list
    Owner * o = new Owner();
    o->citizen = currCit;

    Car * c = new Car();
    c->rz = strdup(rz);
    c->ownersCount = 1;
    c->owners = o;

    if ( ! carsHead ) 
        carsHead = carsTail = c;
    else {
        carsTail->next = c;
        carsTail = c; // add car at the end of the cars list
    }

    return true;
}

// ----------------------------------------------------------------------------------------------------- //
bool CRegister::DelCar ( const char * rz ) {
    Car * prevC = carsHead;
    for ( Car * currC = carsHead; currC; currC = currC->next ) {
        if ( ! strcmp( currC->rz, rz ) ) {
            prevC->next = currC->next;

            if ( currC == carsHead ) // update head
                carsHead = currC->next;
            if ( ! currC->next ) // update tail
                carsTail = prevC;

            delete currC;
            return true;
        }

        prevC = currC;
    }

    return false; // car to delete was not found
}

// ----------------------------------------------------------------------------------------------------- //
bool CRegister::Transfer ( const char * rz, const char * nName, const char * nSurname ) {
    Citizen * currCit = findCitizen( nName, nSurname );

    // find car
    bool found = false;
    Car * currC = carsHead;
    for ( ; currC; currC = currC->next ) {
        if ( ! strcmp( currC->rz, rz ) ) {
            found = true;
            break;
        }
    }

    if ( ! found || ( ! strcmp( currC->owners->citizen->name, nName ) && ! strcmp( currC->owners->citizen->surname, nSurname ) ) )
        return false; // car to transfer was not found or old and new owner is the same person

    // if new owner owned this car in the past, remove him from the owners list
    Owner * prevO = currC->owners;
    for ( Owner * currO = currC->owners; currO; currO = currO->next ) {
        if ( ! strcmp( currO->citizen->name, nName ) && ! strcmp( currO->citizen->surname, nSurname ) ) {
            prevO->next = currO->next;
            delete currO;

            currC->ownersCount--; // to negate below incrementation
            break;
        }
    }

    // add new owner at the beginning of the list
    Owner * o = new Owner();
    o->citizen = currCit;
    o->next = currC->owners;

    currC->owners = o;
    currC->ownersCount++;

    return true;
}

// ----------------------------------------------------------------------------------------------------- //
CCarList CRegister::ListCars ( const char * name, const char * surname ) const {
    return CCarList( carsHead, name, surname );
}

// ----------------------------------------------------------------------------------------------------- //
int CRegister::CountCars ( const char * name, const char * surname ) const {
    int sum = 0;

    for ( Car * currC = carsHead; currC; currC = currC->next )
        if ( ! strcmp( currC->owners->citizen->name, name ) && ! strcmp( currC->owners->citizen->surname, surname ) )
            sum++;

    return sum;
}

// ----------------------------------------------------------------------------------------------------- //
COwnerList CRegister::ListOwners ( const char * RZ ) const {
    for ( Car * currC = carsHead; currC; currC = currC->next )
        if ( ! strcmp( currC->rz, RZ ) )
            return COwnerList( currC->owners );

    return COwnerList( nullptr ); // car not found
}

// ----------------------------------------------------------------------------------------------------- //
int CRegister::CountOwners ( const char * RZ ) const {
    for ( Car * currC = carsHead; currC; currC = currC->next )
        if ( ! strcmp( currC->rz, RZ ) )
            return currC->ownersCount;

    return 0; // car not found
}
