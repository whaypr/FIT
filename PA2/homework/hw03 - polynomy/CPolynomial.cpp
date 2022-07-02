CPolynomial::CPolynomial ( double number = 0 ) {
    if ( number == 0 )
        head = nullptr;
    else {
        Member * m = new Member();
        m->expo = 0;
        m->coef = number;
        m->next = nullptr;

        head = m;
    }
}
// --------------------------------------------------------------------------------------------- //
CPolynomial::CPolynomial ( const CPolynomial & other ) {
    if ( ! other.head )
        head = nullptr;
    else {
        // head
        Member * m = new Member();
        m->expo = other.head->expo;
        m->coef = other.head->coef;
        m->next = nullptr;

        head = m;

        // other members
        Member * prev = head;
        for ( Member * curr = other.head->next; curr; curr = curr->next ) {
            Member * m = new Member();
            m->expo = curr->expo;
            m->coef = curr->coef;
            prev->next = m;

            prev = m;
        }
    }
}
// --------------------------------------------------------------------------------------------- //
CPolynomial::~CPolynomial () {
    Member * curr = head;
    while ( curr ) {
        Member * tmp = curr->next;
        delete curr;
        curr = tmp;
    }
}
// --------------------------------------------------------------------------------------------- //
CPolynomial & CPolynomial::operator = ( const CPolynomial & other ) {
    if ( &other == this )
        return *this;

    // same as destructor
    Member * curr = head;
    while ( curr ) {
        Member * tmp = curr->next;
        delete curr;
        curr = tmp;
    }

    // same as constructor
    if ( ! other.head )
        head = nullptr;
    else {
        // head
        Member * m = new Member();
        m->expo = other.head->expo;
        m->coef = other.head->coef;
        m->next = nullptr;

        head = m;

        // other members
        Member * prev = head;
        for ( Member * curr = other.head->next; curr; curr = curr->next ) {
            Member * m = new Member();
            m->expo = curr->expo;
            m->coef = curr->coef;
            prev->next = m;

            prev = m;
        }
    }

    return *this;
}
// --------------------------------------------------------------------------------------------- //
CPolynomial CPolynomial::operator + ( const CPolynomial & other ) const {
    CPolynomial res = *this;

    for ( Member * curr = other.head; curr; curr = curr->next )
        res[curr->expo] += curr->coef;

    res.removeZeros();
    return res;
}
// --------------------------------------------------------------------------------------------- //
CPolynomial CPolynomial::operator - ( const CPolynomial & other ) const {
    CPolynomial res = *this;

    for ( Member * curr = other.head; curr; curr = curr->next )
        res[curr->expo] -= curr->coef;

    res.removeZeros();
    return res;
}
// --------------------------------------------------------------------------------------------- //
CPolynomial operator * ( const CPolynomial & l, const CPolynomial & r ) {
    CPolynomial res;

    for ( CPolynomial::Member * curr_l = l.head; curr_l; curr_l = curr_l->next ) {
        for ( CPolynomial::Member * curr_r = r.head; curr_r; curr_r = curr_r->next ) {
            int expo = curr_l->expo + curr_r->expo;
            double coef = curr_l->coef * curr_r->coef;

            if ( ! isEqual(coef, 0) )
                res[expo] = res[expo] + coef;
        }
    }

    return res;
}
// --------------------------------------------------------------------------------------------- //
bool CPolynomial::operator == ( const CPolynomial & other ) const {
    Member * curr_l = this->head;
    Member * curr_r = other.head;

    if ( ! curr_l )
        return curr_r;
    if ( ! curr_r )
        return curr_l;

    bool atEnd = false;

    while ( curr_l || curr_r ) {
        //skip zeroes
        while ( isEqual(curr_l->coef, 0) ) {
            curr_l = curr_l->next;
            if ( ! curr_l ) {
                atEnd = true;
                break;
            }
        }
        while ( isEqual(curr_r->coef, 0) ) {
            curr_r = curr_r->next;
            if ( ! curr_r )
                return atEnd;
        }

        // check non-zero members
        if ( curr_l->expo != curr_r->expo || ! isEqual(curr_l->coef, curr_r->coef) )
            return false;

        curr_l = curr_l->next;
        curr_r = curr_r->next;
    }

    return true;
}
// --------------------------------------------------------------------------------------------- //
bool CPolynomial::operator != ( const CPolynomial & other ) const {
    return ! ( *this == other );
}
// --------------------------------------------------------------------------------------------- //
double CPolynomial::operator [] ( int expo ) const {
    for ( Member * curr = head; curr; curr = curr->next )
        if ( curr->expo == expo )
            return curr->coef;

    return 0;
}
// --------------------------------------------------------------------------------------------- //
double & CPolynomial::operator [] ( int expo ) {
    // new member at the beggining (highest expo)
    if ( ! head || expo > head->expo ) {
        Member * m = new Member();
        m->expo = expo;
        m->next = head;
        head = m;
        return m->coef;
    }

    Member * prev = nullptr;
    for ( Member * curr = head; curr; curr = curr->next ) {
        // rewrite expo's coef
        if ( curr->expo == expo )
            return curr->coef;

        // new member between two members
        if ( expo > curr->expo && expo < prev->expo ) {
            Member * m = new Member();
            m->expo = expo;
            m->next = curr;
            prev->next = m;
            return m->coef;
        }

        prev = curr;   
    }

    // new member at the end (smallest expo)
    Member * m = new Member();
    m->expo = expo;
    m->next = nullptr;
    prev->next = m;

    return m->coef;
}
// --------------------------------------------------------------------------------------------- //
double CPolynomial::operator () ( double x ) const {
    double sum = 0;

    for ( Member * curr = head; curr; curr = curr->next )
        sum += curr->coef * pow(x, curr->expo);

    return sum;
}
// --------------------------------------------------------------------------------------------- //
ostream & operator << ( ostream & out, const CPolynomial & poly ) { 
    bool first_member = true;
    bool const_member = false;

    for ( CPolynomial::Member * curr = poly.head; curr; curr = curr->next) {
        if ( curr->expo == 0 )
            const_member = true;

        // there can still be zero memebers in poly
        if ( isEqual(curr->coef, 0) )
            continue;

        stringstream ss;

        // sign
        if ( curr->coef < 0 )
            first_member ? ss << "- " : ss << " - ";
        else // if ( curr->coef > 0 )
            first_member ? ss << "" : ss << " + ";

        // coef
        if ( isEqual(abs(curr->coef), 1) )
            const_member ? ss << "1" : ss << "";
        else /* if ( abs(curr->coef) != 1 ) */ {
            ss << abs(curr->coef);
            const_member ? ss << "" : ss << "*";
        }

        // expo
        if ( const_member )
            ss << "";
        else
            ss << "x^" << curr->expo;

        out << ss.str();

        first_member = false;
    }

    if ( ! poly.head || poly.isZero() )
        out << "0";

    return out;
}
// --------------------------------------------------------------------------------------------- //
uint CPolynomial::Degree () const {
    Member * curr = head;

    // skip possible zeros
    while ( curr && isEqual( curr->coef, 0 ) )
        curr = curr->next;
        
    if ( curr )
        return curr->expo;

    return 0;
}
// --------------------------------------------------------------------------------------------- //
void CPolynomial::removeZeros () {
    if ( ! head )
        return;

    Member * prev = head;
    Member * curr = head->next;

    while ( curr ) {
        if ( isEqual( curr->coef, 0 ) ) {
            prev->next = curr->next;
            delete curr;
            curr = prev->next;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}
// --------------------------------------------------------------------------------------------- //
bool CPolynomial::isZero () const {
    for ( Member * curr = head; curr; curr = curr->next )
        if ( ! isEqual(curr->coef, 0) )
            return false;
    
    return true;
}
