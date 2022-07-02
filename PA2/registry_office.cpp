class CPerson {
public:
	CPerson ( int i, string n ) : ID( i ), name( n ) {}
	virtual ~CPerson() {}

	int GetID () const { return ID; }
	string GetName () const { return name; }
	size_t CountDescendants () const;
	friend ostream & operator << ( ostream & out, const CPerson & p ) { return p.print( out ); }

	shared_ptr<CPerson> father, mother;
	std::vector< weak_ptr<CPerson> > children;

protected:
	int ID;
	string name;

private:
	virtual ostream & print ( ostream & out ) const = 0;
	set< shared_ptr<CPerson> > GetDescendants () const;
};

//==============================================================================
set< shared_ptr<CPerson> > CPerson::GetDescendants () const {
	set< shared_ptr<CPerson> > res;

	for ( const auto & child_ptr : children ) {
		res.insert(child_ptr.lock());

		set< shared_ptr<CPerson> > tmp = child_ptr.lock()->GetDescendants();
		res.insert( tmp.begin(), tmp.end() );
	}

	return res;
}

//------------------------------------------------------------------
size_t CPerson::CountDescendants () const {
	return GetDescendants().size();
}

class CMan : public CPerson {
public:
    CMan ( int i, string n ) : CPerson( i, n ) {}

private:
    ostream & print ( ostream & out ) const override;
};

//==============================================================================
ostream & CMan::print ( ostream & out ) const {
	out << ID << ": " << name << " (man)";
	return out;
}

class CWoman : public CPerson {
public:
    CWoman ( int i, string n ) : CPerson( i, n ), married( false ) {}
    
    string GetBornName () const { return bornName; }
    bool Wedding ( string newName );

private:
    bool married;
    string bornName;

    ostream & print ( ostream & out ) const override;
};

//==============================================================================
ostream & CWoman::print ( ostream & out ) const {
	out << ID << ": " << name;

	if ( ! married )
		out << " (woman)";
	else
		out << " [born: " << bornName << "] (woman)";

	return out;
}

//------------------------------------------------------------------
bool CWoman::Wedding( string newName ) {
	if ( married )
		return false; // already married

	bornName = name;
	name = newName;
	married = true;

	return true;
}

bool ptrSort ( shared_ptr<CPerson> l, shared_ptr<CPerson> r ) {
	return l->GetID() < r->GetID();
}

//==============================================================================
class CRegister {
public:
    CRegister () {}
    bool Add ( shared_ptr<CPerson> person, shared_ptr<CPerson> father, shared_ptr<CPerson> mother );
    shared_ptr<CPerson> FindByID ( int id ) const;
    vector< shared_ptr<CPerson> > FindByName ( string prefix ) const;
	list< shared_ptr<CPerson> > FindRelatives ( int id1, int id2 ) const;

	map< int, shared_ptr<CPerson> > & get() { return reg; }

private:
	map< int, shared_ptr<CPerson> > reg;

};

//==============================================================================
bool CRegister::Add ( shared_ptr<CPerson> person, shared_ptr<CPerson> father, shared_ptr<CPerson> mother ) {
	if ( reg.find( person->GetID() ) != reg.end() ) {
		return false; // already in the register
	}

	person->mother = mother;
	person->father = father;

	if ( father ) 
		father->children.emplace_back( person );
	if ( mother )
		mother->children.emplace_back( person );

	reg[person->GetID()] = person;

	return true;
}

//------------------------------------------------------------------
shared_ptr<CPerson> CRegister::FindByID ( int id ) const {
	try {
		return reg.at( id );
	} catch (...) {
		return nullptr;
	}
}

//------------------------------------------------------------------
vector< shared_ptr<CPerson> > CRegister::FindByName ( string prefix ) const {
	vector< shared_ptr<CPerson> > res;

	for ( auto & item : reg ) {
		string name = item.second->GetName();

		if ( prefix == name.substr( 0, prefix.size() ) )
			res.push_back( item.second );
		else {
			shared_ptr<CWoman> w = std::dynamic_pointer_cast<CWoman>( item.second );

			// if it's women try her born name
			if ( w ) {
				name = w->GetBornName();

				if ( prefix == name.substr( 0, prefix.size() ) )
				res.push_back( item.second );
			}

		}
	}

	sort( res.begin(), res.end(), ptrSort );

	return res;
}

//------------------------------------------------------------------
list< shared_ptr<CPerson> > CRegister::FindRelatives (int id1, int id2) const {
	list< shared_ptr<CPerson> > res;



	return res;
}
