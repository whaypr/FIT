// < DUMP >
for ( Car * currC = carsHead; currC; currC = currC->next ) {
    cout << currC->rz << " : ";
    for ( Owner * currO = currC->owners; currO; currO = currO->next )
        cout << currO->citizen->name << " " << currO->citizen->surname << ", ";
    cout << endl;
}
cout << "---CITIZENS---" << endl;
for ( Citizen * currCit = citizens; currCit; currCit = currCit->next )
    cout << currCit->name << " " << currCit->surname << endl;
cout << "-------------------------" << endl;
// </ DUMP >
