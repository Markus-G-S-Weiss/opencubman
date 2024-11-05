/* opencubman_typdef.h */
/* Header file for OpenCubMan type definitions */
/* Modified by Markus G. S. Weiss on 2024-11-04 */
/* This file is part of OpenCubMan and is licensed under the GNU General Public License v2.0. */
/* See the LICENSE and AUTHORS files for details. */

#ifndef OPENCUBMAN_TYPEDEF_H
#define OPENCUBMAN_TYPEDEF_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Using the standard namespace
using namespace std;

// Structure for 3D coordinates
typedef struct
{
    double x, y, z;
} XYZ;

// Structure for defining a plane
typedef struct
{
    XYZ o; // Origin point on the plane
    XYZ n; // Normal vector to the plane
} PLANE;

// Structure for cube point data
typedef struct
{
    double w;      // Wavefunction value at the point
    double d;      // Density value at the point (w^2)
    double i;      // Integrated density up to this point
    bool z_flag;   // Zeroing flag (true if point is zeroed)
} cubept;

// Structure for cube file information
typedef struct
{
    XYZ o;           // Origin of the cube grid
    XYZ vx, vy, vz;  // Vectors defining the grid spacing
    int nx, ny, nz;  // Number of grid points along each axis
    int n;           // Total number of grid points (nx * ny * nz)
    int uc;          // Units flag for coordinates (0: Bohr, 1: Angstrom)
    int um;          // Units flag for molecules (unused)
    double integ;    // Total integrated density
    char textline1[256]; // First comment line from the cube file
    char textline2[256]; // Second comment line from the cube file
} cubeinfo;

// Structure for atom information
typedef struct
{
    int ian;     // Atomic number
    double an;   // Atomic mass (unused)
    double x, y, z; // Coordinates of the atom
} atm;

#endif // OPENCUBMAN_TYPEDEF_H

