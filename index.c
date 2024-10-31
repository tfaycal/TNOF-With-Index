#include "struct.h"
#include <string.h>
#include <stdio.h>
void addEtudiantIndexPrimary(Etudiant e, IndexEntryDense *index, int *tailleindex, int positionbloc, int positionrecord)
{
    // créer une entry index
    IndexEntryDense entry;
    if (positionbloc == 0)
        positionbloc = 1;
    int length = snprintf(NULL, 0, "%d", e.mat); // Alloue la mémoire pour la chaîne
    char *key = malloc(length + 1);              // Alloue la mémoire pour la chaîne

    if (key != NULL)
        sprintf(key, "%d", e.mat); // Convertit l'entier en chaîne
    strcpy(entry.key, key);
    entry.tabadres[0].bloc = positionbloc;
    entry.tabadres[0].record = positionrecord;
    entry.nbadr = 1;

    // rajouter l'entree dans notre table index et assurer que l'index reste soted

    int i = 0;
    while (i < (*tailleindex))
    {
        if (strcmp(entry.key, index[i].key) < 0)
        {
            // insérer l'entree
            // decalage de la table d'index
            int j;
            for (j = (*tailleindex); j >= i; j--)
                index[j + 1] = index[j];
            index[i] = entry;

            (*tailleindex)++;
            return;
        }
        i++;
    }
    index[(*tailleindex)] = entry;
    (*tailleindex)++;
}
AddresseDense *rechercherEtudiantID(int mat, IndexEntryDense *index, int tailleindex)
{

    int i = 0;
    char key[20];
    sprintf(key, "%d", mat);
    while (i < tailleindex)
    {
        
        if (strcmp(key, index[i].key) == 0)
        {

            return &(index[i].tabadres[0]);
        }
        else if (strcmp(key, index[i].key) < 0)
        {

            return NULL;
        }
        i++;
    }
    return NULL;
}
void saveIndex(IndexEntryDense *index, int tailleindex, char *path)
{
    FILE *f = fopen(path, "w");
    if (f == NULL)
    {
        perror("Erreur d'ouverture du fichier pour sauvegarde de l'index");
        return;
    }

    // Écrit la taille de l'index
    fwrite(&tailleindex, sizeof(int), 1, f);
    // Écrit le contenu de l'index
    fwrite(index, sizeof(IndexEntryDense), tailleindex, f);

    fclose(f);
}

void loadIndex(IndexEntryDense *index, int *tailleindex, char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        perror("Erreur d'ouverture du fichier pour chargement de l'index");
        return;
    }

    // Lit la taille de l'index
    fread(tailleindex, sizeof(int), 1, f);
    // Lit le contenu de l'index
    fread(index, sizeof(IndexEntryDense), *tailleindex, f);

    fclose(f);
}