#include "raylib.h"
#include <iostream>
#include "menu.cpp"
#include "ballon.cpp"
#include <string.h>
#include "singe.cpp"
#include "bouton.cpp"
#include "interface.cpp"
#include "constantes.h"


int main(void)
{
    // Initialisation fenetre
    InitWindow(screenWidth, screenHeight, " classic game: missile commander");
    Image im_singe = LoadImage("C:/Users/maeva/OneDrive/Images/singe3.png");
    Image im_singe2 = LoadImage("C:/Users/maeva/OneDrive/Images/singe3.png");
    ImageResize(&im_singe, 150,150);
    ImageResize(&im_singe2, 100, 100);
    Texture2D texture = LoadTextureFromImage(im_singe);
    Texture2D textsinge = LoadTextureFromImage(im_singe2);
    

    SetTargetFPS(60);

    menu();
    //--------------------------------------------------------------------------------------
    int index = 0;
    int money = MONEY_DEPART;

    // liste chemin
    Rectangle chemin[36] = {};

    Button bouton1 = creer_bouton({13.15* SQUARE_SIZE, 3.8 * SQUARE_SIZE}, {2 * SQUARE_SIZE, 0.75*SQUARE_SIZE}, "20 MONEY");

    // On crée henry, le singe de transition

    Singe henry = creer_singe({0.0});


    // création de la liste de stockage des singes
    Singe singes[100] = {};
    int nb_singes = 0;

    bool pause= false; // jeu en pause ou non, avec la barre espace
    
    Ballon ballon1= creer_ballon({50,350}, 30);

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

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // dessiner_bouton(bouton1);
        index++;
        std::cout << '\r' << index;

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
        DrawText(TextFormat(" ROUND : %4i \n MONEY : %4i \n VIES : %4i", index, money, -index), 1250, 30, 40, MAGENTA);
        DrawTexture(texture, 13.25 * SQUARE_SIZE, 2.25* SQUARE_SIZE, WHITE);
        dessiner_bouton(bouton1);

        // DrawRectangle(0, 100, 300, 80, DARKGRAY);  // gauche, haut, longueur, largeur
        for (int i = 0; i < 36; i++)
        {
            DrawRectangleRec(chemin[i], DARKGRAY);
        }

        if (IsKeyPressed(KEY_SPACE)) pause = !pause; 
        if (!pause){
            ballon1= mouv(ballon1,chemin,nbr_rectangle,pause);
        }
        // Verif si le bouton est cliqué
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {

            if (detect_click(bouton1))
            {
                henry = creer_singe(GetMousePosition());
                henry.etat = true;
            }

            else if (henry.etat == true)
            {   
                henry.position = GetMousePosition();
                henry.etat = false;
                Singe singe1 = henry;
                if (check_collision_singe(singe1, chemin) == false & CheckCollisionPointRec(GetMousePosition(), rect_affichage) ==false & money!=0)
                {
                    singe1.etat = true;
                    singes[nb_singes] = singe1;
                    nb_singes++;
                    money -= 20;
                }
            }
        }

        if (henry.etat == true)
        {
            henry.position = GetMousePosition();
            dessiner_singe(henry, textsinge);
        }

        for (int i = 0; i < nb_singes; i++)
        {
            dessiner_singe(singes[i], textsinge);
        }

        // DrawRectangle(300, 400, 300, 80, DARKGRAY);
        // DrawRectangle(600, 180, 80, 300, DARKGRAY);
        // DrawRectangle(600, 100, 500, 80, DARKGRAY);
        // DrawRectangle(1100, 100, 80, 550, DARKGRAY);
        dessiner_ballon(ballon1);
        EndDrawing();
    }
    UnloadTexture(texture); // Unload texture from VRAM
    UnloadImage(im_singe);
    UnloadTexture(textsinge); // Unload texture from VRAM
    UnloadImage(im_singe2);
    
    
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
