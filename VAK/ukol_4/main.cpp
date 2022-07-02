#include <iostream>
#include <queue>
#include <vector>

using namespace std;


int main() {
    // read number of men and woman;
    int men_number, women_number;
    cin >> men_number >> women_number;


    // prepare data structures
    vector<int> current_woman( men_number, -1 ); // each man's current woman
    vector<queue<int>> men_preferences( men_number ); // each man's woman preferences, starting from the best ones
    queue<int> proposing_men; // queue of all men currently without women

    vector<int> current_man( women_number, -1 ); // each woman's current man
    vector<vector<int>> women_preferences; // each woman's man preferences: men are ids in the inner vector, the higher value the higher preference
    for (int i = 0; i < women_number; ++i)
        women_preferences.push_back( vector<int>(men_number) );

    int number_preferences; // helper var


    // read data and fill in the structures
    for (int man = 0; man < men_number; ++man) {
        cin >> number_preferences;

        while (number_preferences--) {
            int woman;
            cin >> woman;
            men_preferences[man].push(woman - men_number - 1 );
        }

        proposing_men.push(man); // all men are single at start
    }

    for (int woman = 0; woman < women_number; ++woman) {
        cin >> number_preferences;

        for (int i = number_preferences; i > 0; i--) {
            int man;
            cin >> man;
            women_preferences[woman][man-1] = i; // the higher value the higher preference
        }
    }


    // process input data
    while ( !proposing_men.empty() ) { // there are still single men with women they didn't try to propose
        int man = proposing_men.front();
        proposing_men.pop();

        if ( men_preferences[man].empty() ) continue; // no more women to try

        int woman = men_preferences[man].front();
        men_preferences[man].pop();

        int& cm = current_man[woman];

        // woman accepts the offer
        if ( women_preferences[woman][man] > women_preferences[woman][cm] ) {
            // bud already had someone before
            if ( cm != -1 ) {
                proposing_men.push(cm);
                current_woman[cm] = -1;
            }

            cm = man;
            current_woman[man] = woman;
        // woman declines
        } else
            proposing_men.push(man);
    }


    // print results
    for (const auto c : current_woman)
        cout << ( c == -1 ? -1 : (c + men_number + 1) ) << endl;
    for (const auto c : current_man)
        cout << ( c == -1 ? -1 : (c + 1) ) << endl;


    return 0;
}
