/* opencubman.h */
/* Header file for the Cube class */

#ifndef OPENCUBMAN_H
#define OPENCUBMAN_H

#include "opencubman_typdef.h"

// Forward declaration of the Cube class
class Cube;

// Extern declaration of cubepointer used in the comparator function
extern Cube* cubepointer;

/* Cube class definition */
class Cube {
private:
    int natoms;       // Number of atoms
    atm* atoms;       // Array of atom structures
    bool sort_flag;   // Flag indicating if sorting arrays are allocated

public:
    cubept* c;        // Array of cube points
    cubeinfo ci;      // Cube file information
    int* sort;        // Array for sorting indices
    int printlevel;   // Verbosity level

    // Constructor
    Cube();

    // Destructor
    ~Cube();

    // Memory allocation and deallocation
    void allocate(int n, int na);
    void deinit();

    // Cube file reading and writing
    void read_cube(const char* filename);
    void write_cube(const char* filename);
    void write_cube_density(const char* filename);

    // Data processing functions
    void sort_allocate();
    void sortf();
    void scale_d(double val);
    void reset_z_flag();

    // Zeroing functions
    void zero_above_w(double w);
    void zero_below_w(double w);
    void zero_above_d(double d);
    void zero_below_d(double d);
    void zero_plane(PLANE p, int d);

    // Coordinate transformations
    XYZ get_cube_point(int x, int y, int z);
    int get_n_forcubexyz(int ix, int iy, int iz);
    void movebyvector(double x, double y, double z);

    // Electron fraction and contour value calculations
    int n_forxelectron(double x);
    int n_forw(double w);
    double w_forxelectron(double x);
    double x_forw(double w);

    // Accessor functions
    double get_cube_element_w(int i);
    double get_cube_element_d(int i);

    // Internal cube writing function
    void write_cube_new(const char* filename, int opt, double va);
};

// Comparator function for sorting
int comparator(const void* a, const void* b);

#endif // OPENCUBMAN_H

