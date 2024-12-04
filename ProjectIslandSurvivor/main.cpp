#include "raylib.h"
#include "raymath.h"
#include "character.h"
#include "prop.h"
#include "enemy.h"
#include <string>

int main()
{
    // Window dimension
    int windowDimensions[2];
    windowDimensions[0] = 384;
    windowDimensions[1] = 384;

    InitWindow(windowDimensions[0], windowDimensions[1], "Island Survivor");

    Texture2D worldMap = LoadTexture("map/WorldMap.png");
    Vector2 worldMapPos{0.0, 0.0};
    const float mapScale{4.f};

    Character knight{windowDimensions[0], windowDimensions[1]};

    Prop props[2]{
        Prop{Vector2{600.f,300.f},LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f,500.f},LoadTexture("nature_tileset/Log.png")}
    };

    Enemy goblin{Vector2{200.f, 200.f}, LoadTexture("characters/goblin_idle_spritesheet.png"), LoadTexture("characters/goblin_run_spritesheet.png")};
    Enemy slime{Vector2{500.f, 700.f}, LoadTexture("characters/slime_idle_spritesheet.png"), LoadTexture("characters/slime_run_spritesheet.png")};

    Enemy* enemies[]{&goblin, &slime};

    for (auto enemy : enemies)
    {
       enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        worldMapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // DrawMap
        DrawTextureEx(worldMap, worldMapPos, 0.0, mapScale, WHITE);

        //DrawProps
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        // Character
        knight.tick(GetFrameTime());

        //Check Map Bounds
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowDimensions[0] > worldMap.width * mapScale ||
            knight.getWorldPos().y + windowDimensions[1] > worldMap.height * mapScale)
        {
            knight.undoMovement();
        }

        for (auto prop : props)
        {            
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.GetCollisionRec()))
            {
                knight.undoMovement();
            } 
        }    

        if (!knight.getAlive())
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            std::string knightsHealth{"Health: "};
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightsHealth.c_str(), 55.f,45.f, 40, GREEN);
        }
        

        //Enemy
        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }
        

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->GetCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
            

        }

        // Stop Drawing
        EndDrawing();
    }

    UnloadTexture(worldMap);
    CloseWindow();
}