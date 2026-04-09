#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CLIENTS 50
#define MAX_COMPTES 100
#define MAX_EMPLOYES 10
#define SOLDE_MIN 1000.0f
#define MAX_RETRAIT 700.0f

//couleurs ANSI
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define BG_BLUE "\033[44m"
#define BG_GREEN "\033[42m"

typedef struct {
    int id_client;
    char nom[50];
    char prenom[50];
    char profession[50];
    char num_tel[15];
    char mot_de_passe[20];  // Nouveau champ pour l'authentification
} Client;

typedef struct {
    int id_compte;
    int id_client;
    float solde;
    char date_ouverture[11];
} Compte;

typedef struct {
    int id_employe;
    char nom[50];
    char prenom[50];
    char mot_de_passe[20];
} Employe;

Client clients[MAX_CLIENTS];
Compte comptes[MAX_COMPTES];
Employe employes[MAX_EMPLOYES];
int nb_clients = 0;
int nb_comptes = 0;
int nb_employes = 0;

// Variables globales pour la session
int utilisateur_connecte_id = -1;
int est_employe = 0;

// Fonction pour effacer l'écran
void clearScreen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Fonction pour afficher un séparateur
void afficherSeparateur(void) {
    printf(CYAN "====================================================================\n" RESET);
}

// Fonction pour afficher un titre
void afficherTitre(const char* titre) {
    clearScreen();
    printf("\n");
    afficherSeparateur();
    printf(BOLD CYAN "  %s\n" RESET, titre);
    afficherSeparateur();
    printf("\n");
}

int chercherClientIndex(int id_client) {
    for (int i = 0; i < nb_clients; i++)
        if (clients[i].id_client == id_client)
            return i;
    return -1;
}

int chercherCompteIndex(int id_compte) {
    for (int i = 0; i < nb_comptes; i++)
        if (comptes[i].id_compte == id_compte)
            return i;
    return -1;
}

int chercherEmployeIndex(int id_employe) {
    for (int i = 0; i < nb_employes; i++)
        if (employes[i].id_employe == id_employe)
            return i;
    return -1;
}

void afficherUnClient(int i) {
    printf(YELLOW "+------------------------------------------+\n" RESET);
    printf(YELLOW "|" RESET " " BOLD "Id Client   :" RESET " %-25d" YELLOW "|\n" RESET, clients[i].id_client);
    printf(YELLOW "|" RESET " " BOLD "Nom         :" RESET " %-25s" YELLOW "|\n" RESET, clients[i].nom);
    printf(YELLOW "|" RESET " " BOLD "Prenom      :" RESET " %-25s" YELLOW "|\n" RESET, clients[i].prenom);
    printf(YELLOW "|" RESET " " BOLD "Profession  :" RESET " %-25s" YELLOW "|\n" RESET, clients[i].profession);
    printf(YELLOW "|" RESET " " BOLD "Num Tel     :" RESET " %-25s" YELLOW "|\n" RESET, clients[i].num_tel);
    printf(YELLOW "+------------------------------------------+\n" RESET);
}

void afficherUnCompte(int i) {
    printf(CYAN "+------------------------------------------+\n" RESET);
    printf(CYAN "|" RESET " " BOLD "Id Compte      :" RESET " %-21d" CYAN "|\n" RESET, comptes[i].id_compte);
    printf(CYAN "|" RESET " " BOLD "Id Client      :" RESET " %-21d" CYAN "|\n" RESET, comptes[i].id_client);
    printf(CYAN "|" RESET " " BOLD "Solde          :" RESET " %-18.2f DH" CYAN "|\n" RESET, comptes[i].solde);
    printf(CYAN "|" RESET " " BOLD "Date Ouverture :" RESET " %-21s" CYAN "|\n" RESET, comptes[i].date_ouverture);
    printf(CYAN "+------------------------------------------+\n" RESET);
}

// Affichage des comptes en tableau
void afficherTableauComptes(void) {
    if (nb_comptes == 0) {
        printf(RED "  Aucun compte enregistre.\n" RESET);
        return;
    }

    printf(GREEN);
    printf("+==============+==============+=================+=================+\n");
    printf("| Id Compte    | Id Client    | Solde (DH)      | Date Ouverture  |\n");
    printf("+==============+==============+=================+=================+\n");
    printf(RESET);

    for (int i = 0; i < nb_comptes; i++) {
        printf(GREEN "|" RESET " %-12d " GREEN "|" RESET " %-12d " GREEN "|" RESET " %15.2f " GREEN "|" RESET " %-15s " GREEN "|\n" RESET,
               comptes[i].id_compte,
               comptes[i].id_client,
               comptes[i].solde,
               comptes[i].date_ouverture);
    }

    printf(GREEN "+==============+==============+=================+=================+\n" RESET);
}

void pauseConsole(void) {
    printf(YELLOW "\nAppuyez sur Entree pour continuer..." RESET);
    clearInputBuffer();
    getchar();
}

// INITIALISATION - Créer des données de test
void initialiserDonnees(void) {
    // Ajouter un employé par défaut
    employes[0].id_employe = 1;
    strcpy(employes[0].nom, "Admin");
    strcpy(employes[0].prenom, "Banque");
    strcpy(employes[0].mot_de_passe, "admin123");
    nb_employes = 1;

    // Ajouter un client de test
    clients[0].id_client = 101;
    strcpy(clients[0].nom, "Fadl");
    strcpy(clients[0].prenom, "Mohamed");
    strcpy(clients[0].profession, "Etudiant");
    strcpy(clients[0].num_tel, "0612345678");
    strcpy(clients[0].mot_de_passe, "client123");
    nb_clients = 1;

    // Ajouter un compte de test
    comptes[0].id_compte = 1001;
    comptes[0].id_client = 101;
    comptes[0].solde = 5000.0f;
    strcpy(comptes[0].date_ouverture, "01/01/2025");
    nb_comptes = 1;
}

// SYSTÈME D'AUTHENTIFICATION

int authentifierEmploye(int id, const char* mdp) {
    int idx = chercherEmployeIndex(id);
    if (idx == -1) return 0;
    return strcmp(employes[idx].mot_de_passe, mdp) == 0;
}

int authentifierClient(int id, const char* mdp) {
    int idx = chercherClientIndex(id);
    if (idx == -1) return 0;
    return strcmp(clients[idx].mot_de_passe, mdp) == 0;
}

int menuConnexion(void) {
    afficherTitre("BIENVENUE - SYSTEME BANCAIRE");

    printf(GREEN "  1)" RESET " Connexion Employe\n");
    printf(GREEN "  2)" RESET " Connexion Client\n");
    printf(RED "  3)" RESET " Quitter\n");
    printf("\n" CYAN "Choix : " RESET);

    int choix;
    if (scanf("%d", &choix) != 1) {
        clearInputBuffer();
        return 0;
    }

    if (choix == 3) return 3;

    int id;
    char mdp[20];

    if (choix == 1) {
        // Connexion employé
        printf("\n" BOLD "=== CONNEXION EMPLOYE ===\n" RESET);
        printf(CYAN "ID Employe : " RESET);
        if (scanf("%d", &id) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            return 0;
        }

        printf(CYAN "Mot de passe : " RESET);
        scanf("%19s", mdp);

        if (authentifierEmploye(id, mdp)) {
            utilisateur_connecte_id = id;
            est_employe = 1;
            int idx = chercherEmployeIndex(id);
            printf(GREEN "\nConnexion reussie! Bienvenue %s %s\n" RESET,
                   employes[idx].prenom, employes[idx].nom);
            pauseConsole();
            return 1;
        } else {
            printf(RED "\nErreur: Identifiants incorrects.\n" RESET);
            pauseConsole();
            return 0;
        }

    } else if (choix == 2) {
        // Connexion client
        printf("\n" BOLD "=== CONNEXION CLIENT ===\n" RESET);
        printf(CYAN "ID Client : " RESET);
        if (scanf("%d", &id) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            return 0;
        }

        printf(CYAN "Mot de passe : " RESET);
        scanf("%19s", mdp);

        if (authentifierClient(id, mdp)) {
            utilisateur_connecte_id = id;
            est_employe = 0;
            int idx = chercherClientIndex(id);
            printf(GREEN "\nConnexion reussie! Bienvenue %s %s\n" RESET,
                   clients[idx].prenom, clients[idx].nom);
            pauseConsole();
            return 2;
        } else {
            printf(RED "\nErreur: Identifiants incorrects.\n" RESET);
            pauseConsole();
            return 0;
        }
    }

    return 0;
}

//GESTION CLIENTS (EMPLOYÉ)

void ajouterClient(void) {
    afficherTitre("AJOUTER UN NOUVEAU CLIENT");

    if (nb_clients >= MAX_CLIENTS) {
        printf(RED "Erreur: Stock clients plein (%d)\n" RESET, MAX_CLIENTS);
        pauseConsole();
        return;
    }

    int id;
    printf(CYAN "Entrer Id_client : " RESET);
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (chercherClientIndex(id) != -1) {
        printf(RED "Erreur: Client existe deja.\n" RESET);
        pauseConsole();
        return;
    }

    clients[nb_clients].id_client = id;

    printf(CYAN "Nom : " RESET);
    scanf("%49s", clients[nb_clients].nom);

    printf(CYAN "Prenom : " RESET);
    scanf("%49s", clients[nb_clients].prenom);

    printf(CYAN "Profession : " RESET);
    scanf("%49s", clients[nb_clients].profession);

    printf(CYAN "Num_tel : " RESET);
    scanf("%14s", clients[nb_clients].num_tel);

    printf(CYAN "Mot de passe : " RESET);
    scanf("%19s", clients[nb_clients].mot_de_passe);

    nb_clients++;
    printf(GREEN "\nSucces: Client ajoute avec succes!\n" RESET);
    pauseConsole();
}

void modifierClient(void) {
    afficherTitre("MODIFIER UN CLIENT");

    int id;
    printf(CYAN "Entrer Id_client a modifier : " RESET);
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    int i = chercherClientIndex(id);
    if (i == -1) {
        printf(RED "Erreur: Client introuvable.\n" RESET);
        pauseConsole();
        return;
    }

    int choix;
    do {
        clearScreen();
        printf("\n");
        afficherSeparateur();
        printf(BOLD CYAN "  MODIFIER CLIENT - ID: %d\n" RESET, id);
        afficherSeparateur();
        printf("\n");

        printf(YELLOW "  Client actuel:\n" RESET);
        afficherUnClient(i);
        printf("\n");

        printf(CYAN "  1)" RESET " Modifier le nom\n");
        printf(CYAN "  2)" RESET " Modifier le prenom\n");
        printf(CYAN "  3)" RESET " Modifier la profession\n");
        printf(CYAN "  4)" RESET " Modifier le numero de telephone\n");
        printf(CYAN "  5)" RESET " Afficher les informations\n");
        printf(RED "  0)" RESET " Retour\n");
        printf("\n" CYAN "Choix : " RESET);

        if (scanf("%d", &choix) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            return;
        }

        switch (choix) {
            case 1:
                printf(CYAN "Nouveau nom : " RESET);
                scanf("%49s", clients[i].nom);
                printf(GREEN "Succes: Modifie!\n" RESET);
                pauseConsole();
                break;
            case 2:
                printf(CYAN "Nouveau prenom : " RESET);
                scanf("%49s", clients[i].prenom);
                printf(GREEN "Succes: Modifie!\n" RESET);
                pauseConsole();
                break;
            case 3:
                printf(CYAN "Nouvelle profession : " RESET);
                scanf("%49s", clients[i].profession);
                printf(GREEN "Succes: Modifie!\n" RESET);
                pauseConsole();
                break;
            case 4:
                printf(CYAN "Nouveau num_tel : " RESET);
                scanf("%14s", clients[i].num_tel);
                printf(GREEN "Succes: Modifie!\n" RESET);
                pauseConsole();
                break;
            case 5:
                afficherUnClient(i);
                pauseConsole();
                break;
            case 0:
                break;
            default:
                printf(RED "Erreur: Choix invalide.\n" RESET);
                pauseConsole();
        }
    } while (choix != 0);
}

void supprimerClient(void) {
    afficherTitre("SUPPRIMER UN CLIENT");

    int id;
    char rep;

    printf(CYAN "Entrer Id_client a supprimer : " RESET);
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    int i = chercherClientIndex(id);
    if (i == -1) {
        printf(RED "Erreur: Client introuvable.\n" RESET);
        pauseConsole();
        return;
    }

    printf("\n" YELLOW "  Client a supprimer:\n" RESET);
    afficherUnClient(i);

    printf("\n" RED "ATTENTION! " RESET BOLD "Confirmer la suppression (O/N) : " RESET);
    scanf(" %c", &rep);

    if (rep == 'O' || rep == 'o') {
        clients[i] = clients[nb_clients - 1];
        nb_clients--;
        printf(GREEN "\nSucces: Client supprime avec succes.\n" RESET);
    } else {
        printf(YELLOW "\nSuppression annulee.\n" RESET);
    }
    pauseConsole();
}

void rechercherClient(void) {
    afficherTitre("RECHERCHER UN CLIENT");

    int choix;
    printf(CYAN "  1)" RESET " Rechercher par Id_client\n");
    printf(CYAN "  2)" RESET " Rechercher par Nom\n");
    printf("\n" CYAN "Choix : " RESET);

    if (scanf("%d", &choix) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (choix == 1) {
        int id;
        printf(CYAN "Id_client : " RESET);
        if (scanf("%d", &id) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            return;
        }

        int i = chercherClientIndex(id);
        if (i == -1) {
            printf(RED "\nErreur: Client introuvable.\n" RESET);
        } else {
            printf("\n" GREEN "Client trouve:\n" RESET);
            afficherUnClient(i);
        }

    } else if (choix == 2) {
        char nom[50];
        printf(CYAN "Nom : " RESET);
        scanf("%49s", nom);

        int trouve = 0;
        for (int i = 0; i < nb_clients; i++) {
            if (strcmp(clients[i].nom, nom) == 0) {
                if (!trouve) printf("\n" GREEN "Client(s) trouve(s):\n\n" RESET);
                afficherUnClient(i);
                printf("\n");
                trouve = 1;
            }
        }
        if (!trouve) printf(RED "\nErreur: Aucun client avec ce nom.\n" RESET);
    } else {
        printf(RED "Erreur: Choix invalide.\n" RESET);
    }

    pauseConsole();
}

void menuClients(void) {
    int choix;
    do {
        afficherTitre("GESTION DES CLIENTS");

        printf(CYAN "  1)" RESET " Ajouter un client\n");
        printf(CYAN "  2)" RESET " Modifier un client\n");
        printf(CYAN "  3)" RESET " Supprimer un client\n");
        printf(CYAN "  4)" RESET " Rechercher un client\n");
        printf(RED "  0)" RESET " Retour au menu principal\n");
        printf("\n" CYAN "Choix : " RESET);

        if (scanf("%d", &choix) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            continue;
        }

        switch (choix) {
            case 1: ajouterClient(); break;
            case 2: modifierClient(); break;
            case 3: supprimerClient(); break;
            case 4: rechercherClient(); break;
            case 0: break;
            default:
                printf(RED "Erreur: Choix invalide.\n" RESET);
                pauseConsole();
        }
    } while (choix != 0);
}

//GESTION COMPTES (EMPLOYÉ)

void nouveauCompte(void) {
    afficherTitre("CREER UN NOUVEAU COMPTE");

    if (nb_comptes >= MAX_COMPTES) {
        printf(RED "Erreur: Stock comptes plein (%d)\n" RESET, MAX_COMPTES);
        pauseConsole();
        return;
    }

    int id_compte, id_client;
    float solde;

    printf(CYAN "Entrer Id_compte : " RESET);
    if (scanf("%d", &id_compte) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (chercherCompteIndex(id_compte) != -1) {
        printf(RED "Erreur: Compte existe deja.\n" RESET);
        pauseConsole();
        return;
    }

    printf(CYAN "Entrer Id_client proprietaire : " RESET);
    if (scanf("%d", &id_client) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (chercherClientIndex(id_client) == -1) {
        printf(RED "Erreur: Client inexistant.\n" RESET);
        pauseConsole();
        return;
    }

    printf(CYAN "Solde initial (>= %.0f DH) : " RESET, SOLDE_MIN);
    if (scanf("%f", &solde) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (solde < SOLDE_MIN) {
        printf(RED "Erreur: Refuse: solde minimum = %.0f DH.\n" RESET, SOLDE_MIN);
        pauseConsole();
        return;
    }

    comptes[nb_comptes].id_compte = id_compte;
    comptes[nb_comptes].id_client = id_client;
    comptes[nb_comptes].solde = solde;

    printf(CYAN "Date ouverture (JJ/MM/AAAA) : " RESET);
    scanf("%10s", comptes[nb_comptes].date_ouverture);

    nb_comptes++;
    printf(GREEN "\nSucces: Compte cree avec succes!\n" RESET);
    pauseConsole();
}

void consultationComptes(void) {
    afficherTitre("CONSULTATION DES COMPTES");

    int choix;
    printf(CYAN "  1)" RESET " Afficher tous les comptes\n");
    printf(CYAN "  2)" RESET " Afficher un compte specifique\n");
    printf("\n" CYAN "Choix : " RESET);

    if (scanf("%d", &choix) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (choix == 1) {
        if (nb_comptes == 0) {
            printf(RED "\nErreur: Aucun compte enregistre.\n" RESET);
        } else {
            printf("\n");
            afficherTableauComptes();
        }
    } else if (choix == 2) {
        int id;
        printf(CYAN "Id_compte : " RESET);
        if (scanf("%d", &id) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            return;
        }

        int i = chercherCompteIndex(id);
        if (i == -1) {
            printf(RED "\nErreur: Compte introuvable.\n" RESET);
        } else {
            printf("\n" GREEN "Compte trouve:\n\n" RESET);
            afficherUnCompte(i);
        }
    } else {
        printf(RED "Erreur: Choix invalide.\n" RESET);
    }

    pauseConsole();
}

void fermetureCompte(void) {
    afficherTitre("FERMER UN COMPTE");

    int id;
    char rep;

    printf(CYAN "Entrer Id_compte a fermer : " RESET);
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    int i = chercherCompteIndex(id);
    if (i == -1) {
        printf(RED "Erreur: Compte introuvable.\n" RESET);
        pauseConsole();
        return;
    }

    printf("\n" YELLOW "  Compte a fermer:\n\n" RESET);
    afficherUnCompte(i);

    printf("\n" RED "ATTENTION! " RESET BOLD "Confirmer la fermeture (O/N) : " RESET);
    scanf(" %c", &rep);

    if (rep == 'O' || rep == 'o') {
        comptes[i] = comptes[nb_comptes - 1];
        nb_comptes--;
        printf(GREEN "\nSucces: Compte ferme avec succes.\n" RESET);
    } else {
        printf(YELLOW "\nFermeture annulee.\n" RESET);
    }
    pauseConsole();
}

void menuComptes(void) {
    int choix;
    do {
        afficherTitre("GESTION DES COMPTES");

        printf(CYAN "  1)" RESET " Creer un nouveau compte\n");
        printf(CYAN "  2)" RESET " Consulter les comptes\n");
        printf(CYAN "  3)" RESET " Fermer un compte\n");
        printf(RED "  0)" RESET " Retour au menu principal\n");
        printf("\n" CYAN "Choix : " RESET);

        if (scanf("%d", &choix) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            continue;
        }

        switch (choix) {
            case 1: nouveauCompte(); break;
            case 2: consultationComptes(); break;
            case 3: fermetureCompte(); break;
            case 0: break;
            default:
                printf(RED "Erreur: Choix invalide.\n" RESET);
                pauseConsole();
        }
    } while (choix != 0);
}

//GESTION OPERATIONS (EMPLOYÉ)

void retrait(void) {
    afficherTitre("EFFECTUER UN RETRAIT");

    int id;
    float m;

    printf(CYAN "Id_compte : " RESET);
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    int i = chercherCompteIndex(id);
    if (i == -1) {
        printf(RED "Erreur: Compte introuvable.\n" RESET);
        pauseConsole();
        return;
    }

    printf("\n" YELLOW "  Compte selectionne:\n\n" RESET);
    afficherUnCompte(i);

    printf("\n" CYAN "Montant a retirer (<= %.0f DH) : " RESET, MAX_RETRAIT);
    if (scanf("%f", &m) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (m <= 0) {
        printf(RED "Erreur: Montant invalide.\n" RESET);
        pauseConsole();
        return;
    }
    if (m > MAX_RETRAIT) {
        printf(RED "Erreur: Refuse: retrait maximum = %.0f DH.\n" RESET, MAX_RETRAIT);
        pauseConsole();
        return;
    }
    if (m > comptes[i].solde) {
        printf(RED "Erreur: Refuse: solde insuffisant.\n" RESET);
        pauseConsole();
        return;
    }

    comptes[i].solde -= m;
    printf(GREEN "\nSucces: Retrait effectue avec succes!\n" RESET);
    printf(BOLD "  Nouveau solde: " RESET GREEN "%.2f DH\n" RESET, comptes[i].solde);
    pauseConsole();
}

void virement(void) {
    afficherTitre("EFFECTUER UN VIREMENT");

    int src, dst;
    float m;
    char confirmation;

    printf(CYAN "Id_compte source : " RESET);
    if (scanf("%d", &src) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    printf(CYAN "Id_compte destination : " RESET);
    if (scanf("%d", &dst) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    int i = chercherCompteIndex(src);
    int j = chercherCompteIndex(dst);

    if (i == -1 || j == -1) {
        printf(RED "Erreur: Compte source ou destination introuvable.\n" RESET);
        pauseConsole();
        return;
    }
    if (src == dst) {
        printf(RED "Erreur: Virement impossible: meme compte.\n" RESET);
        pauseConsole();
        return;
    }

    printf("\n" YELLOW "  Compte source:\n\n" RESET);
    afficherUnCompte(i);

    printf("\n" YELLOW "  Compte destination:\n\n" RESET);
    afficherUnCompte(j);

    printf("\n" CYAN "Montant a virer : " RESET);
    if (scanf("%f", &m) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (m <= 0) {
        printf(RED "Erreur: Montant invalide.\n" RESET);
        pauseConsole();
        return;
    }
    if (m > comptes[i].solde) {
        printf(RED "Erreur: Refuse: solde insuffisant.\n" RESET);
        pauseConsole();
        return;
    }

    // Demande de confirmation pour le virement
    printf("\n");
    afficherSeparateur();
    printf(BOLD YELLOW "  CONFIRMATION DU VIREMENT\n" RESET);
    afficherSeparateur();
    printf(YELLOW "  Montant      : " RESET BOLD "%.2f DH\n" RESET, m);
    printf(YELLOW "  De           : " RESET "Compte %d\n", src);
    printf(YELLOW "  Vers         : " RESET "Compte %d\n", dst);
    afficherSeparateur();
    printf(BOLD "  Confirmer le virement (O/N) : " RESET);
    scanf(" %c", &confirmation);

    if (confirmation == 'O' || confirmation == 'o') {
        comptes[i].solde -= m;
        comptes[j].solde += m;

        printf(GREEN "\nSucces: Virement effectue avec succes!\n\n" RESET);
        printf(BOLD "  Solde source      : " RESET GREEN "%.2f DH\n" RESET, comptes[i].solde);
        printf(BOLD "  Solde destination : " RESET GREEN "%.2f DH\n" RESET, comptes[j].solde);
    } else {
        printf(YELLOW "\nVirement annule.\n" RESET);
    }

    pauseConsole();
}

void menuOperations(void) {
    int choix;
    do {
        afficherTitre("GESTION DES OPERATIONS");

        printf(CYAN "  1)" RESET " Effectuer un retrait\n");
        printf(CYAN "  2)" RESET " Effectuer un virement\n");
        printf(RED "  0)" RESET " Retour au menu principal\n");
        printf("\n" CYAN "Choix : " RESET);

        if (scanf("%d", &choix) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            continue;
        }

        switch (choix) {
            case 1: retrait(); break;
            case 2: virement(); break;
            case 0: break;
            default:
                printf(RED "Erreur: Choix invalide.\n" RESET);
                pauseConsole();
        }
    } while (choix != 0);
}

// FONCTIONS POUR CLIENT

void consulterMesComptes(void) {
    afficherTitre("MES COMPTES");

    int trouve = 0;
    printf("\n");
    printf(GREEN);
    printf("+==============+=================+=================+\n");
    printf("| Id Compte    | Solde (DH)      | Date Ouverture  |\n");
    printf("+==============+=================+=================+\n");
    printf(RESET);

    for (int i = 0; i < nb_comptes; i++) {
        if (comptes[i].id_client == utilisateur_connecte_id) {
            printf(GREEN "|" RESET " %-12d " GREEN "|" RESET " %15.2f " GREEN "|" RESET " %-15s " GREEN "|\n" RESET,
                   comptes[i].id_compte,
                   comptes[i].solde,
                   comptes[i].date_ouverture);
            trouve = 1;
        }
    }

    if (!trouve) {
        printf(RED "\nVous n'avez aucun compte.\n" RESET);
    } else {
        printf(GREEN "+==============+=================+=================+\n" RESET);
    }

    pauseConsole();
}

void retraitClient(void) {
    afficherTitre("EFFECTUER UN RETRAIT");

    // Afficher mes comptes d'abord
    printf(YELLOW "Vos comptes:\n" RESET);
    int compte_trouve = 0;
    for (int i = 0; i < nb_comptes; i++) {
        if (comptes[i].id_client == utilisateur_connecte_id) {
            printf("  - Compte %d: %.2f DH\n", comptes[i].id_compte, comptes[i].solde);
            compte_trouve = 1;
        }
    }

    if (!compte_trouve) {
        printf(RED "\nVous n'avez aucun compte.\n" RESET);
        pauseConsole();
        return;
    }

    int id;
    float m;

    printf("\n" CYAN "Id_compte : " RESET);
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    int i = chercherCompteIndex(id);
    if (i == -1) {
        printf(RED "Erreur: Compte introuvable.\n" RESET);
        pauseConsole();
        return;
    }

    // Vérifier que le compte appartient au client connecté
    if (comptes[i].id_client != utilisateur_connecte_id) {
        printf(RED "Erreur: Ce compte ne vous appartient pas.\n" RESET);
        pauseConsole();
        return;
    }

    printf("\n" YELLOW "  Compte selectionne:\n\n" RESET);
    afficherUnCompte(i);

    printf("\n" CYAN "Montant a retirer (<= %.0f DH) : " RESET, MAX_RETRAIT);
    if (scanf("%f", &m) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (m <= 0) {
        printf(RED "Erreur: Montant invalide.\n" RESET);
        pauseConsole();
        return;
    }
    if (m > MAX_RETRAIT) {
        printf(RED "Erreur: Refuse: retrait maximum = %.0f DH.\n" RESET, MAX_RETRAIT);
        pauseConsole();
        return;
    }
    if (m > comptes[i].solde) {
        printf(RED "Erreur: Refuse: solde insuffisant.\n" RESET);
        pauseConsole();
        return;
    }

    comptes[i].solde -= m;
    printf(GREEN "\nSucces: Retrait effectue avec succes!\n" RESET);
    printf(BOLD "  Nouveau solde: " RESET GREEN "%.2f DH\n" RESET, comptes[i].solde);
    pauseConsole();
}

void virementClient(void) {
    afficherTitre("EFFECTUER UN VIREMENT");

    // Afficher mes comptes d'abord
    printf(YELLOW "Vos comptes:\n" RESET);
    int compte_trouve = 0;
    for (int i = 0; i < nb_comptes; i++) {
        if (comptes[i].id_client == utilisateur_connecte_id) {
            printf("  - Compte %d: %.2f DH\n", comptes[i].id_compte, comptes[i].solde);
            compte_trouve = 1;
        }
    }

    if (!compte_trouve) {
        printf(RED "\nVous n'avez aucun compte.\n" RESET);
        pauseConsole();
        return;
    }

    int src, dst;
    float m;
    char confirmation;

    printf("\n" CYAN "Id_compte source (votre compte) : " RESET);
    if (scanf("%d", &src) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    printf(CYAN "Id_compte destination : " RESET);
    if (scanf("%d", &dst) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    int i = chercherCompteIndex(src);
    int j = chercherCompteIndex(dst);

    if (i == -1 || j == -1) {
        printf(RED "Erreur: Compte source ou destination introuvable.\n" RESET);
        pauseConsole();
        return;
    }

    // Vérifier que le compte source appartient au client connecté
    if (comptes[i].id_client != utilisateur_connecte_id) {
        printf(RED "Erreur: Le compte source ne vous appartient pas.\n" RESET);
        pauseConsole();
        return;
    }

    if (src == dst) {
        printf(RED "Erreur: Virement impossible: meme compte.\n" RESET);
        pauseConsole();
        return;
    }

    printf("\n" YELLOW "  Compte source:\n\n" RESET);
    afficherUnCompte(i);

    printf("\n" YELLOW "  Compte destination:\n\n" RESET);
    afficherUnCompte(j);

    printf("\n" CYAN "Montant a virer : " RESET);
    if (scanf("%f", &m) != 1) {
        clearInputBuffer();
        printf(RED "Erreur: Saisie invalide.\n" RESET);
        pauseConsole();
        return;
    }

    if (m <= 0) {
        printf(RED "Erreur: Montant invalide.\n" RESET);
        pauseConsole();
        return;
    }
    if (m > comptes[i].solde) {
        printf(RED "Erreur: Refuse: solde insuffisant.\n" RESET);
        pauseConsole();
        return;
    }

    //confirmation pour le virement
    printf("\n");
    afficherSeparateur();
    printf(BOLD YELLOW "  CONFIRMATION DU VIREMENT\n" RESET);
    afficherSeparateur();
    printf(YELLOW "  Montant      : " RESET BOLD "%.2f DH\n" RESET, m);
    printf(YELLOW "  De           : " RESET "Compte %d\n", src);
    printf(YELLOW "  Vers         : " RESET "Compte %d\n", dst);
    afficherSeparateur();
    printf(BOLD "  Confirmer le virement (O/N) : " RESET);
    scanf(" %c", &confirmation);

    if (confirmation == 'O' || confirmation == 'o') {
        comptes[i].solde -= m;
        comptes[j].solde += m;

        printf(GREEN "\nSucces: Virement effectue avec succes!\n\n" RESET);
        printf(BOLD "  Solde source      : " RESET GREEN "%.2f DH\n" RESET, comptes[i].solde);
        printf(BOLD "  Solde destination : " RESET GREEN "%.2f DH\n" RESET, comptes[j].solde);
    } else {
        printf(YELLOW "\nVirement annule.\n" RESET);
    }

    pauseConsole();
}

void modifierMesInfos(void) {
    afficherTitre("MODIFIER MES INFORMATIONS");

    int i = chercherClientIndex(utilisateur_connecte_id);
    if (i == -1) {
        printf(RED "Erreur: Probleme d'authentification.\n" RESET);
        pauseConsole();
        return;
    }

    int choix;
    do {
        clearScreen();
        printf("\n");
        afficherSeparateur();
        printf(BOLD CYAN "  MES INFORMATIONS\n" RESET);
        afficherSeparateur();
        printf("\n");

        printf(YELLOW "  Informations actuelles:\n" RESET);
        afficherUnClient(i);
        printf("\n");

        printf(CYAN "  1)" RESET " Modifier la profession\n");
        printf(CYAN "  2)" RESET " Modifier le numero de telephone\n");
        printf(CYAN "  3)" RESET " Modifier le mot de passe\n");
        printf(RED "  0)" RESET " Retour\n");
        printf("\n" CYAN "Choix : " RESET);

        if (scanf("%d", &choix) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            return;
        }

        switch (choix) {
            case 1:
                printf(CYAN "Nouvelle profession : " RESET);
                scanf("%49s", clients[i].profession);
                printf(GREEN "Succes: Modifie!\n" RESET);
                pauseConsole();
                break;
            case 2:
                printf(CYAN "Nouveau num_tel : " RESET);
                scanf("%14s", clients[i].num_tel);
                printf(GREEN "Succes: Modifie!\n" RESET);
                pauseConsole();
                break;
            case 3:
                printf(CYAN "Nouveau mot de passe : " RESET);
                scanf("%19s", clients[i].mot_de_passe);
                printf(GREEN "Succes: Mot de passe modifie!\n" RESET);
                pauseConsole();
                break;
            case 0:
                break;
            default:
                printf(RED "Erreur: Choix invalide.\n" RESET);
                pauseConsole();
        }
    } while (choix != 0);
}

void menuClient(void) {
    int choix;
    do {
        afficherTitre("ESPACE CLIENT");

        int idx = chercherClientIndex(utilisateur_connecte_id);
        printf(BOLD YELLOW "  Connecte en tant que: %s %s (ID: %d)\n" RESET,
               clients[idx].prenom, clients[idx].nom, utilisateur_connecte_id);
        printf("\n");

        printf(CYAN "  1)" RESET " Consulter mes comptes\n");
        printf(CYAN "  2)" RESET " Effectuer un retrait\n");
        printf(CYAN "  3)" RESET " Effectuer un virement\n");
        printf(CYAN "  4)" RESET " Modifier mes informations\n");
        printf(RED "  0)" RESET " Deconnexion\n");
        printf("\n" CYAN "Choix : " RESET);

        if (scanf("%d", &choix) != 1) {
            clearInputBuffer();
            printf(RED "Erreur: Saisie invalide.\n" RESET);
            pauseConsole();
            continue;
        }

        switch (choix) {
            case 1: consulterMesComptes(); break;
            case 2: retraitClient(); break;
            case 3: virementClient(); break;
            case 4: modifierMesInfos(); break;
            case 0:
                printf(GREEN "\nDeconnexion reussie.\n" RESET);
                pauseConsole();
                break;
            default:
                printf(RED "Erreur: Choix invalide.\n" RESET);
                pauseConsole();
        }
    } while (choix != 0);
}

//MENU PRINCIPAL EMPLOYÉ

void menuEmploye(void) {
    char choix;

    do {
        afficherTitre("SYSTEME DE GESTION BANCAIRE - EMPLOYE");

        int idx = chercherEmployeIndex(utilisateur_connecte_id);
        printf(BOLD YELLOW "  Connecte en tant que: %s %s (ID: %d)\n" RESET,
               employes[idx].prenom, employes[idx].nom, utilisateur_connecte_id);
        printf("\n");

        printf(GREEN "  A)" RESET " Gestion des clients\n");
        printf(GREEN "  B)" RESET " Gestion des comptes\n");
        printf(GREEN "  C)" RESET " Gestion des operations\n");
        printf(RED "  D)" RESET " Deconnexion\n");
        printf("\n" CYAN "Choix : " RESET);
        scanf(" %c", &choix);

        switch (choix) {
            case 'A':
            case 'a':
                menuClients();
                break;

            case 'B':
            case 'b':
                menuComptes();
                break;

            case 'C':
            case 'c':
                menuOperations();
                break;

            case 'D':
            case 'd':
                clearScreen();
                printf("\n");
                afficherSeparateur();
                printf(GREEN BOLD "  Deconnexion reussie!\n" RESET);
                printf(GREEN BOLD "  A bientot.\n" RESET);
                afficherSeparateur();
                printf("\n");
                pauseConsole();
                break;

            default:
                printf(RED "Erreur: Choix invalide.\n" RESET);
                pauseConsole();
        }

    } while (!(choix == 'D' || choix == 'd'));
}

//PROGRAMME PRINCIPAL

int main(void) {
    // Initialiser les données de test
    initialiserDonnees();

    int continuer = 1;

    while (continuer) {
        int resultat_connexion = menuConnexion();

        if (resultat_connexion == 3) {
            // Quitter le programme
            clearScreen();
            printf("\n");
            afficherSeparateur();
            printf(GREEN BOLD "  Merci d'avoir utilise notre systeme bancaire!\n" RESET);
            printf(GREEN BOLD "  Au revoir.\n" RESET);
            afficherSeparateur();
            printf("\n");
            continuer = 0;
        } else if (resultat_connexion == 1) {
            // Employé connecté
            menuEmploye();
            utilisateur_connecte_id = -1;
            est_employe = 0;
        } else if (resultat_connexion == 2) {
            // Client connecté
            menuClient();
            utilisateur_connecte_id = -1;
            est_employe = 0;
        }
    }
    return 0;
}
