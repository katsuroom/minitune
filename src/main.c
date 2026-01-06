#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "program.h"
#include "titlebar.h"
#include "seeker.h"
#include <string.h>
#include <time.h>

int screenWidth = 320;
int screenHeight = CONTROLS_HEIGHT+TITLEBAR_HEIGHT+ART_SZ;
bool isRunning = true;

Vector2 mousePos = {0};
Vector2 mouseOffset = {0};
bool isDragging = false;

unsigned int flags = FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_ALWAYS_RUN | FLAG_MSAA_4X_HINT;

char* preloaded_file = NULL;

void parse_args(int argc, char** argv) {
    // start at 1 to skip program name
    for(int i = 1; i < argc; ++i) {
        if(argv[i][0] == '-') {
            if(strcmp(argv[i], "-full") == 0) {
                artDisplayMode = ART_DISPLAY_FULL;
            }
            else if(strcmp(argv[i], "-top") == 0) {
                // enable always-on-top
                flags |= FLAG_WINDOW_TOPMOST;
            }
        }
        else if(preloaded_file == NULL) {
            preloaded_file = argv[i];
        }
    }

    if(artDisplayMode == ART_DISPLAY_FULL) {
        screenHeight = CONTROLS_HEIGHT+TITLEBAR_HEIGHT+screenWidth;
    }
}

int main(int argc, char** argv) {

    parse_args(argc, argv);

    SetConfigFlags(flags);
    InitWindow(screenWidth, screenHeight, "minitune");
    GuiLoadStyleDefault();

    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    RenderTexture2D visTarget = LoadRenderTexture(screenWidth, screenHeight-CONTROLS_HEIGHT-TITLEBAR_HEIGHT);

    srand(time(NULL));
    init();

    if(preloaded_file != NULL)
        load_file(preloaded_file);

    while(!WindowShouldClose() && isRunning == true) {

        // drag window
        mousePos = GetMousePosition();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            CheckCollisionPointRec(mousePos, (Rectangle){0, 0, screenWidth, screenHeight-CONTROLS_HEIGHT})) {
            mouseOffset = mousePos;
            isDragging = true;
        }

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isDragging = false;
        }

        if(isDragging == true) {
            int dx = mousePos.x - mouseOffset.x;
            int dy = mousePos.y - mouseOffset.y;

            Vector2 wp = GetWindowPosition();
            SetWindowPosition(wp.x + dx, wp.y + dy);
        }

        update();

        BeginTextureMode(visTarget);
        ClearBackground(BLANK);
        draw_visualization();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLANK);
        // ClearBackground(BLACK);
        draw();
        DrawTextureRec(visTarget.texture,
            (Rectangle){0, 0, visTarget.texture.width, -visTarget.texture.height},
            (Vector2){0, 0}, WHITE
        );
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}