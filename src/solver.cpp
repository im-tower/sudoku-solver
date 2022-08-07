#include<iostream>

using namespace std;

struct square
{
    int row = -1;
    int column = -1;
};

void printBoard(int tablero[9][9]){
    for(int i = 0; i < 9; ++i){
        for(int k = 0; k < 9; ++k) printf("%d ",tablero[i][k]);
        printf("\n");
    }
}

void reduceBigSquare(int tablero[9][9], bool posibles[9][9][9], int square){
    bool options[9];
    for(int i = 0; i < 9; ++i) options[i] = true;
    for(int i = square%3*3; i < (square%3 + 1)*3; ++i)
        for(int k = square/3*3; k < (square/3 + 1)*3; ++k) if(tablero[i][k] != 0) options[tablero[i][k]-1] = false;
    for(int i = square%3*3; i < (square%3 + 1)*3; ++i)
        for(int k = square/3*3; k < (square/3 + 1)*3; ++k)
            for(int j = 0; j < 9; ++j) if(!options[j]) posibles[i][k][j] = false;
}

void reduceRow(int tablero[9][9], bool posibles[9][9][9], int row){
    bool options[9];
    for(int i = 0; i < 9; ++i) options[i] = true;
    for(int i = 0; i < 9; ++i) if(tablero[row][i] != 0) options[tablero[row][i]-1] = false;
    for(int i = 0; i < 9; ++i)
        if(tablero[row][i] == 0){
            for(int k = 0; k < 9; ++k) if(!options[k]) posibles[row][i][k] = false;
        }else{
            for(int k = 0; k < 9; ++k) if(k+1 != tablero[row][i]) posibles[row][i][k] = false;
                                            else posibles[row][i][k] = true;
        }
}
void reduceColumn(int tablero[9][9], bool posibles[9][9][9], int column){
    bool options[9];
    for(int i = 0; i < 9; ++i) options[i] = true;
    for(int i = 0; i < 9; ++i) if(tablero[i][column] != 0) options[tablero[i][column]-1] = false;
    for(int i = 0; i < 9; ++i)
        if(tablero[i][column] == 0){
            for(int k = 0; k < 9; ++k) if(!options[k]) posibles[i][column][k] = false;
        }else{
            for(int k = 0; k < 9; ++k) if(k+1 != tablero[i][column]) posibles[i][column][k] = false;
                                            else posibles[i][column][k] = true;
        }
}

int getSolution(bool posibles[9][9][9], int i, int k){
    int solution = -1;
    for(int j = 0; j < 9; ++j)
        if(posibles[i][k][j]){
            if(solution != -1) return -1;
            solution = (j+1);
        }
    if(solution == -1) return 0; // Incongruencia.
    return solution;
}

square getFirstUnsolvedSquare(int tablero[9][9]){
    square s;
    for(int i = 0; i < 9 && s.row == -1; ++i)
        for(int k = 0; k < 9; ++k){
            if(tablero[i][k] == 0){
                s.row = i;
                s.column = k;
                break;
            }
        }
    return s;
}

bool reduceBoard(int tablero[9][9], bool posibles[9][9][9]){
    bool update = true;
    while(update){
        update = false;
        for(int i = 0; i < 9; ++i) reduceRow(tablero,posibles,i);
        for(int i = 0; i < 9; ++i) reduceColumn(tablero,posibles,i);
        for(int i = 0; i < 9; ++i) reduceBigSquare(tablero,posibles,i);
        for(int i = 0; i < 9 && !update; ++i)
            for(int k = 0; k < 9 && !update; ++k){
                if(tablero[i][k] != 0) continue;
                const int solution = getSolution(posibles,i,k);
                if(solution == 0) return false; // Incongruencia en el tablero.
                if(solution != -1){
                    update = true;
                    tablero[i][k] = solution;
                }
            }
    }
    return true;
}

// 0-> No hay solución. 1-> Solución válida.
int solveSquare(int tablero[9][9], bool posibles[9][9][9]){
    if(!reduceBoard(tablero,posibles)) return 0;
    square s = getFirstUnsolvedSquare(tablero);
    if(s.row == -1) return 1;
    for(int p = 0; p < 9; ++p){
        if(!posibles[s.row][s.column][p]) continue;
        int tab[9][9];
        bool pos[9][9][9];
        for(int a = 0; a < 9; ++a)
            for(int b = 0; b < 9; ++b){
                tab[a][b] = tablero[a][b];
                for(int c = 0; c < 9; ++c) pos[a][b][c] = posibles[a][b][c];
            }
        tab[s.row][s.column] = p+1;
        const int solution = solveSquare(tab,pos);
        if( solution != 0){
            for(int a = 0; a < 9; ++a)
            for(int b = 0; b < 9; ++b){
                tablero[a][b] = tab[a][b];
            }
            return 1;
        }
    }
    return 0;
}

int main(){
    int tablero[9][9];
    bool posibles[9][9][9];
    for(int i = 0; i < 9; ++i)
        for(int k = 0; k < 9; ++k){
            scanf("%d",&tablero[i][k]);
            if(tablero[i][k] == 0)
                for(int j = 0; j < 9; ++j) posibles[i][k][j] = true;
        }
    if(solveSquare(tablero,posibles) == 0){
        printf("El sudoku es imposible\n");
        return 0;
    }
    printf("Solucion:\n");
    printBoard(tablero);
    return 0;
}