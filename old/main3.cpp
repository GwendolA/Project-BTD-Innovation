#include "raylib.h"
#include <iostream>
#include "menu.cpp"
#include "ballon.cpp"
#include <string.h>
#include "singe.cpp"
#include "bouton.cpp"
#include "interface.cpp"
#include "constantes.h"
#include "fleche.cpp"

int main(void)
{
    // Initialisation fenetre
    InitWindow(screenWidth, screenHeight, " classic game: missile commander");
    Image im_singe = LoadImage("C:/Users/maeva/OneDrive/Images/singe3.png");
    Image im_singe2 = LoadImage("C:/Users/maeva/OneDrive/Images/singe3.png");
    ImageResize(&im_singe, 150, 150);
    ImageResize(&im_singe2, 100, 100);
    Texture2D texture = LoadTextureFromImage(im_singe);
    Texture2D textsinge = LoadTextureFromImage(im_singe2);

    SetTargetFPS(60);

    menu();
    //--------------------------------------------------------------------------------------
    int round = 0;
    int vies = 40;
    int index = 0;
    int money = MONEY_DEPART;
    int nb_ballons = 0;
    int ballons_cree = 0;
    int compteur = 0;
    bool start_round = false;
    int ballons_vivants = 0;

    // création de la liste de stockage des ballons
    Ballon ballons[1000] = {};

    // liste chemin
    Rectangle chemin[36] = {};

    Button bouton1 = creer_bouton({13 * SQUARE_SIZE, 3.8 * SQUARE_SIZE}, {2.25 * SQUARE_SIZE, 0.75 * SQUARE_SIZE}, "250 MONEY");
    Button bouton_round = creer_bouton({13 * SQUARE_SIZE, 7.75 * SQUARE_SIZE}, {2.25 * SQUARE_SIZE, 0.75 * SQUARE_SIZE}, "START ROUND");

    // On crée henry, le singe de transition

    Singe henry = creer_singe({0.0});

    // création de la liste de stockage des singes
    Singe singes[100] = {};
    int nb_singes = 0;

    // création de la liste de stockage des fleches
    Fleche fleches[1000] = {};
    int nb_fleches = 0;

    bool pause = false; // jeu en pause ou non, avec la barre espace

    // Main game loop
    lignes_chemin(1, 4, 2, chemin, false);
    colonnes_chemin(3, 2, 4, chemin, true);
    lignes_chemin(4, 2, 5, chemin, false);
    colonnes_chemin(5, 3, 6, chemin, false);
    lignes_chemin(3, 6, 4, chemin, true);
    colonnes_chemin(3, 7, 8, chemin, false);
    lignes_chemin(4, 8, 12, chemin, false);
    colonnes_chemin(12, 5, 7, chemin, true);
    lignes_chemin(9, 5, 11, chemin, true);
    colonnes_chemin(9, 2, 4, chemin, true);
    lignes_chemin(10, 2, 11, chemin, false);
    colonnes_chemin(11, 1, 1, chemin, true);

    while (!WindowShouldClose())
    { // Detect window close button or ESC key
        // dessiner_bouton(bouton1);
        index++;
        compteur++;

        for (int s = 0; s < nb_singes; s++)
        {
            if (!singes[s].compteur <= 0)
            {
                singes[s].compteur -= 1;
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++)
        {
            DrawLineV({(float)(SQUARE_SIZE * i), 0.0}, {(float)(SQUARE_SIZE * i), (float)(screenHeight)}, LIGHTGRAY);
        }

        for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++)
        {
            DrawLineV({0.0, (float)(SQUARE_SIZE * i)}, {(float)(screenWidth), (float)(SQUARE_SIZE * i)}, LIGHTGRAY);
        }

        Rectangle rect_affichage{12.5 * SQUARE_SIZE, 0.25 * SQUARE_SIZE, 3.25 * SQUARE_SIZE, 8.5 * SQUARE_SIZE};
        DrawRectangle(12.5 * SQUARE_SIZE, 0.25 * SQUARE_SIZE, 3.25 * SQUARE_SIZE, 8.5 * SQUARE_SIZE, LIGHTGRAY);
        DrawText(TextFormat(" ROUND : %4i \n MONEY : %4i \n VIES : %4i", round, money, vies), 1250, 30, 40, MAGENTA);
        DrawTexture(texture, 13.25 * SQUARE_SIZE, 2.25 * SQUARE_SIZE, WHITE);
        dessiner_bouton(bouton1, 30);
        dessiner_bouton(bouton_round, 25);

        // DrawRectangle(0, 100, 300, 80, DARKGRAY);  // gauche, haut, longueur, largeur
        for (int i = 0; i < 36; i++)
        {
            DrawRectangleRec(chemin[i], DARKGRAY);
        }

        if (start_round)
        {
            if (compteur % 60 == 0 & ballons_cree < nb_ballons)
            {
                Ballon ballon2 = creer_ballon({50, 350}, 30);
                ballons[ballons_cree] = ballon2;
                ballons_cree += 1;
            }
        }


        // Verif si le bouton est cliqué
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (detect_click(bouton1) && money-250>=0)
            {
                henry = creer_singe(GetMousePosition());
                henry.etat = true;
            }

            else if (henry.etat == true)
            { 
                
                henry.position = GetMousePosition();
                henry.etat = false;
                if (check_collision_singe(henry, chemin) == false && CheckCollisionPointRec(GetMousePosition(), rect_affichage) == false )
                {
                    Singe singe1 = henry;
                    singe1.etat = true;
                    singes[nb_singes] = singe1;
                    nb_singes++;
                    money -= 250;
                }
            }

            if (detect_click(bouton_round))
            {   bool fin_round = true;
                for (Ballon ballon : ballons)
                {

                    if (ballon.etat == 1)
                    {
                        fin_round = false;
                    }
                }

                if (fin_round)
                {
                    start_round = true;
                    ballons_cree = 0;
                    round += 1;
                    nb_ballons += 5;
                    ballons_vivants= nb_ballons;
                }
            }
        }

        if (henry.etat == true)
        {
            henry.position = GetMousePosition();
            dessiner_singe(henry, textsinge);
        }

        // DrawRectangle(300, 400, 300, 80, DARKGRAY);
        // DrawRectangle(600, 180, 80, 300, DARKGRAY);
        // DrawRectangle(600, 100, 500, 80, DARKGRAY);
        // DrawRectangle(1100, 100, 80, 550, DARKGRAY);
        for (int i = 0; i < ballons_cree; i++)
        {
            dessiner_ballon(ballons[i]);
            if (IsKeyPressed(KEY_SPACE))
                pause = !pause;
            if (!pause)
            {
                Ballon B = ballons[i];
                Vector2 fin = {(SQUARE_SIZE * 10) + SQUARE_SIZE / 2, SQUARE_SIZE / 2};
                if (B.position.x == fin.x && B.position.y == fin.y && B.etat == 1)
                {
                    vies -= 1; // si un ballon arrive à la fin du chemin, on perd 1 vie
                }
                ballons[i] = mouv(ballons[i], chemin, nbr_rectangle, pause);
            }
        }

        for (int i = 0; i < nb_fleches; i++)
        {
            mouv_fleche(fleches[i]);
        }

        for (int s = 0; s < nb_singes; s++)
        {
            bool singe_tir = false;
            for (int i = 0; i < nb_ballons; i++)
            {
                // for (int j=0; j<4; j++){
                //     if (i==j){
                if (ballons[i].etat == 1)
                {
                    if ((check_coll_s_b(singes[s], ballons[i])) && singes[s].compteur == 0)
                    {
                        if (!singe_tir)
                        {
                            Fleche fleche1 = creer_fleche({singes[s].position.x + ((singes[s].taille.x) / 2), singes[s].position.y + ((singes[s].taille.y) / 2)}, ballons[i]);
                            singe_tir = true;
                            fleches[nb_fleches] = fleche1;
                            nb_fleches++;
                            singes[s].compteur += 120;
                        }
                    }

                    for (int j = 0; j < nb_fleches; j++)
                    {
                        if (fleches[j].cible == 0)
                        {
                            fleches[j] = check_coll_b_f(fleches[j], ballons[i]);
                        }
                        if (fleches[j].cible == 1)
                        {
                            ballons[i].etat = 0;
                            fleches[j].cible = 2;
                            fleches[j].etat = 0;
                            money += 1; // dès qu'on éclate un ballon, on gagne 1 Money
                            ballons_vivants -= 1;
                            if (ballons_vivants==0){
                                money+= 100-(round-1);
                            }
                        }
                        if (fleches[j].cible == 2)
                        {
                            fleches[j].etat = 0;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < nb_singes; i++)
        {
            dessiner_singe(singes[i], textsinge);
        }

        for (int i = 0; i < nb_fleches; i++)
        {
            dessiner_fleche(fleches[i]);
        }

        EndDrawing();
    }
    UnloadTexture(texture); // Unload texture from VRAM
    UnloadImage(im_singe);
    UnloadTexture(textsinge); // Unload texture from VRAM
    UnloadImage(im_singe2);

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
