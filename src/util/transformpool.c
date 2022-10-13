#include <stdlib.h>
#include <string.h>
#include "transform.h"
#include "transformpool.h"

#define INITIAL_TRANSFORMPOOL_SIZE 16

Transform** TransformPool_Pool;
size_t TransformPool_Size;
size_t TransformPool_Length;

void TransformPool_Init() {
    TransformPool_Pool = (Transform**) malloc(INITIAL_TRANSFORMPOOL_SIZE * sizeof(Transform*));
    TransformPool_Size = INITIAL_TRANSFORMPOOL_SIZE;
    TransformPool_Length = 0;
}

void TransformPool_Clear() {

    // Free all transform data.
    for (int i = 0; i < TransformPool_Length; i++) {
        free(TransformPool_Pool[i]);
    }

    // Set the length of the pool list to 0.
    TransformPool_Length = 0;

}

void TransformPool_Free() {

    // Free all transform data.
    for (int i = 0; i < TransformPool_Length; i++) {
        free(TransformPool_Pool[i]);
    }

    // Free the pool.
    free(TransformPool_Pool);

}

Transform* TransformPool_Add(vec2 pos, vec2 size, float rotation) {

    // If the pool is not big enough, allocate more memory.
    if (TransformPool_Length >= TransformPool_Size) {
        TransformPool_Pool = (Transform**) realloc(TransformPool_Pool, TransformPool_Size * 2 * sizeof(Transform*));
        TransformPool_Size = TransformPool_Size * 2;
    }

    // Initialise the transform.
    Transform* newTransform = (Transform*) malloc(sizeof(Transform));
    Transform_Init(newTransform, pos, size, rotation);
    TransformPool_Pool[TransformPool_Length] = newTransform;
    TransformPool_Length++;

    // Return the new transform.
    return newTransform;


}

void TransformPool_Remove(Transform* t) {

    for (int i = 0; i < TransformPool_Length; i++) {
        if (TransformPool_Pool[i] == t) {
            memmove(TransformPool_Pool + i, TransformPool_Pool + i + 1, (TransformPool_Length - i - 1) * sizeof(Transform*));
            TransformPool_Length--;
        }
    }

}
