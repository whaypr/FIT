template <typename Type>
class CSet {
    struct Node {
        Type value;
        Node * next = nullptr;

        Node ( const Type & value ) : value( value ) {} // because default constructor for value is not guaranteed
    };

    size_t size = 0;
    Node * data = nullptr;

public:
    CSet () {}
    
    //--------------------------------------------------------
    CSet ( const CSet & src ) {
        size = src.size;
        data = nullptr;

        Node * prev = nullptr;
        for ( Node * curr = src.data; curr; curr = curr->next ) {
            Node * n = new Node( curr->value );

            if ( prev )
                prev->next = n;
            else
                data = n;

            prev = n;
        }
    }
    
    //--------------------------------------------------------
    ~CSet () {
        Node * curr = data;
        while ( curr ) {
            Node * tmp = curr->next;
            delete curr;
            curr = tmp;
        }
    }
    
    //--------------------------------------------------------
    CSet & operator = ( const CSet & src ) {
        if ( &src == this )
            return *this;
        
        // destructor
        Node * curr = data;
        while ( curr ) {
            Node * tmp = curr->next;
            delete curr;
            curr = tmp;
        }

        // constructor
        size = src.size;
        data = nullptr;

        Node * prev = nullptr;
        for ( Node * curr = src.data; curr; curr = curr->next ) {
            Node * n = new Node( curr->value );

            if ( prev )
                prev->next = n;
            else
                data = n;

            prev = n;
        }

        return *this;
    }
    
    //--------------------------------------------------------
    bool Insert ( Type value ) {
        // set is empty
        if ( ! data ) {
            Node * n = new Node( value );
            data = n;

            size++;
            return true;
        }

        // skip all nodes with value > than the new value
        Node * prev = nullptr;
        Node * curr = data;
        for ( ; curr && value < curr->value; curr = curr->next )
            prev = curr;

        // values are the same ( A<=B && B>=A => A=B )
        if ( curr && ! ( curr->value < value ) )
            return false;

        Node * n = new Node( value );

        if ( prev ) {
            prev->next = n;
            n->next = curr;
        } else {
            n->next = data;
            data = n;
        }

        size++;
        return true;
    }
    
    //--------------------------------------------------------
    bool Remove ( Type value ) {
        Node * prev = nullptr;
        for ( Node * curr = data; curr; curr = curr->next ) {
            if ( ! ( curr->value < value ) && ! ( value < curr->value ) ) {
                prev ? prev->next = curr->next : data = curr->next;
                delete curr;
                
                size--;
                return true;
            }

            prev = curr;
        }

        return false;
    }
    
    //--------------------------------------------------------
    size_t Size () const { return size; }
    
    //--------------------------------------------------------
    bool Contains ( Type value ) const {
        for ( Node * curr = data; curr; curr = curr->next )
            if ( ! ( curr->value < value ) && ! ( value < curr->value ) )
                return true;

        return false;
    }
};
