/* fetocv.cpp */
/* Fraction of Electrons (FE) to Contour Value (CV) Program */
/* Modified by Markus G. S. Weiss on 2024-11-04 */
/* This file is part of OpenCubMan and is licensed under the GNU General Public License v2.0. */
/* See the LICENSE and AUTHORS files for details. */

#include "opencubman.h"

int main(int argc, char* argv[])
{
    Cube cube1;
    double fe;

    if (argc >= 3)
    {
        // Parse input arguments
        fe = atof(argv[2]);

        // Read cube file with auto-detection
        cube1.read_cube(argv[1]);

        // Allocate and sort data
        cube1.sort_allocate();
        cube1.sortf();

        // Calculate Contour Value (CV) for given Fraction of Electrons (FE)
        double cv = cube1.w_forxelectron(fe);

        // Output result
        printf("FE = %.4lf CV = %.4lf\n", fe, cv);
    }
    else
    {
        // Print help message
        cout << " O P E N C U B M A N   P A C K A G E   version 1.00 " << endl;
        cout << endl;
        cout << " Fraction of Electrons (FE) to Contour Value (CV) Program " << endl << endl;
        cout << " This program reads a cube file and calculates the CV corresponding to a selected FE." << endl << endl;
        cout << " Usage: fetocv input.cube FE" << endl << endl;
        cout << " The program auto-detects the cube file format (Orca or Q-Chem) and data type (density or orbital)." << endl;
        cout << " If needed, please ensure that the first two lines of the cube file contain appropriate information." << endl;
    }

    return 0;
}

