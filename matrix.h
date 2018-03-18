#include <string>

// Unfortunately, no one can be told what the Matrix is.
// You have to see it for yourself.

struct Matrix {
    float** data;
    int sizeX;
    int sizeY;

    Matrix (int sizeX, int sizeY);
    static Matrix multiply (Matrix a, Matrix b);
    operator std::string () const;
    float* operator[] (int i);
};
