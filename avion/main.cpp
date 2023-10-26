#include "Simulateur.h"

int main(int argc, char *argv[])
{
    /* Déclaration des ressources */
    Simulateur sim;

    /* execute les fonctionnalites du simulateur */
    sim.v_menu_console();

    /* detruit les elements du simulateur */
    sim.v_detruire();

    return 0;
}END_OF_MAIN();
