#include "scores.h"


bool addScore(const scoreField *e)
{
    FILE *f = fopen("scores", "r+b");
    if(f == NULL)
        return false;

    fseek(f, 0, SEEK_END);
    fwrite(e, sizeof *e, 1, f);

    fclose(f);
    return true;
}

scVec *scLoad()
{
    scVec *v = malloc(sizeof *v);
    if(v == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Memory problem", NULL);
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen("scores", "rb");
    if(f == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Memory problem", NULL);
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);

    v->size = ftell(f) / sizeof *v->elements;

    v->elements = malloc(v->size * sizeof *v->elements);
    if(v->elements == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Memory problem", NULL);
        exit(EXIT_FAILURE);
    }

    rewind(f);
    fread(v->elements, sizeof *v->elements, v->size, f);

    fclose(f);

    return v;
}

void scRelease(scVec *v)
{
    free(v->elements);
    free(v);
}
