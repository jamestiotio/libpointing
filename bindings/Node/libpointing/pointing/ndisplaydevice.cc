/* -*- mode: c++ -*-
 *
 * pointing/ndisplaydevice.cc
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

#include "ndisplaydevice.h"

bool custom_isnan(double var)
{
    volatile double d = var;
    return d != d;
}

using namespace pointing;

Napi::FunctionReference NDisplayDevice::constructor;

NDisplayDevice::~NDisplayDevice()
{
  // std::cerr << "~NDisplayDevice" << std::endl;
  delete output;
}

Napi::Object NDisplayDevice::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "DisplayDevice", {});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("DisplayDevice", func);

  return exports;
}

NDisplayDevice::NDisplayDevice(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NDisplayDevice>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return;
  }
  std::string uri = info[0].ToString().Utf8Value();
  this->output = DisplayDevice::create(uri);

  addProperty<NDisplayDevice>(info, "size", &NDisplayDevice::getSize);
  addProperty<NDisplayDevice>(info, "bounds", &NDisplayDevice::getBounds);
  addProperty<NDisplayDevice>(info, "resolution", &NDisplayDevice::getResolution);
  addProperty<NDisplayDevice>(info, "refreshRate", &NDisplayDevice::getRefreshRate);
  addProperty<NDisplayDevice>(info, "uri", &NDisplayDevice::getURI);
}

Napi::Object NDisplayDevice::NewInstance(Napi::Env env, Napi::Value uri){
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({uri});
  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value NDisplayDevice::getSize(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();

  DisplayDevice::Size size = this->output->getSize();
  Napi::Object sizeObj = Napi::Object::New(env);
  sizeObj.Set("width", size.width);
  sizeObj.Set("height", size.height);

  return sizeObj;
}

Napi::Value NDisplayDevice::getBounds(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();

  DisplayDevice::Bounds bounds = this->output->getBounds();
  Napi::Object boundsObj = Napi::Object::New(env);

  Napi::Object sizeObj = Napi::Object::New(env);
  sizeObj.Set("width", bounds.size.width);
  sizeObj.Set("height", bounds.size.height);
  boundsObj.Set("size", sizeObj);

  Napi::Object originObj = Napi::Object::New(env);
  originObj.Set("x", bounds.origin.x);
  originObj.Set("y", bounds.origin.y);
  boundsObj.Set("origin", originObj);

  return boundsObj;
}

Napi::Value NDisplayDevice::getResolution(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();

  double hppi = 0, vppi = 0;
  this->output->getResolution(&hppi, &vppi);
  if (custom_isnan(hppi) || custom_isnan(vppi))
  {
	  hppi = 0;
	  vppi = 0;
  }
  Napi::Object resObj = Napi::Object::New(env);
  resObj.Set("hppi", hppi);
  resObj.Set("vppi", vppi);

  return resObj;
}

Napi::Value NDisplayDevice::getRefreshRate(const Napi::CallbackInfo& info){
  return Napi::Number::New(info.Env(), this->output->getRefreshRate());
}

Napi::Value NDisplayDevice::getURI(const Napi::CallbackInfo& info){
  return Napi::String::New(info.Env(), this->output->getURI().asString());
}
