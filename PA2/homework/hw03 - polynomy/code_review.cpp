                        /***************************\
                        |                           |
                        |    P O L Y N O M I A L    |
                        |                           |
                        \***************************/
/**
 * This file declare and implement data structure representing polynomials and some
 * of the fundamental operations with them.
 */

                            /*******************\
                            |    DECLARATION    |
                            \*******************/

/**
 * Class representing polynomial
 *
 * Polynomial is implemented as singly linked list of members ordered in descending order
 * with "head" pointing on the first one.
 * Can contain zero members.
 */
class CPolynomial {
  public:
                       CPolynomial  ( double number );
                       CPolynomial  ( const CPolynomial & other );
                       ~CPolynomial ();

    CPolynomial &      operator =   ( const CPolynomial & other );
    CPolynomial        operator +   ( const CPolynomial & other ) const;
    CPolynomial        operator -   ( const CPolynomial & other ) const;
    friend CPolynomial operator *   ( const CPolynomial & l, const CPolynomial & r );
    bool               operator ==  ( const CPolynomial & other ) const;
    bool               operator !=  ( const CPolynomial & other ) const;
    double             operator []  ( int expo ) const;
    double &           operator []  ( int expo );
    double             operator ()  ( double x ) const;
    friend ostream &   operator <<  ( ostream & out, const CPolynomial & poly );

    uint               Degree () const;
    friend bool        dumpMatch    ( const CPolynomial & x, const vector<double> & ref );

  private:
    /* Member of the polynomial in a form "coef * x^expo" */
    struct Member {
      int expo;
      double coef;
      Member * next;
    };
    Member * head;

    void copy( const CPolynomial & other );
    void destroy();

    void removeZeros ();
    bool isZero () const;
};
                            /********************\
                            |   IMPLEMENTATION   |
                            \********************/

/* Helper function for comparing doubles with threshold 0.1‰ */
bool isEqual ( double x, double y ) {
    return fabs(x - y) <= 0.0001 * DBL_EPSILON * max( fabs(x), fabs(y) );
}

//----------------------------------------------------------------------------------
/**
 * Make a deep copy of the given polynomial.
 * Used in copy constructor and asignment operator.
 */
void CPolynomial::copy( const CPolynomial & other ) {
    if ( ! other.head )
        head = nullptr;
    else {
        Member * prev = nullptr;
        for ( Member * curr = other.head; curr; curr = curr->next ) {
            // skip possible zeros
            if ( isEqual(curr->coef, 0) )
                continue;

            Member * m = new Member();
            m->expo = curr->expo;
            m->coef = curr->coef;

            if ( prev )
                prev->next = m;
            else
                head = m; // handling the first member

            prev = m;
        }
    }
}

//----------------------------------------------------------------------------------
/**
 * Delete all members (allocated on a heap) to prevent memory leaks.
 * Used in destructor and asignment operator.
 */
void CPolynomial::destroy() {
    Member * curr = head;
    while ( curr ) {
        Member * tmp = curr->next;
        delete curr;
        curr = tmp;
    }
}

//----------------------------------------------------------------------------------
/**
 * Default constructor and the one that accepts double as a coeficient of the constant member (x^0)
 */
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

//----------------------------------------------------------------------------------
/**
 * Copy constructor (deep copy)
 */
CPolynomial::CPolynomial ( const CPolynomial & other ) {
    copy(other);
}

//----------------------------------------------------------------------------------
/**
 * Destructor
 */
CPolynomial::~CPolynomial () {
    destroy();
}

//----------------------------------------------------------------------------------
/**
 * Assignment operator (deep copy)
 *
 * Essentially just destrutor and constructor with equality check on the beginning.
 */
CPolynomial & CPolynomial::operator = ( const CPolynomial & other ) {
    if ( &other == this )
        return *this;

    destroy();
    copy(other);

    return *this;
}

//----------------------------------------------------------------------------------
/**
 * Addition
 *
 * Copy the left polynomial and then iterate through the second one to add its members
 * to the copy.
 */
CPolynomial CPolynomial::operator + ( const CPolynomial & other ) const {
    CPolynomial res = *this; // new (deep) copy done by copy constructor

    for ( Member * curr = other.head; curr; curr = curr->next )
        res[curr->expo] += curr->coef;            

    res.removeZeros(); // iterate through the result poly and remove zero members
    return res;
}

//----------------------------------------------------------------------------------
/**
 * Subtraction
 *
 * Same principle as in addition.
 */
CPolynomial CPolynomial::operator - ( const CPolynomial & other ) const {
    CPolynomial res = *this;

    for ( Member * curr = other.head; curr; curr = curr->next )
        res[curr->expo] -= curr->coef;

    res.removeZeros(); // iterate through the result poly and remove zero members
    return res;
}

//----------------------------------------------------------------------------------
/**
 * Multiplication (naive algorithm)
 *
 * For each member of the left poly iterate through the members of the right poly
 * and store result of their multiplication into the result poly.
 */
CPolynomial operator * ( const CPolynomial & l, const CPolynomial & r ) {
    CPolynomial res;

    for ( CPolynomial::Member * curr_l = l.head; curr_l; curr_l = curr_l->next ) {
        for ( CPolynomial::Member * curr_r = r.head; curr_r; curr_r = curr_r->next ) {
            int expo = curr_l->expo + curr_r->expo;
            double coef = curr_l->coef * curr_r->coef;

            if ( ! isEqual(coef, 0) ) // add only if the result member is not zero
                res[expo] = res[expo] + coef;
        }
    }

    return res;
}

//----------------------------------------------------------------------------------
/**
 * Comparison: equality
 *
 * Compare two polynomials and return true if all their members are
 * equal (same coeficient for each member, no non-zero member is missing or is extra).
 */
bool CPolynomial::operator == ( const CPolynomial & other ) const {
    Member * curr_l = this->head;
    Member * curr_r = other.head;

    // if one of the polynomials is empty, return head of the other one as bool
    if ( ! curr_l )
        return curr_r;
    if ( ! curr_r )
        return curr_l;

    bool atEnd = false;

    while ( curr_l || curr_r ) {
        //skip possible zeros in the beginning
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

        // check other members
        if ( curr_l->expo != curr_r->expo || ! isEqual(curr_l->coef, curr_r->coef) )
            return false;

        curr_l = curr_l->next;
        curr_r = curr_r->next;
    }

    return true;
}

//----------------------------------------------------------------------------------
/**
 * Comparison: inequality
 *
 * Check equality and return the opposite.
 */
bool CPolynomial::operator != ( const CPolynomial & other ) const {
    return ! ( *this == other );
}

//----------------------------------------------------------------------------------
/**
 * Member access: read
 *
 * Traverse the polynomial until member with given exponent is found.
 * If not found, return 0.
 */
double CPolynomial::operator [] ( int expo ) const {
    for ( Member * curr = head; curr; curr = curr->next )
        if ( curr->expo == expo )
            return curr->coef;

    return 0;
}

//----------------------------------------------------------------------------------
/**
 * Member access: write
 *
 * Traverse the polynomial until member with given exponent is found.
 * If found, return pointer to the coeficient of this member.
 * If not found, create a new member and connect it to the list. (There are 
 * 3 disjunct situations for new member can be in: in the beginning, in the middle or at the end.)
 */
double & CPolynomial::operator [] ( int expo ) {
    // new member in the beggining (highest expo)
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

//----------------------------------------------------------------------------------
/**
 * Evaluation
 *
 * Calculate polynomial's value for given x and return it.
 */
double CPolynomial::operator () ( double x ) const {
    double sum = 0;

    for ( Member * curr = head; curr; curr = curr->next )
        sum += curr->coef * pow(x, curr->expo);

    return sum;
}

//----------------------------------------------------------------------------------
/**
 * Printing (friend function)
 *
 * Print out given polynomial to given output stream.
 * Iterate through all members, in each iteration build output string in string stream,
 * send it to given stream and continue to next member. At the end return back the stream.
 * Printing rules:
 *    - don't print "+" if first member
 *    - don't print zero members
 *    - don't print coeficient "1" if it is not constant member
 *    - print "0" if the polynomial has no member or all of them are zeros
 */
ostream & operator << ( ostream & out, const CPolynomial & poly ) { 
    bool first_member = true;
    bool const_member = false;

    for ( CPolynomial::Member * curr = poly.head; curr; curr = curr->next) {
        if ( curr->expo == 0 )
            const_member = true;

        // skip possible zeros
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

    if ( poly.isZero() )
        out << "0";

    return out;
}

//----------------------------------------------------------------------------------
/**
 * Degree of the polynomial
 *
 * Find degree (exponent of the first non-zero member) and return it.
 * Return zero if polynomial has no members or all of them are zeros.
 */
uint CPolynomial::Degree () const {
    Member * curr = head;

    // skip possible zeros
    while ( curr && isEqual(curr->coef, 0) )
        curr = curr->next;
        
    if ( curr )
        return curr->expo;

    return 0;
}

//----------------------------------------------------------------------------------
/**
 * Remove zeros
 *
 * Traverse through the polynomial and remove all zero members from it.
 */
void CPolynomial::removeZeros () {
    if ( ! head )
        return;

    Member * prev = head;
    Member * curr = head->next;

    while ( curr ) {
        if ( isEqual(curr->coef, 0) ) {
            prev->next = curr->next;
            delete curr;
            curr = prev->next;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

//----------------------------------------------------------------------------------
/**
 * Check zero
 *
 * Return true if polynomial has no members or all of them are zeros.
 */
bool CPolynomial::isZero () const {
    for ( Member * curr = head; curr; curr = curr->next )
        if ( ! isEqual(curr->coef, 0) )
            return false;
    
    return true;
}
