#include "Case.h"

/* constructeur et destructeur */
Case::Case()
{
    /* rien a mettre */
}

Case::Case( int i_x, int i_y )
{
    /* initialise la position de la carte */
    v_set_poseX( i_x );
    v_set_poseY( i_y );
}

Case::~Case()
{
    /* rien a mettre */
}

/* mehtodes */
/* calcul la distance entre 2 cases */
int Case::i_calcul_dist( Case *second )
{
    /* Declaration des ressources */
    int i_x = second->i_get_poseX() - mi_pose[0];
    int i_y = second->i_get_poseY() - mi_pose[1];

    /* calcul de la distance au carre */
    return i_x*i_x + i_y*i_y ;
}

/* fonction heuristique */
void Case::v_heuristique( Case *first, Case *second )
{
    mi_heuris = i_calcul_dist( first ) + i_calcul_dist( second );
}

/* affiche la perturbation si elle existe */
void Case::v_print_meteo( BITMAP *&page )
{
    /* case indisponible = presence perturbation */
    if( false == b_get_dispo() )
    {
        m_alleg.myrectfill( page, mi_pose[0]*10, mi_pose[1]*10, (mi_pose[0]+1)*10, (mi_pose[1]+1)*10, 0, 0, 0 );
    }
}

/* getter */
int Case::i_get_poseX() const
{
    return mi_pose[0];
}

int Case::i_get_poseY() const
{
    return mi_pose[1];
}

std::string Case::s_get_meteo() const
{
    return ms_meteo;
}

int Case::i_get_explore() const
{
    return mi_explore;
}

int Case::i_get_distance() const
{
    return mi_distance;
}

int Case::i_get_predX() const
{
    return mi_pred[0];
}

int Case::i_get_predY() const
{
    return mi_pred[1];
}

int Case::i_get_heuris() const
{
    return mi_heuris;
}

bool Case::b_get_dispo() const
{
    return mb_dispo;
}

/* setter */
void Case::v_set_poseX( int i_x )
{
    mi_pose[0] = i_x;
}

void Case::v_set_poseY( int i_y )
{
    mi_pose[1] = i_y;
}

void Case::v_set_meteo( std::string s_meteo )
{
    ms_meteo = s_meteo;
}

void Case::v_set_explore( int i_done )
{
    mi_explore = i_done;
}

void Case::v_set_distance( int i_d )
{
    mi_distance = i_d;
}

void Case::v_set_pred( int i_x, int i_y )
{
    mi_pred[0] = i_x;
    mi_pred[1] = i_y;
}

void Case::v_set_dispo( bool b_dispo )
{
    mb_dispo = b_dispo;
}
