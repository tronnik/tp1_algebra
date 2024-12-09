#include "raylib.h"
#include <cmath>
#include <vector>

using namespace std;

struct Point
{
    float x;
    float y;
};

bool LinesInterception(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, vector<Point>& intersections)
{
    float denominator = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

    if (denominator == 0) 
        return false;

    float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
    float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;

    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
    {
        float intersectionX = x1 + (uA * (x2 - x1));
        float intersectionY = y1 + (uA * (y2 - y1));

        intersections.push_back({ intersectionX, intersectionY });
        DrawCircle(intersectionX, intersectionY, 5, RED);
        return true;
    }

    return false;
}

float Distance(Point p1, Point p2)
{
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

float CalculateArea(Point p1, Point p2, Point p3, Point p4)
{
    return 0.5 * fabs(p1.x * (p2.y - p4.y) + p2.x * (p3.y - p1.y) + p3.x * (p4.y - p2.y) + p4.x * (p1.y - p3.y));
}

float CalculatePerimeter(Point p1, Point p2, Point p3, Point p4)
{
    return Distance(p1, p2) + Distance(p2, p3) + Distance(p3, p4) + Distance(p4, p1);
}

void main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Cuadrilatero - Area y Perimetro");

    vector<Point> points;
    vector<Point> intersections;

    Point start = { 0, 0 };
    Point end = { 0, 0 };

    bool rmbDown = false;
    bool lmbDown = false;

    bool isQuadrilateral = false;
    bool isValidQuadrilateral = true;

    float area = 0.0f;
    float perimeter = 0.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (points.size() < 8)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 aux = GetMousePosition();
                start.x = aux.x;
                start.y = aux.y;
                lmbDown = true;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                Vector2 aux = GetMousePosition();
                end.x = aux.x;
                end.y = aux.y;
                rmbDown = true;
            }

            if (!lmbDown && rmbDown)
            {
                start.x = GetMouseX();
                start.y = GetMouseY();
                DrawLine(end.x, end.y, start.x, start.y, BLACK);
            }
            else if (lmbDown && !rmbDown)
            {
                end.x = GetMouseX();
                end.y = GetMouseY();
                DrawLine(start.x, start.y, end.x, end.y, BLACK);
            }

            if (lmbDown && rmbDown)
            {
                points.push_back(start);
                points.push_back(end);

                DrawLine(start.x, start.y, end.x, end.y, BLACK);

                lmbDown = false;
                rmbDown = false;
            }
        }

        for (int i = 0; i < points.size(); i += 2)
        {
            DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, BLACK);
        }

        if (points.size() == 8 && !isQuadrilateral)
        {
            for (int i = 0; i < points.size(); i += 2)
            {
                for (int j = i + 2; j < points.size(); j += 2)
                {
                    LinesInterception(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y,
                        points[j].x, points[j].y, points[j + 1].x, points[j + 1].y, intersections);
                }
            }

            if (intersections.size() == 4)
            {
                Point p1 = intersections[0];
                Point p2 = intersections[1];
                Point p3 = intersections[2];
                Point p4 = intersections[3];

                isQuadrilateral = true;
                area = CalculateArea(p1, p2, p3, p4);
                perimeter = CalculatePerimeter(p1, p2, p3, p4);
            }
            else
            {
                isValidQuadrilateral = false;
            }
        }

        if (isQuadrilateral)
        {
            DrawText(TextFormat("Area: %.2f", area), 10, 10, 20, RED);
            DrawText(TextFormat("Perimetro: %.2f", perimeter), 10, 40, 20, RED);
        }
        else if (!isValidQuadrilateral)
        {
            DrawText("No es un cuadrilatero valido.", 10, 10, 20, RED);
            DrawText("No se pueden calcular el area ni el perimetro.", 10, 40, 20, RED);
        }
        else
        {
            DrawText("Haga click izq/der para definir los segmentos (4 lineas)", 10, 10, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
}
