#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "transform.h"
#include "transformpool.h"

static List TransformPool;

void TransformPool_Init() {
    List_Init(&TransformPool, sizeof(Transform*));
}

void TransformPool_Clear() {

    // Free all transform data.
    Transform*** transforms = (Transform***) List_Elements(&TransformPool);
    int n = List_Length(&TransformPool);
    for (int i = 0; i < n; i++) {
        free(*transforms[i]);
    }
    List_Clear(&TransformPool);

}

void TransformPool_Free() {

    // Free all transform data.
    Transform*** transforms = (Transform***) List_Elements(&TransformPool);
    int n = List_Length(&TransformPool);
    for (int i = 0; i < n; i++) {
        free(*transforms[i]);
    }
    List_Free(&TransformPool);

}

Transform* TransformPool_Add(vec2 pos, vec2 size, float rotation) {

    // Initialise the transform.
    Transform* transform = (Transform*) malloc(sizeof(Transform));
    Transform_Init(transform, pos, size, rotation);

    // Push it on the list.
    List_Push(&TransformPool, &transform);

    // Return the new transform.
    return transform;

}

void TransformPool_Remove(Transform* t) {

    Transform*** transforms = (Transform***) List_Elements(&TransformPool);
    int n = List_Length(&TransformPool);
    for (int i = 0; i < n; i++) {
        if (t == *transforms[i]) {
            List_Remove(&TransformPool, i);
            break;
        }
    }

}
