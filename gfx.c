#include "gfx.h"

static texture getTextureFromGlyph(TTF_Font *font, Uint16 c, SDL_Color clr)
{
    texture tx;
    SDL_Surface *s;

    s = TTF_RenderGlyph_Blended(font, c, clr);
    if(s == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", TTF_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    tx.tx = SDL_CreateTextureFromSurface(renderer, s);
    if(tx.tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    tx.rect.w = s->w;
    tx.rect.h = s->h;
    SDL_FreeSurface(s);

    return tx;
}

static texture getTextureFromText(TTF_Font *font, const char *str, SDL_Color clr)
{
    texture tx;
    SDL_Surface *s;

    s = TTF_RenderText_Blended(font, str, clr);
    if(s == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", TTF_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    tx.tx = SDL_CreateTextureFromSurface(renderer, s);
    if(tx.tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    tx.rect.w = s->w;
    tx.rect.h = s->h;
    SDL_FreeSurface(s);

    return tx;
}

bool pointInRect(const pos2D *point, const SDL_Rect *rect)
{
    return (point->x >= rect->x && point->x <= rect->x+rect->w
            && point->y >= rect->y && point->y <= rect->y+rect->h);
}

pos2D pos2DFromPos(const position *pos)
{
    pos2D out = {xStart, yStart};

    out.x += pos->j*(unmarkedpi->rect.w + X_OFFSET);
    out.y += pos->i*(unmarkedpi->rect.h + Y_OFFSET);

    if(pos->i % 2 > 0)
        out.x += unmarkedpi->rect.w/2;

    return out;
}

position posFromPos2D(const pos2D *pos, bool *exist, size_t mapSize)
{
    position out = {0, 0};
    pos2D p;

    do{
        p = pos2DFromPos(&out);
        if(p.y + unmarkedpi->rect.h < pos->y)
            ++out.i;
    }while(p.y + unmarkedpi->rect.h < pos->y);

    do{
        p = pos2DFromPos(&out);
        if(p.x + unmarkedpi->rect.w < pos->x)
            ++out.j;
    }while(p.x + unmarkedpi->rect.w < pos->x);

    *exist = (out.i < mapSize && out.j < mapSize
             && pos->x >= p.x && pos->x <= p.x + unmarkedpi->rect.w
             && pos->y >= p.y && pos->y <= p.y + unmarkedpi->rect.y);

    return out;
}

void gfxInit()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS) == -1){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }

    if(IMG_Init(IMG_INIT_PNG) == -1){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", IMG_GetError(), NULL);
        exit(EXIT_FAILURE);
    }

    if(TTF_Init() == -1){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", TTF_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    bgi = malloc(sizeof *bgi);
    if(bgi == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Memory problem", NULL);
        exit(EXIT_FAILURE);
    }
    unmarkedpi = malloc(sizeof *unmarkedpi);
    if(unmarkedpi == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Memory problem", NULL);
        exit(EXIT_FAILURE);
    }
    markedpi = malloc(sizeof *markedpi);
    if(bgi == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Memory problem", NULL);
        exit(EXIT_FAILURE);
    }

    cati = malloc(sizeof *cati);
    if(cati == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Memory problem", NULL);
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Circle the cat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
                              , W_WINDOW, H_WINDOW, 0);
    if(window == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }

    SDL_Surface *bgimg = IMG_Load("gfx/background.png");
    if(bgimg == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", IMG_GetError(), NULL);
        exit(EXIT_FAILURE);
    }

    bgi->rect.w = bgimg->w;
    bgi->rect.h = bgimg->h;
    bgi->tx = SDL_CreateTextureFromSurface(renderer, bgimg);
    if(bgi->tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(bgimg);

    SDL_Surface *markedpimg = IMG_Load("gfx/marked_place.png");
    if(markedpimg == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", IMG_GetError(), NULL);
        exit(EXIT_FAILURE);
    }

    markedpi->rect.w = markedpimg->w;
    markedpi->rect.h = markedpimg->h;
    markedpi->tx = SDL_CreateTextureFromSurface(renderer, markedpimg);
    if(markedpi->tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(markedpimg);

    SDL_Surface *unmarkedpimg = IMG_Load("gfx/unmarked_place.png");
    if(unmarkedpimg == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", IMG_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    unmarkedpi->rect.w = unmarkedpimg->w;
    unmarkedpi->rect.h = unmarkedpimg->h;
    unmarkedpi->tx = SDL_CreateTextureFromSurface(renderer, unmarkedpimg);
    if(unmarkedpi->tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(unmarkedpimg);

    SDL_Surface *catimg = IMG_Load("gfx/cat.png");
    if(catimg == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", IMG_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    cati->rect.w = catimg->w;
    cati->rect.h = catimg->h;
    cati->tx = SDL_CreateTextureFromSurface(renderer, catimg);
    if(cati->tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(catimg);

    mainFont = TTF_OpenFont("gfx/Norasi-Italic.ttf", MAIN_FONT_SIZE);
    if(mainFont == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", TTF_GetError(), NULL);
        exit(EXIT_FAILURE);
    }

}

void gfxRelease()
{
    SDL_DestroyTexture(bgi->tx);
    SDL_DestroyTexture(markedpi->tx);
    SDL_DestroyTexture(unmarkedpi->tx);
    SDL_DestroyTexture(cati->tx);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(bgi);
    free(markedpi);
    free(unmarkedpi);
    free(cati);
    IMG_Quit();
    SDL_Quit();
    TTF_CloseFont(mainFont);
    TTF_Quit();
}

void renderMainMenu(SDL_Rect *p1, SDL_Rect *p2, SDL_Rect *p3, SDL_Rect *p4)
{
    SDL_Color clr = {0x00, 0x00, 0x00, 0x00};
    texture txNewGame, txTopScores, txOptions, txQuit;

    SDL_Surface *s = TTF_RenderText_Blended(mainFont, "NEW GAME", clr);
    if(s == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", TTF_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    txNewGame.tx = SDL_CreateTextureFromSurface(renderer, s);
    if(txNewGame.tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    txNewGame.rect.w = s->w;
    txNewGame.rect.h = s->h;
    txNewGame.rect.x = (W_WINDOW - txNewGame.rect.w)/2;
    SDL_FreeSurface(s);

    s = TTF_RenderText_Blended(mainFont, "TOP SCORES", clr);
    if(s == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    txTopScores.tx = SDL_CreateTextureFromSurface(renderer, s);
    if(txTopScores.tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    txTopScores.rect.w = s->w;
    txTopScores.rect.h = s->h;
    txTopScores.rect.x = (W_WINDOW - txTopScores.rect.w)/2;
    SDL_FreeSurface(s);

    s = TTF_RenderText_Blended(mainFont, "Options", clr);
    if(s == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    txOptions.tx = SDL_CreateTextureFromSurface(renderer, s);
    if(txOptions.tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    txOptions.rect.w = s->w;
    txOptions.rect.h = s->h;
    txOptions.rect.x = (W_WINDOW - txOptions.rect.w)/2;
    SDL_FreeSurface(s);

    s = TTF_RenderText_Blended(mainFont, "QUIT", clr);
    if(s == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    txQuit.tx = SDL_CreateTextureFromSurface(renderer, s);
    if(txQuit.tx == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
    txQuit.rect.w = s->w;
    txQuit.rect.h = s->h;
    txQuit.rect.x = (W_WINDOW - txQuit.rect.w)/2;
    SDL_FreeSurface(s);

    const int yStart = (H_WINDOW - Y_START - (txNewGame.rect.h + txOptions.rect.h + txTopScores.rect.h + txQuit.rect.h))/2;

    txNewGame.rect.y = yStart;
    txTopScores.rect.y = txNewGame.rect.y + txNewGame.rect.h + Y_OFFSET;
    txOptions.rect.y = txTopScores.rect.y + txTopScores.rect.h + Y_OFFSET;
    txQuit.rect.y = txOptions.rect.y + txOptions.rect.h + Y_OFFSET;
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, bgi->tx, NULL, NULL);
    SDL_RenderCopy(renderer, txNewGame.tx, NULL, &txNewGame.rect);
    SDL_RenderCopy(renderer, txTopScores.tx, NULL, &txTopScores.rect);
    SDL_RenderCopy(renderer, txOptions.tx, NULL, &txOptions.rect);
    SDL_RenderCopy(renderer, txQuit.tx, NULL, &txQuit.rect);

    SDL_RenderPresent(renderer);

    memcpy(p1, &txNewGame.rect, sizeof *p1);
    memcpy(p2, &txTopScores.rect, sizeof *p2);
    memcpy(p3, &txOptions.rect, sizeof *p3);
    memcpy(p4, &txQuit.rect, sizeof *p4);

    SDL_DestroyTexture(txNewGame.tx);
    SDL_DestroyTexture(txOptions.tx);
    SDL_DestroyTexture(txTopScores.tx);
    SDL_DestroyTexture(txQuit.tx);
}

void loadGameMenuTx(texture *diff, texture *size, texture *dp, texture *dm
                    , texture *sp, texture *sm, texture *start, texture *back)
{

    SDL_Color clr = {0, 0, 0, 0};
    texture tdp, tdm, tsp, tsm, tdiff, tsize, tstart, tback;

    tdp = getTextureFromGlyph(mainFont, '+', clr);
    tdm = getTextureFromGlyph(mainFont, '-', clr);
    tsp = getTextureFromGlyph(mainFont, '+', clr);
    tsm = getTextureFromGlyph(mainFont, '-', clr);
    tdiff = getTextureFromText(mainFont, "DIFFICULTY", clr);
    tsize = getTextureFromText(mainFont, "SIZE", clr);
    tstart = getTextureFromText(mainFont, "START", clr);
    tback = getTextureFromText(mainFont, "BACK", clr);

    memcpy(dp, &tdp, sizeof *dp);
    memcpy(dm, &tdm, sizeof *dm);
    memcpy(sp, &tsp, sizeof *sp);
    memcpy(sm, &tsm, sizeof *sm);
    memcpy(diff, &tdiff, sizeof *diff);
    memcpy(size, &tsize, sizeof *size);
    memcpy(start, &tstart, sizeof *start);
    memcpy(back, &tback, sizeof *back);

}

void renderGameMenu(texture *diff, texture *size, texture *dp, texture *dm, texture *sp, texture *sm,
                    texture *start, texture *back, const gameLevel *lvl)
{
    const int yStart = (H_WINDOW - Y_START)/4;
    const int xStart = (W_WINDOW - X_START)/4;
    int maxDiffWidth, maxDiffHeight;
    SDL_Color clr = {0, 0, 0, 0};
    char diffStr[7];
    char lvlStr[3];

    TTF_SizeText(mainFont, "MEDIUM", &maxDiffWidth, &maxDiffHeight);
    switch(lvl->diff)
    {
    case EASY:
        strcpy(diffStr, "EASY");
        break;
    case MEDIUM:
        strcpy(diffStr, "MEDIUM");
        break;
    case HARD:
        strcpy(diffStr, "HARD");
        break;
    }

    sprintf(lvlStr, "%d", lvl->size);
    texture txSize = getTextureFromText(mainFont, lvlStr, clr);
    texture txDiff = getTextureFromText(mainFont, diffStr, clr);

    diff->rect.x = xStart;
    diff->rect.y = yStart;

    dm->rect.x = diff->rect.x + diff->rect.w + 5*X_OFFSET;
    dm->rect.y = diff->rect.y;

    txDiff.rect.x = dm->rect.x + dm->rect.w + X_OFFSET;
    txDiff.rect.y = dm->rect.y;

    dp->rect.x = txDiff.rect.x + maxDiffWidth + X_OFFSET;
    dp->rect.y = txDiff.rect.y;

    size->rect.x = diff->rect.x ;
    size->rect.y = diff->rect.y + diff->rect.h + Y_OFFSET;

    sm->rect.x = dm->rect.x;
    sm->rect.y = size->rect.y;

    txSize.rect.x = txDiff.rect.x + (maxDiffWidth - size->rect.w)/2;
    txSize.rect.y = size->rect.y;

    sp->rect.x = dp->rect.x;
    sp->rect.y = size->rect.y;
    txSize.rect.y = size->rect.y;

    start->rect.y = txSize.rect.y + txSize.rect.h + Y_OFFSET;
    back->rect.y = start->rect.y;
    start->rect.x = X_START + (W_WINDOW - X_START - (start->rect.w + back->rect.w + 5*X_OFFSET))/2;
    back->rect.x = start->rect.x + start->rect.w + 5*X_OFFSET;

    SDL_RenderClear(renderer);

    texture *t[] = {diff, size, dp, dm, sp, sm, &txDiff, &txSize, start, back};
    SDL_RenderCopy(renderer, bgi->tx, NULL, NULL);
    int i;
    for(i = 0; i < sizeof t/sizeof *t; i++)
        SDL_RenderCopy(renderer, t[i]->tx, NULL, &t[i]->rect);

    SDL_RenderPresent(renderer); // Finally !!!!

    SDL_DestroyTexture(txDiff.tx);
    SDL_DestroyTexture(txSize.tx);

}

void renderMap(const gameMap *map, const position *catPos)
{

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, bgi->tx, NULL, NULL);

    texture *tx;
    position p;
    pos2D p2d;
    for(p.i = 0; p.i < map->size; p.i++){
        for(p.j = 0; p.j < map->size; p.j++){
            if(map->mat[p.i][p.j] == UNMARKED)
                tx = unmarkedpi;
            else
                tx = markedpi;
            p2d = pos2DFromPos(&p);
            tx->rect.x = p2d.x; tx->rect.y = p2d.y;
            SDL_RenderCopy(renderer, tx->tx, NULL, &tx->rect);
        }
    }

    tx = cati;
    p2d = pos2DFromPos(catPos);
    tx->rect.x = p2d.x + (unmarkedpi->rect.w - cati->rect.w)/4;
    tx->rect.y = p2d.y + (unmarkedpi->rect.h - cati->rect.h)/4;
    SDL_RenderCopy(renderer, tx->tx, NULL, &tx->rect);

    SDL_RenderPresent(renderer);
}

void renderOutput(bool win, char name[])
{
    char msg[20];
    SDL_Color clr = {0, 0, 0, 0};

    if(win){
        clr.g = 0xff;
        strcpy(msg, "YOU WON !!!");
    }
    else{
        clr.r = 0xff;
        strcpy(msg, "YOU LOST");
    }

    texture txMsg = getTextureFromText(mainFont, msg, clr);
    clr.a = clr.b = clr.g = clr.r = 0;
    texture txName = getTextureFromText(mainFont, "NAME : ", clr);
    texture txOk = getTextureFromText(mainFont, "OK", clr);

    txMsg.rect.x = 300;
    txMsg.rect.y = 150;

    txName.rect.x = 330;
    txName.rect.y = 250;

    txOk.rect.x = 350;
    txOk.rect.y = 350;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgi->tx, NULL, NULL);
    SDL_RenderCopy(renderer, txMsg.tx, NULL, &txMsg.rect);
    SDL_RenderCopy(renderer, txName.tx, NULL, &txName.rect);
    SDL_RenderCopy(renderer, txOk.tx, NULL, &txOk.rect);
    SDL_RenderPresent(renderer);

    int i = 0;
    name[0] = '\0';
    bool ok = false;
    while(!ok){
        SDL_Event e;
        SDL_WaitEvent(&e);
        if(e.type == SDL_QUIT)
            exit(EXIT_SUCCESS);
        else if(e.type == SDL_MOUSEBUTTONDOWN){
            pos2D p = {e.button.x, e.button.y};
            if(pointInRect(&p, &txOk.rect))
                ok = true;
        }
        else if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_RETURN){
                ok = true;
            }
            else {
                if(e.key.keysym.sym != SDLK_BACKSPACE && i+1 < NAME_LENGTH){
                    name[i++] = e.key.keysym.sym;
                    name[i] = '\0';
                }
                else if(e.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && i > 0)
                    name[i--] = '\0';
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, bgi->tx, NULL, NULL);
            SDL_RenderCopy(renderer, txMsg.tx, NULL, &txMsg.rect);
            SDL_RenderCopy(renderer, txName.tx, NULL, &txName.rect);
            SDL_RenderCopy(renderer, txOk.tx, NULL, &txOk.rect);

            if(*name != '\0'){
                puts(name);
                texture txN = getTextureFromText(mainFont, name, clr);
                txN.rect.x = txName.rect.x + txName.rect.w + 5*X_OFFSET;
                txN.rect.y = txName.rect.y;
                SDL_RenderCopy(renderer, txN.tx, NULL, &txN.rect);
                SDL_DestroyTexture(txN.tx);

            }
            SDL_RenderPresent(renderer);
            SDL_FlushEvent(SDL_KEYDOWN);
        }
    }
}

void renderScores(SDL_Rect *backRect, const scVec *v, size_t max)
{
    SDL_Color clr = {0, 0, 0, 0};
    texture txName = getTextureFromText(mainFont, "NAME", clr);
    texture txScore = getTextureFromText(mainFont, "SCORE", clr);
    texture txBack = getTextureFromText(mainFont, "BACK", clr);

    txName.rect.x = 300;
    txName.rect.y = 100;

    txScore.rect.x = 600;
    txScore.rect.y = 100;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgi->tx, NULL, NULL);
    SDL_RenderCopy(renderer, txName.tx, NULL, &txName.rect);
    SDL_RenderCopy(renderer, txScore.tx, NULL, &txScore.rect);

    char scoreStr[15];
    int y = txName.rect.y;
    size_t i = 0;
    texture txn, txs;
    while(i < 5 && i < v->size){
        sprintf(scoreStr, "%d", v->elements[i].val);
        if(v->elements[i].name != '\0')
            txn = getTextureFromText(mainFont, v->elements[i].name, clr);
        txs = getTextureFromText(mainFont, scoreStr, clr);

        y += 50;
        txn.rect.x = 300;
        txn.rect.y = y;
        txs.rect.x = 600;
        txs.rect.y = y;
        if(*scoreStr != '\0')
            SDL_RenderCopy(renderer, txn.tx, NULL, &txn.rect);
        SDL_RenderCopy(renderer, txs.tx, NULL, &txs.rect);

        if(v->elements[i].name != '\0')
            SDL_DestroyTexture(txn.tx);
        SDL_DestroyTexture(txs.tx);

        ++i;
    }

    txBack.rect.x = 400;
    txBack.rect.y = y+100;

    SDL_RenderCopy(renderer, txBack.tx, NULL, &txBack.rect);
    memcpy(backRect, &txBack.rect, sizeof *backRect);

    SDL_DestroyTexture(txName.tx);
    SDL_DestroyTexture(txScore.tx);
    SDL_DestroyTexture(txBack.tx);

    SDL_RenderPresent(renderer);
}
