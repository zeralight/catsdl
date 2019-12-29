#ifndef SCORES_H_INCLUDED
#define SCORES_H_INCLUDED
#define NAME_LENGTH 11
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char name[NAME_LENGTH];
  int val;
} scoreField;

typedef struct {
  scoreField *elements;
  size_t size;
} scVec;

bool addScore(const scoreField *e);
scVec *scLoad();
void scRelease(scVec *v);
#endif  // SCORES_H_INCLUDED
