/*
    This file is part of Sorts, an interface between Soar and ORTS.
    (c) 2006 James Irizarry, Sam Wintermute, and Joseph Xu

    Sorts is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Sorts is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Sorts; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA    
*/
#ifndef SDLCanvasCompound_h
#define SDLCanvasCompound_h

#include <list>
#include <utility>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "SDLCanvasShape.h"

typedef pair<SDLCanvasShape*, SDLCanvasPoint> elementType;

class SDLCanvasCompound : public SDLCanvasShape {
public:
  SDLCanvasCompound
    ( SDL_Surface* surface,
      SDLCanvasSpace* space,
      double cx,
      double cy )
    : SDLCanvasShape(surface, space, cx, cy, "") 
  { }

  void draw() const {
    for(list<elementType>::const_iterator
        i  = shapes.begin();
        i != shapes.end();
        ++i)
    {
      i->first->draw();
    }
  }

  void addShape(SDLCanvasShape* s) {
    assert(s->getSpace() == getSpace());
    SDLCanvasPoint absCenter = getSpace()->toCS(s->getCenter());

    SDLCanvasPoint offset(
        absCenter.x - getCenterX(),
        absCenter.y - getCenterY());
    shapes.push_back(pair<SDLCanvasShape*, SDLCanvasPoint>(s, offset));
  }

  void removeShape(SDLCanvasShape* s) {
    for(list<elementType>::iterator
        i  = shapes.begin();
        i != shapes.end();
        ++i)
    {
      if (i->first == s) {
        shapes.erase(i);
        return;
      }
    }
  }

  void moveTo(double x, double y) {
    for(list<elementType>::iterator
        i  = shapes.begin();
        i != shapes.end();
        ++i)
    {
      i->first->moveTo(x + i->second.x, y + i->second.y);
    }
    SDLCanvasShape::moveTo(x, y);
  }

  void translate(double dx, double dy) {
    for(list<elementType>::iterator
        i  = shapes.begin();
        i != shapes.end();
        ++i)
    {
      i->first->translate(dx, dy);
    }
    SDLCanvasShape::translate(dx, dy);
  }

  void getElements(list<SDLCanvasShape*>& elements) {
    elements.clear();
    for(list<elementType>::iterator
        i  = shapes.begin();
        i != shapes.end();
        ++i)
    {
      elements.push_back(i->first);
    }
  }

  void clear() {
    shapes.clear();
  }

private:
  list<elementType> shapes;
};

#endif
