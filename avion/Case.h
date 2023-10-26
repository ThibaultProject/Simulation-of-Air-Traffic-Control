#ifndef CASE_H_INCLUDED
#define CASE_H_INCLUDED

#include "Myallegro.h"

/* Classe d'une case */
class Case{
/* attributs prives */
private:
    Myallegro m_alleg;
    int mi_pose[2] = {0, 0};    /* position de la case */
    std::string ms_meteo = "";  /* meteo */
    int mi_explore = 0;         /* case decouvert */
    int mi_distance = 0;        /* distance  */
    bool mb_dispo = true;      /* case disponible ? */
    int mi_pred[2] = {-1, -1};           /* predecesseur */
    int mi_heuris = 0;

/* interface */
public:
    /* constructeur et destructeur */
    Case();
    Case( int i_x, int i_y );
    ~Case();

    /* mehtodes */
    /* calcul la distance entre 2 cases */
    int i_calcul_dist( Case* second );
    /* fonction heuristique */
    void v_heuristique( Case *first, Case *second );
    /* affiche la perturbation si elle existe */
    void v_print_meteo( BITMAP *&page );

    /* getter */
    int i_get_poseX() const;
    int i_get_poseY() const;
    std::string s_get_meteo() const;
    int i_get_explore() const;
    int i_get_distance() const;
    int i_get_predX() const;
    int i_get_predY() const;
    int i_get_heuris() const;
    bool b_get_dispo() const;

    /* setter */
    void v_set_poseX( int i_x );
    void v_set_poseY( int i_y );
    void v_set_meteo( std::string s_meteo );
    void v_set_explore( int i_done );
    void v_set_distance( int i_d );
    void v_set_pred( int i_x, int i_y );
    void v_set_dispo( bool b_dispo );
};

#endif // CASE_H_INCLUDED
