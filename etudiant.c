#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "global.h"
#include "index.h"
#include "etudiant.h"


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
        write_entet(tnof->ent, tnof);
        return tnof;
    }
}
void read_entete(TNOF *tnof)
{
    FILE *f = fopen(tnof->fichier, "r+b");
    fread(&(tnof->ent), sizeof(Entete), 1, f);
    fclose(f);
}

void write_entet(Entete e, TNOF *tnof)
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
    for (i = 0; i < TempBloc.nb; i++)
    {
        if (TempBloc.tab_enreg[i].mat == mat)
        {
            *etudiant = TempBloc.tab_enreg[i];
            AddresseDense *tempAdr = malloc(sizeof(AddresseDense));
            tempAdr->bloc = -1;
            tempAdr->record = -1;
            return tempAdr;
        }
    }

    if (taille == 0)
        return NULL;
    AddresseDense *position = rechercherEtudiantID(mat, index, taille);

    if (position == NULL)
        return -1;
    Bloc b;

    read_bloc(tnof, &b, position->bloc );
    *etudiant = b.tab_enreg[position->record];
   
    return position;
}

void insert_etudiant(TNOF *tnof, Etudiant etudiant, IndexEntryDense *index, int *taille)
{
    Etudiant temp;
    // rechercher etudiant et stocker son numéro du bloc
    rechercher_etudiant_index(tnof, etudiant.mat, &temp, index, taille);
    AddresseDense *nbloc = NULL;
    if (nbloc != NULL)
        return;
    int positionbloc, positionrecord;
    // Le dernier bloc contient de l'espace
    if (TempBloc.nb < MAX_REC)
    {
        if (tnof->ent.nb_bloc == 0)
            tnof->ent.nb_bloc = 1;
        TempBloc.tab_enreg[TempBloc.nb] = etudiant;
        positionbloc = tnof->ent.nb_bloc;
        positionrecord = TempBloc.nb;
        TempBloc.nb++;
    }
    else
    {
        // le bloc est complet
        write_bloc(tnof, TempBloc, tnof->ent.nb_bloc);
        TempBloc.tab_enreg[0] = etudiant;
        positionbloc = tnof->ent.nb_bloc + 1;
        positionrecord = 0;
        TempBloc.nb = 1;
        tnof->ent.nb_bloc++;
        write_entet(tnof->ent, tnof);
    }

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