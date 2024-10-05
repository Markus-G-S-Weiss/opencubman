/* Fraction of Electron (Fe) to Contour Value (CV) Program */

#include "opencubman.h"

int main(int argc, char* argv[])
{
    Cube cube1;
    double fe;
    int orbn;

    if (argc == 3 || argc == 4)
    {
        // Parse input arguments
        fe = atof(argv[2]);

        // Default to orbital number 0
        orbn = 0;
        if (argc == 4)
            orbn = atoi(argv[3]);

        // Read cube file
        cube1.read_cube(argv[1], orbn);

        // Allocate and sort data
        cube1.sort_allocate();
        cube1.sortf();

        // Calculate Contour Value (CV) for given Fraction of Electron (Fe)
        double cv = cube1.w_forxelectron(fe);

        // Output result
        printf("Fe= %lf CV= %lf\n", fe, cv);
    }
    else
    {
        // Print help message
        cout << " O P E N C U B M A N   P A C K A G E   version 1.00 " << endl;
        cout << endl;
        cout << " Fraction of Electron (Fe) to Contour Value (CV) Program " << endl << endl;
        cout << " This program reads a cube file and calculates the CV corresponding to a selected Fe." << endl << endl;
        cout << " Usage: fetocv input.cube Fe (orbital) " << endl << endl;
        cout << " where Fe is a float number between 0 and 1, and (orbital) is an optional parameter" << endl;
        cout << " specifying which orbital to consider (starting from 0, default is 0)." << endl;
    }

    return 0;
}

