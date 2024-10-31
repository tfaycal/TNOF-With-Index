#include "struct.h"
TNOF* open_file(char *,char );
void read_entete(TNOF *);
void write_entete(Entete, TNOF *);
AddresseDense * rechercher_etudiant_index(TNOF *tnof, int mat, Etudiant *etudiant, IndexEntryDense *index, int taille);
void insert_etudiant(TNOF *tnof, Etudiant etudiant, IndexEntryDense *index, int *taille);
int rechercher_etudiant(TNOF *,int,Etudiant * );
int read_bloc(TNOF *, Bloc *,int);
int write_bloc(TNOF*, Bloc,int);
void printetudiant(Etudiant);