#ifndef __PROGTEST__
#include "Maze.h"
#endif

using namespace std;


class Hamster {
public:
    Hamster(const Maze* m):
        M(m),
        jun_number( m->junctions.size() ),
        corridors( vector<list<Junction>>(jun_number) ),
        corridors_reversed( vector<list<Junction>>(jun_number) ),
        found( vector<bool>(jun_number, false) ),
        component( vector<int>(jun_number, -1) ),
        yummies( vector<int>(jun_number, 0 )),
        max_yumm_reach( vector<int>(jun_number, 0 ))
    {}

    /***  RUN  ***/
    int run() {
        //------ create faster corridors structure
        for (const auto& c : M->corridors) {
            corridors[c.first].push_back(c.second);
            corridors_reversed[c.second].push_back(c.first);
        }

        //------ create stack - find 'target' components
        DFS_reversed(M->exit);
        if ( !hamster_found ) return -1;

        //------ identify components and create condensed graph
        while ( ! out_sorted.empty() ) {
            Junction j = out_sorted.top();
            out_sorted.pop();

            if ( component[j] == -1 ) {
                max_yumm = 0;
                DFS_component(j, j);
                max_yumm_reach[j] += yummies[j] + max_yumm;
            }
        }

        return max_yumm_reach[component[M->hamster]];
    }

private:
    /***  DFS_REVERSED  ***/
    void DFS_reversed(const Junction j) {
        found[j] = true;
        if (j == M->hamster) hamster_found = true;

        for (const auto& c : corridors_reversed[j])
            if ( ! found[c] )
                DFS_reversed(c);


        out_sorted.push(j);
    }

    /***  DFS_COMPONENT  ***/
    void DFS_component(const Junction curr, const Junction comp) {
        //cout << "Vertex " << curr << " is in component " << comp << "." << endl;
        component[curr] = comp;
        yummies[comp] += M->junctions[curr];

        for (const auto& next : corridors[curr]) {
            if ( !found[next] ) continue;

            if ( component[next] == -1 ) // edge within the component
                DFS_component(next, comp);
            else if ( component[next] != comp ) // edge to another component
                max_yumm = max( max_yumm, max_yumm_reach[component[next]] );
        }
    }

    const Maze* M;
    size_t jun_number;

    vector<list<Junction>> corridors;
    vector<list<Junction>> corridors_reversed;

    vector<bool> found;
    stack<Junction> out_sorted;
    bool hamster_found = false;

    vector<int> component;
    vector<int> yummies;
    int max_yumm;
    vector<Junction> max_yumm_reach;
};


//-----------------------------------------------------------------
int hungry_hamster(const Maze& M) {
    Hamster hamster(&M);
    return hamster.run();
}
