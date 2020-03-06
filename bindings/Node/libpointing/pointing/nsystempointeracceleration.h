/* -*- mode: c++ -*-
 *
 * pointing/nsystempointeracceleration.h
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

#ifndef NSYSTEMPOINTERACCELERATION_H
#define NSYSTEMPOINTERACCELERATION_H

#include <napi.h>
#include <pointing/pointing.h>

class NSystemPointerAcceleration : public Napi::ObjectWrap<NSystemPointerAcceleration>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  NSystemPointerAcceleration(const Napi::CallbackInfo& info);
  static Napi::Object NewInstance(Napi::Env env);

private:
  static Napi::FunctionReference constructor;

  Napi::Value get(const Napi::CallbackInfo& info);
  void set(const Napi::CallbackInfo& info);
};

#endif // NSYSTEMPOINTERACCELERATION_H
