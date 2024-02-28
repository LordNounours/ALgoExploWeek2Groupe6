#include <stdbool.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <random>
#define ORDI 1 
#define HUMAIN 2 
#define COTE 3 
#define DEPLACEMENT_ORDINATEUR 'O' 
#define DEPLACEMENT_HUMAIN 'X' 
#include "EvalPerf.hpp"
#include "EvalPerf.cpp"

int nb= 0;
int victory = 0;
int noeud=0;
struct Déplacement { 
    int ligne, colonne; 
}; 

char joueur = 'x', adversaire = 'o'; 

// Vérifie s'il reste des mouvements possibles sur le plateau
bool mouvementsRestants(char plateau[3][3]) 
{ 
    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) 
            if (plateau[i][j] == '_') 
                return true; 
    return false; 
} 

// Évalue le plateau pour déterminer le score actuel
int evaluer(char b[3][3]) 
{ 
    // Vérifie les lignes
  
    for (int ligne = 0; ligne < 3; ligne++) { 
        if (b[ligne][0] == b[ligne][1] && b[ligne][1] == b[ligne][2]) { 
            if (b[ligne][0] == joueur) 
                return +1; 
            else if (b[ligne][0] == adversaire) 
                return -1; 
        } 
    } 

    // Vérifie les colonnes
    for (int colonne = 0; colonne < 3; colonne++) { 
        if (b[0][colonne] == b[1][colonne] && b[1][colonne] == b[2][colonne]) { 
            if (b[0][colonne] == joueur) 
                return +1; 
            else if (b[0][colonne] == adversaire) 
                return -1; 
        } 
    } 

    // Vérifie les diagonales
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) { 
        if (b[0][0] == joueur) 
            return +1; 
        else if (b[0][0] == adversaire) 
            return -1; 
    } 

    if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) { 
        if (b[0][2] == joueur) 
            return +1; 
        else if (b[0][2] == adversaire) 
            return -1; 
    } 

    return 0; 
} 

// Implémente l'algorithme Minimax pour choisir le meilleur mouvement
int minimax(char plateau[3][3], int profondeur, bool estMax) 
{ 
    int score = evaluer(plateau); 

    if (score == 1) 
        return score; 

    if (score == -1) 
        return score; 

    if (mouvementsRestants(plateau) == false) 
        return 0; 

    if (estMax) { 
        int meilleur = -1000; 

        for (int i = 0; i < 3; i++) { 
            for (int j = 0; j < 3; j++) { 
                if (plateau[i][j] == '_') { 
                    plateau[i][j] = joueur; 
                    int valeur = minimax(plateau, profondeur + 1, !estMax); 
                    if (valeur > meilleur) { 
                        meilleur = valeur; 
                    } 

                    plateau[i][j] = '_'; 
                } 
            } 
        } 
        return meilleur; 
    } 

    else { 
        int meilleur = 1000; 

        for (int i = 0; i < 3; i++) { 
            for (int j = 0; j < 3; j++) { 
                if (plateau[i][j] == '_') { 
                    plateau[i][j] = adversaire; 

                    int valeur = minimax(plateau, profondeur + 1, !estMax); 
                    if (valeur < meilleur) { 
                        meilleur = valeur; 
                    } 
                    plateau[i][j] = '_'; 
                } 
            } 
        } 
        return meilleur; 
    } 
} 

// Trouve le meilleur mouvement possible pour l'ordinateur
struct Déplacement trouverMeilleurMouvement(char plateau[3][3]) 
{ 
    int meilleurValeur = -1000; 
    struct Déplacement meilleurMouvement; 
    meilleurMouvement.ligne = -1; 
    meilleurMouvement.colonne = -1; 

    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) { 
            if (plateau[i][j] == '_') { 
                plateau[i][j] = joueur; 
                int valeurMouvement = minimax(plateau, 0, false); 
                plateau[i][j] = '_'; 

                if (valeurMouvement > meilleurValeur) { 
                    meilleurMouvement.ligne = i; 
                    meilleurMouvement.colonne = j; 
                    meilleurValeur = valeurMouvement; 
                } 
            } 
        } 
    } 

    return meilleurMouvement; 
} 

// Affiche le plateau de jeu
void afficherPlateau(char plateau[][COTE]) 
{ 
    printf("\n\n"); 
    printf("\t\t\t %c | %c | %c \n", plateau[0][0], plateau[0][1], plateau[0][2]); 
    printf("\t\t\t--------------\n"); 
    printf("\t\t\t %c | %c | %c \n", plateau[1][0], plateau[1][1], plateau[1][2]); 
    printf("\t\t\t--------------\n"); 
    printf("\t\t\t %c | %c | %c \n\n", plateau[2][0], plateau[2][1], plateau[2][2]); 
} 

// Affiche les instructions du jeu
void afficherInstructions() 
{ 
    printf("\t\t\t Tic-Tac-Toe\n\n"); 
    printf("Choisissez une cellule numérotée de 1 à 9 comme ci-dessous et jouez\n\n"); 

    printf("\t\t\t 1 | 2 | 3 \n"); 
    printf("\t\t\t--------------\n"); 
    printf("\t\t\t 4 | 5 | 6 \n"); 
    printf("\t\t\t--------------\n"); 
    printf("\t\t\t 7 | 8 | 9 \n\n"); 

    printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n\n"); 
} 

// Initialise le plateau et les mouvements possibles
void initialiser(char plateau[][COTE], int mouvements[]) 
{ 
    srand(time(NULL)); 

    for (int i = 0; i < COTE; i++) { 
        for (int j = 0; j < COTE; j++) 
            plateau[i][j] = ' '; 
    } 

    for (int i = 0; i < COTE * COTE; i++) 
        mouvements[i] = i; 

    for (int i = 0; i < COTE * COTE; i++) { 
        int indexAleatoire = rand() % (COTE * COTE); 
        int temp = mouvements[i]; 
        mouvements[i] = mouvements[indexAleatoire]; 
        mouvements[indexAleatoire] = temp; 
    } 
} 

// Détermine et annonce le vainqueur
void déclarerVainqueur(int tourDe) 
{ 
    if (tourDe == ORDI) 
        printf("ORDINATEUR a gagné\n"); 
    else
        printf("HUMAIN a gagné\n"); 
        victory++;
} 

// Vérifie si une ligne est complétée
int ligneComplétée(char plateau[][COTE]) 
{ 
    for (int i = 0; i < COTE; i++) { 
        if (plateau[i][0] == plateau[i][1] && plateau[i][1] == plateau[i][2] && plateau[i][0] != ' ') 
            return 1; 
    } 
    return 0; 
} 

// Vérifie si une colonne est complétée
int colonneComplétée(char plateau[][COTE]) 
{ 
    for (int i = 0; i < COTE; i++) { 
        if (plateau[0][i] == plateau[1][i] && plateau[1][i] == plateau[2][i] && plateau[0][i] != ' ') 
            return 1; 
    } 
    return 0; 
} 


// Vérifie si une diagonale est complétée
int diagonaleComplétée(char plateau[][COTE]) 
{ 
    if ((plateau[0][0] == plateau[1][1] 
         && plateau[1][1] == plateau[2][2] 
         && plateau[0][0] != ' ') 
        || (plateau[0][2] == plateau[1][1] 
            && plateau[1][1] == plateau[2][0] 
            && plateau[0][2] != ' ')) 
        return 1; 

    return 0; 
} 
  
// Vérifie si le jeu est terminé
int jeuTerminé(char plateau[][COTE]) 
{ 
    return (ligneComplétée(plateau) || colonneComplétée(plateau) || diagonaleComplétée(plateau)); 
} 

// Joue un tour de Tic-Tac-Toe
void jouerTicTacToe(int tourDe) 
{ 
    char plateau[COTE][COTE]; 
    int mouvements[COTE * COTE]; 
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dist(0, 2); 

    
    initialiser(plateau, mouvements); 
    afficherInstructions(); 
    int random_number = dist(gen); 
    int random_number2 = dist(gen); 
    plateau[random_number][random_number2]='X';

    int indexMouvement = 0, x, y; 

    while (!jeuTerminé(plateau) && indexMouvement != COTE * COTE) { 
        if (tourDe == ORDI) { 
            char plateauTemp[3][3]; 
            for (int i = 0; i < 3; i++) { 
                for (int j = 0; j < 3; j++) { 
                    if (plateau[i][j] == 'X') { 
                        plateauTemp[i][j] = 'x'; 
                    } 
                    else if (plateau[i][j] == 'O') { 
                        plateauTemp[i][j] = 'o'; 
                    } 
                    else { 
                        plateauTemp[i][j] = '_'; 
                    } 
                } 
            } 
            struct Déplacement mouvement = trouverMeilleurMouvement(plateauTemp); 
              noeud++;
            x = mouvement.ligne; 
            y = mouvement.colonne; 

            plateau[x][y] = DEPLACEMENT_ORDINATEUR; 
            printf("ORDINATEUR a mis un %c dans la cellule %d %d\n", DEPLACEMENT_ORDINATEUR, x, y); 
            afficherPlateau(plateau); 
            indexMouvement++; 
            tourDe = HUMAIN; 
        } 
        else if (tourDe == HUMAIN) { 
            char plateauTemp[3][3]; 
            for (int i = 0; i < 3; i++) { 
                for (int j = 0; j < 3; j++) { 
                    if (plateau[i][j] == 'X') { 
                        plateauTemp[i][j] = 'x'; 
                    } 
                    else if (plateau[i][j] == 'O') { 
                        plateauTemp[i][j] = 'o'; 
                    } 
                    else { 
                        plateauTemp[i][j] = '_'; 
                    } 
                } 
            } 
            struct Déplacement mouvement = trouverMeilleurMouvement(plateauTemp); 
              //noeud++;
            x = mouvement.ligne; 
            y = mouvement.colonne; 

            plateau[x][y] = DEPLACEMENT_HUMAIN; 
            printf("ORDINATEUR a mis un %c dans la cellule %d %d\n", DEPLACEMENT_HUMAIN, x, y); 
            afficherPlateau(plateau); 
            indexMouvement++; 
            tourDe = ORDI; 
        } 
    } 

    if (!jeuTerminé(plateau) && indexMouvement == COTE * COTE) 
        printf("C'est un match nul\n"); 
    else { 
        if (tourDe == ORDI) 
            tourDe = HUMAIN; 
        else if (tourDe == HUMAIN) 
            tourDe = ORDI; 

        déclarerVainqueur(tourDe); 
    } 
} 

int main() 
{   
    EvalPerf PE;
    PE.start();
    for(int i=0;i<1000;i++){
        jouerTicTacToe(ORDI); 
        nb++;
    }
    PE.stop();
    std::cout<<"nbr cycles : "<<PE.nbCycles() <<std::endl;
    std::cout<<"nbr secondes : "<< PE.get_seconds() <<std::endl ;
    std::cout<<"nbr millisecondes : "<< PE.get_milliseconds() <<std::endl;

    std::cout<<"Taux victoire joueur qui commence: "<<(float)victory/nb<<std::endl;
    std::cout<<"Taux victoire joueur qui enchaine: "<<(float)(1.0-((float)victory/(float)nb))<<std::endl;
    std::cout<<"Noeuds visités moyens :  "<<(float)(float)noeud/(float)nb<<std::endl;
    return 0; 
}