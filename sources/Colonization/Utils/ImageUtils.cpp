#include <Colonization/BuildConfiguration.hpp>
#include "ImageUtils.hpp"
#include <cmath>

namespace Colonization
{
namespace ImageUtils
{
void DrawLine (Urho3D::Image *image, Urho3D::Color color, int x1, int y1, int x2, int y2)
{
    float xDiff = x2 - x1;
    float yDiff = y2 - y1;

    if (xDiff == 0.0f && yDiff == 0.0f)
    {
        image->SetPixel (x1, y1, color);
        return;
    }

    if (Urho3D::Abs (xDiff) > Urho3D::Abs (yDiff))
    {
        float xMax;
        float xMin;

        if (x1 > x2)
        {
            xMax = x1;
            xMin = x2;
        }
        else
        {
            xMax = x2;
            xMin = x1;
        }

        float tangent = yDiff / xDiff;
        for (float x = xMin; x <= xMax; x += 1.0f)
        {
            float y = y1 + (x - x1) * tangent;
            image->SetPixel (x, y, color);
        }
    }
    else
    {
        float yMax;
        float yMin;

        if (y1 > y2)
        {
            yMax = y1;
            yMin = y2;
        }
        else
        {
            yMax = y2;
            yMin = y1;
        }

        float tangent = xDiff / yDiff;
        for (float y = yMin; y <= yMax; y += 1.0f)
        {
            float x = x1 + (y - y1) * tangent;
            image->SetPixel (x, y, color);
        }
    }
}

void DrawCircle (Urho3D::Image *image, Urho3D::Color color, int x, int y, int radius, bool fill)
{
    int drawX = radius;
    int drawY = 0;
    int error = 0;

    while (drawX >= drawY)
    {
        if (fill)
        {
            DrawLine (image, color, x - drawX, y + drawY, x + drawX, y + drawY);
            DrawLine (image, color, x - drawY, y + drawX, x + drawY, y + drawX);
            DrawLine (image, color, x - drawY, y - drawX, x + drawY, y - drawX);
            DrawLine (image, color, x - drawX, y - drawY, x + drawX, y - drawY);
        }
        else
        {
            image->SetPixel (x + drawX, y + drawY, color);
            image->SetPixel (x + drawY, y + drawX, color);
            image->SetPixel (x - drawY, y + drawX, color);
            image->SetPixel (x - drawX, y + drawY, color);
            image->SetPixel (x - drawX, y - drawY, color);
            image->SetPixel (x - drawY, y - drawX, color);
            image->SetPixel (x + drawY, y - drawX, color);
            image->SetPixel (x + drawX, y - drawY, color);
        }

        if (error <= 0)
        {
            drawY += 1;
            error += 2 * drawY + 1;
        }

        if (error > 0)
        {
            drawX -= 1;
            error -= 2 * drawX + 1;
        }
    }
}

void FloodFill (Urho3D::Image *image, Urho3D::Color color, int x, int y)
{
    Urho3D::Color sourceColor = image->GetPixel (x, y);
    image->SetPixel (x, y, color);

    for (int pointX = x - 1; pointX <= x + 1; pointX++)
    {
        for (int pointY = y - 1; pointY <= y + 1; pointY++)
        {
            if (pointX > 0 && pointX < image->GetWidth () &&
                    pointY > 0 && pointY < image->GetWidth () &&
                    image->GetPixel (pointX, pointY) == sourceColor)
            {
                FloodFill (image, color, pointX, pointY);
            }
        }
    }
}
}
}

