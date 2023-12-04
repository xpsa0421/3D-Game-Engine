#include "Collision.h"

//----------------------------------------------------------------
//----------------------------------------------------------------

BoundingCircle::BoundingCircle()
{
    cx = cy = r = 0.0f;
}

BoundingCircle::BoundingCircle(float x, float y, float r)
{
    cx = x;
    cy = y;
    this->r = r;
}

void BoundingCircle::Set(float cx, float cy, float w, float h)
{
    this->cx = cx;
    this->cy = cy;
    float x = w / 2.0f;
    float y = h / 2.0f;
    this->r = sqrt(x * x + y * y);
}

void BoundingCircle::Set(Vector2 pos, Vector2 size)
{
    Set(pos.x, pos.y, size.x, size.y);
}

//----------------------------------------------------------------
//----------------------------------------------------------------

bool BoundingRect::operator == (BoundingRect& rect)
{
    if ((fabs(x1 - rect.x1) < EPSILON)
        && (fabs(y1 - rect.y1) < EPSILON)
        && (fabs(w - rect.w) < EPSILON)
        && (fabs(h - rect.h) < EPSILON))
    {
        return true;
    }
    return false;
}

BoundingRect::BoundingRect()
{
    x1 = x2 = y1 = y2 = w = h = cx = cy = 0.0f;
}

BoundingRect::BoundingRect(float x, float y, float w, float h)
{
    Set(x, y, w, h);
}

void BoundingRect::Set(float x, float y, float w, float h)
{
    x1 = x;
    x2 = x + w;
    y1 = y;
    y2 = y + h;
    this->w = w;
    this->h = h;
    cx = (x + x2) / 2.0f;
    cy = (y + y2) / 2.0f;
}

void BoundingRect::Set(Vector2 pos, Vector2 size)
{
    Set(pos.x, pos.y, size.x, size.y);
}

//----------------------------------------------------------------
//----------------------------------------------------------------

CollisionType Collision::RectToRect(BoundingRect& a, BoundingRect& b)
{
    float minX = a.x1 < b.x1 ? a.x1 : b.x1;
    float minY = a.y1 < b.y1 ? a.y1 : b.y1;
    float maxX = a.x2 > b.x2 ? a.x2 : b.x2;
    float maxY = a.y2 > b.y2 ? a.y2 : b.y2;

    if ((a.w + b.w) >= (maxX - minX))
    {
        if ((a.h + b.h) >= (maxY - minY))
        {
            float x, y, x2, y2;
            BoundingRect intersectRegion;
            x = a.x1 > b.x1 ? a.x1 : b.x1;
            y = a.y1 > b.y1 ? a.y1 : b.y1;
            x2 = a.x2 < b.x2 ? a.x2 : b.x2;
            y2 = a.y2 < b.y2 ? a.y2 : b.y2;
            intersectRegion.Set(x, y, x2 - x, y2 - y);
            if (intersectRegion == a || intersectRegion == b)
            {
                return CollisionType::RECT_IN;
            }
            return CollisionType::RECT_OVERLAP;
        }
    }
    return CollisionType::RECT_OUT;
}

bool Collision::RectToInRect(BoundingRect& a, BoundingRect& b)
{
    if ((a.x1 <= b.x1)
        && ((a.x1 + a.w) >= b.x1 + b.w)
        && (a.y1 <= b.y1)
        && ((a.y1 + a.h) >= b.y1 + b.h))
    {
        return true;
    }
    return false;
}

bool Collision::CircleToCircle(BoundingCircle& a, BoundingCircle& b)
{
    float x = a.cx - b.cx;
    float y = a.cy - b.cy;
    float distance = sqrt(x * x + y * y);
    if (distance <= (a.r + b.r))
    {
        return true;
    }
    return false;
}

bool  Collision::RectToPoint(BoundingRect& a, POINT& p)
{
    if (a.x1 <= p.x && a.x2 >= p.x)
    {
        if (a.y1 <= p.y && a.y2 >= p.y)
        {
            return true;
        }
    }
    return false;
}