/* -*- mode: c++ -*-
 *
 * pointing/nsystempointeracceleration.cc
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

#include "nsystempointeracceleration.h"

#ifdef __APPLE__
#include <pointing/transferfunctions/osx/osxSystemPointerAcceleration.h>
#elif defined(__linux__)
#include <pointing/transferfunctions/linux/xorgSystemPointerAcceleration.h>
#elif defined(_WIN32)
#include <pointing/transferfunctions/windows/winSystemPointerAcceleration.h>
#endif

using namespace pointing;

Napi::FunctionReference NSystemPointerAcceleration::constructor;

Napi::Value NSystemPointerAcceleration::get(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  Napi::Object result = Napi::Object::New(env);
#ifdef __APPLE__
  double res = 0;
  osxSystemPointerAcceleration acc;
  Napi::Value tarStr = Napi::Value::From(env, "mouse");
  if (info.Length())
  {
    Napi::Object argsObj = info[0].As<Napi::Object>();
    if (argsObj.Has("target"))
      tarStr = argsObj.Get("target");
    std::string target(tarStr.ToString());
    res = acc.get(target.c_str());
  }
  else
    res = acc.get();

  result.Set("value", Napi::Number::New(env, res));
  result.Set("target", tarStr);

#elif defined __linux__
	xorgSystemPointerAcceleration acc;
	int num = 2, den = 1, thr = 4;
	acc.get(&num, &den, &thr);
  result.Set("numerator", Napi::Number::New(env, num));
  result.Set("denominator", Napi::Number::New(env, den));
  result.Set("threshold", Napi::Number::New(env, thr));
#elif defined(_WIN32)
  winSystemPointerAcceleration acc;
  std::string version;
  int sliderPosition;
  bool epp;
  acc.get(&version, &sliderPosition, &epp);
  result.Set("version", Napi::String::New(env, version));
  result.Set("sliderPosition", Napi::Number::New(env, sliderPosition));
  result.Set("enhancePointerPrecision", Napi::Number::New(env, epp));
#endif
  return result;
}

void NSystemPointerAcceleration::set(const Napi::CallbackInfo& info){
  Napi::Object argsObj = info[0].As<Napi::Object>();

#ifdef __APPLE__
  if (argsObj.Has("value"))
  {
    Napi::Env env = info.Env();
    double value = argsObj.Get("value").As<Napi::Number>().DoubleValue();
    Napi::Value tarStr = Napi::Value::From(env, "mouse");

    Napi::Object argsObj = info[0].As<Napi::Object>();
    if (argsObj.Has("target"))
      tarStr = argsObj.Get("target");
    std::string target(tarStr.ToString());
    osxSystemPointerAcceleration acc;
    acc.set(value, target.c_str());
  }
#elif defined(__linux__)
  int num = argsObj.Get("numerator").As<Napi::Number>().Uint32Value();
  int den = argsObj.Get("denominator").As<Napi::Number>().Uint32Value();
  int thr = argsObj.Get("threshold").As<Napi::Number>().Uint32Value();

  num = num ? num : 2;
  den = den ? den : 1;
  thr = thr ? thr : 4;

	xorgSystemPointerAcceleration acc;
	acc.set(num, den, thr);
#elif defined(_WIN32)
  int sliderPosition = argsObj.Get("sliderPosition").As<Napi::Number>().Uint32Value();
  bool epp = argsObj.Get("enhancePointerPrecision").As<Napi::Boolean>().Value();

  winSystemPointerAcceleration acc;
  acc.set(sliderPosition, epp);
#endif
}

NSystemPointerAcceleration::NSystemPointerAcceleration(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NSystemPointerAcceleration>(info){
  Napi::HandleScope scope(info.Env());
}

Napi::Object NSystemPointerAcceleration::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
  Napi::Function func = DefineClass(env, "SystemPointerAcceleration", {
    InstanceMethod("set", &NSystemPointerAcceleration::set),
    InstanceMethod("get", &NSystemPointerAcceleration::get)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("SystemPointerAcceleration", func);

  return exports;
}

Napi::Object NSystemPointerAcceleration::NewInstance(Napi::Env env){
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({});
  return scope.Escape(napi_value(obj)).ToObject();
}
