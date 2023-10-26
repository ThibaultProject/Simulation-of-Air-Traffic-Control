#ifndef AEROPORT_H_INCLUDED
#define AEROPORT_H_INCLUDED

#include "Myallegro.h"
#include "Souris.h"
#include "Avion.h"

/* classe d'un aeroport */
class Aeroport
{
/* attributs prives */
private:
    Myallegro m_alleg;                              /* les fonctions allegro */
    std::string ms_nom = "";                        /* nom de l'aeroport */
    int mi_pose[2] = {0, 0};                        /* position de l'aeroport */
    int mi_nb_pistes = 0;                           /* nombre de pistes */
    int mi_nb_places = 0;                           /* nombre de places */
    int mi_tps_charge = 0;                          /* attente pour charger et decharger avion */
    int mi_tps_acces = 0;                           /* temps d'acces aux pistes */
    int mi_tps_colli = 0;                           /* delai d'anticollision */
    int mi_tps_decol = 0;                           /* temps de decollage et d'atterrissage */
    int mi_tps_boucle = 0;                          /* temps d'attente d'un avion pour pouvoir atterrir */
    std::vector < std::pair<int, int> > ma_aers;    /* distance par rapport aux autres aeroports */
    std::vector < int > ma_flot_max;
    std::vector < int > ma_flot_act;
    std::vector < bool > mab_lien_dispo;            /* disponibilite entre connexions avec aeroports */
    uint8_t mui8_marquage = 0;                      /* marquage du sommet */
    int mi_distance = 0;                            /* distance parcourue */
    int mi_pred = -1;                               /* predecesseur */
    int mi_id = 0;                                  /* id de l'aeroport */
    std::queue < Avion *> ma_avions;                /* avions disponibles a l'aeroport */
    int mi_piste_pris = 0;                          /* nombre de pistes prises */
    bool mb_dispo = true;                            /* aeroport disponible ou non */
    std::string ms_coloration = "blanc";
    int mi_pred2 = -1;
    int mi_delai = 0;                               /* delai pour reouvrir ou refermer un aeroport */
    uint8_t mui8_cc = 0;

/* interface */
public:
    /* constructeur et destructeur */
    Aeroport();
    Aeroport(std::ifstream &file, int i_id);
    ~Aeroport();

    /* methodes */

    /* sauvegarde les caractéristiques de l'aeroport */
    //void v_save( std::ofstream &file );
    /* charge les caractéristiques de l'aeroport */
    void v_load( std::ifstream &file );
    /* affiche les caractéristiques de l'aeroport et sa localisation */
    void v_display_all( BITMAP *&page, Souris &souris, int i_x, int i_y, bool b_droit, int i_current, int i_curseur, std::vector < Aeroport *> const&all_aers );
    /* affiche les caractéristiques de l'aeroport */
    void v_display( BITMAP *&page, int i_x, int i_y, std::vector < Aeroport *> const&all_aers );
    /* ajoute un nouvel avion */
    void v_ajout_avion( Avion *avion );
    /* verifie si un avion peut partir */
    bool b_vol_possible();
    /* affiche sur console un aeroport */
    void v_console( std::vector < Aeroport *> const&all_aers );
    /* affiche la file d'attente des avions dans l'aeroport */
    void v_print_avions();
    /* calcul la distance entre un aeroport et un avion */
    int i_dist_av_aer( Avion *avion );
    /* aeroport ouvre ou ferme ? */
    void v_evenement();
    /* retourne tous les types d'avions existants dans l'aeroport */
    void v_avion_types( std::vector <bool> &a_tab );
    /* met en tete de file l'avion au type concerne */
    void v_tete_file_type( int i_choix );
    /* recherche du rang du chemin vers l'aeroport suivant */
    int i_get_rang_aers( int i_next );

    /* getter */
    std::string s_get_nom() const;  /* getter du nom */
    int i_get_poseX() const;        /* getter sur la position en x */
    int i_get_poseY() const;        /* getter sur la position en y */
    int i_get_nb_pistes() const;    /* getter sur le nombre de pistes */
    int i_get_nb_places() const;    /* getter sur le nombre de places */
    int i_get_tps_charge() const;   /* getter sur le temps de charge */
    int i_get_tps_acces() const;    /* getter sur le temps d'acces aux pistes */
    int i_get_tps_colli() const;    /* getter sur le delai d'anticollision */
    int i_get_tps_decol() const;    /* getter sur le temps de decollage */
    int i_get_tps_boucle() const;   /* getter sur le temps de boucle */
    int i_get_id() const;     /* getter sur l'id du sommet */
    uint8_t ui8_get_marquage() const;     /* getter sur le marquage du sommet */
    uint8_t ui8_get_cc() const;
    int i_get_distance() const;     /* getter sur la distance du sommet par rapport au sommet initial */
    int i_get_pred() const;     /* getter sur le predecesseur du sommet  */
    int i_get_pred2() const;     /* getter sur le predecesseur du sommet  */
    int i_get_piste_pris() const;     /* getter sur le nombre de pistes pris  */
    bool b_get_dispo() const;
    std::vector < std::pair<int, int> > get_aers() const;
    /* getter sur le premier avion */
    Avion *get_avion();
    std::queue < Avion *> get_all_avion() const;
    std::string s_get_coloration() const;
    bool b_get_lien_dispo( uint8_t ui8_rang ) const;
    int i_get_flot_max( uint8_t ui8_rang ) const;
    int i_get_flot_actuel( uint8_t ui8_rang ) const;

    /* setter */
    void v_set_nom( std::string s_nom );        /* setter du nom */
    void v_set_poseX( int i_poseX );            /* setter sur la position en x */
    void v_set_poseY( int i_poseY );            /* setter sur la position en y */
    void v_set_nb_pistes( int i_nb_pistes );    /* setter sur le nombre de pistes */
    void v_set_nb_places( int i_nb_places );    /* setter sur le nombre de places */
    void v_set_tps_charge( int i_tps_charge );  /* setter sur le temps de charge */
    void v_set_tps_acces( int i_tps_acces );    /* setter sur le temps d'acces aux pistes */
    void v_set_tps_colli( int i_tps_colli );    /* setter sur le delai d'anticollision */
    void v_set_tps_decol( int i_tps_decol );    /* setter sur le temps de decollage */
    void v_set_tps_boucle( int i_tps_boucle );  /* setter sur le temps de boucle */
    /* setter sur la distance par rapport a un autre aeroport */
    void v_set_distances( std::vector < Aeroport *> const&all_aers );
    void v_set_marquage( uint8_t ui8_marquage );/* setter sur le marquage du sommet */
    void v_set_cc( uint8_t ui8_cc );
    void v_set_distance( int i_distance );/* setter sur la distance du sommet par rapport a l'initial */
    void v_set_pred( int i_pred );/* setter sur le predecesseur du sommet */
    void v_set_pred2( int i_pred );/* setter sur le predecesseur du sommet */
    void v_set_piste_pris( int i_value );
    void v_set_dispo( bool b_dispo );
    void v_set_coloration( std::string s_value );
    void v_set_lien_dispo( uint8_t ui8_rang, bool b_value );
    void v_set_flot_actuel( uint8_t ui8_rang, int i_value );
};

#endif // AEROPORT_H_INCLUDED
