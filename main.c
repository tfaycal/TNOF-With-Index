#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "global.h"
#include "etudiant.h"
#include "index.h"

// Prototypes des fonctions
void afficher_menu();
void gerer_menu(TNOF *tnof, int);
void afficher_index(IndexEntryDense monindex[], int nbrelementindex);
void afficherBloc(Bloc b);
IndexEntryDense monindex[100000];
int nbrelementindex = 0;
Bloc TempBloc;

int main()
{
    TempBloc.nb = 0;

    TNOF *tnof = NULL;
    char path[100];
    char mode;
    int choix;

    printf("Entrez le chemin du fichier : ");
    scanf("%s", path);

    printf("Mode d'ouverture (A: Ouvrir un fichier existant, N: Nouveau fichier) : ");
    scanf(" %c", &mode);

    tnof = open_file(path, mode);

    if (tnof == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return -1;
    }

    do
    {
        afficher_menu();
        printf("Choix : ");
        scanf("%d", &choix);

        gerer_menu(tnof, choix);

    } while (choix != 0);

    printf("Fermeture du programme.\n");
    return 0;
}

void afficher_menu()
{
    printf("\n---- Menu ----\n");
    printf("1. Inserer un etudiant\n");
    printf("2. Lire un TempBloc\n");
    printf("3. Afficher l'entete\n");
    printf("4. Afficher un etudiant (recherche par matricule)\n");
    printf("5. Afficher l'index\n"); // Nouvelle option ajoutée
    printf("6. save l'index\n");     // Nouvelle option ajoutée
    printf("7. load l'index\n");     // Nouvelle option ajoutée
    printf("0. Quitter\n");
    printf("----------------\n");
}

void gerer_menu(TNOF *tnof, int choix)
{
    Etudiant etudiant;
    int mat, position;

    switch (choix)
    {
    case 1:
        printf("Insertion d'un etudiant.\n");
        printf("Matricule : ");
        scanf("%d", &etudiant.mat);
        printf("Nom : ");
        scanf("%s", etudiant.nom);
        printf("Prenom : ");
        scanf("%s", etudiant.prenom);
        int n = 230, i = 0;
        while (i != n)
        {
            etudiant.mat++;

            insert_etudiant(tnof, etudiant, monindex, &nbrelementindex);
            i++;
        }

        break;

    case 2:
        printf("Lecture d'un TempBloc.\n");
        printf("Entrez la position du TempBloc à lire : ");
        scanf("%d", &position);
        Bloc b;
        if (read_bloc(tnof, &b, position) == 1)
        {
            int i;
            printf("TempBloc %d lu avec succes.\n", position);
            afficherBloc(b);
        }
        else
        {
            printf("Erreur de lecture du TempBloc.\n");
        }
        break;

    case 3:
        printf("Affichage de l'entete.\n");
        printf("Nombre de TempBlocs : %d\n", tnof->ent.nb_bloc);
        break;

    case 4:
        printf("Recherche d'un etudiant.\n");
        printf("Entrez le matricule de l'etudiant : ");
        scanf("%d", &mat);
        AddresseDense *position;
        if ((position = rechercher_etudiant_index(tnof, mat, &etudiant, monindex, nbrelementindex)) != NULL)
        {
            printf("Etudiant trouve : Bloc %d Record %d\n",position->bloc,position->record);
            printetudiant(etudiant);
        }
        else
        {
            printf("Etudiant non trouve.\n");
        }
        break;

    case 5:
        afficher_index(monindex, nbrelementindex); // Appel de la fonction afficher_index
        break;
    case 6:
        saveIndex(monindex, nbrelementindex, "index.indx"); // Appel de la fonction afficher_index
        break;
    case 7:
        loadIndex(monindex, &nbrelementindex, "index.indx"); // Appel de la fonction afficher_index
        break;
    case 8:
        afficherBloc(TempBloc);
        break;
    case 0:
        printf("Saving index !\n");
        saveIndex(monindex, nbrelementindex, "index.indx");
        printf("Quitter.\n");
        break;

    default:
        printf("Choix non valide.\n");
        break;
    }
}
void afficherBloc(Bloc b)
{
    int i;
    for (i = 0; i < b.nb; i++)
    {
        printetudiant(b.tab_enreg[i]);
    }
}
void afficher_index(IndexEntryDense monindex[], int nbrelementindex)
{
    printf("\n---- Index ----\n");
    int i;
    for (i = 0; i < nbrelementindex; i++)
    {
        printf("Key: %s, Adrs: <%d,%d>\n", monindex[i].key, monindex[i].tabadres[0].bloc, monindex[i].tabadres[0].record);
    }
    printf("----------------\n");
}
