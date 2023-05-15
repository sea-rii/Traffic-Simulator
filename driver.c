#include <stdlib.h>
#include <stdio.h>

#include "trafficSimulator.h"

int main( int argc, char *argv[] )
{
    int i;
    int numFiles = 7;
    /* defaultFilenames contains file names to be tested.  Set i-th int in enableTest to disable one the i-th test */
    char *defaultFilenames[] = {"data-Trivial1.txt", "data-Trivial2.txt", "data-Simple.txt", "data-Merge.txt", "data-Complex.txt", "data-NearGridlock.txt", "data-Gridlock.txt"};
    int enableTest[]         = { 1                , 1                  , 1                , 1               , 1                  , 1                      , 1    };

    if( argc==2 ){ /* Usage: ./driver <filename> */
        printf("\n\n--------------- START OF OUTPUT FOR %s -----------------\n\n", argv[1]);
        TrafficData* pTrafficData = createTrafficData( argv[1] );
        printf("\nSIMULATION\n");
        trafficSimulator( pTrafficData );
        printf("\nFREE DATA\n");
        freeTrafficData( pTrafficData );
        printf("\n--------------- END OF OUTPUT FOR %s -----------------\n\n", argv[1]);
    }
    else{
        printNames( );
        printf("\nRunning default test files: \n\n");

        for( i=0; i<numFiles; i++ ){
            if( enableTest[i]== 1 ){
                printf("\n\n--------------- START OF OUTPUT FOR %s -----------------\n\n", defaultFilenames[i]);
                TrafficData* pTrafficData = createTrafficData( defaultFilenames[i] );
                printf("\nSIMULATION\n");
                trafficSimulator( pTrafficData );
                printf("\nFREE DATA\n");
                freeTrafficData( pTrafficData );
                printf("\n--------------- END OF OUTPUT FOR %s -----------------\n\n", defaultFilenames[i]);
            }
            else
                printf("\n\n--------------- SKIPPED TESTING FOR %s -----------------\n\n", defaultFilenames[i]);
        }
    }

    return 0;
}
