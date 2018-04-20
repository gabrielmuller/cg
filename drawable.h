#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <string>

struct Drawable {
    std::string name;
    Drawable (std::string name);
    virtual void draw () = 0;
};

#endif
