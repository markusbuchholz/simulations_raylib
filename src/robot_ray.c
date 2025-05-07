#include "raylib.h"
#include <math.h>

// number of rays and fan span in degrees
#define NUM_RAYS      12
#define SPAN_DEGREES  90.0f

static float Dot(Vector2 a, Vector2 b) {
    return a.x*b.x + a.y*b.y;
}

static Vector2 Normalize(Vector2 v) {
    float len = sqrtf(v.x*v.x + v.y*v.y);
    if (len == 0) return (Vector2){ 0, 0 };
    return (Vector2){ v.x / len, v.y / len };
}

// reflect direction d around normal n
static Vector2 Reflect(Vector2 d, Vector2 n) {
    float dn = Dot(d, n);
    return (Vector2){ d.x - 2*dn*n.x,
                      d.y - 2*dn*n.y };
}

// ray-segment intersection. Returns true if hit, outputs ray t and segment u
static bool RayLineIntersection(Vector2 start, Vector2 dir,
                                Vector2 p1, Vector2 p2,
                                float *outT, float *outU)
{
    Vector2 v = { p2.x - p1.x, p2.y - p1.y };
    float denom = dir.x * v.y - dir.y * v.x;
    if (fabsf(denom) < 1e-6f) return false;
    Vector2 w = { p1.x - start.x, p1.y - start.y };
    float t = (w.x * v.y - w.y * v.x) / denom;
    float u = (w.x * dir.y - w.y * dir.x) / denom;
    if (t > 0 && u >= 0 && u <= 1) {
        *outT = t;
        *outU = u;
        return true;
    }
    return false;
}

static float Clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

int main(void)
{
    const int screenW = 800;
    const int screenH = 600;
    InitWindow(screenW, screenH, "Lift + Multi-Ray Reflection");
    SetTargetFPS(60);

    // robot
    Rectangle box = { 100, screenH/2 - 25, 50, 50 };
    const float boxSpeed = 300.0f;

    // mirror line endpoints
    Vector2 mA = { 600, 150 };
    Vector2 mB = { 700, 300 };
    
    Vector2 mirrorDir = { mB.x - mA.x, mB.y - mA.y };
    Vector2 mirrorNorm = Normalize((Vector2){ mirrorDir.y, -mirrorDir.x });

    while (!WindowShouldClose())
    {
        // move robot up/down
        if (IsKeyDown(KEY_UP))    box.y -= boxSpeed * GetFrameTime();
        if (IsKeyDown(KEY_DOWN))  box.y += boxSpeed * GetFrameTime();
        // keep inside window
        box.y = Clampf(box.y, 0.0f, (float)(screenH - box.height));

        BeginDrawing();
          ClearBackground(BLACK);

          DrawRectangleRec(box, YELLOW);

          Vector2 origin = { box.x + box.width, box.y + box.height/2 };
          float halfSpan = SPAN_DEGREES * 0.5f * (PI/180.0f);

          for (int i = 0; i < NUM_RAYS; i++)
          {
              float angle = (NUM_RAYS == 1)
                          ? 0.0f
                          : -halfSpan + (SPAN_DEGREES*(PI/180.0f)) * i/(NUM_RAYS - 1);
              Vector2 dir = Normalize((Vector2){ cosf(angle), sinf(angle) });

              float rayT, segU;
              bool hit = RayLineIntersection(origin, dir, mA, mB, &rayT, &segU);

              if (hit)
              {
            
                  Vector2 hitPt = { origin.x + dir.x * rayT,
                                    origin.y + dir.y * rayT };
                  Vector2 refl = Reflect(dir, mirrorNorm);

                  // Draw incoming and reflected rays
                  DrawLineV(origin, hitPt, GREEN);
                  DrawLineV(hitPt,
                            (Vector2){ hitPt.x + refl.x * 300.0f,
                                       hitPt.y + refl.y * 300.0f },
                            GREEN);
                            
                  DrawCircleV(hitPt, 3, GREEN);
              }
              else
              {
                 
                  DrawLineV(origin,
                            (Vector2){ origin.x + dir.x * 800.0f,
                                       origin.y + dir.y * 800.0f },
                            GREEN);
              }
          }

          DrawLineEx(mA, mB, 4.0f, LIGHTGRAY);

          DrawText("Use Up/Down to move the lift; rays reflect off the mirror", 10, 10, 18, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
