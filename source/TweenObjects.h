/*
    Uxplore
    Copyright (C) 2019-2021, Kuruyia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TWEENABLEOBJECTS_H
#define TWEENABLEOBJECTS_H

#include <TweenEngine/Tweenable.h>
#include "GUI/Drawable.h"

template<class T>
class TweenTransformable : public T, public TweenEngine::Tweenable {
public:
    static const int POSITION_X = 1;
    static const int POSITION_Y = 2;
    static const int POSITION_XY = 3;
    static const int SCALE = 4;

protected:
    virtual int getValues(int tweenType, float *returnValues)
    {
        switch (tweenType) {
            case POSITION_X:
                returnValues[0] = T::getRelativePosition().x;
                return 1;
            case POSITION_Y:
                returnValues[0] = T::getRelativePosition().y;
                return 1;
            case POSITION_XY:
                returnValues[0] = T::getRelativePosition().x;
                returnValues[1] = T::getRelativePosition().y;
                return 2;
            case SCALE:
                returnValues[0] = T::getScale();
                return 1;
            default:
                return -1;
        }
    }

    virtual void setValues(int tweenType, float *newValues)
    {
        switch (tweenType) {
            case POSITION_X:
                T::setRelativePosition(newValues[0], T::getRelativePosition().y);
                break;
            case POSITION_Y:
                T::setRelativePosition(T::getRelativePosition().x, newValues[0]);
                break;
            case POSITION_XY:
                T::setRelativePosition(newValues[0], newValues[1]);
                break;
            case SCALE:
                T::setScale(newValues[0]);
                break;
            default:
                break;
        }
    }
};

typedef TweenTransformable<Drawable> TweenDrawable;

#endif // TWEENABLEOBJECTS_H
