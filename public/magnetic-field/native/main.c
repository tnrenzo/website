#include "raylib.h"
#include "rlgl.h"
#include <math.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

typedef struct {
    /* all parameters are taken from wikipedia
     */

    /* general parameters */
    float equatorial_radius; // units of 10000km
    float polar_radius;
    float oblateness;
    Vector3 orbital_position;

    /* rotation/spin */
    float rotation_period; // seconds
    float rotation_phase;
    Vector3 rotation_angle;

    /* magnetic field */
    float dipole_tilt;
    Vector3 magnetic_axis;
} Planet;

static Camera camera = {
    .position = {10.0f, 4.0f, 5.0f},
    .target = {0.0f, 0.0f, 0.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE,
};

static Planet neptune = {.equatorial_radius = 2.4764,
                         .polar_radius = 2.4341,
                         .oblateness = 0.01708,
                         .rotation_period = 57479,
                         .rotation_phase = 0.0f,
                         .rotation_angle = {28.32f, 0, 0},
                         .dipole_tilt = 47.00,
                         .magnetic_axis = {0, -0.55f, 0}};

static float sim_time_scale = 3600;

void DrawPlanet(Planet *p) {
    float axislen =
        p->equatorial_radius * 1.6f; // for lines to stick out the planet
    rlPushMatrix();
    rlTranslatef(p->orbital_position.x, p->orbital_position.y,
                 p->orbital_position.z);
    rlRotatef(p->rotation_angle.x, 1.0f, 0.0f,
              0.0f); // fixed axial tilt for now

    /* geographic poles */
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, axislen, 0}, GREEN);
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, -axislen, 0}, SKYBLUE);

    rlRotatef(p->rotation_phase, 0.0f, 1.0f, 0.0f);

    // draw the planet
    DrawSphereWires((Vector3){0, 0, 0}, p->equatorial_radius, 32, 32, DARKBLUE);

    rlPushMatrix();
    // calculate position of the dipole-dipole line through the correct offset
    // from the planet core
    rlTranslatef(p->magnetic_axis.x * p->equatorial_radius,
                 p->magnetic_axis.y * p->equatorial_radius,
                 p->magnetic_axis.z * p->equatorial_radius);
    rlRotatef(p->dipole_tilt, 0.0f, 0.0f, 1.0f);

    DrawLine3D((Vector3){0, -axislen, 0}, (Vector3){0, axislen, 0}, RED);
    rlPopMatrix();
    rlPopMatrix();

    /* line along orbit */
    DrawLine3D((Vector3){-2.0f * p->equatorial_radius, 0, 0},
               (Vector3){2.0f * p->equatorial_radius, 0, 0}, WHITE);
    DrawCylinderEx((Vector3){2.0f * p->equatorial_radius, 0, 0},
                   (Vector3){2.5f * p->equatorial_radius, 0, 0}, 0.12f, 0.0f,
                   12, WHITE); // orbit direction
}

static void frame(void) {
    float ft = GetFrameTime();
    neptune.rotation_phase +=
        360 * ft * sim_time_scale / neptune.rotation_period;
    neptune.rotation_phase =
        fmodf(neptune.rotation_phase,
              360.0f); // keep rotation between 0 and 360 degrees

    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(camera);

    DrawPlanet(&neptune);

    EndMode3D();
    EndDrawing();
}

int main(void) {
    InitWindow(800, 600, "magnetic field");

#ifdef PLATFORM_WEB
    // the browser drives the loop instead
    emscripten_set_main_loop(frame, 0, 1);
#else
    while (!WindowShouldClose())
        frame();
#endif

    CloseWindow();
    return 0;
}
