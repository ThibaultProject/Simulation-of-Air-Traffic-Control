#ifndef SIMULATEUR_H_INCLUDED
#define SIMULATEUR_H_INCLUDED

#include "Aeroport.h"
#include "Avion.h"
#include "Horloge.h"
#include "Myallegro.h"
#include "Souris.h"
#include "Vol.h"
#include "Case.h"

/* classe du simulateur */
class Simulateur
{
/* attributs prives */
private:
    Myallegro m_alleg;                          /* les fonctions allegro */
    std::vector < Aeroport *> ma_aeroports;     /* les aeroports de la simulation */
    std::vector < Avion *> ma_avions;           /* les avions de la simulation */
    std::vector < Vol *> ma_vols;               /* les vols de la simulation */
    Souris m_souris;                            /* la souris du simulateur */
    BITMAP *m_page = NULL;                      /* page a afficher */
    BITMAP *m_carte = NULL;                     /* carte du monde */
    SAMPLE *m_music = NULL;                     /* musique de fond */
    int mi_command = 0;                         /* commande */
    int mi_curseur = 0;                         /* le curseur du simulateur (page actuelle) */
    Horloge m_horloge;                          /* horloge de la simulation */
    Horloge m_UT;
    int m_UT_max = 1000;                        /* nombre d'UT max */
    int mi_PCC[2] = { 0, 0 };                   /* le depart et l'arrivee du PCC */
    std::vector <std::vector < Case *>> ma_cases;/* cases de la grille */
    bool mb_fuite_res = false;                   /* fuite des reservoir des avions ? */
    bool mb_ferme_front = false;                 /* fermeture des aeroports ? */
    bool mb_vol_rand = false;                   /* creer des vols aleatoirement */
    bool mb_ford = false;                       /* activer l'algo de ford fulkerson ou non */
    int mi_flot_max = 0;                        /* flux max */

/* interface */
public:
    /* constructeur et destructeur */
    Simulateur();
    ~Simulateur();

    /* methodes */

    /* initialise tout le simulateur */
    void v_init_all();
    /* initialise allegro */
    void v_init_allegro();
    /* initialise le son */
    void v_init_sound();
    /* charge toutes les images */
    void v_init_all_img();
    /* initialise tous les sons */
    void v_init_all_sound();
    /* initialise tous les aeroports (distances par rapport aux autres) */
    void v_init_aers();
    /* initialise les cases */
    void v_init_cases();
    /* detruit tous les elemens du simulateur */
    void v_detruire();
    /* execute les fonctionnalites du simulateur */
    void v_run();
    /* affichage ON (true) ou OFF (false) */
    std::string s_ON_OFF( bool const&b_choix );
    /* menu console */
    void v_menu_console();
    /* affiche tous les avions sur console */
    void v_console_avions();
    /* affiche tous les aeroports sur console */
    void v_console_aeroports();
    /* affiche tous les vols sur console */
    void v_console_vols();
    /* on choisit si on veut faire fuiter les reservoirs des avions */
    void v_choix_fuite_front( bool &b_choix, std::string s_txt );
    /* mise en place des vols aleatoire */
    void v_vols_alea();
    /* on choisit la meteo sur un rectangle choisi */
    void v_choix_meteo();
    /* affiche toutes les perturbation meteo */
    void v_print_meteo();
    /* affiche la barre de commandes */
    void v_commandes();
    /* affiche tous les aeroports */
    void v_print_aeroports( bool &b_left, bool &b_right );
    /* affiche tous les avions */
    void v_print_avions( bool &b_left, bool &b_right );
    /* affiche tous les vols */
    void v_print_vols( bool &b_left, bool &b_right );
    /* trace la grille de la carte */
    void v_grille();
    /* sauvegarde les caractéristiques du simulateur */
    //void v_save();
    /* sauve les caractéristiques de tous les avions */
    //void v_save_avions();
    /* sauve les caractéristiques de tous les aeroports */
    //void v_save_aeroports();
    /* charge les caractéristiques du simulateur */
    void v_load();
    /* charge les caractéristiques de tous les avions */
    void v_load_avions();
    /* charge les caractéristiques de tous les aeroports */
    void v_load_aeroports();
    /* rafraichit l'ecran */
    void v_refresh();
    /* saisies des sommets de depart et d'arrivee */
    void v_choose_way( bool const&b_rand, int &i_begin, int &i_end, std::vector <std::vector <int>> const&a_composantes );
    /* choix des sommets de départ et d'arrivee et recherche du chemin le plus court (dijkstra) */
    void v_chemin_plus_court( bool const&b_rand, bool b_auto = false, int i_b = 0, int i_d = 0, Vol *urgence = NULL );
    /* algo de dijkstra pour le PCC */
    void v_dijkstra( const int &i_begin, const int &i_end, Vol *urgence = NULL );
    /* initialisation pour algo de dijkstra */
    void v_init_dijkstra( const int &i_begin, const int &i_end );
    /* affiche le resultat du PCC */
    void v_print_PCC( const int &i_begin, const int &i_end, Vol *v );
    /* affiche le resultat du PCC par recurrence */
    void v_rec_PCC(uint8_t ui8_rang, int &i_poids, Vol *v, const int &i_begin, const int &i_end, bool b_cdt);
    /* cherche le poids pour un arc donne : ui8_id1 = depart, ui8_id2 = arrivee */
    int i_find_pds_arc( uint8_t ui8_id1, uint8_t ui8_id2 );
    /* observe si des avions sont dans des aeroports */
    void v_check_aer_av();
    /* des vols peuvent-ils enfin etre effectues ? */
    void v_vol_dispo();
    /* cherche l'aeroport le plus proche de l'avion qui est disponible */
    void v_find_urgence( Vol *vol );
    /* Trace le PCC d'un aeroport aux autres */
    void v_trace_PCC();
    /* Trace le PCC d'un aeroport aux autres */
    void v_rec_trace_PCC(uint8_t ui8_rang, int i_r, int i_g, int i_b);
    /* Trace une arete entre deux aeroports */
    void v_trace_arete( Aeroport *first, Aeroport *second, int i_r, int i_g, int i_b, int i_flot_act = -1, int i_flot_max = -1, int i_poids = 0 );
    /* recherche du poids entre deux aeroports souhaites */
    int i_cherche_pds( Aeroport *first, Aeroport *second, int i_poids );
    /* trace toutes les aretes */
    void v_all_aretes();
    /* cherche les altitudes des avions pour eviter une collision */
    void v_anti_collision();
    /* creation du graphe, avec les avions comme sommet, les intersections entre avions comme arete */
    void v_create_graphe();
    /* on fixe une altitude pour chaque avion */
    void v_fixer_altitudes( int const&i_color_max );
    /* algo de Welsh et Powell */
    void attribuerWelshPowell(int& nbFrequences);
    /* cherche les composantes connexes */
    void v_cherche_composantes(std::vector <std::vector <int>> &a_all, bool const&b_auto);
    /* tous les sommets trouves pour composantes connexes ? */
    bool b_all_found(std::vector <bool> const &a_done);
    /* reinitialise la couleur et le predecesseur de tous les sommets du graphe */
    void v_reinit_colpred();
    /* initialisation (DFS) */
    void v_init_DFS(Aeroport* sommet_zero);
    /* affiche le resultat du BFS ou DFS */
    void v_print_BFS_DFS(std::string s_type);
    /* affiche le resultat du BFS ou DFS par recurrence */
    void v_rec_BFS_DFS(uint8_t ui8_rang);
    /* parcours en profondeur (DFS) */
    void v_parcours_DFS(Aeroport* sommet);
    /* parcours en largeur (BFS) */
    void v_parcours_BFS(Aeroport* sommet_zero);
    /* affiche les composantes connexes */
    void v_print_composantes(std::vector <std::vector <int>> const&a_all);
    /* on cherche dans quelle composante connexe un aeroport fait parti */
    int i_cherche_CC( std::vector <std::vector <int>> const&a_all, int const&i_id );
    /* choix du type de l'avion */
    void v_choix_type( Aeroport *depart );
    /* on verifie qu'un vol peut etre saisie */
    bool b_vol_possible( std::vector <std::vector <int>> const&a_composantes );
    /* pour kruskal, nous creons la liste des aretes entre aeroports sans doublon */
    void v_crearete_kruskal( std::vector < std::vector <int>> &a_aretes );
    /* algo de kruskal */
    void v_kruskal( std::vector < std::vector <int>> &a_aretes );
    /* attribue a chaque arete des aeroports leur existence ou non */
    void v_reac_arete_kruskal( std::vector < std::vector <int>> const&a_aretes );
    /* realise toutes les fonctionnalites de kruskal */
    void v_run_kruskal();
    /* remet toutes les aretes de kruskal disponibles */
    void v_reinit_kruskal();
    /* renitialise tous les flots ! */
    void v_reinit_flot();
    /* BFS pour ford */
    bool b_BFS_ford( Aeroport* sommet_zero, Aeroport *arrivee );
    /* recherche de l'augmentation possible du flot sur un chemin */
    void v_flot_aug( Aeroport *current, int &i_min );
    /* on augmente les flots des aretes concernees ! */
    void v_flot_plus_plus( Aeroport *current, int const&i_min );
    /* ford fulkerson ! */
    void v_ford_fulkerson( Aeroport* sommet_zero, Aeroport *arrivee );
    /* calcul le flot max ! */
    void v_calcul_flot_max( Aeroport *depart );
    /* affiche le flot max */
    void v_print_flot_max();
};

#endif // SIMULATEUR_H_INCLUDED
