#ifndef AB_H
#define AB_H

#include "vector2.h"

/* Line é um Shape, tem nome etc.
 * Essa struct é uma linha mais simples, apenas com dois pontos
 * 'a' e 'b' (inicial e final).
 */

struct AB {
    // true quando não precisar desenhar
    bool empty;

    Vector2 a;
    Vector2 b;

    AB ();
    AB (Vector2 a, Vector2 b);
};

#endif
