#pragma once
#include <Urho3D/Resource/Image.h>

namespace Colonization
{
namespace ImageUtils
{
void DrawLine (Urho3D::Image *image, Urho3D::Color color, int x1, int y1, int x2, int y2, int width);
void DrawCircle (Urho3D::Image *image, Urho3D::Color color, int x, int y, int radius, bool fill);
void FloodFill (Urho3D::Image *image, Urho3D::Color color, int x, int y);

void LineFloodFill (Urho3D::Image *image, unsigned fillColor, unsigned seedColor, int x, int y);
void LineFloodFillDrawLine (Urho3D::Image *image, unsigned fillColor, unsigned seedColor, int x, int y, int direction);
void LineFloodFillScanForLinesStep (Urho3D::Image *image, unsigned fillColor, unsigned seedColor,
                                int x, int y, int xDirection, int yDirection);
}
}
