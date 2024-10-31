#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "global.h"
#include "index.h"

TNOF *open_file(char *path, char mode)
{
    FILE *f;
    TNOF *tnof = (TNOF *)malloc(sizeof(TNOF));
    strcpy(tnof->fichier, path);
    switch (mode)
    {
    // ouverture d'un fichier existant
    case 'A':
        read_entete(tnof);
        return tnof;

    case 'N':
        f = fopen(path, "w");
        fclose(f);
        tnof->ent.nb_bloc = 0;
        write_entete(tnof->ent, tnof);
        return tnof;
    }
}
void read_entete(TNOF *tnof)
{
    FILE *f = fopen(tnof->fichier, "r+b");
    fread(&(tnof->ent), sizeof(Entete), 1, f);
    fclose(f);
}

void write_entete(Entete e, TNOF *tnof)
{
    FILE *f = fopen(tnof->fichier, "r+b");
    fwrite(&(tnof->ent), sizeof(Entete), 1, f);
    fflush(f);
    fclose(f);
}
int write_bloc(TNOF *tnof, Bloc bloc, int position)
{

    if (position == -1)
        position = 0;
    FILE *f = fopen(tnof->fichier, "r+b");
    if (tnof->ent.nb_bloc < position)
        return 0;
    // decaler le poiteur de lecture après l'entete
    fseek(f, sizeof(Entete) + (sizeof(Bloc) * position), SEEK_SET);
    fwrite(&bloc, sizeof(bloc), 1, f);
    fclose(f);
    return 1;
}
int read_bloc(TNOF *tnof, Bloc *bloc, int position)
{
    if (position == -1)
        position = 0;

    FILE *f = fopen(tnof->fichier, "r+b");
    if (tnof->ent.nb_bloc == 0 || tnof->ent.nb_bloc < position)
        return -1;

    // decaler le poiteur de lecture après l'entete
    fseek(f, sizeof(Entete) + (sizeof(Bloc) * position), SEEK_SET);
    fread(bloc, sizeof(Bloc), 1, f);
    fclose(f);
    return 1;
}
AddresseDense *rechercher_etudiant_index(TNOF *tnof, int mat, Etudiant *etudiant, IndexEntryDense *index, int taille)
{
    int i;

    // Parcourt les enregistrements dans le Bloc temporaire 'TempBloc' pour rechercher l'étudiant avec le matricule `mat`
    for (i = 0; i < TempBloc.nb; i++)
    {
        // Si le matricule correspond, l'étudiant est trouvé dans TempBloc
        if (TempBloc.tab_enreg[i].mat == mat)
        {
            *etudiant = TempBloc.tab_enreg[i]; // Copie les informations de l'étudiant trouvé
            AddresseDense *tempAdr = malloc(sizeof(AddresseDense));
            tempAdr->bloc = -1; // Indique que l'adresse est dans le bloc temporaire
            tempAdr->record = -1;
            return tempAdr; // Retourne l'adresse temporaire
        }
    }

    // Si l'index est vide, l'étudiant n'est pas présent
    if (taille == 0)
        return NULL;

    // Recherche l'étudiant dans l'index
    AddresseDense *position = rechercherEtudiantID(mat, index, taille);

    // Si l'étudiant n'est pas dans l'index, retourne NULL
    if (position == NULL)
        return NULL;

    Bloc b;

    // Lit le bloc contenant l'enregistrement de l'étudiant trouvé dans l'index
    read_bloc(tnof, &b, position->bloc);
    *etudiant = b.tab_enreg[position->record]; // Copie les informations de l'étudiant

    return position; // Retourne l'adresse dense trouvée
}

void insert_etudiant(TNOF *tnof, Etudiant etudiant, IndexEntryDense *index, int *taille)
{
    Etudiant temp;

    // Recherche si l'étudiant est déjà présent dans le fichier
    rechercher_etudiant_index(tnof, etudiant.mat, &temp, index, *taille);

    AddresseDense *nbloc = NULL;

    // Si l'étudiant est trouvé, ne rien faire (insertion annulée)
    if (nbloc != NULL)
        return;

    int positionbloc, positionrecord;

    // Si le dernier bloc a de l'espace, insère l'étudiant dans TempBloc
    if (TempBloc.nb < MAX_REC)
    {
        // Si aucun bloc n'existe, initialise le nombre de blocs à 1
        if (tnof->ent.nb_bloc == 0)
            tnof->ent.nb_bloc = 1;

        TempBloc.tab_enreg[TempBloc.nb] = etudiant; // Ajoute l'étudiant dans le bloc temporaire
        positionbloc = tnof->ent.nb_bloc;
        positionrecord = TempBloc.nb;
        TempBloc.nb++; // Augmente le nombre d'enregistrements dans TempBloc
    }
    else
    {
        // Si le bloc temporaire est plein, le sauvegarde dans le fichier
        write_bloc(tnof, TempBloc, tnof->ent.nb_bloc);

        // Ajoute l'étudiant au début d'un nouveau bloc
        TempBloc.tab_enreg[0] = etudiant;
        positionbloc = tnof->ent.nb_bloc + 1;
        positionrecord = 0;
        TempBloc.nb = 1;               // Réinitialise le compteur d'enregistrements
        tnof->ent.nb_bloc++;           // Incrémente le nombre total de blocs
        write_entete(tnof->ent, tnof); // Sauvegarde l'entête mise à jour
    }

    // Ajoute une entrée pour l'étudiant dans l'index, en maintenant l'ordre
    addEtudiantIndexPrimary(etudiant, index, taille, positionbloc, positionrecord);
}

int rechercher_etudiant(TNOF *tnof, int mat, Etudiant *etudiant)
{
    Bloc bloc;
    int i, j;
    // Parcourir tous les blocs du fichier
    for (i = 0; i < tnof->ent.nb_bloc; i++)
    {
        // Lire le bloc actuel
        if (read_bloc(tnof, &bloc, i) == -1)
        {
            printf("Erreur de lecture du bloc %d\n", i);
            return -1;
        }

        // Parcourir les enregistrements dans ce bloc
        for (j = 0; j < bloc.nb; j++)
        {
            // Vérifier si le matricule correspond
            if (bloc.tab_enreg[j].mat == mat)
            {
                // Si l'étudiant est trouvé, remplir la structure 'etudiant'
                *etudiant = bloc.tab_enreg[j];
                return i; // Retourner le numéro du bloc
            }
        }
    }

    // Si aucun étudiant n'est trouvé
    return -1;
}

void printetudiant(Etudiant e)
{
    printf("Etudiant[%d %s %s]\n", e.mat, e.nom, e.prenom);
}