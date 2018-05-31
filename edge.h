#ifndef Edge_H
#define Edge_H

#include "vector2.h"

/* Line é um Shape, tem nome etc.
 * Essa struct é uma linha mais simples, apenas com dois pontos
 * 'a' e 'b' (inicial e final).
 */

struct Edge {
    Vector2 a;
    Vector2 b;

    Edge ();
    Edge (Vector2 a, Vector2 b);
};

#endif
