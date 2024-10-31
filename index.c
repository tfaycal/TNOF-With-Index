#include "struct.h"
#include <string.h>
#include <stdio.h>
void addEtudiantIndexPrimary(Etudiant e, IndexEntryDense *index, int *tailleindex, int positionbloc, int positionrecord)
{
    // Créer une nouvelle entrée d'index pour l'étudiant
    IndexEntryDense entry;
    
    // Si le numéro de bloc est à 0, on le met à 1 (pour éviter une position non valide)
    if (positionbloc == 0)
        positionbloc = 1;

    // Convertit le matricule de l'étudiant (`e.mat`) en une chaîne de caractères
    // et la copie dans `entry.key`
    char key[20];              // Buffer pour stocker la version en chaîne de `mat`
    sprintf(key, "%d", e.mat); // Convertit l'entier en chaîne de caractères
    strcpy(entry.key, key);

    // Initialise les adresses dans `entry` avec le bloc et l'enregistrement spécifiés
    entry.tabadres[0].bloc = positionbloc;
    entry.tabadres[0].record = positionrecord;
    entry.nbadr = 1;  // Initialise le nombre d'adresses à 1 pour cette entrée

    // Insère l'entrée dans la table d'index tout en maintenant un ordre trié
    int i = 0;
    while (i < (*tailleindex))
    {
        // Compare la clé de `entry` avec celles déjà présentes pour trouver sa position
        if (strcmp(entry.key, index[i].key) < 0)
        {
            // Décale les entrées pour créer un espace pour la nouvelle entrée
            int j;
            for (j = (*tailleindex); j >= i; j--)
                index[j + 1] = index[j];
            
            // Place l'entrée `entry` dans la position correcte
            index[i] = entry;

            // Incrémente la taille de l'index
            (*tailleindex)++;
            return;  // Fin de la fonction après insertion
        }
        i++;
    }
    // Si `entry` a la plus grande clé, l'ajoute à la fin de l'index
    index[(*tailleindex)] = entry;
    (*tailleindex)++;  // Incrémente la taille de l'index
}
AddresseDense *rechercherEtudiantID(int mat, IndexEntryDense *index, int tailleindex)
{
    // Initialise un compteur pour parcourir l'index
    int i = 0;
    
    // Convertit le matricule de l'étudiant (`mat`) en une chaîne de caractères
    char key[20];
    sprintf(key, "%d", mat);  // Convertit l'entier en chaîne pour la comparaison

    // Parcourt l'index pour rechercher l'étudiant correspondant
    while (i < tailleindex)
    {
        // Compare la clé `key` avec celle de l'entrée actuelle dans l'index
        if (strcmp(key, index[i].key) == 0)
            // Si la clé correspond, retourne l'adresse du premier enregistrement associé
            return &(index[i].tabadres[0]);

        // Si la clé recherchée est inférieure à la clé actuelle de l'index, elle n'est pas présente
        else if (strcmp(key, index[i].key) < 0)
            return NULL;

        // Passe à l'entrée suivante de l'index
        i++;
    }

    // Si aucune correspondance n'est trouvée dans l'index, retourne NULL
    return NULL;
}

void saveIndex(IndexEntryDense *index, int tailleindex, char *path)
{
    FILE *f = fopen(path, "wb"); // Open in binary write mode
    if (f == NULL)
    {
        perror("Erreur d'ouverture du fichier pour sauvegarde de l'index");
        return;
    }

    // Write the size of the index
    fwrite(&tailleindex, sizeof(int), 1, f);
    // Write the content of the index
    fwrite(index, sizeof(IndexEntryDense), tailleindex, f);

    fclose(f);
}

void loadIndex(IndexEntryDense *index, int *tailleindex, char *path)
{
    FILE *f = fopen(path, "rb"); // Open in binary read mode
    if (f == NULL)
    {
        perror("Erreur d'ouverture du fichier pour chargement de l'index");
        return;
    }

    // Read the size of the index
    fread(tailleindex, sizeof(int), 1, f);
    // Read the content of the index, ensuring space is allocated for `index`
    fread(index, sizeof(IndexEntryDense), *tailleindex, f);

    fclose(f);
}
