/* Example program demonstrating how to use OpenCubMan functions */

#include "opencubman.h"

int main(int argc, char* argv[])
{
    // Check for proper usage
    if (argc != 2)
    {
        cout << "Usage: example input_cube_file.cube" << endl;
        return 1;
    }

    // Define a Cube object
    Cube cube1;

    // Read the cube file (assuming orbital number 0)
    cube1.read_cube(argv[1], 0);

    // Define points and vectors to create planes
    XYZ p1 = {0.0, 0.0, 0.0};
    XYZ v1 = {0.0, 1.0, 0.0};

    XYZ p2 = {1.0, 0.0, 0.0};
    XYZ v2 = {0.0, 1.0, 0.0};

    // Create planes using points and normal vectors
    PLANE pl1 = {p1, v1};
    PLANE pl2 = {p2, v2};

    // Zero the cube data above the first plane
    cube1.zero_plane(pl1, 1);

    // Zero the cube data above the second plane
    cube1.zero_plane(pl2, 1);

    // Write the modified cube data to a new file
    cube1.write_cube_density("output.cube");

    // Optionally scale the density values if they are too small for visualization
    cube1.scale_d(1000.0);
    cube1.write_cube_density("output_scaled.cube");

    // Reset zeroing flags
    cube1.reset_z_flag();

    // Clean up
    cube1.deinit();

    return 0;
}

