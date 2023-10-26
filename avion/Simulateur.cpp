#include "Simulateur.h"

/* compare les distances de 2 arcs */
struct comparison{
    bool operator()( Aeroport *p_first, Aeroport *p_second)
    {
        /* Declaration des ressources */
        bool b_valid = false;

        /* aucune distance infinie ? */
        if (( -1 != p_first->i_get_distance() ) && ( -1 != p_second->i_get_distance() ))
        {
            /* priorite pour la plus petite distance */
            b_valid = p_first->i_get_distance() > p_second->i_get_distance();
        }
        /* au moins une distance infinie */
        else if ( -2 != p_first->i_get_distance()+p_second->i_get_distance() )
        {
            /* -1 est forcement plus petit qu'une autre distance -> changement de condition */
            b_valid = p_first->i_get_distance() <= p_second->i_get_distance();
        }
        /* 2 distances infinies */
        else
        {
            /* on compare leur id : ordre croissant */
            b_valid = p_first->i_get_id() > p_second->i_get_id();
        }

        /* retourne le resultat */
        return b_valid;
    }
};

/* compare les distances de 2 arcs */
struct compar_kruskal{
    bool operator()( std::vector <int> first, std::vector <int> second)
    {
        /* priorite pour le plus petit poids */
        return first[2] > second[2];
    }
};

/* constructeur et destructeur */
Simulateur::Simulateur()
{
    /* charge les donnees du simulateur */
    v_init_all();//initialise le simulateur
    v_load();
    v_init_all_img(); /* initialise toutes les images */
    v_init_all_sound(); /* initialise tous les sons */
    m_souris.v_init_souris();//initialise la souris
    v_init_aers();      /* initialise les aeroports */
    /* observe si des avions sont dans des aeroports */
    v_check_aer_av();
    /* initialise les cases */
    v_init_cases();
}

Simulateur::~Simulateur()
{
    //on sort d'allegro
    m_alleg.myallegro_exit();

    /* libere les aeroports */
    for ( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
        delete ma_aeroports[ui8_i];
    }

    /* libere les avions */
    for ( uint8_t ui8_i = 0; ui8_i < ma_avions.size(); ui8_i++ )
    {
        delete ma_avions[ui8_i];
    }

    /* libere les cases */
    for ( uint8_t ui8_i = 0; ui8_i < ma_cases.size(); ui8_i++ )
    {
        for ( uint8_t ui8_j = 0; ui8_j < ma_cases[ui8_i].size(); ui8_j++ )
        {
            delete ma_cases[ui8_i][ui8_j];
        }
    }
}

/* initialise les cases */
void Simulateur::v_init_cases()
{
    /* Declaration des ressources */
    std::vector <Case *> a_cases;

    /* cases en x */
    for( uint8_t ui8_i = 0; ui8_i < 88; ui8_i++ )
    {
        /* on vide le vecteur */
        while( 0 == a_cases.empty() )
        {
            a_cases.pop_back();
        }

        /* cases en y */
        for( uint8_t ui8_j = 0; ui8_j < 55; ui8_j++ )
        {
            /* Creation d'une nouvelle case */
            a_cases.push_back( new Case( ui8_i, ui8_j ) );
        }

        /* ajout dans la grille de cases */
        ma_cases.push_back( a_cases );
    }
}

//initialise tout le jeu
void Simulateur::v_init_all()
{
    srand(time(NULL));  /*initialise l'aléatoire*/
    v_init_allegro();   /*initialise allegro*/
    v_init_sound();     /*initialise le son*/
}

/* source du sous programme v_init_allegro : https://drive.google.com/file/d/19n9Itt_EVLrJ71etpWq2t6_5lwdjHdyY/view */
/* initialise allegro */
void Simulateur::v_init_allegro()
{
    /*initialisation d'allegro*/
    allegro_init();

    install_mouse();/*initialisation de la souris*/
    install_keyboard();/*initialisation du clavier*/

    set_color_depth(desktop_color_depth());/*mode graphique*/

    if(0 != set_gfx_mode(GFX_AUTODETECT_WINDOWED, 860, 800, 0, 0))/*erreur de création du mode graphique ?*/
    {
        m_alleg.myallegro_message("Probleme de gfx !");/*message erreur*/
        m_alleg.myallegro_exit();/*on arrete allegro*/
        m_alleg.myexit();/*on sort du programme*/
    }
}

/* initialise le son */
void Simulateur::v_init_sound()
{
    install_sound(DIGI_AUTODETECT, MIDI_NONE, 0);/*initialise le son*/

    /*existence du son ?*/
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0) != 0) {
        m_alleg.myallegro_message("Echec de l'installation du son !");/*message erreur*/
        m_alleg.myallegro_exit();/*on quitte allegro*/
        m_alleg.myexit();/*on sort du programme*/
    }
}

/* charge toutes les images */
void Simulateur::v_init_all_img()
{
    //charge la page d'affichage
    m_alleg.create_page(m_page, m_alleg.getSCREEN_W(), m_alleg.getSCREEN_H(), makecol(0,0,0));

    m_carte=m_alleg.load_img("images/carte.bmp");
}

/* initialise tous les sons */
void Simulateur::v_init_all_sound()
{
    m_alleg.load_one_sound( m_music, "music/sound_background.wav" );
}

/* initialise tous les aeroports (distances par rapport aux autres) */
void Simulateur::v_init_aers()
{
    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
        ma_aeroports[ui8_i]->v_set_distances( ma_aeroports );
    }
}

/*detruit les elements du simulateur*/
void Simulateur::v_detruire()
{
    //detruit toutes les images
    m_alleg.mydestroy_bitmap(m_page);   /* detruit la page d'affichage */

    m_souris.v_detruire();/*detruit les images de la souris*/

    m_alleg.mydestroy_bitmap( m_carte );    /* detruit la carte */

    m_alleg.mydestroy_sample( m_music );    /* detruit la musique */
}


/* methodes */

/* execute les fonctionnalites du simulateur */
void Simulateur::v_run()
{
    /* Declaration des ressources */
    bool b_left = false;
    bool b_right = false;

    /* tant qu'on n'appuie pas sur echap */
    while( !key[KEY_ESC] )
    {
        /* on avance dans les UT */
        m_UT.v_display( m_page, 710, 0 );

        /* affiche la carte */
        m_alleg.transparence( m_page, m_carte, 1, 0 );

        /* trace la grille de la carte */
        v_grille();

        /* affiche toutes les perturbation meteo */
        v_print_meteo();

        v_all_aretes(); /* affiche toutes les aretes */
        v_trace_PCC();  /* trace le PCC d'un point A a B */

        /* les commandes pour l'affichage */
        v_commandes();

        /* affiche tous les aeroports */
        v_print_aeroports( b_left, b_right );
        /* affiche tous les avions */
        v_print_avions( b_left, b_right );
        /* affiche tous les vols */
        v_print_vols( b_left, b_right );

        /* affiche l'heure */
        m_horloge.v_display( m_page, 0, 0 );
       //m_UT.v_display( m_page, 0, 50 );
       /* affiche le flot max */
        v_print_flot_max();

        /* des vols peuvent-ils enfin etre effectues ? */
        v_vol_dispo();

        /* mise en place des vols aleatoires */
        v_vols_alea();

        v_refresh();

        if(!key[KEY_LEFT])  /* pas appui sur gauche */
        {
            b_left = false;
        }
        if(!key[KEY_RIGHT])  /* pas appui sur droite */
        {
            b_right = false;
        }
    }
}

/* des vols peuvent-ils enfin etre effectues ? */
void Simulateur::v_vol_dispo()
{
    /* Declaration des ressources */
    bool b_deb_fuite = false;

    /* des vols s'effectuent ? */
    if(( 0 == m_UT.i_get_min()%3 ) && ( 0 == m_UT.i_get_sec() ))
    {
        for( uint8_t ui8_i = 0; ui8_i < ma_vols.size(); ui8_i++ )/* etudie tous les vols */
        {
            if( true == ma_vols[ui8_i]->b_get_ok_vol() )/* vol en cours ? */
            {
                /* deplacement de l'avion ! */
                /* arrivee atteinte ? */
                if( true == ma_vols[ui8_i]->b_vol_en_cours( ma_cases, b_deb_fuite, mb_fuite_res ) )
                {
                    //std::cout << "enter" << std::endl;
                    /* si le vol est fini (et pas d'escale d'urgence suite a une fuite) ou si on est arrive a l'aeroport d'arrivee on l'elimine */
                    if( false == ma_vols[ui8_i]->get_avion()->b_get_fuite() )
                    {
                        /* on elimine le vol */
                        ma_vols.erase( ma_vols.begin() + ui8_i );
                        /* cherche les altitudes des avions pour eviter une collision */
                        //v_anti_collision();
                    }
                    /* sinon on recherche un chemin */
                    else
                    {
                        //std::cout << "repare" << std::endl;
                        /* on cherche un autre chemin pour atteindre l'aeroport final */
                        v_chemin_plus_court( false, true, ma_vols[ui8_i]->get_aer_urg()->i_get_id(), ma_vols[ui8_i]->get_aer_arr()->i_get_id(), ma_vols[ui8_i] );
                        //std::cout << ma_vols[ui8_i]->get_aer_arr()->i_get_id() << std::endl;
                    }
                }
                /* est-ce un debut de fuite ? */
                if( true == b_deb_fuite )
                {
                    //std::cout << "begin" << std::endl;
                    /* on cherche l'aeroport le plus proche */
                    v_find_urgence( ma_vols[ui8_i] );
                    //std::cout << ma_vols[ui8_i]->get_aer_urg()->i_get_id() << std::endl;
                }
            }
        }
    }

    for( uint8_t ui8_i = 0; ui8_i < ma_vols.size(); ui8_i++ )/* etudie tous les vols */
    {
        ma_vols[ui8_i]->v_partir_enfin( m_horloge );    /* le vol peut il enfin partir ? */
    }
}

/* cherche l'aeroport le plus proche de l'avion qui est disponible */
void Simulateur::v_find_urgence( Vol *vol )
{
    /* Declaration des ressources */
    int i_d_min = 0;
    int i_d_actuel = 0;
    uint8_t ui8_aer = 0;

    /* parcours de tous les aeroports */
    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
        /* si c'est le premier */
        if( 0 == ui8_i )
        {
            /* on garde en memoire la premiere distance */
            i_d_min = ma_aeroports[ui8_i]->i_dist_av_aer( vol->get_avion() );
            /* on garde en memoire le rang de l'aeroport concerne */
            ui8_aer = ui8_i;
        }
        /* sinon on cherche si la distance minimale est battue */
        else
        {
            /* on calcule la nouvelle distance */
            i_d_actuel = ma_aeroports[ui8_i]->i_dist_av_aer( vol->get_avion() );

            /* distance minimale battue ? */
            if( i_d_actuel < i_d_min )
            {
                /* on sauve cette nouvelle distance */
                i_d_min = i_d_actuel;
                /* on garde en memoire le rang de l'aeroport concerne */
                ui8_aer = ui8_i;
            }
        }
    }

    /* on note l'aeroport le plus proche de l'avion */
    vol->v_set_aer_urg( ma_aeroports[ui8_aer] );
}

/* observe si des avions sont dans des aeroports */
void Simulateur::v_check_aer_av()
{
    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )/* etudie tous les aeroports */
    {
        for( uint8_t ui8_j = 0; ui8_j < ma_avions.size(); ui8_j++ ) /* etudie tous les avions */
        {
            /* meme coordonnees */
            if(( ma_avions[ui8_j]->i_get_poseX() == ma_aeroports[ui8_i]->i_get_poseX()*10 ) && ( ma_avions[ui8_j]->i_get_poseY() == ma_aeroports[ui8_i]->i_get_poseY()*10 ) )
            {
                /* on ajoute l'avion dans la file de l'aeroport */
                ma_aeroports[ui8_i]->v_ajout_avion( ma_avions[ui8_j] );
            }
        }
    }
}

/* affichage ON (true) ou OFF (false) */
std::string Simulateur::s_ON_OFF( bool const&b_choix )
{
    /* Declaration des ressources */
    std::string s_txt = "";

    /* true = ON */
    if( true == b_choix )
    {
        s_txt = "ON";
    }
    /* false = OFF */
    if( false == b_choix )
    {
        s_txt = "OFF";
    }

    /* on retourne le resultat */
    return s_txt;
}

/* menu console */
void Simulateur::v_menu_console()
{
    /* Declaration des ressources */
    std::string s_txt = "";
    int i_choix = -1;

    m_alleg.myplay_sample(m_music, 255, 128, 1000, 1);  /* lance la musique de fond */
    m_horloge.v_init( 0, 0, 0, 10 );                 /* initialise l'horloge */
    m_UT.v_set_cpt(3);                                  /* initialise l'UT */
    //m_UT.v_set_cpt(10);                                  /* initialise l'UT */

   // if(key[KEY_C])
    //{

    do
    {
        /* affichage du menu */
        std::cout << "Menu :" << std::endl << std::endl;
        std::cout << "1 : Creer un vol" << std::endl;
        std::cout << "2 : Afficher les aeroports" << std::endl;
        std::cout << "3 : Afficher les avions" << std::endl;
        std::cout << "4 : Afficher les vols" << std::endl;
        //std::cout << "5 : Sauvegarder les aeroports et avions" << std::endl;
        std::cout << "5 : Perturbation meteo" << std::endl;
        std::cout << "6 : Fuite de reservoir (" + s_ON_OFF( mb_fuite_res ) + ")" << std::endl;
        std::cout << "7 : Fermeture des frontieres (" + s_ON_OFF( mb_ferme_front ) + ")" << std::endl;
        std::cout << "8 : Vols aleatoires (" + s_ON_OFF( mb_vol_rand ) + ")" << std::endl;
        std::cout << "9 : Etude des flots (" + s_ON_OFF( mb_ford ) + ")" << std::endl;
        std::cout << "10 : Crise mondiale" << std::endl;
        std::cout << "11 : Acceder au simulateur" << std::endl;
        std::cout << "0 : Quitter" << std::endl;
        std::cin >> s_txt;

        /* saisie d'un nombre ? */
        if( true == m_alleg.b_only_digit( s_txt ) )
        {
            /* conversion du texte en nombre */
            i_choix = stoi( s_txt );

            if( 0 == i_choix )
            {
                /* message d'extinction du programme */
                std::cout << "Extinction en cours..." << std::endl;
            }
            else if( 1 == i_choix )
            {
                /* saisie d'un nouveau vol */
                v_chemin_plus_court( false );
            }
            else if( 2 == i_choix )
            {
                /* affiche tous les aeroports */
                v_console_aeroports();
            }
            else if( 3 == i_choix )
            {
                /* affiche tous les avions */
                v_console_avions();
            }
            else if( 4 == i_choix )
            {
                /* affiche tous les vols */
                v_console_vols();
            }
            //else if( 5 == i_choix )
            //{
                /* sauvegarde */
            //    v_save();
            //}
            else if( 5 == i_choix )
            {
                /* choix meteo */
                v_choix_meteo();
            }
            else if( 6 == i_choix )
            {
                /* faire fuiter les avions ? */
                v_choix_fuite_front( mb_fuite_res, "faire fuiter les réservoirs des avions" );
            }
            else if( 7 == i_choix )
            {
                /* faire fermer les frontieres des aeroports ? */
                v_choix_fuite_front( mb_ferme_front, "fermer les frontieres des aeroports" );
            }
            else if( 8 == i_choix )
            {
                /* faire fermer les frontieres des aeroports ? */
                v_choix_fuite_front( mb_vol_rand, "creer des vols aleatoirement" );
            }
            else if( 9 == i_choix )
            {
                /* activer l'etude des flots ? ? */
                v_choix_fuite_front( mb_ford, "faire l'etude des flots" );
            }
            else if( 10 == i_choix )
            {
                /* lance les fonctions de kruskal */
                v_run_kruskal();
            }
            else if( 11 == i_choix )
            {
                /* on lance le simulateur */
                v_run();
            }
            else
            {
                std::cout << "Saisie hors de l'intervalle..." << std::endl;
            }
        }

    }while( 0 != i_choix );
   // }
}

/* on choisit si on veut faire fuiter les reservoirs des avions */
void Simulateur::v_choix_fuite_front( bool &b_choix, std::string s_txt )
{
    /* Declaration des ressources */
    int i_val = 0;

    /* affichage de la demande */
    std::cout << std::endl << "Mettre en place la possibilité de " << s_txt << " ?" << std::endl;
    std::cout << "0 : Non" << std::endl;
    std::cout << "1 : Oui" << std::endl;
    std::cout << "Choix : ";

    /* saisie du choix */
    m_alleg.v_saisie_blinde( i_val, 0, 1 );

    /* non */
    if( 0 == i_val )
    {
        b_choix = false;
    }
    /* oui */
    else
    {
        b_choix = true;
    }

    /* retour a la ligne */
    std::cout << std::endl;
}

/* mise en place des vols aleatoire */
void Simulateur::v_vols_alea()
{
    /* vols aleatoires actives ? */
    if( true == mb_vol_rand )
    {
        /* creation d'un vol toutes les UT */
        if(( 0 == m_UT.i_get_min()%3 ) && ( 0 == m_UT.i_get_sec() ))
        {
            v_chemin_plus_court( true );
        }
    }
}

/* on choisit la meteo sur un rectangle choisi */
void Simulateur::v_choix_meteo()
{
    /* Declaration des ressources */
    std::vector <std::string> s_tab = { "Soleil", "Orage", "Typhon", "Trous_d_air" };
    int i_choix = 0;
    int i_x1 = 0;
    int i_x2 = 0;
    int i_y1 = 0;
    int i_y2 = 0;

    /* choix de la meteo */
    std::cout << "Choix meteo : " << std::endl;
    std::cout << "1 : Soleil" << std::endl;
    std::cout << "2 : Orage" << std::endl;
    std::cout << "3 : Typhon" << std::endl;
    std::cout << "4 : Trous d'air" << std::endl << std::endl;
    std::cout << "Votre choix = ";

    /* saisie du choix */
    m_alleg.v_saisie_blinde( i_choix, 1, 4 );

    /* choix de la zone impactée */
    std::cout << std::endl << std::endl << "Veuillez choisir la zone impactée (X dans [0, 87] et Y dans [0,54]) : " << std::endl;

    std::cout << "x1 dans [0, 87] : ";
    m_alleg.v_saisie_blinde( i_x1, 0, 87 );   /* saisie du choix */

    std::cout << "x2 dans [" << i_x1+1 << ", 87] : ";
    m_alleg.v_saisie_blinde( i_x2, i_x1+1, 87 );   /* saisie du choix */

    std::cout << "y1 dans [0, 54] : ";
    m_alleg.v_saisie_blinde( i_y1, 0, 54 );   /* saisie du choix */

    std::cout << "y2 dans [" << i_y1+1 << ", 54] : ";
    m_alleg.v_saisie_blinde( i_y2, i_y1+1, 54 );   /* saisie du choix */

    /* on change la meteo des cases concernees */
    for( uint8_t ui8_i = i_x1; ui8_i < i_x2; ui8_i++ )
    {
        for( uint8_t ui8_j = i_y1; ui8_j < i_y2; ui8_j++ )
        {
            ma_cases[ui8_i][ui8_j]->v_set_meteo( s_tab[i_choix] );  /* on ecrit la meteo */

            /* zone disponible ? (Soleil) */
            if( 1 == i_choix )
            {
                ma_cases[ui8_i][ui8_j]->v_set_dispo( true );
            }
            /* sinon zone indisponible */
            else
            {
                ma_cases[ui8_i][ui8_j]->v_set_dispo( false );
            }
        }
    }
}

/* affiche tous les avions sur console */
void Simulateur::v_console_avions()
{
    std::cout << std::endl << "Affichage des avions : " << std::endl;

    /* affiche tous les avions */
    for( uint8_t ui8_j = 0; ui8_j < ma_avions.size(); ui8_j++ )
    {
        ma_avions[ui8_j]->v_console();
    }

    /* aucun avion a afficher ? */
    if( 0 == ma_avions.size() )
    {
        std::cout << std::endl << "Aucun avion enregistre..." << std::endl;
    }
}

/* affiche tous les aeroports sur console */
void Simulateur::v_console_aeroports()
{
    std::cout << std::endl << "Affichage des aeroports : " << std::endl;

    /* affiche tous les avions */
    for( uint8_t ui8_j = 0; ui8_j < ma_aeroports.size(); ui8_j++ )
    {
        ma_aeroports[ui8_j]->v_console( ma_aeroports );
    }

    /* aucun aeroport a afficher ? */
    if( 0 == ma_aeroports.size() )
    {
        std::cout << std::endl << "Aucun aeroport enregistre..." << std::endl;
    }
}

/* affiche tous les vols sur console */
void Simulateur::v_console_vols()
{
    std::cout << std::endl << "Affichage des vols : " << std::endl;

    /* affiche tous les avions */
    for( uint8_t ui8_j = 0; ui8_j < ma_vols.size(); ui8_j++ )
    {
        ma_vols[ui8_j]->v_console();
    }

    /* aucun vol a afficher ? */
    if( 0 == ma_vols.size() )
    {
        std::cout << std::endl << "Aucun vol enregistre..." << std::endl;
    }
}

/* affiche la barre de commandes */
void Simulateur::v_commandes()
{
    /* Declaration des ressources */
    std::vector <std::string> a_txt = { "Aeroports", "Avions", "Vols" };
    m_alleg.drawrect(m_page, false, false, 0, 552, 860, 800);
    /* affiche les 3 boutons */
    for( uint8_t ui8_i = 0; ui8_i < 3; ui8_i++ )
    {
        /* affiche les rectangles */
        m_alleg.drawrect(m_page, true, false, 0, 552+83*ui8_i, 100, 552+83*(ui8_i+1));

        /* affiche les textes associes */
        m_alleg.txtlangue( m_page, a_txt[ui8_i], 50-4*a_txt[ui8_i].size(), 585+83*ui8_i, false );

        /* collision et click gauche ? */
        if( m_souris.b_verifclick( 1, 0, 552+83*ui8_i, 100, 552+83*(ui8_i+1) ))
        {
            mi_command = ui8_i + 1;  /* onglet selectionne */
            mi_curseur = 0;         /* on reinitialise le curseur */
        }
    }
}

/* affiche tous les aeroports */
void Simulateur::v_print_aeroports( bool &b_left, bool &b_right )
{
    /* Declaraation des ressources */
    int i_taille = ma_aeroports.size()-3;


    if( 1 == mi_command )
    {
        /* trop d'aeroports a afficher ? */
        if( 3 < ma_aeroports.size() )
        {
            /* aller a gauche ? */
            if(( key[KEY_LEFT] ) && ( 0 != mi_curseur ) && ( false == b_left ))
            {
                mi_curseur--;
                b_left = true;  /* click gauche */
            }
            /* aller a droite ? */
            else if(( key[KEY_RIGHT] ) && ( i_taille != mi_curseur ) && ( false == b_right ))
            {
                mi_curseur++;
                b_right = true;  /* click gauche */
            }
        }
        else
        {
            /* on peut tous les afficher */
            mi_curseur = 0;
        }
    }

    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
        ma_aeroports[ui8_i]->v_display_all( m_page, m_souris, 120, 552, ( 1 == mi_command ), ui8_i, mi_curseur, ma_aeroports );  /* affiche un aeroport */
    }

    /* changement d'UT et possibilite de fermer les frontieres ? ? */
    if(( 0 == m_UT.i_get_min()%3 ) && ( 0 == m_UT.i_get_sec() ) && ( true == mb_ferme_front ))
    {
        for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
        {
            ma_aeroports[ui8_i]->v_evenement( );  /* aeroport va ouvrir ou fermer ? */
        }
    }
}

/* affiche toutes les perturbation meteo */
void Simulateur::v_print_meteo()
{
    for ( uint8_t ui8_i = 0; ui8_i < ma_cases.size(); ui8_i++ )
    {
        for ( uint8_t ui8_j = 0; ui8_j < ma_cases[ui8_i].size(); ui8_j++ )
        {
            ma_cases[ui8_i][ui8_j]->v_print_meteo( m_page );
        }
    }
}

/* affiche tous les avions */
void Simulateur::v_print_avions( bool &b_left, bool &b_right )
{
    /* Declaration des ressources */
    int i_taille = ma_avions.size()-3;

    if( 2 == mi_command )
    {
        /* trop d'avions a afficher ? */
        if( 3 < ma_avions.size() )
        {
            /* aller a gauche ? */
            if(( key[KEY_LEFT] ) && ( 0 != mi_curseur ) && ( false == b_left ))
            {
                mi_curseur--;
                b_left = true;  /* click gauche */
            }
            /* aller a droite ? */
            else if(( key[KEY_RIGHT] ) && ( i_taille != mi_curseur ) && ( false == b_right ))
            {
                mi_curseur++;
                b_right = true;  /* click gauche */
            }
        }
        else
        {
            /* on peut tous les afficher */
            mi_curseur = 0;
        }
    }

    for( uint8_t ui8_i = 0; ui8_i < ma_avions.size(); ui8_i++ )
    {
        ma_avions[ui8_i]->v_display_all( m_page, m_souris, 120, 552, ( 2 == mi_command ), ui8_i, mi_curseur );  /* affiche un avion */
    }
}


/* affiche tous les vols */
void Simulateur::v_print_vols( bool &b_left, bool &b_right )
{
    /* Declaration des ressources */
    int i_taille = ma_avions.size()-3;

    if( 3 == mi_command )
    {
        /* trop d'avions a afficher ? */
        if( 3 < ma_vols.size() )
        {
            /* aller a gauche ? */
            if(( key[KEY_LEFT] ) && ( 0 != mi_curseur ) && ( false == b_left ))
            {
                mi_curseur--;
                b_left = true;  /* click gauche */
            }
            /* aller a droite ? */
            else if(( key[KEY_RIGHT] ) && ( i_taille != mi_curseur ) && ( false == b_right ))
            {
                mi_curseur++;
                b_right = true;  /* click gauche */
            }
        }
        else
        {
            /* on peut tous les afficher */
            mi_curseur = 0;
        }
    }

    for( uint8_t ui8_i = 0; ui8_i < ma_vols.size(); ui8_i++ )
    {
        ma_vols[ui8_i]->v_display_all( m_page, m_souris, 120, 552, ( 3 == mi_command ), ui8_i, mi_curseur );  /* affiche un avion */
    }
}

/* trace la grille de la carte */
void Simulateur::v_grille()
{
    /* colonnes */
    for( uint8_t ui8_i = 0; ui8_i < 86; ui8_i++ )
    {
        m_alleg.myline( m_page, ui8_i*10, 0, ui8_i*10, 552, 100, 100, 100 );
    }

    /* lignes */
    for( uint8_t ui8_i = 0; ui8_i < 55; ui8_i++ )
    {
        m_alleg.myline( m_page, 0, ui8_i*10, 860, ui8_i*10, 100, 100, 100 );
    }
}

/* sauvegarde les caractéristiques du simulateur */
//void Simulateur::v_save()
//{
    /* sauve les caractéristiques de tous les avions */
    //v_save_avions();
    /* sauve les caractéristiques de tous les aeroports */
    //v_save_aeroports();
//}

/* sauve les caractéristiques de tous les avions */
//void Simulateur::v_save_avions()
//{
    //Declaration des ressources
//    std::ofstream file("save/avions.txt");  /* ouverture du fichier en ecriture */
//    int i_nb_avions = ma_avions.size();     /* le nombre d'avions a sauvegarder */

    /* existence du fichier ? */
//    if(!file)
//    {
//        std::cout << "Erreur d'ouverture du fichier de sauvegarde des avions..." << std::endl;  /* message erreur */
//    }
//    else
//    {
        /* sauve le nombre d'avions a sauvegarder */
//        file << i_nb_avions << std::endl;

        /* sauve les avions */
//        for( int i_i = 0; (i_i < i_nb_avions); i_i++ )
//        {
            /* sauve l'avion */
//            ma_avions[i_i]->v_save( file );
//        }

//        std::cout << "La sauvegarde des avions est un succes !" << std::endl;  /* message de reussite */
//        file.close();   /* fermeture du fichier */
//    }
//}

/* sauve les caractéristiques de tous les aeroports */
//void Simulateur::v_save_aeroports()
//{
    //Declaration des ressources
//    std::ofstream file("save/aeroports.txt");  /* ouverture du fichier en ecriture */
//    int i_nb_aers = ma_aeroports.size();       /* le nombre d'avions a sauvegarder */

    /* existence du fichier ? */
//    if(!file)
//    {
//        std::cout << "Erreur d'ouverture du fichier de sauvegarde des aeroports..." << std::endl;  /* message erreur */
//    }
//    else
//    {
        /* sauve le nombre d'avions a sauvegarder */
//        file << i_nb_aers << std::endl;

        /* sauve les avions */
//        for( int i_i = 0; (i_i < i_nb_aers); i_i++ )
//        {
            /* sauve l'avion */
//            ma_aeroports[i_i]->v_save( file );
//        }

//        std::cout << "La sauvegarde des aeroports est un succes !" << std::endl;  /* message de reussite */
//        file.close();   /* fermeture du fichier */
//    }
//}

/* charge les caractéristiques du simulateur */
void Simulateur::v_load()
{
    /* charge les caractéristiques de tous les avions */
    v_load_avions();
    /* charge les caractéristiques de tous les aeroports */
    v_load_aeroports();
}

/* charge les caractéristiques de tous les avions */
void Simulateur::v_load_avions()
{
    //Declaration des ressources
    std::ifstream file("save/avions.txt");  /* ouverture du fichier en lecture */
    int i_nb_avions = 0;

    /* existence du fichier ? */
    if(!file)
    {
        std::cout << "Erreur d'ouverture du fichier de sauvegarde des avions..." << std::endl;  /* message erreur */
    }
    else
    {
        /* charge le nombre d'avions a charger */
        file >> i_nb_avions;

        /* charge les avions */
        for( int i_i = 0; (i_i < i_nb_avions); i_i++ )
        {
            /* charge l'avion */
            /* ajout dans le tableau des avions */
            ma_avions.push_back( new Avion(file, i_i) );
        }

        std::cout << "Le chargement des avions est un succes !" << std::endl;  /* message de reussite */
        file.close();   /* fermeture du fichier */
    }
}

/* charge les caractéristiques de tous les aeroports */
void Simulateur::v_load_aeroports()
{
    //Declaration des ressources
    std::ifstream file("save/aeroports.txt");  /* ouverture du fichier en lecture */
    int i_nb_aers = 0;

    /* existence du fichier ? */
    if(!file)
    {
        std::cout << "Erreur d'ouverture du fichier de sauvegarde des avions..." << std::endl;  /* message erreur */
    }
    else
    {
        /* charge le nombre d'aeroports a charger */
        file >> i_nb_aers;

        /* charge les avions */
        for( int i_i = 0; (i_i < i_nb_aers); i_i++ )
        {
            /* charge l'aeroport */
            /* ajout dans le tableau des avions */
            ma_aeroports.push_back( new Aeroport(file, i_i) );
        }

        std::cout << "Le chargement des aeroports est un succes !" << std::endl;  /* message de reussite */
        file.close();   /* fermeture du fichier */
    }
}

/*rafraichit l'ecran*/
void Simulateur::v_refresh()
{
    //gère le click gauche et droite de la souris
    for(int i=1; i<3; i++)
        m_souris.v_gere_souris(i);

    //affiche la souris
    m_souris.v_display(m_page);
    //affiche le tout
    m_alleg.myblit(m_page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    //pause de 10ms
    m_alleg.myrest(10);
}


/* choix des sommets de départ et d'arrivee et recherche du chemin le plus court (dijkstra) */
void Simulateur::v_chemin_plus_court( bool const&b_rand, bool b_auto, int i_b, int i_d, Vol *urgence )
{
    /* Declaration des ressources */
    std::vector <std::vector <int>> a_composantes = {};
    int i_begin = i_b;
    int i_end = i_d;
    Vol* v = new Vol;

    v_cherche_composantes(a_composantes, b_rand);       /* cherche les composantes connexes */
    v_print_composantes(a_composantes);         /* affiche les composantes connexes */

    /* on s'assure qu'on peut bien saisir un vol */
    if( true == b_vol_possible(a_composantes) )
    {
        /* est-ce l'administrateur qui cree un vol ? */
        if( false == b_auto )
        {
            /* saisie des sommets de départ et d'arrivee */
            v_choose_way( b_rand, i_begin, i_end, a_composantes );

            /* algo de dijkstra pour le PCC */
            v_dijkstra( i_begin, i_end );
        }
        else
        {
            v_dijkstra( i_begin, i_end, urgence );
        }

        /* affiche le PCC */
        v_print_PCC( i_begin, i_end, v );

        /* est-ce l'administrateur qui cree un vol ? */
        if(( false == b_auto ) && ( false == b_rand ))
        {
            /* saisit de l'heure de depart */
            v->v_change_heure();
        }
        /* sinon on ecrit l'heure si c'est aleatoire */
        else if( true == b_rand )
        {
            /* l'heure est saisie aleatoirement */
            v->v_set_heure( m_horloge.i_get_heure(), m_horloge.i_get_min()+(rand()%60) );
        }

        /* ajoute le nouveau vol si l'arrivee est bien asservie */
        if(NULL == v->get_aer_dep())
        {
            std::cout << "Vol non créé car l'arrivée n'est pas atteint..." << std::endl;
        }
        else
        {
            std::cout << "Vol créé !!!" << std::endl;

            /* algo de ford active ? */
            if( true == mb_ford )
            {
                /* on lance l'algo ! */
                v_ford_fulkerson( ma_aeroports[ v->get_aer_dep()->i_get_id() ], ma_aeroports[ v->get_aer_arr()->i_get_id() ] );
            }

            //on inverse l'ordre les aeroports intermediaires pour coherence
            v->v_reverse();

            /* est-ce l'administrateur qui cree un vol ? */
            if( false == b_auto )
            {
                /* ajout du nouveau vol */
                ma_vols.push_back( v );
                /* ajout du nouvel avion */
                v->v_set_avion( ma_aeroports[ v->get_aer_dep()->i_get_id() ]->get_avion() );
                /* on garde en memoire les arcs (aeroports) de l'avion */
                v->v_create_aretes();
            }
            else
            {
                /* modification du vol d'urgence */
                urgence->v_vide_inter();

                /* plus de vol d'urgence */
                urgence->v_set_aer_urg( NULL );

                /* copie des vols intermediaires */
                for( uint8_t ui8_i = 0; ui8_i < v->a_get_aer_int().size(); ui8_i++ )
                {
                    urgence->v_ajout_int( v->a_get_aer_int()[ui8_i] );
                }

                /* on change l'aeroport de depart */
                urgence->v_set_aer_dep( v->get_aer_dep() );

                /* on garde en memoire les ares (aeroports) de l'avion */
                urgence->v_create_aretes();

                //std::cout << "dep = " << urgence->get_aer_dep()->i_get_id() << std::endl;
                //std::cout << "arr = " << urgence->get_aer_arr()->i_get_id() << std::endl;
            }
            /* on sauve l'aeroport de depart et d'arrivee */
            mi_PCC[0] = i_begin;
            mi_PCC[1] = i_end;

            /* cherche les altitudes des avions pour eviter une collision */
            v_anti_collision();
        }
    }
    else
    {
        /* sinon aucun vol saisissable */
        std::cout << "Impossible de saisir un vol : aucun aeroport lie et/ou aucun avion disponible..." << std::endl;
    }
}

/* saisies des sommets de depart et d'arrivee */
void Simulateur::v_choose_way( bool const&b_rand, int &i_begin, int &i_end, std::vector <std::vector <int>> const&a_composantes )
{
    /* Declaration des ressources */
    int i_taille = ma_aeroports.size();
    std::string s_txt = "";
    bool b_valid = false;

        /* saisie par l'administrateur ? */
        if( false == b_rand )
        {
            /* saisie du sommet de départ */
            std::cout << "Veuillez saisir le sommet de depart : " ;

            /* initialisation de la valeur */
            i_begin = -1;

            do
            {
                /* saisie de l'id */
                std::cin >> s_txt;

                if( true == m_alleg.b_only_digit( s_txt ) )
                {
                    /* conversion du texte en nombre */
                    i_begin = stoi( s_txt );

                    /* saisie hors de l'intervalle ? */
                    if (( 0 > i_begin ) || ( i_taille <= i_begin ))
                    {
                        /* message erreur */
                        std::cout << "Erreur d'intervalle de saisie... Recommencez : " ;
                    }
                    /* pas d'avion disponible ? */
                    else if ( ma_aeroports[ i_begin ]->get_all_avion().empty() )
                    {
                        /* message erreur */
                        std::cout << "Aucun avion n'est disponible dans cet aeroport... Recommencez : " ;
                    }
                    /* aeroport indisponible ? */
                    else if ( false == ma_aeroports[ i_begin ]->b_get_dispo() )
                    {
                        /* message erreur */
                        std::cout << "Cet aeroport n'est pas disponible... Recommencez : " ;
                    }
                    /* aeroport dans une cc seule ? */
                    else if( 1 == a_composantes[ i_cherche_CC( a_composantes, i_begin ) ].size() )
                    {
                        /* message erreur */
                        std::cout << "Cet aeroport est dans une composante connexe seule... Recommencez : " ;
                    }
                    /* sinon toutes les conditions sont validees */
                    else
                    {
                        b_valid = true;
                    }
                }

            }while( false == b_valid );
            /* on ne peut saisir qu'un aeroport libre avec un id dans l'intervalle et des avions disponibles */
            /* en plus il ne peut pas faire parti d'une composante connexe a un aeroport */

            /* saisie du sommet d'arrivee */
            std::cout << std::endl << "Veuillez saisir le sommet d'arrivee : " ;

            /* initialisation des valeurs */
            i_end = -1;
            b_valid = false;

            do
            {
                /* saisie de l'id */
                std::cin >> s_txt;

                if( true == m_alleg.b_only_digit( s_txt ) )
                {
                    /* conversion du texte en nombre */
                    i_end = stoi( s_txt );

                    /* saisie hors de l'intervalle ? */
                    if (( 0 > i_end ) || ( i_taille <= i_end ))
                    {
                        /* message erreur */
                        std::cout << "Erreur d'intervalle de saisie... Recommencez : " ;
                    }
                    /* meme aeroport saisi */
                    else if ( i_begin == i_end )
                    {
                        /* message erreur */
                        std::cout << "Meme aeroport saisi... Recommencez : " ;
                    }
                    /* aeroport indisponible ? */
                    else if ( false == ma_aeroports[ i_end ]->b_get_dispo() )
                    {
                        /* message erreur */
                        std::cout << "Cet aeroport n'est pas disponible... Recommencez : " ;
                    }
                    /* aeroport de depart et de fin dans des composantes connexes differentes ? */
                    else if( i_cherche_CC( a_composantes, i_begin ) != i_cherche_CC( a_composantes, i_end ) )
                    {
                        /* message erreur */
                        std::cout << "Aeroport de depart et de fin dans CC differentes... Recommencez : " ;
                    }
                    /* sinon toutes les conditions sont validees */
                    else
                    {
                        b_valid = true;
                    }
                }

            }while( false == b_valid );
            /* on ne peut saisir qu'un aeroport libre avec un id dans l'intervalle et des avions disponibles */
            /* en plus il doit faire parti de la meme composante connexe du premier aeroport */

            /* choix du type de l'avion */
            v_choix_type( ma_aeroports[ i_begin ] );
        }
        else
        {
            /* random ! */
            /* initialisation de la valeur */
            i_begin = -1;

            do
            {
                /* nombre random */
                i_begin = rand()%(ma_aeroports.size());

                /* saisie hors de l'intervalle ? */
                if (( 0 > i_begin ) || ( i_taille <= i_begin )){}
                /* pas d'avion disponible ? */
                else if ( ma_aeroports[ i_begin ]->get_all_avion().empty() ){}
                /* aeroport indisponible ? */
                else if ( false == ma_aeroports[ i_begin ]->b_get_dispo() ){}
                /* aeroport dans une cc seule ? */
                else if( 1 == a_composantes[ i_cherche_CC( a_composantes, i_begin ) ].size() ){}
                /* sinon toutes les conditions sont validees */
                else
                {
                    b_valid = true;
                }

            }while( false == b_valid );

            /* initialisation des valeurs */
            i_end = -1;
            b_valid = false;

            do
            {
                /* nombre random */
                i_end = rand()%(ma_aeroports.size());

                /* saisie hors de l'intervalle ? */
                if (( 0 > i_end ) || ( i_taille <= i_end )){}
                /* meme aeroport saisi */
                else if ( i_begin == i_end ){}
                /* aeroport indisponible ? */
                else if ( false == ma_aeroports[ i_end ]->b_get_dispo() ){}
                /* aeroport de depart et de fin dans des composantes connexes differentes ? */
                else if( i_cherche_CC( a_composantes, i_begin ) != i_cherche_CC( a_composantes, i_end ) ){}
                /* sinon toutes les conditions sont validees */
                else
                {
                    b_valid = true;
                }

            }while( false == b_valid );
        }

}

/* choix du type de l'avion */
void Simulateur::v_choix_type( Aeroport *depart )
{
    /* Declaration des ressources */
    std::vector < std::string > a_txt = { "0 : Court Courrier", "1 : Moyen Courrier", "2 : Long Courrier" };
    std::vector < bool > a_tab = { false, false, false };
    int i_choix = -1;

    /* on observe les types d'avion disponible a l'aeroport */
    depart->v_avion_types( a_tab );

    /* affiche les types disponibles */
    std::cout << "Veuillez choisir le type de l'avion qui va partir." << std::endl;

    for( uint8_t ui8_i = 0; ui8_i < 3; ui8_i++ )
    {
        /* disponible ? */
        if( true == a_tab[ ui8_i ] )
        {
            /* on l'affiche ! */
            std::cout << a_txt[ ui8_i ] << std::endl;
        }
    }

    /* saisie du choix tant que le type saisie n'est pas disponible */
    do{

        m_alleg.v_saisie_blinde( i_choix, 0, 2 );

        /* type non present ? */
        if( false == a_tab[ i_choix ] )
        {
            /* message erreur */
            std::cout << "Ce type n'existe pas dans cet aeroport..." << std::endl;
        }

    }while( false == a_tab[ i_choix ] );

    /* l'avion en tete de file doit correspondre au type choisi */
    depart->v_tete_file_type( i_choix );
}

/* algo de dijkstra pour le PCC */
void Simulateur::v_dijkstra( const int &i_begin, const int &i_end, Vol *urgence )
{
    /* Declaration des ressources */
    std::priority_queue <Aeroport*, std::vector<Aeroport*> , comparison > a_queue;
    Aeroport *sommet;
    bool b_cdt = false;
    int i_dis_max = 0;

    /* s'il n'y a pas de vol d'urgence, on prend l'avion en tete de file */
    if( NULL == urgence )
    {
        /* calcul de la distance max que peut effectuer l'avion en tete de file */
        i_dis_max = 100*ma_aeroports[ i_begin ]->get_all_avion().front()->i_get_capa()/ma_aeroports[ i_begin ]->get_all_avion().front()->i_get_consom();
    }
    /* sinon on regarde pour l'avion qui a fuite */
    else
    {
        /* calcul de la distance max que peut effectuer l'avion en tete de file */
        i_dis_max = 100*urgence->get_avion()->i_get_capa()/urgence->get_avion()->i_get_consom();
    }

    /* Initialiation */
    v_init_dijkstra( i_begin, i_end );

    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
        a_queue.push( ma_aeroports[ui8_i] );  /* insere tous les sommets dans le conteneur */
    }

    /* tant que les sommets ne sont tous pas marques */
    while( false == b_cdt )
    {
        /* on recupere le sommet non marque ayant la plus petite distance de s0 */
        do
        {
            sommet = a_queue.top();     /* on recupere le sommet prioritaire (plus petite distance de s0) */
            a_queue.pop();              /* on la retire de la file prioritaire */

            /* on ne peut pas étudier un sommet qui apparait plusieurs fois dans la file */
            /* on ne peut pas prendre un aeroport non disponible */
        }while(( 0 != ma_aeroports[sommet->i_get_id()]->ui8_get_marquage() ) || ( false == ma_aeroports[ sommet->i_get_id() ]->b_get_dispo() ) );

        /* on marque le sommet choisi */
        ma_aeroports[sommet->i_get_id()]->v_set_marquage(1);

        /* on observe les successeurs du sommet actuel */
        for ( uint8_t ui8_i = 0; ui8_i < sommet->get_aers().size(); ui8_i++ )
        {
            /* successeur non marque et aeroport disponible et arete disponible et distance realisable ? */
            if (( 0 == ma_aeroports[sommet->get_aers()[ui8_i].first]->ui8_get_marquage() ) && ( true == ma_aeroports[ sommet->get_aers()[ui8_i].first ]->b_get_dispo() ) && ( true == ma_aeroports[ sommet->i_get_id() ]->b_get_lien_dispo( ui8_i ) ) && ( sommet->get_aers()[ui8_i].second <= i_dis_max ))
            {
                /* chemin plus court ou distance infini ? */
                if (( ma_aeroports[sommet->get_aers()[ui8_i].first]->i_get_distance() > sommet->i_get_distance() + sommet->get_aers()[ui8_i].second ) || ( -1 == ma_aeroports[sommet->get_aers()[ui8_i].first]->i_get_distance() ))
                {
                    /* on met a jour la distance */
                    ma_aeroports[ma_aeroports[sommet->get_aers()[ui8_i].first]->i_get_id()]->v_set_distance( sommet->i_get_distance() + sommet->get_aers()[ui8_i].second );

                    /* on note le predecesseur du successeur */
                    ma_aeroports[ma_aeroports[sommet->get_aers()[ui8_i].first]->i_get_id()]->v_set_pred( sommet->i_get_id() );

                    /* on vide la file prioritaire */
                    while ( 0 == a_queue.empty() )
                    {
                        a_queue.pop();
                    }

                    /* on la recree pour tout reactualiser */
                    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
                    {
                        a_queue.push( ma_aeroports[ui8_i] );
                    }
                }
            }
        }

        /* reinitialise la valeur */
        b_cdt = true;

        /* tous les sommets marques ? */
        for ( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
        {
            /* un sommet non marque accessible ? */
            if (( -1 != ma_aeroports[ui8_i]->i_get_distance() ) && ( 0 == ma_aeroports[ui8_i]->ui8_get_marquage() ) && ( true == ma_aeroports[ui8_i]->b_get_dispo() ))
            {
                /* on continue l'algo */
                b_cdt = false;
            }
        }
    }
}

/* initialisation pour algo de dijkstra */
void Simulateur::v_init_dijkstra( const int &i_begin, const int &i_end )
{
    /* initialisation des sommets */
    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
        /* tous les sommets sont non marques */
        ma_aeroports[ui8_i]->v_set_marquage(0);

        /* les sommets n'ont pas de predecesseur */
        ma_aeroports[ui8_i]->v_set_pred(-1);

        /* les sommets sont a une distance infinie */
        ma_aeroports[ui8_i]->v_set_distance(-1);
    }

    /* so est a une distance 0 de lui meme */
    ma_aeroports[i_begin]->v_set_distance(0);
}

/* affiche le resultat du PCC */
void Simulateur::v_print_PCC( const int &i_begin, const int &i_end, Vol *v )
{
    /* Declaration des ressources */
    int i_poids = 0;

    std::cout << "Resultat du PCC a partir du sommet " << i_begin << " :" << std::endl;

    /* affichage pour tous les sommets existants */
    for (int i_i = 0; (i_i < (int)ma_aeroports.size()); i_i++)
    {
        /* reinitialise le poids */
        i_poids = 0;

        /* affiche le resultat du BFS par recurrence */
        v_rec_PCC(i_i, i_poids, v, i_begin, i_end, (i_end==i_i));
    }
}

/* affiche le resultat du PCC par recurrence */
void Simulateur::v_rec_PCC(uint8_t ui8_rang, int &i_poids, Vol *v, const int &i_begin, const int &i_end, bool b_cdt)
{
    /* affichage du numero du sommet actuel */
    std::cout << ma_aeroports[ui8_rang]->i_get_id();

    /* ajout des aeroports dans le vol */
    if(b_cdt)
    {
        if( i_begin == ma_aeroports[ui8_rang]->i_get_id() )/* aeroport de depart ? */
        {
            v->v_set_aer_dep( ma_aeroports[ui8_rang] );
        }
        else if( i_end == ma_aeroports[ui8_rang]->i_get_id() )/* aeroport de arrivee ? */
        {
            v->v_set_aer_arr( ma_aeroports[ui8_rang] );
        }
        else        /* aeroport intermediaire */
        {
            v->v_ajout_int( ma_aeroports[ui8_rang] );
        }
    }

    /* le predecesseur existe ? */
    if ( -1 != ma_aeroports[ui8_rang]->i_get_pred())
    {
        /* on augmente le poids pour atteindre le sommet concerne */
        i_poids += i_find_pds_arc( ma_aeroports[ui8_rang]->i_get_pred(), ma_aeroports[ui8_rang]->i_get_id() ) ;

        /* affichage predecesseur */
        std::cout << " <-- ";
        v_rec_PCC( ma_aeroports[ui8_rang]->i_get_pred(), i_poids, v, i_begin, i_end, b_cdt );
    }
    else    /* le predecesseur n'existe pas */
    {
        std::cout << std::endl;  /* retour a la ligne */

        /* on affiche le poids */
        std::cout << "Poids = " << i_poids << std::endl;
    }
}

/* cherche le poids pour un arc donne : ui8_id1 = depart, ui8_id2 = arrivee */
int Simulateur::i_find_pds_arc( uint8_t ui8_id1, uint8_t ui8_id2 )
{
    /* Declaration des ressources */
    int i_poids = 0;

    /* on cherche l'arc concerne */
    for ( uint8_t ui8_i = 0; (( ui8_i < ma_aeroports[ui8_id1]->get_aers().size() ) && ( 0 == i_poids )); ui8_i++ )
    {
        /* on trouve l'id du sommet d'arrivee concerne ? */
        if ( ui8_id2 == ma_aeroports[ma_aeroports[ui8_id1]->get_aers()[ui8_i].first]->i_get_id() )
        {
            /* on sauve le poids de l'arc */
            i_poids = ma_aeroports[ui8_id1]->get_aers()[ui8_i].second;
        }
    }

    /* on retourne le poids de l'arc si trouve */
    return i_poids;
}

/* Trace le PCC d'un aeroport aux autres */
void Simulateur::v_trace_PCC()
{
    /* affichage pour tous les sommets existants */
    for (int i_i = 0; (i_i < (int)ma_aeroports.size()); i_i++)
    {
        /* si ce n'est pas le chemin concerne */
        if( mi_PCC[1] != i_i )
        {
            /* affiche le resultat du PCC par recurrence en vert */
            v_rec_trace_PCC(i_i, 0, 255, 0);
        }
    }

    /* affiche le resultat du PCC par recurrence du final vers le depart en rouge */
    v_rec_trace_PCC(mi_PCC[1], 255, 0, 0);
}

/* Trace le PCC d'un aeroport aux autres */
void Simulateur::v_rec_trace_PCC(uint8_t ui8_rang, int i_r, int i_g, int i_b)
{
    /* le predecesseur existe ? */
    if ( -1 != ma_aeroports[ui8_rang]->i_get_pred())
    {
        /* on trace l'arete entre les deux aeroports */
        v_trace_arete( ma_aeroports[ui8_rang], ma_aeroports[ma_aeroports[ui8_rang]->i_get_pred()], i_r, i_g, i_b );
        v_rec_trace_PCC( ma_aeroports[ui8_rang]->i_get_pred(), i_r, i_g, i_b );
    }
}

/* Trace une arete entre deux aeroports */
void Simulateur::v_trace_arete( Aeroport *first, Aeroport *second, int i_r, int i_g, int i_b, int i_flot_act, int i_flot_max, int i_poids )
{
    /* Declaration des ressources */
    std::string s_txt = m_alleg.s_int_to_str( i_cherche_pds( first, second, i_poids ));
    int i_mil_x = (first->i_get_poseX()*10+5+second->i_get_poseX()*10+5)/2 - 4*s_txt.size();
    int i_mil_y = (first->i_get_poseY()*10+5+second->i_get_poseY()*10+5)/2;
    std::string s_txt2 = "";
    std::string s_txt3 = "";

    /* on trace l'arete si les deux aeroports sont disponibles */
    if(( true == first->b_get_dispo() ) && ( true == second->b_get_dispo() ))
    {

        /* trace la ligne : on la rend plus épaisse en creant plusieurs lignes */
        m_alleg.myline( m_page, first->i_get_poseX()*10+5, first->i_get_poseY()*10+5, second->i_get_poseX()*10+5, second->i_get_poseY()*10+5, i_r, i_g, i_b );
        m_alleg.myline( m_page, first->i_get_poseX()*10+5+1, first->i_get_poseY()*10+5, second->i_get_poseX()*10+5+1, second->i_get_poseY()*10+5, i_r, i_g, i_b );
        m_alleg.myline( m_page, first->i_get_poseX()*10+5-1, first->i_get_poseY()*10+5, second->i_get_poseX()*10+5-1, second->i_get_poseY()*10+5, i_r, i_g, i_b );
        m_alleg.myline( m_page, first->i_get_poseX()*10+5, first->i_get_poseY()*10+5+1, second->i_get_poseX()*10+5, second->i_get_poseY()*10+5+1, i_r, i_g, i_b );
        m_alleg.myline( m_page, first->i_get_poseX()*10+5, first->i_get_poseY()*10+5-1, second->i_get_poseX()*10+5, second->i_get_poseY()*10+5-1, i_r, i_g, i_b );

        /* correction de position de texte pour eviter superposition */
        if((( 4 == first->i_get_id() ) && ( 11 == second->i_get_id() )) || (( 11 == first->i_get_id() ) && ( 4 == second->i_get_id() )))
        {
            i_mil_y -= 40;
        }
        else if((( 4 == first->i_get_id() ) && ( 3 == second->i_get_id() )) || (( 3 == first->i_get_id() ) && ( 4 == second->i_get_id() )))
        {
            i_mil_x -= 20;
            i_mil_y -= 30;
        }
        else if((( 10 == first->i_get_id() ) && ( 11 == second->i_get_id() )) || (( 11 == first->i_get_id() ) && ( 10 == second->i_get_id() )))
        {
            i_mil_x += 20;
            i_mil_y -= 30;
        }
        else if((( 1 == first->i_get_id() ) && ( 11 == second->i_get_id() )) || (( 11 == first->i_get_id() ) && ( 1 == second->i_get_id() )))
        {
            i_mil_x -= 90;
        }
        else if((( 3 == first->i_get_id() ) && ( 8 == second->i_get_id() )) || (( 8 == first->i_get_id() ) && ( 3 == second->i_get_id() )))
        {
            i_mil_x -= 15;
        }
        else if((( 4 == first->i_get_id() ) && ( 8 == second->i_get_id() )) || (( 8 == first->i_get_id() ) && ( 4 == second->i_get_id() )))
        {
            i_mil_x += 10;
            i_mil_y += 20;
        }
        else if((( 11 == first->i_get_id() ) && ( 12 == second->i_get_id() )) || (( 12 == first->i_get_id() ) && ( 11 == second->i_get_id() )))
        {
            i_mil_y += 5;
        }
        else if((( 0 == first->i_get_id() ) && ( 1 == second->i_get_id() )) || (( 1 == first->i_get_id() ) && ( 0 == second->i_get_id() )))
        {
            i_mil_y -= 30;
        }

        /* ecrit le poids */
        m_alleg.txtlangue( m_page, s_txt, i_mil_x,  i_mil_y);

        /* ecrit les donnees du flot s'il existe */
        if(( -1 != i_flot_act ) && ( -1 != i_flot_max ) && ( true == mb_ford ))
        {
            i_mil_y += 15;

            /* flot actuel */
            s_txt2 = m_alleg.s_int_to_str( i_flot_act );
            i_mil_x += 4*s_txt.size() - 4*s_txt2.size() - 12;
            m_alleg.txtlangue( m_page, s_txt2, i_mil_x,  i_mil_y);

            /* flot max */
            s_txt3 = m_alleg.s_int_to_str( i_flot_max );
            i_mil_x += 4*s_txt.size() - 4*s_txt3.size() + 12;
            m_alleg.txtlangue( m_page, s_txt3, i_mil_x,  i_mil_y);
        }
    }
}

/* recherche du poids entre deux aeroports souhaites */
int Simulateur::i_cherche_pds( Aeroport *first, Aeroport *second, int i_poids )
{
    /* parcours tout les aeroports adjacents du premier pour trouver le deuxieme */
    for ( uint8_t ui8_i = 0; ((ui8_i < first->get_aers().size()) && (0 == i_poids)); ui8_i++ )
    {
        /* meme id ? */
        if( second->i_get_id() == first->get_aers()[ui8_i].first )
        {
            i_poids = first->get_aers()[ui8_i].second;
        }
    }

    return i_poids;
}

/* trace toutes les aretes */
void Simulateur::v_all_aretes()
{
    /* regarde tout les aeroports adjacents du premier pour trouver le deuxieme */
    for ( uint8_t ui8_i = 0; (ui8_i < ma_aeroports.size()); ui8_i++ )
    {
        /* trace toutes les aretes des aeroports adjacents du premier */
        for ( uint8_t ui8_j = 0; (ui8_j < ma_aeroports[ui8_i]->get_aers().size()); ui8_j++ )
        {
           // std::cout << ma_aeroports[ui8_i]->b_get_lien_dispo( ui8_j ) << std::endl;
            /* arete disponible ? */
            if( true == ma_aeroports[ui8_i]->b_get_lien_dispo( ui8_j ) )
            {
                /* on trace l'arete entre les deux aeroports */
                v_trace_arete( ma_aeroports[ui8_i], ma_aeroports[ma_aeroports[ui8_i]->get_aers()[ui8_j].first], 16, 215, 255, ma_aeroports[ui8_i]->i_get_flot_actuel( ui8_j ), ma_aeroports[ui8_i]->i_get_flot_max( ui8_j ), ma_aeroports[ui8_i]->get_aers()[ui8_j].second );
            }
        }
    }
   //std::cout << std::endl;
}

/* cherche les altitudes des avions pour eviter une collision */
void Simulateur::v_anti_collision()
{
    /* Declaration des ressources */
    int i_color_max = 0;

    /* D'abord on cree le graphe reliant les avions */
    v_create_graphe();

    /* Ensuite on lance l'algo de Welsh et Powell pour determiner une coloration */
    attribuerWelshPowell( i_color_max );

    /* Enfin on fixe une altitude pour chaque avion */
    v_fixer_altitudes( i_color_max );
}

/* creation du graphe, avec les avions comme sommet, les intersections entre avions comme arete */
void Simulateur::v_create_graphe()
{
    /* Declaration des ressources */
    std::vector< std::pair< int, int > > deb;
    std::vector< std::pair< int, int > > fin;
    bool b_done = false;

    /* on vides les listes des aretes de chaque avion */
    for( uint8_t ui8_i = 0; ui8_i < ma_avions.size(); ui8_i++ )
    {
        ma_avions[ui8_i]->v_vide_adj();
    }

    /* on observe les aretes de chaque avion */
    for( uint8_t ui8_i = 0; ui8_i < ma_avions.size(); ui8_i++ )
    {
        /* on stocke en memoire ses aretes */
        deb = ma_avions[ui8_i]->get_aretes();

        /* on observe les aretes de tous les autres avions */
        for( uint8_t ui8_j = ui8_i+1; ui8_j < ma_avions.size(); ui8_j++ )
        {
            /* on stocke en memoire les aretes de l'aute avion */
            fin = ma_avions[ui8_j]->get_aretes();

            /* reinit de la valeur */
            b_done = false;

            /* on compare toutes les aretes des deux avions */
            for( uint8_t ui8_k = 0; ((ui8_k < deb.size()) && (false == b_done)); ui8_k++ )
            {
                for( uint8_t ui8_l = 0; ((ui8_l < fin.size()) && (false == b_done)); ui8_l++ )
                {
                    /* meme arete ? */
                    if( deb[ui8_k] == fin[ui8_l] )
                    {
                        /* dans chaque avion on ajoute l'adjacence */
                        ma_avions[ui8_i]->v_ajout_adj( ma_avions[ui8_j] );
                        ma_avions[ui8_j]->v_ajout_adj( ma_avions[ui8_i] );
                        /* c'est fait */
                        b_done = true;
                    }
                }
            }
        }
    }
}

/* on fixe une altitude pour chaque avion */
void Simulateur::v_fixer_altitudes( int const&i_color_max )
{
    /* on parcourt tous les avions */
    for( uint8_t ui8_i = 0; ui8_i < ma_avions.size(); ui8_i++ )
    {
        /* on attribue une altitude pour les avions qui volent */
        if(( true == ma_avions[ui8_i]->b_get_en_vie() ) && ( 0 != ma_avions[ui8_i]->i_get_degre() ))
        {
            /* on attribue a l'avion une altitude en fonction de sa couleur */
            //ma_avions[ui8_i]->v_set_altitude( ma_avions[ui8_i]->i_get_color()*30+200 );
            ma_avions[ui8_i]->v_set_alt_to_do( ma_avions[ui8_i]->i_get_color()*30+200 );
        }
    }

    /* s'il n'y a qu'une couleur */
    if( 1 == i_color_max )
    {
        /* on met l'altitude des avions concernes a 200 km */
        for( uint8_t ui8_i = 0; ui8_i < ma_vols.size(); ui8_i++ )
        {
            ma_vols[ui8_i]->v_set_alt_to_do( 200 );
        }
    }

}

/* algo de Welsh et Powell */
void Simulateur::attribuerWelshPowell(int& nbFrequences)
{
    /* Déclaration des ressources */
    unsigned int ui_nbdone = 0;  /* nombre de stations faites */
    int i_freq = 1;              /* frequence actuelle */
    std::vector < Avion *> a_all;

    /* on copie tous les avions et on reinitialise leur couleur */
    for( uint8_t ui8_i = 0; ui8_i < ma_avions.size(); ui8_i++ )
    {
        ma_avions[ui8_i]->v_set_color( 0 );
        a_all.push_back( ma_avions[ui8_i] );
    }

    /* tri des stations par ordre décroissant */
    std::sort(a_all.begin(), a_all.end(), [](Avion* s1, Avion* s2) { return s1->i_get_degre() > s2->i_get_degre(); });

    /* Tant que les fréquences n'ont pas été attribuées à toutes les stations */
    while(ui_nbdone < a_all.size())
    {
        /* parcours de toutes les stations */
        for (auto s : a_all)
        {
            // pas d'incompatibilité et pas toutes les stations affectées et fréquence non attribuées ?
            if ( s->b_testFrequence(i_freq) && (ui_nbdone < a_all.size()) && (0 == s->i_get_color()))
            {
                s->v_set_color(i_freq);/* Fréquence attribuée à la station */
                ui_nbdone++;    /* une attribution de fréquence de plus */
            }
        }

        // pas toutes les stations affectées ?
        if (ui_nbdone < a_all.size())
        {
            /* on passe à la fréquence suivante */
            i_freq++;
        }
    }

    /* Attribution de la fréquence max */
    nbFrequences = i_freq;
}


/* cherche les composantes connexes */
void Simulateur::v_cherche_composantes(std::vector <std::vector <int>> &a_all, bool const&b_auto)
{
    /* Declaration des ressources */
    std::vector <int> a_group = {};
    std::vector <bool> a_done = {};
    int i_choix = -1;

    /* saisie du type d'arborescence */
    if( false == b_auto )
    {
        std::cout << "Choix du type d'arborescence : " << std::endl;
        std::cout << "0 : DFS" << std::endl;
        std::cout << "1 : BFS" << std::endl;
        std::cout << "Votre choix : " ;

        /* saisie blindee */
        m_alleg.v_saisie_blinde( i_choix, 0, 1 );
    }
    else
    {
        i_choix = 0;
    }


    /* on s'assure que le vecteur recu en parametre est vide */
    while ( 0 == a_all.empty() )
    {
        while ( 0 == a_all[a_all.size()-1].empty() )    /* on s'assure que la case concernée est vide */
        {
            a_all[a_all.size()-1].pop_back();           /* retire une case du sous vecteur */
        }
        a_all.pop_back();                               /* retire une case du vecteur */
    }

    /* initialise le tableau permettant de connaitre si un sommet est parcouru ou non */
    for (uint8_t ui8_i = 0; (ui8_i < ma_aeroports.size()); ui8_i++)
    {
        /* aucun sommet trouve */
        a_done.push_back(false);
    }

    /* on fait un DFS ou BFS a partir des sommets non trouves (s'il y en a toujours) */
    for (uint8_t ui8_i = 0; ((ui8_i < ma_aeroports.size()) && (false == b_all_found(a_done) )); ui8_i++)
    {
        /* sommet non trouve ? */
        if ( false == a_done[ui8_i] )
        {
            /* choix DFS ? */
            if( 0 == i_choix )
            {
                /* parcours en profondeur (DFS) */
                v_init_DFS(ma_aeroports[ui8_i]);
            }
            /* choix BFS */
            else
            {
                /* parcours en largeur */
                v_parcours_BFS( ma_aeroports[ui8_i] );
            }

            /* on sauve les nouveaux sommets trouves dans un tableau */
            for (uint8_t ui8_j = 0; (ui8_j < ma_aeroports.size()); ui8_j++)
            {
                /* si le sommet est noir (trouve) et n'a pas ete deja trouve */
                if (( "noir" == ma_aeroports[ui8_j]->s_get_coloration() ) && ( false == a_done[ui8_j] ))
                {
                    /* on ajoute un nouveau sommet dans la composante connexe */
                    a_group.push_back(ui8_j);

                    /* sommet trouve ! */
                    a_done[ui8_j] = true;
                }
            }

            /* on l'ajoute comme nouvelle composante connexe dans le tableau general */
            a_all.push_back(a_group);

            /* on vide le tableau de composante connexe */
            while ( 0 == a_group.empty() )
            {
                a_group.pop_back();
            }
        }
    }

    /* affichage arborescence obtenue sur console */

    /* choix DFS ? */
    if( 0 == i_choix )
    {
        v_print_BFS_DFS("DFS");
    }
    /* choix BFS */
    else
    {
        v_print_BFS_DFS("BFS");
    }
}

/* tous les sommets trouves pour composantes connexes ? */
bool Simulateur::b_all_found(std::vector <bool> const &a_done)
{
    /* Declaration des ressources */
    uint8_t ui8_trouve = 0;
    bool b_cdt = false;

    /* parcours de tous les sommets */
    for (uint8_t ui8_j = 0; (ui8_j < ma_aeroports.size()); ui8_j++)
    {
        /* sommet trouve ? */
        if ( true == a_done[ui8_j] )
        {
            /* 1 de trouve en + */
            ui8_trouve++;
        }
    }

    /* tous les sommets trouves ? */
    if ( ui8_trouve == ma_aeroports.size() )
    {
        b_cdt = true;
    }
    else    /* tous les sommets ne sont pas trouves */
    {
        b_cdt = false;
    }

    return b_cdt;
}

/* reinitialise la couleur et le predecesseur de tous les sommets du graphe */
void Simulateur::v_reinit_colpred()
{
    /* cherche tous les sommets */
    for (uint8_t ui8_i = 0; (ui8_i < ma_aeroports.size()); ui8_i++)
    {
        /* reinitialisation d'un sommet */
        ma_aeroports[ui8_i]->v_set_coloration("blanc");
        ma_aeroports[ui8_i]->v_set_pred2(-1);
    }
}

/* initialisation (DFS) */
void Simulateur::v_init_DFS(Aeroport* sommet_zero)
{
    /* initialise tous les sommets en blanc et reinitialise les predecesseurs et dates */
    v_reinit_colpred();

    /* parcours en profondeur (DFS) */
    v_parcours_DFS(sommet_zero);
}

/* affiche le resultat du BFS ou DFS */
void Simulateur::v_print_BFS_DFS(std::string s_type)
{
    std::cout << "Arborescence du " << s_type << " sur tous les sommets :" << std::endl;

    /* affichage pour tous les sommets existants */
    for (uint8_t ui8_i = 0; (ui8_i < ma_aeroports.size()); ui8_i++)
    {
        /* affiche le resultat du BFS par recurrence */
        v_rec_BFS_DFS(ui8_i);
    }
}

/* affiche le resultat du BFS ou DFS par recurrence */
void Simulateur::v_rec_BFS_DFS(uint8_t ui8_rang)
{
    /* affichage du numero du sommet actuel */
    std::cout << ma_aeroports[ui8_rang]->i_get_id();

    /* le predecesseur existe ? */
    if ( -1 != ma_aeroports[ui8_rang]->i_get_pred2())
    {
        /* affichage predecesseur */
        std::cout << " <-- ";
        v_rec_BFS_DFS( ma_aeroports[ui8_rang]->i_get_pred2() );
    }
    else    /* le predecesseur n'existe pas */
    {
        std::cout << std::endl;  /* retour a la ligne */
    }
}

/* parcours en profondeur (DFS) */
void Simulateur::v_parcours_DFS(Aeroport* sommet)
{
    /* Declaration des ressources */
    uint8_t ui8_i = 0;
    /* le sommet devient gris */
    sommet->v_set_coloration("gris");

    /* si l'aeroport est disponible */
    if( true == sommet->b_get_dispo() )
    {
        /* observe les voisins du sommet actuel */
        for ( auto voisin : sommet->get_aers() )
        {
            /* voisin blanc et aeroport disponible et arete disponible ? */
            if (( "blanc" == ma_aeroports[voisin.first]->s_get_coloration() ) && ( true == ma_aeroports[voisin.first]->b_get_dispo() ) && ( true == sommet->b_get_lien_dispo( ui8_i ) ))
            {
                /* on note le predecesseur du voisin */
                ma_aeroports[voisin.first]->v_set_pred2( sommet->i_get_id() );

                /* parcours en profondeur (DFS) */
                v_parcours_DFS(ma_aeroports[voisin.first]);
            }

            ui8_i++;
        }
    }

    /* le sommet devient noir */
    sommet->v_set_coloration("noir");
}

/* parcours en largeur (BFS) */
void Simulateur::v_parcours_BFS(Aeroport* sommet_zero)
{
    /* Declaration des ressources */
    std::queue <Aeroport*> a_queue = {};
    Aeroport *st_actuel;
    uint8_t ui8_i = 0;

    /* initialise tous les sommets en blanc et reinitialise les predecesseurs */
    v_reinit_colpred();

    /* enfile s0 */
    a_queue.push(sommet_zero);

    /* s0 devient gris */
    sommet_zero->v_set_coloration("gris");

    /* tant que la file n'est pas vide */
    while ( 0 == a_queue.empty() )
    {
        /* copie le sommet sortant de la file */
        st_actuel = a_queue.front();

        /* defile le prochain sommet de la file */
        a_queue.pop();

        /* reinit la valeur */
        ui8_i = 0;

        /* observe les voisins du sommet actuel */
        for ( auto voisin : st_actuel->get_aers() )
        {
            /* voisin blanc et aeroport disponible et arete disponible ? */
            if (( "blanc" == ma_aeroports[voisin.first]->s_get_coloration() ) && ( true == ma_aeroports[voisin.first]->b_get_dispo() ) && ( true == st_actuel->b_get_lien_dispo( ui8_i ) ))
            {
                /* le voisin devient gris */
                ma_aeroports[voisin.first]->v_set_coloration( "gris" );

                /* on note le predecesseur du voisin */
                ma_aeroports[voisin.first]->v_set_pred2( st_actuel->i_get_id() );

                /* enfile le voisin */
                a_queue.push( ma_aeroports[voisin.first] );
            }

            ui8_i++;
        }

        /* le sommet actuel devient noir */
        st_actuel->v_set_coloration("noir");
    }
}

/* affiche les composantes connexes */
void Simulateur::v_print_composantes(std::vector <std::vector <int>> const&a_all)
{
    std::cout << std::endl << "Affichage des composantes connexes :" << std::endl;

    /* Affichage des composantes connexes */
    for (uint8_t ui8_i = 0; (ui8_i < a_all.size()); ui8_i++)
    {
        std::cout << "Composante connexe " << (int)(ui8_i+1) << " : ";

        /* Affichage des sommets d'une composante connexe */
        for (uint8_t ui8_j = 0; (ui8_j < a_all[ui8_i].size()); ui8_j++)
        {
            std::cout << a_all[ui8_i][ui8_j] << " " ;
        }

        /* retour a la ligne */
        std::cout << std::endl;
    }
}

/* on cherche dans quelle composante connexe un aeroport fait parti */
int Simulateur::i_cherche_CC( std::vector <std::vector <int>> const&a_all, int const&i_id )
{
    /* Declaration des ressources */
    int i_cc = -1;

    /* recherche des composantes connexes */
    for (uint8_t ui8_i = 0; ((ui8_i < a_all.size()) && ( -1 == i_cc )); ui8_i++)
    {
        /* recherche des sommets d'une composante connexe */
        for (uint8_t ui8_j = 0; ((ui8_j < a_all[ui8_i].size()) && ( -1 == i_cc )); ui8_j++)
        {
            /* meme id ? */
            if( i_id == a_all[ui8_i][ui8_j] )
            {
                /* numero de la composante connexe trouve ! */
                i_cc = ui8_i;
            }
        }
    }

    /* on retourne le resultat */
    return i_cc;
}

/* on verifie qu'un vol peut etre saisie */
bool Simulateur::b_vol_possible( std::vector <std::vector <int>> const&a_composantes )
{
    /* Declaration des ressources */
    bool b_valid1 = false;
    bool b_valid2 = false;
    bool b_valid = false;

    /* un vol est valide s'il y a au moins 1 avion disponible dans au moins 1 aeroport */
    for( uint8_t ui8_i = 0; (( ui8_i < ma_aeroports.size() ) && ( false == b_valid1 )); ui8_i++ )
    {
        /* au moins 1 avion disponible ? */
        if( 0 != ma_aeroports[ui8_i]->get_all_avion().size() )
        {
            /* c'est le cas ! */
            b_valid1 = true;
        }
    }

    /* il faut aussi qu'il y ait moins de CC que d'aeroports */
    if( a_composantes.size() < ma_aeroports.size() )
    {
        /* c'est bien le cas ! */
        b_valid2 = true;
    }

    /* si les 2 conditions sont valides on peut saisir un vol ! */
    if(( true == b_valid1 ) && ( true == b_valid2 ))
    {
        b_valid = true;
    }

    return b_valid;
}

/* pour kruskal, nous creons la liste des aretes entre aeroports sans doublon */
void Simulateur::v_crearete_kruskal( std::vector < std::vector <int>> &a_aretes )
{
    /* Declaration des ressources */
    std::vector <int> a_new = { 0, 0, 0, 0, 0 };   /* sommet 1, sommet 2, distance, choisi, numero arete */
    bool b_in = false;

    /* on parcourt tous les sommets (aeroports) */
    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
       /* on parcourt toutes ses aretes (connexions avec autres aeroports) */
        for( uint8_t ui8_j = 0; ui8_j < ma_aeroports[ui8_i]->get_aers().size(); ui8_j++ )
        {
            /* on verifie si l'arete ne figure pas deja dans le tableau */
            b_in = false;

            for( uint8_t ui8_k = 0; (( ui8_k < a_aretes.size() ) && ( false == b_in )); ui8_k++ )
            {
                /* deja dans le tableau ? */
                if((( a_aretes[ui8_k][0] == ma_aeroports[ui8_i]->i_get_id() ) && ( a_aretes[ui8_k][1] == ma_aeroports[ma_aeroports[ui8_i]->get_aers()[ui8_j].first]->i_get_id() )) || (( a_aretes[ui8_k][1] == ma_aeroports[ui8_i]->i_get_id() ) && ( a_aretes[ui8_k][0] == ma_aeroports[ma_aeroports[ui8_i]->get_aers()[ui8_j].first]->i_get_id() )))
                {
                    /* c'est le cas */
                    b_in = true;
                }
            }

            /* si l'arete n'est pas dans le tableau */
            if( false == b_in )
            {
                /* on cree l'arete et on l'ajoute ! */
                a_new[0] = ma_aeroports[ui8_i]->i_get_id();
                a_new[1] = ma_aeroports[ma_aeroports[ui8_i]->get_aers()[ui8_j].first]->i_get_id();
                a_new[2] = ma_aeroports[ui8_i]->get_aers()[ui8_j].second;
                a_aretes.push_back( a_new );
            }
        }
    }
}

/* algo de kruskal */
void Simulateur::v_kruskal( std::vector < std::vector <int>> &a_aretes )
{
    /* Delaration des ressources */
    std::priority_queue < std::vector <int>, std::vector< std::vector <int>> , compar_kruskal > a_queue;
    uint8_t ui8_nb_aretes = 0;  /* le nombre d'aretes selectionne est nul */
    uint8_t ui8_cc = 0;         /* la composante connexe a traiter */
    std::vector <int> actuel;
    int ui16_poids = 0;

    /* initialise les numeros des composantes connexes */
    for ( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
        ma_aeroports[ui8_i]->v_set_cc( ui8_i );
    }

    /* initialise les aretes */
    for ( int i_i = 0; i_i < (int)a_aretes.size(); i_i++ )
    {
        a_aretes[i_i][3] = 0; /* arete non choisi */
        a_aretes[i_i][4] = i_i; /* numero de l'arete */
    }

    /* Avec la priority queue les aretes sont deja triees par ordre croissant  */
    /* On les insere toutes dedans */
    for ( uint8_t ui8_i = 0; ui8_i < a_aretes.size(); ui8_i++ )
    {
        a_queue.push( a_aretes[ui8_i] );    /* ajoute un nouveau numero */
    }

    /* affiche les aretes triées */
    std::cout << std::endl << "Aretes triees dans l'ordre croissant avec le poids P :" << std::endl;

    /* tant que tous les sommets ne sont pas connectes */
    while ( ui8_nb_aretes < (int)ma_aeroports.size()-1 )
    {
        /* on choisi la prochaine arete de la liste */
        actuel = a_queue.top();

        /* on l'elimine de la file prioritaire */
        a_queue.pop();

        /* on affiche l'arete avec le poids */
        std::cout << ma_aeroports[ actuel[0] ]->i_get_id() << " " << ma_aeroports[ actuel[1] ]->i_get_id() << " P:" << actuel[2] << std::endl;

        /* composantes connexes differentes ? */
        if ( ma_aeroports[ actuel[0] ]->ui8_get_cc() != ma_aeroports[ actuel[1] ]->ui8_get_cc() )
        {
            /* l'arete est ajoute a l'arbre */
            a_aretes[ actuel[4] ][3] = 1 ;

            /* on garde en memoire la composante connexe a traiter */
            ui8_cc = ma_aeroports[ actuel[1] ]->ui8_get_cc();

            /* tous les sommets de meme CC du sommet 2 sont desormais de meme CC du sommet 1 */
            for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
            {
                /* meme CC que le sommet 2 ? */
                if ( ma_aeroports[ui8_i]->ui8_get_cc() == ui8_cc )
                {
                    /* desormais fait parti de la meme CC du sommet 1 */
                    ma_aeroports[ui8_i]->v_set_cc( ma_aeroports[ actuel[0] ]->ui8_get_cc() );
                }
            }

            /* une arete ajoutee en plus */
            ui8_nb_aretes++;
        }
    }

    /* retour a la ligne */
    std::cout << std::endl;

    std::cout << "Arbre couvrant obtenu : " << std::endl << std::endl;
    std::cout << "Liste des aretes selectionnees avec le poids P : " << std::endl;

    /* liste des aretes obtenues */
    for ( uint8_t ui8_i = 0; ui8_i < a_aretes.size(); ui8_i++ )
    {
        /* arete choisie ? */
        if( true == a_aretes[ui8_i][3] )
        {
            /* on affiche l'arete ave le poids */
            std::cout << ma_aeroports[ a_aretes[ui8_i][0] ]->i_get_id() << " " << ma_aeroports[ a_aretes[ui8_i][1] ]->i_get_id() << " P:" << a_aretes[ui8_i][2] << std::endl;

            /* on augmente le poids total */
            ui16_poids +=a_aretes[ui8_i][2];
        }
    }

    /* affiche le poids total */
    std::cout << std::endl << "Poids = " << (int)ui16_poids << std::endl;
}

/* attribue a chaque arete des aeroports leur existence ou non */
void Simulateur::v_reac_arete_kruskal( std::vector < std::vector <int>> const&a_aretes )
{
    /* Declaration des ressources */
    bool b_in = false;

    /* on parcourt tous les sommets (aeroports) */
    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
       /* on parcourt toutes ses aretes (connexions avec autres aeroports) */
        for( uint8_t ui8_j = 0; ui8_j < ma_aeroports[ui8_i]->get_aers().size(); ui8_j++ )
        {
            /* on verifie si l'arete est choisie */
            b_in = false;

            for( uint8_t ui8_k = 0; (( ui8_k < a_aretes.size() ) && ( false == b_in )); ui8_k++ )
            {
                /* trouve dans le tableau ? */
                if(( a_aretes[ui8_k][0] == ma_aeroports[ui8_i]->i_get_id() ) && ( a_aretes[ui8_k][1] == ma_aeroports[ma_aeroports[ui8_i]->get_aers()[ui8_j].first]->i_get_id() ))
                {
                    /* c'est le cas */
                    b_in = true;
                    /* on marque bien son choix ! */
                    ma_aeroports[ui8_i]->v_set_lien_dispo( ui8_j, a_aretes[ui8_k][3] );
                }
                else if(( a_aretes[ui8_k][1] == ma_aeroports[ui8_i]->i_get_id() ) && ( a_aretes[ui8_k][0] == ma_aeroports[ma_aeroports[ui8_i]->get_aers()[ui8_j].first]->i_get_id() ))
                {
                    /* c'est le cas */
                    b_in = true;
                    /* on marque bien son choix ! */
                    ma_aeroports[ui8_i]->v_set_lien_dispo( ui8_j, a_aretes[ui8_k][3] );
                }
            }
        }
    }
}

/* realise toutes les fonctionnalites de kruskal */
void Simulateur::v_run_kruskal()
{
    /* Declaration des ressources */
    std::vector < std::vector <int>> a_aretes;
    int i_choix = 0;

    /* affichage de la demande */
    std::cout << std::endl << "Activer l'algo de Kruskal ou restaurer les disponibilites des trajets inter-aeroports ?" << std::endl;
    std::cout << "0 : Activer l'algo de Kruskal" << std::endl;
    std::cout << "1 : Restaurer les disponibilites des trajets inter-aeroports" << std::endl;
    std::cout << "Choix : ";

    /* saisie du choix */
    m_alleg.v_saisie_blinde( i_choix, 0, 1 );

    /* Activer l'algo de Kruskal */
    if( 0 == i_choix )
    {
        /* ferme un certain nombre d'aeroports aleatoirement (anviron 1 sur 3) */
        for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
        {
            if( 0 == rand()%3 )
                ma_aeroports[ ui8_i ]->v_set_dispo( false );
        }

        /* nous creons la liste des aretes entre aeroports sans doublon */
        v_crearete_kruskal( a_aretes );

        /* algo de kruskal */
        v_kruskal( a_aretes );

        /* attribue a chaque arete des aeroports leur existence ou non */
        v_reac_arete_kruskal( a_aretes );
    }
    /* Restaurer les disponibilites des trajets inter-aeroports */
    else
    {
        /* remet toutes les aretes de kruskal disponibles */
        v_reinit_kruskal();

        /* reouvre tous les aeroports */
        for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
        {
            ma_aeroports[ ui8_i ]->v_set_dispo( true );
        }
    }

    /* retour a la ligne */
    std::cout << std::endl;
}

/* remet toutes les aretes de kruskal disponibles */
void Simulateur::v_reinit_kruskal()
{
    /* on parcourt tous les sommets (aeroports) */
    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
       /* on parcourt toutes ses aretes (connexions avec autres aeroports) */
        for( uint8_t ui8_j = 0; ui8_j < ma_aeroports[ui8_i]->get_aers().size(); ui8_j++ )
        {
            /* on marque bien sa disponibilite ! */
            ma_aeroports[ui8_i]->v_set_lien_dispo( ui8_j, 1 );
        }
    }
}

/* renitialise tous les flots ! */
void Simulateur::v_reinit_flot()
{
    /* on parcourt tous les sommets (aeroports) */
    for( uint8_t ui8_i = 0; ui8_i < ma_aeroports.size(); ui8_i++ )
    {
       /* on parcourt toutes ses aretes (connexions avec autres aeroports) */
        for( uint8_t ui8_j = 0; ui8_j < ma_aeroports[ui8_i]->get_aers().size(); ui8_j++ )
        {
            /* reinitialisation ! */
            ma_aeroports[ui8_i]->v_set_flot_actuel( ui8_j, 0 );
        }
    }
}

/* BFS pour ford */
bool Simulateur::b_BFS_ford(Aeroport* sommet_zero, Aeroport *arrivee)
{
    /* Declaration des ressources */
    std::queue <Aeroport*> a_queue = {};
    Aeroport *st_actuel;
    uint8_t ui8_i = 0;
    bool b_suite = true;
    bool b_cdt = false;

    /* initialise tous les sommets en blanc et reinitialise les predecesseurs */
    v_reinit_colpred();

    /* enfile s0 */
    a_queue.push(sommet_zero);

    /* s0 devient gris */
    sommet_zero->v_set_coloration("gris");

    /* tant que la file n'est pas vide et qu'il est possible d'avancer dans le BFS */
    while (( 0 == a_queue.empty() ) && ( true == b_suite ))
    {
        /* copie le sommet sortant de la file */
        st_actuel = a_queue.front();

        /* defile le prochain sommet de la file */
        a_queue.pop();

        /* si le sommet actuel est l'aeroport d'arrivee */
        if( st_actuel->i_get_id() == arrivee->i_get_id() )
        {
            /* on arrete ! */
            b_suite = false;
            /* chemin augmentant trouve ! */
            b_cdt = true;
        }
        /* ce n'est pas l'arrivee donc on continue */
        else
        {
            /* reinit les valeurs */
            ui8_i = 0;

            /* observe les voisins du sommet actuel */
            for ( auto voisin : st_actuel->get_aers() )
            {
                /* voisin blanc et aeroport disponible et arete disponible et flot pas max ? */
                if (( "blanc" == ma_aeroports[voisin.first]->s_get_coloration() ) && ( true == ma_aeroports[voisin.first]->b_get_dispo() ) && ( true == st_actuel->b_get_lien_dispo( ui8_i ) ) && ( st_actuel->i_get_flot_actuel( ui8_i ) != st_actuel->i_get_flot_max( ui8_i ) ))
                {
                    /* le voisin devient gris */
                    ma_aeroports[voisin.first]->v_set_coloration( "gris" );

                    /* on note le predecesseur du voisin */
                    ma_aeroports[voisin.first]->v_set_pred2( st_actuel->i_get_id() );

                    /* enfile le voisin */
                    a_queue.push( ma_aeroports[voisin.first] );
                }

                ui8_i++;
            }
        }

        /* le sommet actuel devient noir */
        st_actuel->v_set_coloration("noir");
    }

    return b_cdt;
}

/* recherche de l'augmentation possible du flot sur un chemin */
void Simulateur::v_flot_aug( Aeroport *current, int &i_min )
{
    /* le predecesseur existe ? */
    if ( -1 != current->i_get_pred2())
    {
        /* plus petit flot augmentant battu ? */
        if( current->i_get_flot_max( current->i_get_rang_aers( current->i_get_pred2() ) ) - current->i_get_flot_actuel( current->i_get_rang_aers( current->i_get_pred2() ) ) < i_min )
        {
            /* on sauve le record */
            i_min = current->i_get_flot_max( current->i_get_rang_aers( current->i_get_pred2() ) ) - current->i_get_flot_actuel( current->i_get_rang_aers( current->i_get_pred2() ) );
        }

        /* on continue avec le predecesseur */
        v_flot_aug( ma_aeroports[ current->i_get_pred2() ], i_min );
    }
}

/* on augmente les flots des aretes concernees ! */
void Simulateur::v_flot_plus_plus( Aeroport *current, int const&i_min )
{
    /* Declaration des ressources */
    int i_rang = 0;
    int i_rang2 = 0;

    /* le predecesseur existe ? */
    if ( -1 != current->i_get_pred2())
    {
        /* initialisation des rangs */
        i_rang = current->i_get_rang_aers( current->i_get_pred2() );
        i_rang2 = ma_aeroports[ current->i_get_pred2() ]->i_get_rang_aers( current->i_get_id() );

        /* on augmente le flot de l'arete dans les 2 sens ! */
        current->v_set_flot_actuel( i_rang, i_min + current->i_get_flot_actuel( i_rang ) );
        ma_aeroports[ current->i_get_pred2() ]->v_set_flot_actuel( i_rang2, i_min + ma_aeroports[ current->i_get_pred2() ]->i_get_flot_actuel( i_rang2 ) );

        /* on continue avec le predecesseur ! */
        v_flot_plus_plus( ma_aeroports[ current->i_get_pred2() ], i_min );
    }
}

/* ford fulkerson ! */
void Simulateur::v_ford_fulkerson( Aeroport* sommet_zero, Aeroport *arrivee )
{
    /* Declaration des ressources */
    int i_min = 0;

    /* renitialise tous les flots ! */
    v_reinit_flot();

    /* tant qu'on trouve un chemin augmentant */
    while( true == b_BFS_ford( sommet_zero, arrivee ) )
    {
        /* initialisation de la valeur minimale */
        i_min = arrivee->i_get_flot_max( arrivee->i_get_rang_aers( arrivee->i_get_pred2() ) ) - arrivee->i_get_flot_actuel( arrivee->i_get_rang_aers( arrivee->i_get_pred2() ) );

        /* on cherche la valeur du flot a augmenter */
        v_flot_aug( arrivee, i_min );

        /* on augmente le flot des aretes concernees ! */
        v_flot_plus_plus( arrivee, i_min );
    }

    /* calcul du flux max */
    v_calcul_flot_max( sommet_zero );

    /* on lance un BFS car c'est demande */
    v_parcours_BFS( sommet_zero );
}

/* calcul le flot max ! */
void Simulateur::v_calcul_flot_max( Aeroport *depart )
{
    /* reinitialise le flot max */
    mi_flot_max = 0;

    /* observe les voisins du sommet actuel */
    for( uint8_t ui8_j = 0; ui8_j < depart->get_aers().size(); ui8_j++ )
    {
        /* on augmente le flot max ! */
        mi_flot_max += depart->i_get_flot_actuel( ui8_j );
    }
}

/* affiche le flot max */
void Simulateur::v_print_flot_max()
{
    if( true == mb_ford )
    {
        m_alleg.txtlangue( m_page, "Flot max : " + m_alleg.s_int_to_str( mi_flot_max ), 700, 0, true );
    }
}
