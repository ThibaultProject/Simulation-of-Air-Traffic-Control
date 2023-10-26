#ifndef VOL_H_INCLUDED
#define VOL_H_INCLUDED

#include "Aeroport.h"
#include "Myallegro.h"
#include "Horloge.h"
#include "Case.h"

/* classe d'un vol */
class Vol{
private:
    //attributs
    Myallegro m_alleg;                      /* les fonctions allegro */
    Aeroport *m_aer_dep = NULL;             /* aeroport de depart */
    Aeroport *m_aer_arr = NULL;             /* aeroport d'arrivee */
    Aeroport *m_aer_urg = NULL;             /* aeroport d'urgence */
    std::vector <Aeroport *> m_aer_int;     /* aeroport intermediaires */
    Avion *m_avion = NULL;                  /* avion qui s'occupe du vol */
    Horloge m_heure;                        /* heure de depart */
    bool mb_ok_heure = false;               /* horaire ok ou pas */
    bool mb_ok_vol = false;                 /* vol possible ou pas */
    int mi_etape = 0;                       /* etape de vol pour atteindre l'arrivee */
    int mi_wait = 0;                        /* temps d'attente */
    std::vector <std::vector <int>> ma_new_way;    /* nouveau chemin a emprunter */
    bool mb_changer_voix = false;                  /* doit-on changer de chemin ? */

public:
    //constructeur et destructeur
    Vol();
    ~Vol();

    /* vide les aeroports intermediaires */
    void v_vide_inter();
    /* ajout d'un aeroport intermediaire */
    void v_ajout_int( Aeroport *aer );
    /* on change l'heure */
    void v_change_heure();
    /* affiche les caractéristiques de l'vol */
    void v_display_all( BITMAP *&page, Souris &souris, int i_x, int i_y, bool b_droit, int i_current, int i_curseur );
    /* affiche le vol */
    void v_display( BITMAP *&page, int i_x, int i_y );
    /* affiche le vol en console */
    void v_console();
    /* inverse l'ordre des aerports intermediaires */
    void v_reverse();
    /* toutes les conditions reunies pour partir ? */
    void v_partir_enfin( Horloge const&montre );
    /* atterrissage possible ? */
    bool b_atterrir( Aeroport *arrivee, bool &b_to_end, bool &b_boucle );
    /* temps d'attente pour decoller */
    bool b_decollage( Aeroport *current );
    //détecte collision de 2 rectangles
    bool b_collirect(int i_rect1[4], int i_rect2[4]);
    /* un vol a lieu ! */
    bool b_vol_en_cours( std::vector <std::vector < Case* >> &a_cases, bool &b_deb_fuite, bool const&b_faire_fuite );
    /* l'avion fait une boucle */
    void v_fait_boucle( Aeroport *arrivee );
    /* on ecrit trois valeurs pour la boucle d'attente */
    void v_write_boucle( int i_sens[2], int i_x, int i_y, float &f_angle, float f_val );
    /* l'avion se dirige vers un en URGENCE */
    void v_motion_URG( Aeroport *depart, Aeroport *arrivee, bool &b_done, std::vector <std::vector < Case* >> &a_cases, bool &b_deb_fuite, bool const&b_boucle );
    /* l'avion se dirige vers un aeroprot */
    void v_motion_avion( Aeroport *depart, Aeroport *arrivee, bool &b_done, std::vector <std::vector < Case* >> &a_cases, bool &b_deb_fuite, bool const&b_faire_fuite, bool const&b_boucle );
    /* ecrit le module de l 'avion */
    void v_write_module(int i_dx, int i_dy, std::vector <std::vector < Case* >> &a_cases, Aeroport *arrivee, Aeroport *depart);
    /* ecrit l'angle de rotation de l'avion */
    void v_write_rotate(int i_dx, int i_dy);
    /* angle dans le bon interval ? */
    void v_test_angle( float &f_angle );
    /* agrandi les petites valeurs pour eviter que l'avion devie de sa trajectoire */
    void v_petites_val( int &i_val );
    /* initialise A* */
    void v_init_A_star( std::vector <std::vector < Case* >> &a_cases, Aeroport *aer_fin );
    /* algo A* */
    bool b_A_star( std::vector <std::vector < Case* >> &a_cases, Aeroport *aer_fin );
    /* on sauve le nouveau chemin trouve avec A star */
    void v_new_way( std::vector <std::vector < Case* >> &a_cases, int i_x, int i_y );
    /* affichage du nouveau plan de vol */
    void v_print_new_way();
    /* cree les aretes */
    void v_create_aretes();
    /* ajoute une arete */
    void v_ajout_arete( Aeroport *dep, Aeroport *arr );

    /* accesseurs */
    /* getter */
    Aeroport *get_aer_dep() const;
    Aeroport *get_aer_arr() const;
    Aeroport *get_aer_urg() const;
    Avion *get_avion() const;
    bool b_get_ok_vol() const;
    std::vector <Aeroport *> a_get_aer_int() const;

    /* setter */
    void v_set_aer_dep( Aeroport *aer );
    void v_set_aer_arr( Aeroport *aer );
    void v_set_aer_urg( Aeroport *aer );
    void v_set_avion( Avion *avion );
    void v_set_alt_to_do( int i_alt );
    void v_set_heure( int i_h, int i_m );
};

#endif // VOL_H_INCLUDED
