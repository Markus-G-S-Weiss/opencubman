/* Implementation of the Cube class functions */

#include "opencubman.h"

// Global pointer used in comparator function
Cube* cubepointer;

// Comparator function for sorting cube points based on density magnitude
int comparator(const void* a, const void* b)
{
    int e = *(int*)a;
    int f = *(int*)b;

    // Compare based on absolute value of the density
    double diff = fabs(cubepointer->c[e].d) - fabs(cubepointer->c[f].d);

    if (diff < 0.0)
        return 1;
    else if (diff > 0.0)
        return -1;
    else
        return 0;
}

// Constructor
Cube::Cube()
{
    ci.n = 0;
    ci.uc = 0;
    ci.um = 0;
    sort_flag = false;
    printlevel = 0;
    atoms = nullptr;
    c = nullptr;
    sort = nullptr;
}

// Destructor
Cube::~Cube()
{
    deinit();
}

// Allocate memory for cube points and atoms
void Cube::allocate(int n, int na)
{
    c = new cubept[n];
    atoms = new atm[na];

    for (int i = 0; i < n; i++)
    {
        c[i].w = 0.0;
        c[i].d = 0.0;
        c[i].z_flag = false;
    }
}

// Deallocate memory
void Cube::deinit()
{
    delete[] c;
    delete[] atoms;
    if (sort_flag)
        delete[] sort;
}

// Read cube file and store orbital number 'orbn'
void Cube::read_cube(const char* filename, int orbn)
{
    fstream file1;
    int i, x, y, z;
    int norb;
    double v;
    int count;

    file1.open(filename);
    if (file1.is_open())
    {
        // Read comment lines
        file1.getline(ci.textline1, 256);
        file1.getline(ci.textline2, 256);

        // Read number of atoms and origin
        file1 >> natoms >> ci.o.x >> ci.o.y >> ci.o.z;

        // Check for negative atom count indicating unit cell
        if (natoms < 0)
        {
            natoms = -natoms;
            ci.uc = 0;
        }
        else
        {
            ci.uc = 1;
        }

        // Read grid dimensions and vectors
        file1 >> ci.nx >> ci.vx.x >> ci.vx.y >> ci.vx.z;
        if (ci.nx < 0)
        {
            ci.nx = -ci.nx;
            ci.um = 0;
        }
        else
        {
            ci.um = 1;
        }

        file1 >> ci.ny >> ci.vy.x >> ci.vy.y >> ci.vy.z;
        file1 >> ci.nz >> ci.vz.x >> ci.vz.y >> ci.vz.z;

        ci.n = ci.nx * ci.ny * ci.nz; // Total number of grid points

        // Allocate memory
        allocate(ci.n, natoms);

        // Read atom data
        for (i = 0; i < natoms; i++)
        {
            file1 >> atoms[i].ian >> atoms[i].an >> atoms[i].x >> atoms[i].y >> atoms[i].z;
        }

        // Set the cube pointer for use in comparator
        cubepointer = this;

        if (orbn >= 0)
        {
            // If orbital number is specified, assume one orbital
            norb = 1;
        }

        count = 0;
        // Read data values
        for (x = 0; x < ci.nx; x++)
        {
            for (y = 0; y < ci.ny; y++)
            {
                for (z = 0; z < ci.nz; z++)
                {
                    if (orbn >= 0)
                    {
                        // Read wavefunction values
                        for (i = 0; i < norb; i++)
                        {
                            file1 >> v;

                            if (i == orbn)
                            {
                                c[count].w = v;
                                c[count].d = v * v; // Calculate density
                                count++;
                            }
                        }
                    }
                    else
                    {
                        // Read density values
                        file1 >> v;
                        c[count].w = v;       // Store wavefunction value
                        c[count].d = v * v;   // Calculate density
                        count++;
                    }
                }
            }
        }

        file1.close();
    }
    else
    {
        cout << "Error opening cube file " << filename << endl;
    }
}

// Allocate memory for sorting indices
void Cube::sort_allocate()
{
    sort = new int[ci.n];

    for (int j = 0; j < ci.n; j++)
    {
        sort[j] = j;
    }

    sort_flag = true;
}

// Sorting function
void Cube::sortf()
{
    ci.integ = 0.0; // Initialize total integrated density

    // Sort the indices based on density magnitude
    qsort(sort, ci.n, sizeof(int), comparator);

    // Calculate cumulative integrated density
    for (int i = 0; i < ci.n; i++)
    {
        if (i == 0)
        {
            c[sort[i]].i = fabs(c[sort[i]].d);
        }
        else
        {
            c[sort[i]].i = fabs(c[sort[i]].d) + c[sort[i - 1]].i;
        }
        ci.integ += fabs(c[sort[i]].d);
    }

    // Debug: Print out ci.integ to check the total integrated density
    // printf("Total integrated density (ci.integ) = %lf\n", ci.integ);
}

// Get cube point coordinates for indices x, y, z
XYZ Cube::get_cube_point(int x, int y, int z)
{
    XYZ p;
    p.x = ci.o.x + x * ci.vx.x + y * ci.vy.x + z * ci.vz.x;
    p.y = ci.o.y + x * ci.vx.y + y * ci.vy.y + z * ci.vz.y;
    p.z = ci.o.z + x * ci.vx.z + y * ci.vy.z + z * ci.vz.z;
    return p;
}

// Zeroing functions based on wavefunction or density values
void Cube::zero_above_w(double w)
{
    for (int i = 0; i < ci.n; i++)
    {
        if (fabs(c[i].w) > fabs(w))
            c[i].z_flag = true;
    }
}

void Cube::zero_below_w(double w)
{
    for (int i = 0; i < ci.n; i++)
    {
        if (fabs(c[i].w) < fabs(w))
            c[i].z_flag = true;
    }
}

void Cube::zero_above_d(double d)
{
    for (int i = 0; i < ci.n; i++)
    {
        if (c[i].d > d)
            c[i].z_flag = true;
    }
}

void Cube::zero_below_d(double d)
{
    for (int i = 0; i < ci.n; i++)
    {
        if (c[i].d < d)
            c[i].z_flag = true;
    }
}

// Zeroing function based on a plane
void Cube::zero_plane(PLANE p, int d)
{
    double r = sqrt(p.n.x * p.n.x + p.n.y * p.n.y + p.n.z * p.n.z);
    int j = 0;

    for (int x = 0; x < ci.nx; x++)
    {
        for (int y = 0; y < ci.ny; y++)
        {
            for (int z = 0; z < ci.nz; z++)
            {
                XYZ cpt = get_cube_point(x, y, z);
                double dist = p.n.x * (p.o.x - cpt.x) + p.n.y * (p.o.y - cpt.y) + p.n.z * (p.o.z - cpt.z);
                dist = dist / r;

                if ((dist > 0 && d > 0) || (dist < 0 && d < 0))
                    c[j].z_flag = true;

                j++;
            }
        }
    }
}

// Move cube by a vector
void Cube::movebyvector(double x, double y, double z)
{
    ci.o.x += x;
    ci.o.y += y;
    ci.o.z += z;

    for (int i = 0; i < natoms; i++)
    {
        atoms[i].x += x;
        atoms[i].y += y;
        atoms[i].z += z;
    }
}

const double epsilon = 1e-8;  // Define a small tolerance value

// Find index corresponding to a fraction of electron density
int Cube::n_forxelectron(double x)
{
    int n = ci.n - 1; // Initialize to the last valid index

    if (x >= 1.0 - epsilon)  // Edge case when Fe is very close to 1.0
    {
        return n;  // Use the last index
    }

    if (x <= epsilon)  // Edge case when Fe is very close to 0.0
    {
        return 0;  // Use the first index
    }

    for (int i = 0; i < ci.n; i++)
    {
        if (c[sort[i]].i / ci.integ > x + epsilon)
        {
            n = i;
            break;
        }
    }
    return n;
}

// Find the index corresponding to the given contour value (w)
int Cube::n_forw(double w)
{
    w = fabs(w);      // Ensure w is positive
    int n = ci.n;     // Initialize n to total number of points

    if (w <= epsilon)  // Edge case for very small contour values
    {
        return ci.n - 1; // Return the last index where the density is highest
    }

    for (int i = 0; i < ci.n; i++)
    {
        if (fabs(c[sort[i]].w) < w - epsilon) // Compare with tolerance
        {
            n = i;
            break;
        }
    }
    return n - 1; // Return the last index where |ψ| ≥ w
}

// Get contour value (CV) for a fraction of electron density
double Cube::w_forxelectron(double x)
{
    double cv = c[sort[n_forxelectron(x)]].w;
    return fabs(cv); // Return absolute value to ensure positive CV
}

// Get fraction of electron density (Fe) for a given contour value (CV)
double Cube::x_forw(double w)
{
    int n = n_forw(w); // Find the index for the given CV value
    return c[sort[n]].i / ci.integ; // Calculate and return the fraction of electron density
}

// Reset zeroing flags
void Cube::reset_z_flag()
{
    for (int i = 0; i < ci.n; i++)
    {
        c[i].z_flag = false;
    }
}

// Write cube file with wavefunction values
void Cube::write_cube(const char* filename)
{
    write_cube_new(filename, 0, 0.0);
}

// Write cube file with density values
void Cube::write_cube_density(const char* filename)
{
    write_cube_new(filename, -1, 0.0);
}

// Internal function to write cube file
void Cube::write_cube_new(const char* filename, int opt, double va)
{
    FILE* fp1 = fopen(filename, "w");

    if (!fp1)
    {
        fprintf(stderr, "Error: Unable to open file %s for writing.\n", filename);
        return;
    }

    // Write comment lines
    fprintf(fp1, "%s\n", ci.textline1);
    fprintf(fp1, "%s\n", ci.textline2);

    // Write number of atoms and origin
    fprintf(fp1, "%5d %12.6lf %12.6lf %12.6lf\n", natoms, ci.o.x, ci.o.y, ci.o.z);

    // Write grid dimensions and vectors
    fprintf(fp1, "%5d %12.6lf %12.6lf %12.6lf\n", ci.nx, ci.vx.x, ci.vx.y, ci.vx.z);
    fprintf(fp1, "%5d %12.6lf %12.6lf %12.6lf\n", ci.ny, ci.vy.x, ci.vy.y, ci.vy.z);
    fprintf(fp1, "%5d %12.6lf %12.6lf %12.6lf\n", ci.nz, ci.vz.x, ci.vz.y, ci.vz.z);

    // Write atom data
    for (int i = 0; i < natoms; i++)
    {
        fprintf(fp1, "%5d %12.6lf %12.6lf %12.6lf %12.6lf\n",
                atoms[i].ian, atoms[i].an, atoms[i].x, atoms[i].y, atoms[i].z);
    }

    // Write data values
    int k = 0;
    for (int idx = 0; idx < ci.n; idx++)
    {
        double value = 0.0;

        if (!c[idx].z_flag)
        {
            if (opt == -1)
                value = c[idx].d; // Density value
            else
                value = c[idx].w; // Wavefunction value
        }

        fprintf(fp1, " %13.5E", value);
        k++;

        // Format output: 6 values per line
        if (k == 6)
        {
            fprintf(fp1, "\n");
            k = 0;
        }
    }

    // If last line didn't end with a newline, add one
    if (k != 0)
    {
        fprintf(fp1, "\n");
    }

    // Close the file
    fclose(fp1);
}

// Get wavefunction value at index i
double Cube::get_cube_element_w(int i)
{
    return c[i].w;
}

// Get density value at index i
double Cube::get_cube_element_d(int i)
{
    return c[i].d;
}

// Scale density values by a factor
void Cube::scale_d(double val)
{
    for (int i = 0; i < ci.n; i++)
    {
        c[i].d *= val;
    }
}

