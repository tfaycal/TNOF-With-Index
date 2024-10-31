
#ifndef HEADER_ETUDIANT
#define HEADER_ETUDIANT
#define MAX_REC 100
typedef struct entete
{
    int nb_bloc;
} Entete;

typedef struct tnof
{
    struct entete ent;
    char fichier[200];
} TNOF;

typedef struct etudiant
{
    char nom[100];
    char prenom[100];
    int mat;
    int etat;
} Etudiant;

typedef struct Bloc
{
    Etudiant tab_enreg[MAX_REC];
    int nb;
} Bloc;
typedef struct addresseDense
{
    int bloc;
    int record;
}AddresseDense;
typedef struct indexentrydense
{
    char key[30];
    struct  addresseDense tabadres[10];
    int nbadr;
     
}IndexEntryDense;
typedef struct indexentrynondense
{
    char * key;
    int adr;
}IndexEntryNonDense;
#endif