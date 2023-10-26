#include "Vol.h"

/* compare les distances de 2 cases */
struct compare2{
    bool operator()( Case* first, Case* second )
    {
        /* Declaration des ressources */
        bool b_valid = false;

        /* priorite pour la plus petite distance */
        b_valid = first->i_get_heuris() > second->i_get_heuris();

        /* retourne le resultat */
        return b_valid;
    }
};

//constructeur et destructeur
Vol::Vol()
{
    /* rien a mettre */
}

Vol::~Vol()
{
    /* rien a mettre */
}

/* vide les aeroports intermediaires */
void Vol::v_vide_inter()
{
    while( 0 == m_aer_int.empty() )
    {
        m_aer_int.pop_back();
    }
}

/* ajoute un aeroport intermediaire */
void Vol::v_ajout_int( Aeroport *aer )
{
    m_aer_int.push_back( aer );
}

/* on change l'heure */
void Vol::v_change_heure()
{
    m_heure.v_saisi();
}

/* affiche les caractéristiques de l'aeroport et sa localisation */
void Vol::v_display_all( BITMAP *&page, Souris &souris, int i_x, int i_y, bool b_droit, int i_current, int i_curseur )
{
    /* Declaration des ressources */
    int i_page = i_current - i_curseur;

    /* si on a demande d'afficher tous les aeroports */
    if( b_droit && ( 0 <= i_page ) && ( 2 >= i_page ) )
    {
        /* on affiche les caracteristiques de l'aeroport en bas de l'ecran */
        v_display( page, i_x+250*i_page, i_y );
    }

}

/* affiche les caractéristiques de l'aeroport */
void Vol::v_display( BITMAP *&page, int i_x, int i_y )
{
    /* Declaration des ressources */
    uint8_t ui8_i = 0;
    std::string s_txt = "";
    std::string s_txt2 = "";

    m_alleg.txtlangue( page, "Heure depart : ", i_x, i_y );
    m_heure.v_display( page, i_x+120, i_y );
    if(NULL != m_avion)
    {
        /* si l'avion ne fuite pas */
        if( false == m_avion->b_get_fuite() )
        {
            s_txt = "Reservoir operationnel !";   /* message de l'etat actuel du reservoir */
        }
        /* sinon l'avion fuite */
        else
        {
            s_txt = "Danger : fuite reservoir !";
        }

        /* si l'avion est en vie */
        if( true == m_avion->b_get_en_vie() )
        {
            s_txt2 = "Avion en vie !";   /* message de l'etat actuel du reservoir */
        }
        /* sinon l'avion a crashe... */
        else
        {
            s_txt2 = "Crash de l'avion...";
        }

        m_alleg.txtlangue( page, "Avion : " + m_avion->s_get_type(), i_x, i_y+15 );
        m_alleg.txtlangue( page, "Id" + m_alleg.s_int_to_str(m_avion->i_get_id()) + " ("+ m_alleg.s_int_to_str(m_avion->i_get_poseX()) + ", " + m_alleg.s_int_to_str(m_avion->i_get_poseY()) + ") Speed : " + m_alleg.s_int_to_str(m_avion->i_get_speed()) + "km/UT", i_x, i_y+30 );
        m_alleg.txtlangue( page, "Carburant : " + m_alleg.s_int_to_str(m_avion->i_get_carburant()), i_x, i_y+45 );
        m_alleg.txtlangue( page, "Altitude : " + m_alleg.s_int_to_str(m_avion->i_get_altitude()) + "km", i_x, i_y+60 );
        m_alleg.txtlangue( page, s_txt, i_x, i_y+75 );
        m_alleg.txtlangue( page, s_txt2, i_x, i_y+90 );
    }
    if(NULL != m_aer_dep)
        m_alleg.txtlangue( page, "Depart : " + m_aer_dep->s_get_nom()+ " ("+ m_alleg.s_int_to_str(m_aer_dep->i_get_poseX()*10) + ", " + m_alleg.s_int_to_str(m_aer_dep->i_get_poseY()*10) + ")", i_x, i_y+105 );

    for( ui8_i = 0; ui8_i < m_aer_int.size(); ui8_i++ )
    {
        if(NULL != m_aer_int[ui8_i])
            m_alleg.txtlangue( page, "Inter : " + m_aer_int[ui8_i]->s_get_nom()+ " ("+ m_alleg.s_int_to_str(m_aer_int[ui8_i]->i_get_poseX()*10) + ", " + m_alleg.s_int_to_str(m_aer_int[ui8_i]->i_get_poseY()*10) + ")", i_x, i_y+120+ui8_i*15 ); /* id aeroport et distance */
    }

    if(NULL != m_aer_arr)
        m_alleg.txtlangue( page, "Arrivee : " + m_aer_arr->s_get_nom()+ " ("+ m_alleg.s_int_to_str(m_aer_arr->i_get_poseX()*10) + ", " + m_alleg.s_int_to_str(m_aer_arr->i_get_poseY()*10) + ")", i_x, i_y+120+ui8_i*15 );
}

/* affiche le vol en console */
void Vol::v_console()
{
    /* Declaration des ressources */
    uint8_t ui8_i = 0;
    std::string s_txt = "";
    std::string s_txt2 = "";

    std::cout << std::endl << "Heure depart : " << m_heure.i_get_heure() << " : " << m_heure.i_get_min() << std::endl;

    if(NULL != m_avion)
    {
        /* si l'avion ne fuite pas */
        if( false == m_avion->b_get_fuite() )
        {
            s_txt = "Reservoir operationnel !";   /* message de l'etat actuel du reservoir */
        }
        /* sinon l'avion fuite */
        else
        {
            s_txt = "Danger : fuite reservoir !";
        }

        /* si l'avion est en vie */
        if( true == m_avion->b_get_en_vie() )
        {
            s_txt2 = "Avion en vie !";   /* message de l'etat actuel du reservoir */
        }
        /* sinon l'avion a crashe... */
        else
        {
            s_txt2 = "Crash de l'avion...";
        }

        std::cout << "Avion : " + m_avion->s_get_type() << std::endl;
        std::cout << "Id" + m_alleg.s_int_to_str(m_avion->i_get_id()) + " ("+ m_alleg.s_int_to_str(m_avion->i_get_poseX()) + ", " + m_alleg.s_int_to_str(m_avion->i_get_poseY()) + ") Speed : " + m_alleg.s_int_to_str(m_avion->i_get_speed()) + "km/UT" << std::endl;
        std::cout << "Carburant : " + m_alleg.s_int_to_str(m_avion->i_get_carburant()) << std::endl;
        std::cout << "Altitude : " + m_alleg.s_int_to_str(m_avion->i_get_altitude()) + "km" << std::endl;
        std::cout << s_txt << std::endl;
        std::cout << s_txt2 << std::endl;
    }

    if(NULL != m_aer_dep)
        std::cout << "Depart : " + m_aer_dep->s_get_nom()+ " ("+ m_alleg.s_int_to_str(m_aer_dep->i_get_poseX()*10) + ", " + m_alleg.s_int_to_str(m_aer_dep->i_get_poseY()*10) + ")" << std::endl;

    for( ui8_i = 0; ui8_i < m_aer_int.size(); ui8_i++ )
    {
        if(NULL != m_aer_int[ui8_i])
            std::cout << "Inter : " + m_aer_int[ui8_i]->s_get_nom()+ " ("+ m_alleg.s_int_to_str(m_aer_int[ui8_i]->i_get_poseX()*10) + ", " + m_alleg.s_int_to_str(m_aer_int[ui8_i]->i_get_poseY()*10) + ")" << std::endl;
    }

    std::cout << "Arrivee : " + m_aer_arr->s_get_nom()+ " ("+ m_alleg.s_int_to_str(m_aer_arr->i_get_poseX()*10) + ", " + m_alleg.s_int_to_str(m_aer_arr->i_get_poseY()*10) + ")" << std::endl;

    /* affiche le nouveau chemin s'il y en a 1 */
    v_print_new_way();
}

/* inverse l'ordre des aerports intermediaires */
void Vol::v_reverse()
{
    /* Declaration des ressources */
    Aeroport *inter=NULL;

    for( uint8_t ui8_i = 0; ui8_i < (uint8_t)((m_aer_int.size())/2); ui8_i++ )
    {
        inter = m_aer_int[ui8_i];
        m_aer_int[ui8_i] = m_aer_int[m_aer_int.size()-1-ui8_i];
        m_aer_int[m_aer_int.size()-1-ui8_i] = inter;
    }
}

/* toutes les conditions reunies pour partir ? */
void Vol::v_partir_enfin( Horloge const&montre )
{
    /* heure correcte ? */
    if(( m_heure.i_get_heure() == montre.i_get_heure() ) &&  ( m_heure.i_get_min() == montre.i_get_min() ))
    {
        mb_ok_heure = true;
    }

    /* heure correcte ? */
    if ( true == mb_ok_heure ) //&& ( true == m_aer_dep->b_vol_possible() ) )
    {
        /* on sauve l'avion qui va partir */
        //m_avion = m_aer_dep->get_avion();

        /* le vol peut commencer */
        mb_ok_vol = true;
    }
}

/* atterrissage possible ? */
bool Vol::b_atterrir( Aeroport *arrivee, bool &b_to_end, bool &b_boucle )
{
    /* Declaration des ressources */
    int i_rect1[4] = { arrivee->i_get_poseX()*10, arrivee->i_get_poseY()*10, 10, 10 };
    int i_rect2[4] = { m_avion->i_get_poseX()-3, m_avion->i_get_poseY()-3, 16, 16 };
    bool b_done = false;

    /* si l'avion entre dans la case concernee */
    if( b_collirect( i_rect1, i_rect2 ) )
    {
        //std::cout << "collsiion " << b_collirect( i_rect1, i_rect2 ) << std::endl;
        /* on verifie si l'aeroport peut acceuillir un nouvel avion */
        if( (int)arrivee->get_all_avion().size() < arrivee->i_get_nb_places() )
        {
            b_done = true;

            /* l'avion peut atterrir : attente */
            if( 0 == mi_wait )
            {
                /* on attend qu'une piste soit libre */
                if( arrivee->i_get_piste_pris() != arrivee->i_get_nb_pistes() )
                {
                    /* une piste de plus de prise */
                    arrivee->v_set_piste_pris( arrivee->i_get_nb_pistes()+1 );
                    mi_wait++;
                }
                else
                {
                    /* affichage sur console */
                    std::cout << m_avion->s_get_type() << " est en attente d'une piste libre." << std::endl;
                }
            }
            else if( mi_wait < arrivee->i_get_tps_decol() )
            {
                /* en train d'atterrir */
                mi_wait++;

                /* affichage sur console */
                std::cout << m_avion->s_get_type() << " est en train d'atterrir." << std::endl;
            }
            else if( mi_wait == arrivee->i_get_tps_decol() )
            {
                /* a fini d'atterrir */
                mi_wait++;
                /* piste libre */
                arrivee->v_set_piste_pris( arrivee->i_get_nb_pistes()-1 );

                /* affichage sur console */
                std::cout << m_avion->s_get_type() << " a fini d'atterrir !" << std::endl;
            }
            else if( mi_wait < arrivee->i_get_tps_acces()+arrivee->i_get_tps_decol() )
            {
                /* temps d'attente */
                mi_wait++;

                /* affichage sur console */
                std::cout << m_avion->s_get_type() << " est en train d'acceder a l'aeroport." << std::endl;
            }
            else if( mi_wait < arrivee->i_get_tps_charge()+arrivee->i_get_tps_acces()+arrivee->i_get_tps_decol() )
            {
                /* temps d'attente */
                mi_wait++;

                /* affichage sur console */
                std::cout << m_avion->s_get_type() << " est en train de se faire decharger." << std::endl;
            }
            else
            {
                /* fini */
                mi_wait = 0;

                /* changement d'altitude */
                m_avion->v_set_altitude( 0 );

                /* n'a plus de vitesse */
                m_avion->v_set_speed( 0 );

                /* affichage sur console */
                std::cout << m_avion->s_get_type() << " est dechargee !" << std::endl;

                /* l'avion arrive : il est en vie */
                m_avion->v_set_en_vie( true );

                /* on repositionne l'avion */
                m_avion->v_set_poseX(arrivee->i_get_poseX()*10);
                m_avion->v_set_poseY(arrivee->i_get_poseY()*10);

                /* si l'aeroport d'arrivee est l'aeroport final ou l'aeroport d'urgence */
                if(( m_aer_arr == arrivee ) || ( m_aer_urg == arrivee ))
                {
                    /* ce vol sera detruit */
                    b_to_end = true;

                    if ( m_aer_arr == arrivee )/* si l'avion est arrive a l'aeroport d'arrivee */
                    {
                        m_avion->v_set_fuite( false );/* son reservoir est directement repare */
                        /* l'aeroport recoit l'avion */
                        arrivee->v_ajout_avion( m_avion );
                    }
                }
                /* sinon c'est une escale */
                else
                {
                    mi_etape++;
                }
            }
        }
        /* sinon l'aeroport n'a plus de place */
        else
        {
            /* boucle d'attente : l'avion ne bouge plus a l'ecran */
            b_boucle = true;
        }
    }

    return b_done;
}

//détecte collision de 2 rectangles
bool Vol::b_collirect(int i_rect1[4], int i_rect2[4])
{
    //Déclaration des ressources
    bool b_colli = false;

    //on a collision si un des angles du rectangle 1 est dans le rectangle 2 ou inversement
    if((i_rect2[0] <= i_rect1[0]) && (i_rect2[0]+i_rect2[2] >= i_rect1[0]) && (i_rect2[1] <= i_rect1[1]) && (i_rect2[1]+i_rect2[3] >= i_rect1[1]))                                                  //angle haut gauche rect 1
        b_colli = true;
    else if((i_rect2[0] <= i_rect1[0]+i_rect1[2]) && (i_rect2[0]+i_rect2[2] >= i_rect1[0]+i_rect1[2]) && (i_rect2[1] <= i_rect1[1]) && (i_rect2[1]+i_rect2[3] >= i_rect1[1]))                       //angle haut droite rect 1
        b_colli = true;
    else if((i_rect2[0] <= i_rect1[0]) && (i_rect2[0]+i_rect2[2] >= i_rect1[0]) && (i_rect2[1] <= i_rect1[1]+i_rect1[2]) && (i_rect2[1]+i_rect2[3] >= i_rect1[1]+i_rect1[2]))                       //angle bas gauche rect 1
        b_colli = true;
    else if((i_rect2[0] <= i_rect1[0]+i_rect1[2]) && (i_rect2[0]+i_rect2[2] >= i_rect1[0]+i_rect1[2]) && (i_rect2[1] <= i_rect1[1]+i_rect1[2]) && (i_rect2[1]+i_rect2[3] >= i_rect1[1]+i_rect1[2])) //angle bas droite rect 1
        b_colli = true;
    else if((i_rect1[0] <= i_rect2[0]) && (i_rect1[0]+i_rect1[2] >= i_rect2[0]) && (i_rect1[1] <= i_rect2[1]) && (i_rect1[1]+i_rect1[3] >= i_rect2[1]))                                             //angle haut gauche rect 2
        b_colli = true;
    else if((i_rect1[0] <= i_rect2[0]+i_rect2[2]) && (i_rect1[0]+i_rect1[2] >= i_rect2[0]+i_rect2[2]) && (i_rect1[1] <= i_rect2[1]) && (i_rect1[1]+i_rect1[3] >= i_rect2[1]))                       //angle haut droite rect 2
        b_colli = true;
    else if((i_rect1[0] <= i_rect2[0]) && (i_rect1[0]+i_rect1[2] >= i_rect2[0]) && (i_rect1[1] <= i_rect2[1]+i_rect2[2]) && (i_rect1[1]+i_rect1[3] >= i_rect2[1]+i_rect2[2]))                       //angle bas gauche rect 2
        b_colli = true;
    else if((i_rect1[0] <= i_rect2[0]+i_rect2[2]) && (i_rect1[0]+i_rect1[2] >= i_rect2[0]+i_rect2[2]) && (i_rect1[1] <= i_rect2[1]+i_rect2[2]) && (i_rect1[1]+i_rect1[3] >= i_rect2[1]+i_rect2[2])) //angle bas droite rect 2
        b_colli = true;

    return b_colli; //on retourne le résultat
}

/* un vol a lieu ! */
bool Vol::b_vol_en_cours( std::vector <std::vector < Case* >> &a_cases, bool &b_deb_fuite, bool const&b_faire_fuite )
{
    /* Declaration des ressources */
    bool b_done = false;
    bool b_boucle = false;

    /* y a-t-il des aeroports intermediaires ? */
    if( 0 == m_aer_int.size() )
    {
        /* si le decollage est bien effectue et qu'on n'a pas atterri */
        if ( b_decollage(m_aer_dep) && !b_atterrir( m_aer_arr, b_done, b_boucle ) )
        {
            /* y a-t-il une escale d'urgence ? */
            if( NULL != m_aer_urg )
            {
                /* si on n'a pas atterri */
                if(!b_atterrir( m_aer_urg, b_done, b_boucle ))
                    v_motion_URG( m_aer_dep, m_aer_urg, b_done, a_cases, b_deb_fuite, b_boucle );   /* l'avion se deplace vers l'aeroport d'urgence */
            }
            else
            {
                v_motion_avion( m_aer_dep, m_aer_arr, b_done, a_cases, b_deb_fuite, b_faire_fuite, b_boucle );   /* l'avion se deplace normalement */
            }
        }
    }
    else
    {
        /* on observe l'etape actuelle */
        if( 0 == mi_etape )  /* debut ? */
        {
            /* si le decollage est bien effectue et qu'on n'a pas atterri */
            if ( b_decollage(m_aer_dep) && !b_atterrir( m_aer_int[0], b_done, b_boucle ) )
            {
                /* y a-t-il une escale d'urgence ? */
                if( NULL != m_aer_urg )
                {
                    /* si on n'a pas atterri */
                    if(!b_atterrir( m_aer_urg, b_done, b_boucle ))
                        v_motion_URG( m_aer_dep, m_aer_urg, b_done, a_cases, b_deb_fuite, b_boucle );   /* l'avion se deplace vers l'aeroport d'urgence */
                }
                else
                {
                    v_motion_avion( m_aer_dep, m_aer_int[0], b_done, a_cases, b_deb_fuite, b_faire_fuite, b_boucle );    /* l'avion se deplace normalement */
                }
            }
        }
        else if( (int)m_aer_int.size() == mi_etape )  /* fin ? */
        {
            /* si le decollage est bien effectue et qu'on n'a pas atterri */
            if ( b_decollage(m_aer_int[(int)m_aer_int.size() - 1]) && !b_atterrir( m_aer_arr, b_done, b_boucle ) )
            {
                /* y a-t-il une escale d'urgence ? */
                if( NULL != m_aer_urg )
                {
                    /* si on n'a pas atterri */
                    if(!b_atterrir( m_aer_urg, b_done, b_boucle ))
                        v_motion_URG( m_aer_int[(int)m_aer_int.size() - 1], m_aer_urg, b_done, a_cases, b_deb_fuite, b_boucle );   /* l'avion se deplace vers l'aeroport d'urgence */
                }
                else
                {
                    v_motion_avion( m_aer_int[(int)m_aer_int.size() - 1], m_aer_arr, b_done, a_cases, b_deb_fuite, b_faire_fuite, b_boucle );    /* l'avion se deplace normalement */
                }
            }
        }
        else    /* aeroport intermediaire */
        {
            /* si le decollage est bien effectue et qu'on n'a pas atterri */
            if ( b_decollage(m_aer_int[ mi_etape-1 ]) && !b_atterrir( m_aer_int[ mi_etape ], b_done, b_boucle ) )
            {
                /* y a-t-il une escale d'urgence ? */
                if( NULL != m_aer_urg )
                {
                    /* si on n'a pas atterri */
                    if(!b_atterrir( m_aer_urg, b_done, b_boucle ))
                        v_motion_URG( m_aer_int[ mi_etape-1 ], m_aer_urg, b_done, a_cases, b_deb_fuite, b_boucle );   /* l'avion se deplace vers l'aeroport d'urgence */
                }
                else
                {
                    v_motion_avion( m_aer_int[ mi_etape-1 ], m_aer_int[ mi_etape ], b_done, a_cases, b_deb_fuite, b_faire_fuite, b_boucle ); /* l'avion se deplace normalement */
                }
            }
        }
    }

    return b_done;
}

/* temps d'attente pour decoller */
bool Vol::b_decollage( Aeroport *current )
{
    /* Declaration des ressources */
    bool b_done = false;

    /* temps d'attente fini et avion en vie ? */
    if(( 0 == m_avion->i_get_altitude() ) && ( true == m_avion->b_get_en_vie() ))
    {
        /* temps d'attente pour charger l'avion */
        if( mi_wait < current->i_get_tps_charge() )
        {
            mi_wait++;

            /* affichage sur console */
            std::cout << "Chargement de " << m_avion->s_get_type() << std::endl;

            /* on met le carburant au max */
            m_avion->v_set_carburant( m_avion->i_get_capa() );
        }
        /* temps d'attente pour acceder a une piste */
        else if( mi_wait < current->i_get_tps_charge()+current->i_get_tps_acces() )
        {
            mi_wait++;

            /* affichage sur console */
            std::cout << m_avion->s_get_type() << " accede a une piste." << std::endl;
        }
        else if ( mi_wait == current->i_get_tps_charge()+current->i_get_tps_acces() )
        {
            /* on attend qu'une piste soit libre */
            if( current->i_get_piste_pris() != current->i_get_nb_pistes() )
            {
                /* une piste de plus de prise */
                current->v_set_piste_pris( current->i_get_nb_pistes()+1 );
                mi_wait++;

                /* affichage sur console */
                std::cout << m_avion->s_get_type() << " occupe une piste !" << std::endl;
            }
            else
            {
                /* affichage sur console */
                std::cout << m_avion->s_get_type() << " attend une piste libre." << std::endl;
            }
        }
        /* temps d'attente pour charger l'avion et le decollage */
        else if( mi_wait < current->i_get_tps_charge()+current->i_get_tps_acces()+current->i_get_tps_acces()+current->i_get_tps_decol() )
        {
            mi_wait++;

            /* affichage sur console */
            std::cout << m_avion->s_get_type() << " est en train de decoller." << std::endl;

            /* si l'avion vient de decoller */
            if( mi_wait == current->i_get_tps_charge()+current->i_get_tps_acces()+current->i_get_tps_acces()+current->i_get_tps_decol() )
            {
                /* la piste est liberee */
                current->v_set_piste_pris( current->i_get_nb_pistes()-1 );
                /* changement d'altitude */
                m_avion->v_set_altitude( m_avion->i_get_alt_to_do() );
                /* temps d'attente fini */
                mi_wait = 0;
                /* nouvelle vitesse */
                m_avion->v_set_speed( 200 );

                /* la fuite du reservoir est repare */
                m_avion->v_set_fuite( false );

                /* affichage sur console */
                std::cout << m_avion->s_get_type() << " est parti de l'aeroport !" << std::endl;
            }
        }
    }
    /* si l'avion n'a pas crash */
    else if( true == m_avion->b_get_en_vie() )
    {
        /* temps d'attente fini ! */
        b_done = true;
    }
    /* sinon l'avion a crash */
    else
    {
        v_write_rotate( 0, 0 );
    }

    return b_done;
}

/* l'avion fait une boucle */
void Vol::v_fait_boucle( Aeroport *arrivee )
{
    /* Declaration des ressources */
    int i_dif_x = m_avion->i_get_poseX() - arrivee->i_get_poseX()*10 + 5;
    int i_dif_y = m_avion->i_get_poseY() - arrivee->i_get_poseY()*10 + 5;
    int i_mil_x = m_avion->i_get_poseX()+5;
    int i_mil_y = m_avion->i_get_poseY()+5;
    int i_sens[2] = {0, 0};
    float f_angle = 0.0;
    int i_hbgd = 0;

    /* l'avion est-il sur le cote gauche, droite, haut ou bas de l'aeroport ? */
    if(( arrivee->i_get_poseX()*10 <= i_mil_x ) && ( (arrivee->i_get_poseX()*10+10) > i_mil_x ))
    {
        /* cote haut bas */
        i_hbgd = 1;
    }
    else if(( arrivee->i_get_poseY()*10 <= i_mil_y ) && ( (arrivee->i_get_poseY()*10+10) > i_mil_y ))
    {
        /* cote gauche droite */
        i_hbgd = -1;
    }

    /* on determine le sens de deplacement de l'avion */
    /* repere cartesien */
    /* x negatif et y negatif */
    if(( 0 > i_dif_x ) && ( 0 > i_dif_y ))
    {
        v_write_boucle( i_sens, 1, 0, f_angle, 45.0 );
    }
    /* x negatif et y positif */
    else if(( 0 > i_dif_x ) && ( 0 <= i_dif_y ))
    {
        v_write_boucle( i_sens, 0, -1, f_angle, 315.0 );
    }
    /* x positif et y negatif */
    else if(( 0 <= i_dif_x ) && ( 0 > i_dif_y ))
    {
        /* avion sur cote haut ? */
        if( 1 == i_hbgd )
        {
            v_write_boucle( i_sens, 1, 0, f_angle, 45.0 );
        }
        /* sinon sur cote */
        else
        {
            v_write_boucle( i_sens, 0, 1, f_angle, 135.0 );
        }
    }
    /* x positif et y positif */
    else
    {
        /* avion sur cote droite ? */
        if( -1 == i_hbgd )
        {
            v_write_boucle( i_sens, 0, 1, f_angle, 135.0 );
        }
        /* sinon sur cote bas ou coin */
        else
        {
            v_write_boucle( i_sens, -1, 0, f_angle, 225.0 );
        }
    }

    /* deplacement de l'avion */
    m_avion->v_set_poseX( m_avion->i_get_poseX() + 5*i_sens[0] );
    m_avion->v_set_poseY( m_avion->i_get_poseY() + 5*i_sens[1] );

    /* on ecrit l'angle */
    m_avion->v_set_angle( f_angle );

    /* nouvelle vitesse */
    m_avion->v_set_speed( 100 );

    /* affichage sur console */
    std::cout << m_avion->s_get_type() << " fait une boucle." << std::endl;
}

/* on ecrit trois valeurs pour la boucle d'attente */
void Vol::v_write_boucle( int i_sens[2], int i_x, int i_y, float &f_angle, float f_val )
{
    i_sens[0] = i_x;
    i_sens[1] = i_y;
    f_angle = f_val;
}

/* l'avion se dirige vers un en URGENCE */
void Vol::v_motion_URG( Aeroport *depart, Aeroport *arrivee, bool &b_done, std::vector <std::vector < Case* >> &a_cases, bool &b_deb_fuite, bool const&b_boucle )
{
    /* Declaration des ressources */
    int i_dif_x = 0;
    int i_dif_y = 0;

    /* si l'avion n'a pas atterri et ne fait pas de boucle il avance */
    if(( false == b_done ) && ( false == b_boucle ))
    {
            //std::cout << "dx " << arrivee->i_get_poseX()*10 << " " << depart->i_get_poseX()*10 << std::endl;
            //std::cout << "dy " << arrivee->i_get_poseY()*10 << " " << depart->i_get_poseY()*10 << std::endl;
            /* vol direct ! */
            i_dif_x = arrivee->i_get_poseX()*10 - m_avion->i_get_poseX();
            i_dif_y = arrivee->i_get_poseY()*10 - m_avion->i_get_poseY();

            /* ecrit l'angle de rotation de l'avion */
            v_write_rotate( i_dif_x, i_dif_y );

            /* ecrit le module de l 'avion */
            v_write_module( i_dif_x, i_dif_y, a_cases, arrivee, depart );

            /* l'avion fuite deja : il consomme 2x plus */

            /* l'avion consomme du carburant */
            /* L/100km => L/200km : x2 */
            m_avion->v_set_carburant( m_avion->i_get_carburant() - m_avion->i_get_consom()*2*2 );
    }
    /* sinon si l'avion fait une boucle */
    else if( true == b_boucle )
    {
        /* l'avion fait une boucle */
        v_fait_boucle( arrivee );

        /* l'avion consomme du carburant */
        /* L/100km => 80% de consommation */
        m_avion->v_set_carburant( m_avion->i_get_carburant() - m_avion->i_get_consom()*2*0.8 );
    }
}

/* l'avion se dirige vers un aeroprot */
void Vol::v_motion_avion( Aeroport *depart, Aeroport *arrivee, bool &b_done, std::vector <std::vector < Case* >> &a_cases, bool &b_deb_fuite, bool const&b_faire_fuite, bool const&b_boucle )
{
    /* Declaration des ressources */
    int i_dif_x = 0;
    int i_dif_y = 0;

    /* si l'avion n'a pas atterri et ne fait pas de boucle il avance */
    if(( false == b_done ) && ( false == b_boucle ))
    {
            /* vol direct ! */
            i_dif_x = arrivee->i_get_poseX()*10 - depart->i_get_poseX()*10;
            i_dif_y = arrivee->i_get_poseY()*10 - depart->i_get_poseY()*10;

            /* ecrit l'angle de rotation de l'avion */
            v_write_rotate( i_dif_x, i_dif_y );

            /* ecrit le module de l 'avion */
            v_write_module( i_dif_x, i_dif_y, a_cases, arrivee, depart );

            /* l'avion consomme du carburant */
            /* L/100km => L/200km : x2 */
            m_avion->v_set_carburant( m_avion->i_get_carburant() - m_avion->i_get_consom()*2 );

            /* si on a active l'option de faire fuiter les reservoirs des avions */
            if( true == b_faire_fuite )
            {
                /* l'avion va-t-il fuiter ? */
                b_deb_fuite = m_avion->b_rand_fuite();
            }
    }
    /* sinon si l'avion fait une boucle */
    else if( true == b_boucle )
    {
        /* l'avion fait une boucle */
        v_fait_boucle( arrivee );

        /* l'avion consomme du carburant */
        /* L/100km => 80% de consommation */
        m_avion->v_set_carburant( m_avion->i_get_carburant() - m_avion->i_get_consom()*0.8 );
    }
}

/** l'avion ne peut avancer que d'un bloc par UT
        1 bloc = 10 pixels
        2 equations : (1) x2 + y2 = R2, R = 10 pixels
                      (2) droite : y = ax, a cte

                      x2 + (ax)2 = R2
                      x = sqrt( R2/(1+a2) )

                      x = y/a
                      (y/a)2 + y2 = R2
                      y = sqrt( R2/(1+(1/a)2) ) = sqrt( a2*R2/(1+a2) )
**/
/* ecrit le module de l 'avion */
void Vol::v_write_module(int i_dx, int i_dy, std::vector <std::vector < Case* >> &a_cases, Aeroport *arrivee, Aeroport *depart)
{
    /* Declaration des ressources */
    float f_R = 10.0;
    int i_x = 0;
    int i_y = 0;
    float f_a = 0;

    /* division par 0 interdit ! */
    if( i_dx != 0 )
    {
        f_a = (float)i_dy/(float)i_dx;                      /* coef de la droite y=ax */
        i_x = (int)sqrt( f_R*f_R/(1.0+f_a*f_a) );           /* calcul la distance en x a parcourir */
        i_y = (int)sqrt( f_a*f_a*f_R*f_R/(1.0+f_a*f_a) );   /* calcul la distance en y a parcourir */

        /* eviter les petites valeurs */
        v_petites_val( i_x );
        if( 10 != i_x )
            v_petites_val( i_y );
    }
    else
    {
        i_y = 10;   /* deplacement uniquement en y */
    }

    /* sens des composantes */
    if( i_dx < 0 )
    {
        i_x *= -1;
    }

    if(( i_dy < 0 ) && ( i_y > 0 ) )
    {
        i_y *= -1;
    }

    /* correction de l'angle d'avance en fonction des trajectoires entre aeroports */
    if(( 1 == depart->i_get_id() ) && ( 3 == arrivee->i_get_id() ))
    {
        i_y+=1;
    }
    else if(( 3 == depart->i_get_id() ) && ( 1 == arrivee->i_get_id() ))
    {
        i_y-=1;
    }
    if(( 12 == depart->i_get_id() ) && ( 13 == arrivee->i_get_id() ))
    {
        i_x-=1;
    }
    else if(( 13 == depart->i_get_id() ) && ( 12 == arrivee->i_get_id() ))
    {
        i_x+=1;
    }

    /* fuite de reservoir ? */
    if( true == m_avion->b_get_fuite() )
    {
        /* vitesses divisées par 2 */
        i_x/=2;
        i_y/=2;
    }

    /* si l'avion est en train de crash il n'avance plus */
    if( false == m_avion->b_get_en_vie() )
    {
        /* il n'avance plus */
        i_x = 0;
        i_y = 0;

        /* on vide la liste d'aretes de l'avion */
        m_avion->v_vide_arete();
    }

    /* si l'avion est en vie il avance */
    if( true == m_avion->b_get_en_vie() )
    {
        /* chemin nouveau du a une perturbation ? */
        if(0 != ma_new_way.size())
        {
            /* nouveau chemin trouve : on l'emprunte */
            /* deplacement de l'avion */
            m_avion->v_set_poseX( ma_new_way[ma_new_way.size()-1][0]*10 );
            m_avion->v_set_poseY( ma_new_way[ma_new_way.size()-1][1]*10 );

            /* case passee : on ne la garde plus dans notre chemin */
            ma_new_way.pop_back();

            /* nouvelle vitesse */
            m_avion->v_set_speed( 200 );
        }
        /* on verifie que la prochaine case est disponible et qu'on n'a pas change de voix */
        else if(( true == a_cases[(m_avion->i_get_poseX()+i_x)/10][(m_avion->i_get_poseY()+i_y)/10]->b_get_dispo()) && ( false == mb_changer_voix ) )
        {
            /* deplacement de l'avion */
            m_avion->v_set_poseX( m_avion->i_get_poseX()+i_x );
            m_avion->v_set_poseY( m_avion->i_get_poseY()+i_y );
        }
        /* prochaine case non disponible */
        else
        {
            /* on cherche un nouveau chemin */
            if(( true == b_A_star( a_cases, arrivee )) )
            {
                /* nouveau chemin trouve : on l'emprunte */
                /* deplacement de l'avion */
                m_avion->v_set_poseX( ma_new_way[ma_new_way.size()-1][0]*10 );
                m_avion->v_set_poseY( ma_new_way[ma_new_way.size()-1][1]*10 );
               // std::cout << ma_new_way[ma_new_way.size()-1][0] << " " << ma_new_way[ma_new_way.size()-1][1] << std::endl;

                /* case passee : on ne la garde plus dans notre chemin */
                ma_new_way.pop_back();
            }
            else
            {
                /* pas de nouveau chemin trouve : on est oblige de passer par la perturbation */

                /* diminution de la vitesse par 2 */
                i_x/=2;
                i_y/=2;

                /* deplacement de l'avion */
                m_avion->v_set_poseX( m_avion->i_get_poseX()+i_x );
                m_avion->v_set_poseY( m_avion->i_get_poseY()+i_y );

                /* consommation divise par 2 et consommation de 80% */
                m_avion->v_set_carburant( m_avion->i_get_carburant() + (int)1.2*m_avion->i_get_consom() );

                /* nouvelle vitesse */
                m_avion->v_set_speed( 100 );
            }
        }

        /* fuite de reservoir ? */
        if( true == m_avion->b_get_fuite() )
        {
            /* vitesses divisées par 2 */
            i_x/=2;
            i_y/=2;

            /* consommation de 80% */
            m_avion->v_set_carburant( m_avion->i_get_carburant() + (int)0.2*m_avion->i_get_consom() );

            /* nouvelle vitesse */
            m_avion->v_set_speed( 100 );
        }
    }
}

/* agrandi les petites valeurs pour eviter que l'avion devie de sa trajectoire */
void Vol::v_petites_val( int &i_val )
{
    if(( 1 == i_val ) || ( -1 == i_val ))
    {
        i_val*=2;
    }
    else if( 0 == i_val )
    {
        i_val = 1;
    }
}

/* ecrit l'angle de rotation de l'avion */
void Vol::v_write_rotate(int i_dx, int i_dy)
{
    /* Declaration des ressources */
    float f_angle = 0;

    /* si l'avion est en train de crash il n'avance plus */
    if( false == m_avion->b_get_en_vie() )
    {
        /* l'avion fait des tours sur lui meme */
        f_angle = m_avion->f_get_angle()+90;
    }
    else
    {
        /*  calcul l'angle de rotation*/
        if(0!= i_dx) /* division par 0 impossible */
        {
            f_angle = 45+(float)atan((double)i_dy/(double)i_dx)*180.0/3.14;

            /* correction de l'angle di la difference en x est negative */
            if( 0 > i_dx )
            {
                f_angle += 180;
            }

        }
        else    /* angles fixes */
        {
            if(i_dx < 0)
                f_angle = 225.0 ;
            else
                f_angle = 45.0 ;
        }
    }

    /* on verifie que 'angle est dans le bon interval */
    v_test_angle( f_angle );

    /* on ecrit l'angle */
    m_avion->v_set_angle( f_angle );
}

/* angle dans le bon interval ? */
void Vol::v_test_angle( float &f_angle )
{
    if( f_angle < 0.0 )
    {
        f_angle += 360.0;
    }
    if( f_angle > 360.0 )
    {
        f_angle -= 360.0;
    }
   // std::cout << "angle = " << f_angle << std::endl;
}

/* initialise A* */
void Vol::v_init_A_star( std::vector <std::vector < Case* >> &a_cases, Aeroport *aer_fin )
{
    /* tous les sommets sont non decouverts */
    for( uint8_t ui8_i = 0; ui8_i < 88; ui8_i++ )       /* cases en x */
    {
        for( uint8_t ui8_j = 0; ui8_j < 55; ui8_j++ )   /* cases en y */
        {
            a_cases[ui8_i][ui8_j]->v_set_explore( 0 );   /* aucune case exploree */
            a_cases[ui8_i][ui8_j]->v_set_pred( -1, -1 );   /* aucun predecesseur */
            //a_cases[ui8_i][ui8_j]->v_set_distance( 0 );   /* aucune distance */
            /* distance par rapport au sommet initial */
            a_cases[ui8_i][ui8_j]->v_set_distance( a_cases[ui8_i][ui8_j]->i_calcul_dist( a_cases[m_avion->i_get_poseX()/10][m_avion->i_get_poseY()/10] ));
            /* calcul des heuristiques */
            a_cases[ui8_i][ui8_j]->v_heuristique( a_cases[m_avion->i_get_poseX()/10][m_avion->i_get_poseY()/10], a_cases[aer_fin->i_get_poseX()][aer_fin->i_get_poseY()] );
        }
    }

    /* creation d'un nouveau chemin : on vide l'ancien */
    while( 0 == ma_new_way.empty() )
    {
        ma_new_way.pop_back();
    }

    /* on signale qu'on change de voix */
    mb_changer_voix = true;
}

/* algo A* */
bool Vol::b_A_star( std::vector <std::vector < Case* >> &a_cases, Aeroport *aer_fin )
{
    /* Declaration des ressources */
    std::priority_queue <Case*, std::vector<Case*> , compare2 > a_queue;
    int a_autour[4][2] = { { 1, 0 } , { 0, 1 } , { -1, 0 } , { 0, -1 } };
    int i_x_begin = m_avion->i_get_poseX()/10;    /* On recupere la case occupee par l'avion */
    int i_y_begin = m_avion->i_get_poseY()/10;
    bool b_finish = false;
    Case *sommet = NULL;
    int i_x = 0;
    int i_y = 0;

    /* on ne peut faire tourner l'algo que si la case finale est accessible, ainsi que l'aeroport et l'avion */
    if(( true == a_cases[aer_fin->i_get_poseX()][aer_fin->i_get_poseY()]->b_get_dispo() ) && ( true == aer_fin->b_get_dispo() ) && ( true == a_cases[i_x_begin][i_y_begin]->b_get_dispo() ))
    {
        /* Initialisation */
        v_init_A_star( a_cases, aer_fin );

        /* On insere la case de depart dans la file de priorite */
        a_queue.push( a_cases[i_x_begin][i_y_begin] );

        /* tant qu'on n'a pas fini */
        while( false == b_finish )
        {
            do{
                /* on recupere le sommet prioritaire (plus petite distance de s0) */
                sommet = a_queue.top();
                /* on la retire de la file prioritaire */
                a_queue.pop();

            }while(( 2 == a_cases[sommet->i_get_poseX()][sommet->i_get_poseY()]->i_get_explore() ) || ( false == a_cases[sommet->i_get_poseX()][sommet->i_get_poseY()]->b_get_dispo() ));
            /* on cherche le premier sommet non explore et operationnel */

            /* sommet d'arrivee ? */
            if(( sommet->i_get_poseX() == aer_fin->i_get_poseX() ) && ( sommet->i_get_poseY() == aer_fin->i_get_poseY() ))
            {
                /* termine */
                b_finish = true;
            }
            else
            {
                /* le sommet est explore */
                a_cases[sommet->i_get_poseX()][sommet->i_get_poseY()]->v_set_explore( 2 );

                /* on observe les successeurs du sommet (cases environnantes haut bas gauche droite) */
                for( uint8_t ui8_i = 0; ui8_i < 4; ui8_i++ )
                {
                    /* on ecrit la position de la nouvelle case */
                    i_x = sommet->i_get_poseX()+a_autour[ui8_i][0];
                    i_y = sommet->i_get_poseY()+a_autour[ui8_i][1];

                    /* la case existe-t-elle bien ? */
                    if(( -1 != i_x ) && ( (int)a_cases.size() != i_x ) && ( -1 != i_y ) && ( (int)a_cases[0].size() != i_y ))
                    {
                        /* sommet accessible ? */
                        if( true == a_cases[i_x][i_y]->b_get_dispo() )
                        {
                            /* sommet non decouvert ? */
                            if( 0 == a_cases[i_x][i_y]->i_get_explore() )
                            {
                                /* on ecrit la nouvelle distance */
                                a_cases[i_x][i_y]->v_set_distance( sommet->i_get_distance() + 1 );
                                /* on note son predecesseur */
                                a_cases[i_x][i_y]->v_set_pred( sommet->i_get_poseX(), sommet->i_get_poseY() );
                                /* le sommet est decouvert */
                                a_cases[i_x][i_y]->v_set_explore( 1 );
                                /* insere le nouveau sommet dans la file */
                                a_queue.push( a_cases[i_x][i_y] );
                            }
                            else
                            {
                                /* si ce sommet avait deja ete decouvert mais pas explore */
                                if( 1 == a_cases[i_x][i_y]->i_get_explore() )
                                {
                                    /* plus petite distance trouvee ? */
                                    if( a_cases[i_x][i_y]->i_get_distance() > sommet->i_get_distance()+1 )
                                    {
                                        /* on change cette distance */
                                        a_cases[i_x][i_y]->v_set_distance( sommet->i_get_distance() + 1 );
                                        /* mise a jour predecesseur */
                                        a_cases[i_x][i_y]->v_set_pred( sommet->i_get_poseX(), sommet->i_get_poseY() );
                                        /* insere le nouveau sommet dans la file */
                                        a_queue.push( a_cases[i_x][i_y] );
                                    }
                                }
                                /* si ce sommet avait deja ete explore */
                                if( 2 == a_cases[i_x][i_y]->i_get_explore() )
                                {
                                    /* plus petite distance trouvee ? */
                                    if( a_cases[i_x][i_y]->i_get_distance() > sommet->i_get_distance()+1 )
                                    {
                                        /* on change cette distance */
                                        a_cases[i_x][i_y]->v_set_distance( sommet->i_get_distance() + 1 );
                                        /* mise a jour predecesseur */
                                        a_cases[i_x][i_y]->v_set_pred( sommet->i_get_poseX(), sommet->i_get_poseY() );
                                        /* insere le nouveau sommet dans la file */
                                        a_queue.push( a_cases[i_x][i_y] );
                                        /* il faudra peut etre reexplorer */
                                        a_cases[i_x][i_y]->v_set_explore( 1 );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /* on sauve le nouveau chemin */
        v_new_way( a_cases, aer_fin->i_get_poseX(), aer_fin->i_get_poseY() );
    }
    else
    {
        /* message erreur */
        //std::cout << "L'aeroport final n'est pas accessible a cause de la meteo" << std::endl;
    }

    return b_finish;
}

/* on sauve le nouveau chemin trouve avec A star */
void Vol::v_new_way( std::vector <std::vector < Case* >> &a_cases, int i_x, int i_y )
{
    /* Declaration des ressources */
    std::vector<int> place = { i_x, i_y };

    /* on ajoute cette case dans le tableau */
    ma_new_way.push_back(place);

    /* nouvelle case a ajouter ? */
    if(( -1 != a_cases[i_x][i_y]->i_get_predX() ) && ( -1 != a_cases[i_x][i_y]->i_get_predY() ))
    {
        /* on sauve la prochaine case */
        v_new_way( a_cases, a_cases[i_x][i_y]->i_get_predX(), a_cases[i_x][i_y]->i_get_predY() );
    }
    /* sinon on a fini */
    else
    {
        /* rien a mettre */
    }
}

/* affichage du nouveau plan de vol */
void Vol::v_print_new_way()
{
    /* s'il y a un changement de voix */
    if( true == mb_changer_voix )
    {
        /* on l'affiche */
        std::cout << "Nouveau plan de vol : " << std::endl;

        for( uint8_t ui8_i = 0; ui8_i < ma_new_way.size(); ui8_i++ )
        {
            std::cout << "[" << ma_new_way[(int)ma_new_way.size()-1-ui8_i][0]*10 << "," << ma_new_way[(int)ma_new_way.size()-1-ui8_i][1]*10 << "] ";
        }

        std::cout << std::endl;
    }
}

/* cree les aretes de l'avion */
void Vol::v_create_aretes()
{
    /* on s'assure que la liste d'aretes est vide */
    m_avion->v_vide_arete();

    /* y a-t-il des aeroports intermediaires ? */
    if( 0 == m_aer_int.size() )
    {
        /* arete dans les deux sens a prendre en compte pour eviter collision */
        v_ajout_arete( m_aer_dep, m_aer_arr );
        v_ajout_arete( m_aer_arr, m_aer_dep );
    }
    /* sinon il y a des aeroports intermediaires */
    else
    {
        /* entre le debut et la premiere intermediaire */
        v_ajout_arete( m_aer_dep, m_aer_int[0] );
        v_ajout_arete( m_aer_int[0], m_aer_dep );

        /* entre les intermediaires */
        for( uint8_t ui8_i = 0; ui8_i < m_aer_int.size()-1; ui8_i++ )
        {
            v_ajout_arete( m_aer_int[ui8_i], m_aer_int[ui8_i + 1] );
            v_ajout_arete( m_aer_int[ui8_i + 1], m_aer_int[ui8_i] );
        }

        /* entre la derniere intermediaire et la fin */
        v_ajout_arete( m_aer_int[m_aer_int.size()-1], m_aer_arr );
        v_ajout_arete( m_aer_arr, m_aer_int[m_aer_int.size()-1] );
    }
}

/* ajoute une arete pour l'avion */
void Vol::v_ajout_arete( Aeroport *dep, Aeroport *arr )
{
    /* Declaration des ressources */
    std::pair < int, int > arete;

    /* on garde seulement les id des aeroports */
    arete.first = dep->i_get_id();
    arete.second = arr->i_get_id();

    /* ajout dans la liste d'aretes de l'avion */
    m_avion->v_ajout_arete_av( arete );
}

/* accesseurs */
/* getter */
Aeroport *Vol::get_aer_dep() const
{
    return m_aer_dep;
}

Aeroport *Vol::get_aer_arr() const
{
    return m_aer_arr;
}

Aeroport *Vol::get_aer_urg() const
{
    return m_aer_urg;
}

Avion *Vol::get_avion() const
{
    return m_avion;
}

bool Vol::b_get_ok_vol() const
{
    return mb_ok_vol;
}

std::vector <Aeroport *> Vol::a_get_aer_int() const
{
    return m_aer_int;
}

/* setter */
void Vol::v_set_aer_dep( Aeroport *aer )
{
    m_aer_dep = aer;
}

void Vol::v_set_aer_arr( Aeroport *aer )
{
    m_aer_arr = aer;
}

void Vol::v_set_aer_urg( Aeroport *aer )
{
    m_aer_urg = aer;
}

void Vol::v_set_avion( Avion *avion )
{
    m_avion = avion;
}

void Vol::v_set_alt_to_do( int i_alt )
{
    m_avion->v_set_alt_to_do( i_alt );
}

void Vol::v_set_heure( int i_h, int i_m )
{
    /* si les minutes dépassent 59 */
    if( 59 < i_m )
    {
        /* 1 heure en plus */
        i_h++;
    }

    /* ecrit la nouvelle heure */
    m_heure.v_set_heure( i_h );
    m_heure.v_set_min( i_m );
}
