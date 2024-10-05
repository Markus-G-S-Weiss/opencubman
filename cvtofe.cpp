/* Contour Value (CV) to Fraction of Electron (Fe) Program */

#include "opencubman.h"

int main(int argc, char* argv[])
{
    Cube cube1;
    double cv;
    int orbn;

    if (argc == 3 || argc == 4)
    {
        // Parse input arguments
        cv = atof(argv[2]);

        // Default to orbital number 0
        orbn = 0;
        if (argc == 4)
            orbn = atoi(argv[3]);

        // Read cube file
        cube1.read_cube(argv[1], orbn);

        // Allocate and sort data
        cube1.sort_allocate();
        cube1.sortf();

        // Calculate Fraction of Electron (Fe) for given Contour Value (CV)
        double fe = cube1.x_forw(cv);

        // Output result
        printf("CV= %lf Fe= %lf\n", cv, fe);
    }
    else
    {
        // Print help message
        cout << " O P E N C U B M A N   P A C K A G E   version 1.00 " << endl;
        cout << endl;
        cout << " Contour Value (CV) to Fraction of Electron (Fe) Program " << endl << endl;
        cout << " This program reads a cube file and calculates the Fe corresponding to a selected CV." << endl << endl;
        cout << " Usage: cvtofe input.cube CV (orbital) " << endl << endl;
        cout << " where CV is a float number, and (orbital) is an optional parameter" << endl;
        cout << " specifying which orbital to consider (starting from 0, default is 0)." << endl;
    }

    return 0;
}

