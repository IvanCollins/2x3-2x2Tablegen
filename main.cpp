// main.cpp
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
using namespace std;

// ---------- Dynamic board size ----------
int ROWS, COLS, CELLS;
uint64_t NSTATES;

// ---------- Board Representation ----------
using State = uint64_t; // enough for 2x3 (24 bits)
inline int getCell(State s, int i){ return (s >> (i*4)) & 0xF; }
inline void setCell(State &s, int i, int val){
    s &= ~(0xFULL << (i*4));
    s |= ((uint64_t)val << (i*4));
}

// ---------- GUI ----------
SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
int TILE_SIZE = 100;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void drawBoard(State s){
    SDL_SetRenderDrawColor(renderer, 250,248,239,255);
    SDL_RenderClear(renderer);
    for(int r=0;r<ROWS;r++){
        for(int c=0;c<COLS;c++){
            int idx = r*COLS+c;
            int exp = getCell(s,idx);
            int val = exp?(1<<exp):0;
            SDL_Rect rect{c*TILE_SIZE, r*TILE_SIZE, TILE_SIZE-5, TILE_SIZE-5};
            if(val==0) SDL_SetRenderDrawColor(renderer,205,193,180,255);
            else SDL_SetRenderDrawColor(renderer,238,228,218,255);
            SDL_RenderFillRect(renderer,&rect);
        }
    }
    SDL_RenderPresent(renderer);
}

// ---------- Move logic ----------
State moveLeft(State s){
    State out=0;
    for(int r=0;r<ROWS;r++){
        vector<int> line;
        for(int c=0;c<COLS;c++){
            int e = getCell(s,r*COLS+c);
            if(e) line.push_back(e);
        }
        for(int i=0;i+1<(int)line.size();i++){
            if(line[i]==line[i+1]){ line[i]++; line.erase(line.begin()+i+1); }
        }
        for(int c=0;c<(int)line.size();c++) setCell(out,r*COLS+c,line[c]);
    }
    return out;
}
State rotate(State s){
    if(ROWS==2 && COLS==3){ // 2x3 mapping
        State out=0;
        int map[6]={3,0,4,1,5,2};
        for(int i=0;i<6;i++) setCell(out,map[i],getCell(s,i));
        return out;
    } else if(ROWS==2 && COLS==2){ // 2x2 mapping
        State out=0;
        int map[4]={2,0,3,1};
        for(int i=0;i<4;i++) setCell(out,map[i],getCell(s,i));
        return out;
    }
    return s;
}
State applyMove(State s,int dir){
    for(int i=0;i<dir;i++) s=rotate(s);
    State moved = moveLeft(s);
    for(int i=dir;i<4;i++) moved=rotate(moved);
    return moved;
}

// ---------- Tablebase ----------
unordered_map<State,int> tb;
int solve(State s){
    if(tb.count(s)) return tb[s];
    int best=0;
    for(int dir=0;dir<4;dir++){
        State nxt=applyMove(s,dir);
        if(nxt!=s) best=max(best,solve(nxt));
    }
    for(int i=0;i<CELLS;i++) best=max(best,1<<getCell(s,i));
    return tb[s]=best;
}

// ---------- Main ----------
int main(){
    cout << "Choose board size:\n1) 2x2\n2) 2x3\nChoice: ";
    int choice;
    cin >> choice;
    if(choice==1){ ROWS=2; COLS=2; } else ROWS=2; COLS=3;
    CELLS=ROWS*COLS;
    NSTATES = 1ULL << (4*CELLS);
    SCREEN_WIDTH = COLS*TILE_SIZE;
    SCREEN_HEIGHT = ROWS*TILE_SIZE;

    // SDL init
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window=SDL_CreateWindow("2048 Tablegen",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH,SCREEN_HEIGHT,0);
    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    font=TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",24);

    State board=0;
    if(ROWS*COLS>=2){ setCell(board,0,1); setCell(board,1,1); }

    // Table generation
    cout << "Generating tablebase for " << ROWS << "x" << COLS << "...\n";
    solve(board); // recursive cache fill
    cout << "Done! Number of cached states: " << tb.size() << "\n";

    // Draw once
    drawBoard(board);
    cout << "Press Enter to quit...\n"; cin.ignore(); cin.get();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
	
