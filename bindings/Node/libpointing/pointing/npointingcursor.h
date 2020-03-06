/* -*- mode: c++ -*-
 *
 * pointing/npointingcursor.h
 *
 * Initial software
 * Authors: Izzatbek Mukhanov, Etienne Orieux
 * Copyright © Inria
 *
 * http://libpointing.org/
 *
 * This software may be used and distributed according to the terms of
 * the GNU General Public License version 2 or any later version.
 *
 */

#ifndef NPOINTINGCURSOR_H
#define NPOINTINGCURSOR_H

#include <napi.h>
#include <pointing/utils/PointingCursor.h>

namespace NPointingCursor
{
  Napi::Value getPosition(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    Napi::Object pos = Napi::Object::New(env);
    double x, y;
    pointing::PointingCursor::getPosition(&x, &y);
    pos.Set("x", Napi::Number::New(env, x));
    pos.Set("y", Napi::Number::New(env, y));
    return pos;
  }

  void setPosition(const Napi::CallbackInfo& info){
    double x = info[0].As<Napi::Number>().DoubleValue();
    double y = info[1].As<Napi::Number>().DoubleValue();
    pointing::PointingCursor::setPosition(x, y);
  }

  Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Object obj = Napi::Object::New(env);

    obj.Set("getPosition", Napi::Function::New(env, getPosition));
    obj.Set("setPosition", Napi::Function::New(env, setPosition));

    exports.Set("pointingCursor", obj);
    return exports;
  }
}
#endif // NPOINTINGCURSOR_H
