class CExam {
  public:
    static const int SORT_NONE   = 0;
    static const int SORT_ID     = 1;
    static const int SORT_NAME   = 2;
    static const int SORT_RESULT = 3;

    bool           Load        ( istream      & cardMap );
    bool           Register    ( const string & cardID,
                                 const string & test );
    bool           Assess      ( unsigned int   studentID,
                                 const string & test,
                                 int            result );
    list<CResult>  ListTest    ( const string & testName,
                                 int            sortBy ) const;
    set<unsigned int> ListMissing ( const string & testName ) const;

  private:
    struct Student {
        string name;
        set<string> cards;
        set<string> testsRegistered;
        set<string> testsEvaluated;
    };

    map<string,unsigned int> cards; // cardID, studentID
    map<unsigned int,Student> students; // studentID, student

    vector<CResult> testResults;

    class evalCmp {
        int OPT;
      public:
        evalCmp ( int o ) : OPT( o ) {}

        bool operator () ( const CResult & l, const CResult & r ) {
            switch( OPT ) {
                case SORT_ID:
                    return l.m_StudentID < r.m_StudentID;
                    break;
                case SORT_NAME:
                    return l.m_Name < r.m_Name;
                    break;
                case SORT_RESULT:
                    return l.m_Result > r.m_Result;
            }
            return true;
        }
    };
};

//=============================================================================
bool CExam::Load ( istream & cardMap ) {
    map<unsigned int,Student> newStudents;
    map<string,unsigned int> newCards;

    // read records
    string line;
    while ( getline(cardMap, line) ) {
        unsigned int sID;
        string name, cID, tmp;

        // parse line
        istringstream ss(line);

            // student id, name
        getline( ss, tmp, ':' );
        sID = stoi(tmp);
        getline( ss, name, ':' );
        
        if ( students.find( sID ) != students.end() || newStudents.find( sID ) != newStudents.end() )
            return false; // student's id is not unique
        Student stud = { name };

            // cards
        while ( getline(ss, cID, ',' ) ) {
            istringstream(cID) >> cID; // to remove whitespaces

            if ( cards.find( cID ) != cards.end() || newCards.find( cID ) != newCards.end() )   
                return false; // card is not unique
            
            newCards[cID] = sID;
            stud.cards.insert( cID );
        }

        // add tmp records
        newStudents[sID] = stud;
    }

    // make tmp records permanent
    cards.insert( newCards.begin(), newCards.end() );
    students.insert( newStudents.begin(), newStudents.end() );

    return true;
}

//-------------------------------------------------------------------------
bool CExam::Register ( const string & cardID, const string & test ) {
    Student * stud;
    
    try {
        stud = &students.at( cards.at(cardID) );
    }
    catch (...) {
        return false; // card or student do not exist
    }

    if ( stud->testsRegistered.find( test ) != stud->testsRegistered.end() )
        return false; // already registered
    
    stud->testsRegistered.insert( test );

    return true;
}

//-------------------------------------------------------------------------
bool CExam::Assess ( unsigned int studentID, const string & test, int result ) {
    Student * stud;
    
    try {
        stud = &students.at(studentID);
    }
    catch (...) {
        return false; // student do not exist
    }

    if ( stud->testsRegistered.find( test ) == stud->testsRegistered.end() )
        return false; // not registered
    if ( stud->testsEvaluated.find( test ) != stud->testsEvaluated.end() )
        return false; // already evaluated

    CResult testRes( stud->name, studentID, test, result );
    testResults.push_back( testRes );

    stud->testsEvaluated.insert( test );

    return true;
}

//-------------------------------------------------------------------------
list<CResult> CExam::ListTest ( const string & testName, int sortBy ) const {
    list<CResult> res;

    for ( const auto & tr : testResults )
        if ( tr.m_Test == testName )
            res.push_back( tr );

    if ( sortBy != SORT_NONE )
        res.sort( evalCmp( sortBy ) );

    return res;
}

//-------------------------------------------------------------------------
set<unsigned int> CExam::ListMissing ( const string & testName ) const {
    set<unsigned int> res;

    for ( const auto & s : students )    
        if ( s.second.testsRegistered.find( testName ) != s.second.testsRegistered.end() &&
             s.second.testsEvaluated.find( testName ) == s.second.testsEvaluated.end()
           )
            res.insert( s.first );

    return res;
}
