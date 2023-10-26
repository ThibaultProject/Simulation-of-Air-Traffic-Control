#include "Aeroport.h"

/* constructeur et destructeur */
Aeroport::Aeroport()
{
    /* rien a mettre */
}

Aeroport::Aeroport(std::ifstream &file, int i_id)
{
    /* charge l'aeroport */
    v_load( file );

    mi_id = i_id;
}

Aeroport::~Aeroport()
{
    /* rien a mettre */
}

/* methodes */

/* sauvegarde les caractéristiques de l'aeroport */
//void Aeroport::v_save( std::ofstream &file )
//{
//    file << ms_nom << std::endl;        /* nom de l'aeroport */
//    file << mb_dispo << std::endl;      /* disponibilite de l'aeroport */
//    file << mi_pose[0] << std::endl;    /* position de l'aeroport en x */
//    file << mi_pose[1] << std::endl;    /* position de l'aeroport en y */
//    file << mi_nb_pistes << std::endl;  /* nombre de pistes */
//    file << mi_nb_places << std::endl;  /* nombre de places */
//    file << mi_tps_charge << std::endl; /* attente pour charger et decharger avion */
//    file << mi_tps_acces << std::endl;  /* temps d'acces aux pistes */
//    file << mi_tps_colli << std::endl;  /* delai d'anticollision */
//    file << mi_tps_decol << std::endl;  /* temps de decollage et d'atterrissage */
//    file << mi_tps_boucle << std::endl; /* temps d'attente d'un avion pour pouvoir atterrir */
//    file << ma_aers.size() << std::endl; /* nombre d'aeroports autres asservis */

//    for( uint8_t ui8_i = 0; ui8_i < ma_aers.size(); ui8_i++ )
//    {
//        file << ma_aers[ui8_i].first << std::endl; /* id aeroport et distance */
//    }
//}

/* charge les caractéristiques de l'aeroport */
void Aeroport::v_load( std::ifstream &file )
{
    /* Declaration des ressources */
    int i_taille = 0;
    std::pair <int, int> mypair;
    int i_flot = 0;

    file >> ms_nom;                     /* charge le nom */
    file >> mb_dispo;                   /* charge la disponibilite */
    file >> mi_pose[0];                 /* charge la position en x */
    file >> mi_pose[1];                 /* charge la position en y */
    file >> mi_nb_pistes;               /* charge le nombre de pistes */
    file >> mi_nb_places;               /* charge le nombre de places */
    file >> mi_tps_charge;              /* charge le temps de charge */
    file >> mi_tps_acces;               /* charge le temps d'acces */
    file >> mi_tps_colli;               /* charge le temps d'anticollision */
    file >> mi_tps_decol;               /* charge le temps de decollage */
    file >> mi_tps_boucle;              /* charge le temps de boucle */
    file >> i_taille;                   /* charge le nombre d'aeroports environnant */

    for( int i_i = 0; i_i < i_taille; i_i++ )
    {
        file >> mypair.first; /* id aeroport et distance */
        ma_aers.push_back( mypair );    /* ajout dans le tableau */
        ma_aers[i_i].second = 0;
        mab_lien_dispo.push_back( true );   /* une connexion disponible */

        file >> i_flot; /* flot maximal */
        ma_flot_max.push_back( i_flot );    /* ajoute le flot max */
        ma_flot_act.push_back( 0 );    /* ajoute le flot actuel */
    }
}

/* affiche les caractéristiques de l'aeroport et sa localisation */
void Aeroport::v_display_all( BITMAP *&page, Souris &souris, int i_x, int i_y, bool b_droit, int i_current, int i_curseur, std::vector < Aeroport *> const&all_aers )
{
    /* Declaration des ressources */
    int i_page = i_current - i_curseur;

    /* rectangle rouge de localisation */
    m_alleg.myrectfill( page, mi_pose[0]*10, mi_pose[1]*10, (mi_pose[0]+1)*10, (mi_pose[1]+1)*10, 255, 0, 0 );
    souris.b_verifclick( 0, mi_pose[0]*10, mi_pose[1]*10, (mi_pose[0]+1)*10, (mi_pose[1]+1)*10 );     /* bruitage si on survole le rectangle */

    /* si on a demande d'afficher tous les aeroports et qu'on est dans le bon interval */
    if( b_droit && ( 0 <= i_page ) && ( 2 >= i_page ) )
    {
        /* on affiche les caracteristiques de l'aeroport en bas de l'ecran */
        v_display( page, i_x+250*i_page, i_y, all_aers );
    }

    /* si la souris est en collision avec le rectangle rouge de l'aeroport */
    if( m_alleg.collimouse(mi_pose[0]*10, mi_pose[1]*10, (mi_pose[0]+1)*10, (mi_pose[1]+1)*10) )
    {
        /* on affiche les caracteristiques de l'aeroport sous le rectangle rouge */
        v_display( page, mi_pose[0]*10, (mi_pose[1]+1)*10, all_aers );
    }

    /* affichage de la file d'attente des avions */
    for( uint8_t ui8_i = 0; ui8_i < ma_avions.size(); ui8_i++ )
    {
        /* par defaut, on affiche un avion sur la case : on n'en superpose pas d'autres */
        if( ui8_i != 0 )
        {
            /* affichage d'un avion */
            m_alleg.transparence( page, ma_avions.front()->get_img(), mi_pose[0]*10-3, mi_pose[1]*10+ui8_i*10-3 );
        }
    }
}

/* affiche les caractéristiques de l'aeroport */
void Aeroport::v_display( BITMAP *&page, int i_x, int i_y, std::vector < Aeroport *> const&all_aers )
{
    /* Declaration des ressources */
    std::string s_txt = "";

    /* si l'avion est disponible */
    if (mb_dispo)
    {
        s_txt += "oui"; /* on affiche sa disponibilite */
    }
    /* sinon il n'est pas disponible */
    else
    {
        s_txt = "non";
    }

    m_alleg.txtlangue( page, "Nom : " + ms_nom + " (" + m_alleg.s_int_to_str(mi_id) + ")", i_x, i_y );
    m_alleg.txtlangue( page, "Disponibilite : "  + s_txt , i_x, i_y+15 );
    m_alleg.txtlangue( page, "Position : " + m_alleg.s_int_to_str(mi_pose[0]*10) + ", " + m_alleg.s_int_to_str(mi_pose[1]*10) , i_x, i_y+30 );
    m_alleg.txtlangue( page, "Nombre de pistes : " + m_alleg.s_int_to_str(mi_nb_pistes), i_x, i_y+45 );
    m_alleg.txtlangue( page, "Nombre de places : " + m_alleg.s_int_to_str(get_all_avion().size()) + "/" + m_alleg.s_int_to_str(mi_nb_places), i_x, i_y+60 );
    m_alleg.txtlangue( page, "Attente pour charger : " + m_alleg.s_int_to_str(mi_tps_charge) + "UT", i_x, i_y+75 );
    m_alleg.txtlangue( page, "Temps d'acces pistes : " + m_alleg.s_int_to_str(mi_tps_acces) + "UT", i_x, i_y+90 );
    m_alleg.txtlangue( page, "Delai d'anticollision : " + m_alleg.s_int_to_str(mi_tps_colli) + "UT", i_x, i_y+105 );
    m_alleg.txtlangue( page, "Temps de decollage : " + m_alleg.s_int_to_str(mi_tps_decol) + "UT", i_x, i_y+120 );
    m_alleg.txtlangue( page, "Attente pour atterrir : " + m_alleg.s_int_to_str(mi_tps_boucle) + "UT", i_x, i_y+135 );
    m_alleg.txtlangue( page, "Aeroports environnants : ", i_x, i_y+150 );

    for( uint8_t ui8_i = 0; ui8_i < ma_aers.size(); ui8_i++ )
    {
        /* aeroport disponible ? */
        if( true == all_aers[ma_aers[ui8_i].first]->b_get_dispo() )
        {
            m_alleg.txtlangue( page,  all_aers[ma_aers[ui8_i].first]->s_get_nom()+" "+ m_alleg.s_int_to_str(ma_aers[ui8_i].second), i_x, i_y+165+ui8_i*15 ); /* id aeroport et distance */
        }
    }
}

/* affiche sur console un aeroport */
void Aeroport::v_console( std::vector < Aeroport *> const&all_aers )
{
    /* Declaration des ressources */
    std::string s_txt = "";

    /* si l'avion est disponible */
    if (mb_dispo)
    {
        s_txt += "oui"; /* on affiche sa disponibilite */
    }
    /* sinon il n'est pas disponible */
    else
    {
        s_txt = "non";
    }

    std::cout << "Nom : " + ms_nom + " (" + m_alleg.s_int_to_str(mi_id) + ")" << std::endl;
    std::cout << "Disponibilite : "  + s_txt << std::endl;
    std::cout << "Position : " + m_alleg.s_int_to_str(mi_pose[0]*10) + ", " + m_alleg.s_int_to_str(mi_pose[1]*10) << std::endl;
    std::cout << "Nombre de pistes : " + m_alleg.s_int_to_str(mi_nb_pistes) << std::endl;
    std::cout << "Nombre de places : " + m_alleg.s_int_to_str(get_all_avion().size()) + "/" + m_alleg.s_int_to_str(mi_nb_places) << std::endl;
    std::cout << "Attente pour charger : " + m_alleg.s_int_to_str(mi_tps_charge) + "UT" << std::endl;
    std::cout << "Temps d'acces pistes : " + m_alleg.s_int_to_str(mi_tps_acces) + "UT" << std::endl;
    std::cout << "Delai d'anticollision : " + m_alleg.s_int_to_str(mi_tps_colli) + "UT" << std::endl;
    std::cout << "Temps de decollage : " + m_alleg.s_int_to_str(mi_tps_decol) + "UT" << std::endl;
    std::cout << "Attente pour atterrir : " + m_alleg.s_int_to_str(mi_tps_boucle) + "UT" << std::endl;
    std::cout << "Aeroports environnants : " << std::endl;

    for( uint8_t ui8_i = 0; ui8_i < ma_aers.size(); ui8_i++ )
    {
        /* aeroport disponible ? */
        if( true == all_aers[ma_aers[ui8_i].first]->b_get_dispo() )
        {
            /* on l'affiche */
            std::cout << all_aers[ma_aers[ui8_i].first]->s_get_nom()+" "+ m_alleg.s_int_to_str(ma_aers[ui8_i].second) << std::endl;
        }
    }

    /* affiche la file d'attente des avions dans l'aeroport */
    v_print_avions();

    std::cout << std::endl;
}

/* affiche la file d'attente des avions dans l'aeroport */
void Aeroport::v_print_avions()
{
    /* Declaration des ressources */
    std::queue < Avion * > bis;

    std::cout << std::endl << "File d'attente des avions dans l'aeroport :" << std::endl;

    /* si la file d'attente est vide */
    if( 1 == ma_avions.empty() )
    {
        /* message associe */
        std::cout << "File vide..." << std::endl;
    }

    /* affiche la file d'attente des avions */
    while( 0 == ma_avions.empty() )
    {
        /* on affiche l'avion en tete */
        ma_avions.front()->v_console();

        /* on ajoute l'avion en tete dans une file intermediaire */
        bis.push( ma_avions.front() );

        /* on elimine l'avion de la file pour passer au prochain */
        ma_avions.pop();
    }

    /* on restaure la file d'attente */
    while( 0 == bis.empty() )
    {
        /* on ajoute l'avion en tete dans une file intermediaire */
        ma_avions.push( bis.front() );

        /* on elimine l'avion de la file pour passer au prochain */
        bis.pop();
    }
}

/* verifie si un avion peut partir */
bool Aeroport::b_vol_possible()
{
    /* file non vide ? */
    if( 0 == ma_avions.empty() )
    {
        return true;    /* un avion peut bien partir */
    }
    else
    {
        return false;   /* aucun avion dispo */
    }
}

/* ajoute un nouvel avion */
void Aeroport::v_ajout_avion( Avion *avion )
{
    ma_avions.push( avion );
}

/* calcul la distance entre un aeroport et un avion */
int Aeroport::i_dist_av_aer( Avion *avion )
{
    /* Declaration des ressources */
    int i_x = avion->i_get_poseX()/10 - mi_pose[0];
    int i_y = avion->i_get_poseY()/10 - mi_pose[1];

    /* calcul de la distance au carre */
    return i_x*i_x + i_y*i_y ;
}

/* aeroport ouvre ou ferme ? */
void Aeroport::v_evenement()
{
    /* Declaration des ressources */
    int i_rand = rand()%50;

    mi_delai +=1;

    if( 2 < mi_delai )
    {
        /* reinit le delai */
        mi_delai = 0;

        /* si l'aeroport est disponible */
        if( true == b_get_dispo())
        {
            /* 2 chance sur 50 pour fermer */
            if( 2 > i_rand )
            {
                v_set_dispo( false );
            }
        }
        /* sinon l'aeroport est ferme */
        else
        {
            /* 4 chance sur 50 pour ouvrir */
            if( 4 > i_rand )
            {
                v_set_dispo( true );
            }
        }
    }
}

/* retourne tous les types d'avions existants dans l'aeroport */
void Aeroport::v_avion_types( std::vector <bool> &a_tab )
{
    /* Declaration des ressources */
    std::queue < Avion * > bis;
    char c = ' ';

    /* affiche la file d'attente des avions */
    while( 0 == ma_avions.empty() )
    {
        /* type de l'avion en tete */
        c = ma_avions.front()->s_get_type()[0];

        /* on met à true la case du tableau correspondant */
        /* type court courrier */
        if( 'C' == c )
        {
            a_tab[0] = true;
        }
        /* type moyen courrier */
        else if( 'M' == c )
        {
            a_tab[1] = true;
        }
        /* type long courrier */
        else if( 'L' == c )
        {
            a_tab[2] = true;
        }
        /* type inconnu */
        else
        {
            /* message erreur */
            std::cout << "Le type de l'avion " << ma_avions.front()->s_get_type() << " est inconnu..." << std::endl;
        }

        /* on ajoute l'avion en tete dans une file intermediaire */
        bis.push( ma_avions.front() );

        /* on elimine l'avion de la file pour passer au prochain */
        ma_avions.pop();
    }

    /* on restaure la file d'attente */
    while( 0 == bis.empty() )
    {
        /* on ajoute l'avion en tete dans une file intermediaire */
        ma_avions.push( bis.front() );

        /* on elimine l'avion de la file pour passer au prochain */
        bis.pop();
    }
}

/* met en tete de file l'avion au type concerne */
void Aeroport::v_tete_file_type( int i_choix )
{
    /* Declaration des ressources */
    std::vector < char > a_type = { 'C', 'M', 'L' };
    std::queue < Avion * > bis;
    bool b_cdt = false;

    /* si la file d'attente est vide */
    if( 1 == ma_avions.empty() )
    {
        /* message associe */
        std::cout << "File vide..." << std::endl;
    }

    /* met en tete de file l'avion au type concerne */
    while( false == b_cdt )
    {
        /* type concerne ? */
        if( a_type[ i_choix ] == ma_avions.front()->s_get_type()[0] )
        {
            /* c'est bon ! */
            b_cdt = true;
        }
        /* ce n'est pas le type concerne */
        else
        {
            /* on ajoute l'avion en tete dans une file intermediaire */
            bis.push( ma_avions.front() );

            /* on elimine l'avion de la file pour passer au prochain */
            ma_avions.pop();
        }
    }

    /* on restaure la file d'attente */
    while( 0 == bis.empty() )
    {
        /* on ajoute l'avion en tete dans une file intermediaire */
        ma_avions.push( bis.front() );

        /* on elimine l'avion de la file pour passer au prochain */
        bis.pop();
    }
}

/* recherche du rang du chemin vers l'aeroport suivant */
int Aeroport::i_get_rang_aers( int i_next )
{
    /* Declaration des ressources */
    int i_rang = -1;

    /* on parcourt toutes ses aretes (connexions avec autres aeroports) */
    for( uint8_t ui8_j = 0; (( ui8_j < get_aers().size() ) && ( -1 == i_rang )); ui8_j++ )
    {
        /* meme numero ? */
        if( get_aers()[ui8_j].first == i_next )
        {
            /* on sauve le rang */
            i_rang = ui8_j;
        }
    }

    /* rang non trouve ? */
    if( -1 == i_rang )
    {
        i_rang = 0;
        /* message erreur */
        std::cout << "Lien avec l'aeroport " << i_next << " inexistant..." << std::endl;
    }

    /* retourne le resultat ! */
    return i_rang;
}

/* getter */

/* getter du nom */
std::string Aeroport::s_get_nom() const
{
    return ms_nom;
}

/* getter sur la position en x */
int Aeroport::i_get_poseX() const
{
    return mi_pose[0];
}

/* getter sur la position en y */
int Aeroport::i_get_poseY() const
{
    return mi_pose[1];
}

/* getter sur le nombre de pistes */
int Aeroport::i_get_nb_pistes() const
{
    return mi_nb_pistes;
}

/* getter sur le nombre de places */
int Aeroport::i_get_nb_places() const
{
    return mi_nb_places;
}

/* getter sur le temps de charge */
int Aeroport::i_get_tps_charge() const
{
    return mi_tps_charge;
}

/* getter sur le temps d'acces aux pistes */
int Aeroport::i_get_tps_acces() const
{
    return mi_tps_acces;
}

/* getter sur le delai d'anticollision */
int Aeroport::i_get_tps_colli() const
{
    return mi_tps_colli;
}

/* getter sur le temps de decollage */
int Aeroport::i_get_tps_decol() const
{
    return mi_tps_decol;
}

/* getter sur le temps de boucle */
int Aeroport::i_get_tps_boucle() const
{
    return mi_tps_boucle;
}

/* getter sur l'id du sommet */
int Aeroport::i_get_id() const
{
    return mi_id;
}

/* getter sur le marquage du sommet */
uint8_t Aeroport::ui8_get_marquage() const
{
    return mui8_marquage;
}

uint8_t Aeroport::ui8_get_cc() const
{
    return mui8_cc;
}

/* getter sur la distance du sommet par rapport au sommet initial */
int Aeroport::i_get_distance() const
{
    return mi_distance;
}

/* getter sur la distance du sommet par rapport au sommet initial */
int Aeroport::i_get_pred() const
{
    return mi_pred;
}

int Aeroport::i_get_pred2() const
{
    return mi_pred2;
}

/* getter sur le nombre de pistes pris  */
int Aeroport::i_get_piste_pris() const
{
    return mi_piste_pris;
}

bool Aeroport::b_get_dispo() const
{
    return mb_dispo;
}

std::vector < std::pair<int, int> > Aeroport::get_aers() const
{
    return ma_aers;
}

/* getter sur le premier avion */
Avion *Aeroport::get_avion()
{
    /* Declaration des ressources */
    Avion *av=NULL;

    /* on recupere le premier avion */
    av = ma_avions.front();

    /* on elimine celui qui vient de partir */
    ma_avions.pop();

    /* on retourne l'avion en question */
    return av;
}

std::queue < Avion *> Aeroport::get_all_avion() const
{
    return ma_avions;
}

std::string Aeroport::s_get_coloration() const
{
    return ms_coloration;
}

bool Aeroport::b_get_lien_dispo( uint8_t ui8_rang ) const
{
    /* bon intervalle ? */
    if(( 0 <= ui8_rang ) && ( mab_lien_dispo.size() > ui8_rang ))
    {
        return mab_lien_dispo[ ui8_rang ];
    }
    else
    {
        /* message erreur */
        std::cout << "Erreur de lecture : case " << (int)ui8_rang << " hors du tableau..." << std::endl;
        return false;
    }
}

int Aeroport::i_get_flot_max( uint8_t ui8_rang ) const
{
    /* bon intervalle ? */
    if(( 0 <= ui8_rang ) && ( ma_flot_max.size() > ui8_rang ))
    {
        return ma_flot_max[ ui8_rang ];
    }
    else
    {
        /* message erreur */
        std::cout << "Erreur de lecture : case " << (int)ui8_rang << " hors du tableau..." << std::endl;
        return false;
    }
}

int Aeroport::i_get_flot_actuel( uint8_t ui8_rang ) const
{
    /* bon intervalle ? */
    if(( 0 <= ui8_rang ) && ( ma_flot_act.size() > ui8_rang ))
    {
        return ma_flot_act[ ui8_rang ];
    }
    else
    {
        /* message erreur */
        std::cout << "Erreur de lecture : case " << (int)ui8_rang << " hors du tableau..." << std::endl;
        return false;
    }
}

/* setter */

/* setter du nom */
void Aeroport::v_set_nom( std::string s_nom )
{
    ms_nom = s_nom;
}

/* setter sur la position en x */
void Aeroport::v_set_poseX( int i_poseX )
{
    mi_pose[0] = i_poseX;
}

/* setter sur la position en y */
void Aeroport::v_set_poseY( int i_poseY )
{
    mi_pose[1] = i_poseY;
}

/* setter sur le nombre de pistes */
void Aeroport::v_set_nb_pistes( int i_nb_pistes )
{
    mi_nb_pistes = i_nb_pistes;
}

/* setter sur le nombre de places */
void Aeroport::v_set_nb_places( int i_nb_places )
{
    mi_nb_places = i_nb_places;
}

/* setter sur le temps de charge */
void Aeroport::v_set_tps_charge( int i_tps_charge )
{
    mi_tps_charge = i_tps_charge;
}

/* setter sur le temps d'acces aux pistes */
void Aeroport::v_set_tps_acces( int i_tps_acces )
{
    mi_tps_acces = i_tps_acces;
}

/* setter sur le delai d'anticollision */
void Aeroport::v_set_tps_colli( int i_tps_colli )
{
    mi_tps_colli = i_tps_colli;
}

/* setter sur le temps de decollage */
void Aeroport::v_set_tps_decol( int i_tps_decol )
{
    mi_tps_decol = i_tps_decol;
}

/* setter sur le temps de boucle */
void Aeroport::v_set_tps_boucle( int i_tps_boucle )
{
    mi_tps_boucle = i_tps_boucle;
}

/* setter sur la distance par rapport a un autre aeroport */
void Aeroport::v_set_distances( std::vector < Aeroport *> const&all_aers )
{
    /* Declaration des ressources */
    double i_x = 0;
    double i_y = 0;
    double f_distance = 0;

    for( uint8_t ui8_j = 0; ui8_j < ma_aers.size(); ui8_j++ )
    {
        i_x = mi_pose[0]*10 - all_aers[ma_aers[ui8_j].first]->i_get_poseX()*10;   /* distance en x */
        i_y = mi_pose[1]*10 - all_aers[ma_aers[ui8_j].first]->i_get_poseY()*10;   /* distance en y */
        f_distance = i_x*i_x + i_y*i_y;                                     /* distance au carre */
        f_distance = sqrtf(f_distance);                                     /* distance finale en pixel */
        ma_aers[ui8_j].second = (int)f_distance*20;                            /* on sauve cette distance en km */
    }
}

/* setter sur le marquage du sommet */
void Aeroport::v_set_marquage( uint8_t ui8_marquage )
{
    mui8_marquage = ui8_marquage;
}

void Aeroport::v_set_cc( uint8_t ui8_cc )
{
    mui8_cc = ui8_cc;
}

/* setter sur la distance du sommet par rapport a l'initial */
void Aeroport::v_set_distance( int i_distance )
{
    mi_distance = i_distance;
}

/* setter sur le predecesseur du sommet */
void Aeroport::v_set_pred( int i_pred )
{
    mi_pred = i_pred;
}

void Aeroport::v_set_pred2( int i_pred )
{
    mi_pred2 = i_pred;
}

/* setter sur le nombre de pistes prises */
void Aeroport::v_set_piste_pris( int i_value )
{
    mi_piste_pris = i_value;
}

void Aeroport::v_set_dispo( bool b_dispo )
{
    mb_dispo = b_dispo;
}

void Aeroport::v_set_coloration( std::string s_value )
{
    ms_coloration = s_value;
}

void Aeroport::v_set_lien_dispo( uint8_t ui8_rang, bool b_value )
{
    /* bon intervalle ? */
    if(( 0 <= ui8_rang ) && ( mab_lien_dispo.size() > ui8_rang ))
    {
        mab_lien_dispo[ ui8_rang ] = b_value;
    }
    else
    {
        /* message erreur */
        std::cout << "Erreur d'ecriture : case " << (int)ui8_rang << " hors du tableau..." << std::endl;
    }
}

void Aeroport::v_set_flot_actuel( uint8_t ui8_rang, int i_value )
{
    /* bon intervalle ? */
    if(( 0 <= ui8_rang ) && ( ma_flot_act.size() > ui8_rang ))
    {
        ma_flot_act[ ui8_rang ] = i_value;
    }
    else
    {
        /* message erreur */
        std::cout << "Erreur d'ecriture : case " << (int)ui8_rang << " hors du tableau..." << std::endl;
    }
}
