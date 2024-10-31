#ifndef GLOBAL_H
#define GLOBAL_H

// Déclare 'monindex' comme un tableau global pouvant être utilisé dans plusieurs fichiers
// sans le définir plusieurs fois. La définition réelle se trouve dans un fichier main.
extern IndexEntryDense monindex[];

// Déclare 'nbrelementindex' comme une variable globale (compteur d'éléments de l'index)
// pour l'utiliser dans différents fichiers sans créer plusieurs copies.
extern int nbrelementindex;

// Déclare 'TempBloc' comme une variable globale, pour partager le bloc temporaire entre les fichiers.
//L'utilisation d'un bloc temporaire (TempBloc) permet d'optimiser les performances en réduisant 
//le nombre d'écritures sur disque. Au lieu d'écrire chaque enregistrement immédiatement, 
//le bloc temporaire accumule plusieurs enregistrements en mémoire et ne les écrit sur disque que lorsque 
//le bloc est plein. Cela améliore l'efficacité des opérations d'E/S, réduit la fragmentation du fichier et 
//garantit que toutes les modifications sont enregistrées de manière atomique, évitant ainsi des états corrompus
// en cas d'erreur d'écriture.
extern Bloc TempBloc;

#endif
