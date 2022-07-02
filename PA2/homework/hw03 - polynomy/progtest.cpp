class CPolynomial
{
  public:
    CPolynomial ( double number );                                                  // default constructor
    CPolynomial ( const CPolynomial & other );
    ~CPolynomial ();
    CPolynomial & operator = ( const CPolynomial & other );
    CPolynomial operator + ( const CPolynomial & other ) const;                     // operator +
    CPolynomial operator - ( const CPolynomial & other ) const;                     // operator -
    friend CPolynomial operator * ( const CPolynomial & l, const CPolynomial & r ); // operator * (polynomial, double)
    bool operator == ( const CPolynomial & other ) const;                           // operator ==
    bool operator != ( const CPolynomial & other ) const;                           // operator !=
    double operator [] ( int exponent ) const;                                      // operator [] READ
    double & operator [] ( int exponent );                                          // operator [] WRITE
    double operator () ( double x ) const;                                          // operator ()
    friend ostream & operator << ( ostream & out, const CPolynomial & poly );
    friend bool dumpMatch ( const CPolynomial & x, const vector<double> & ref );

    uint Degree () const;
  private:
    struct Member {
      int expo;
      double coef;
      Member * next;
    };
    Member * head;

    void removeZeros ();
    bool isZero () const;
};
// --------------------------------------------------------------------------------------------- //
 bool isEqual ( double x, double y ) {
     return fabs( x - y ) <= 0.0001 * DBL_EPSILON * max( fabs(x), fabs(y) );
 }
// --------------------------------------------------------------------------------------------- //

#include "CPolynomial.cpp"
