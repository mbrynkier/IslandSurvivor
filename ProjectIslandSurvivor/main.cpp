#include "raylib.h"
#include "raymath.h"
#include "character.h"
#include "prop.h"
#include "enemy.h"

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

    Enemy goblin{Vector2{}, LoadTexture("characters/goblin_idle_spritesheet.png"), LoadTexture("characters/goblin_run_spritesheet.png")};

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

        //Enemy
        goblin.tick(GetFrameTime());

        // Stop Drawing
        EndDrawing();
    }

    UnloadTexture(worldMap);
    CloseWindow();
}