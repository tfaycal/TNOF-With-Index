#include "struct.h"
void addEtudiantIndexPrimary(Etudiant, IndexEntryDense *,int*,int,int );
AddresseDense* rechercherEtudiantID(int, IndexEntryDense *,int taille);
void loadIndex(IndexEntryDense *index, int * tailleindex,char *path);
void saveIndex(IndexEntryDense *index, int  tailleindex,char *path);