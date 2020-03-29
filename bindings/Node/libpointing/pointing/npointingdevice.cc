/* -*- mode: c++ -*-
 *
 * pointing/npointingdevice.cc
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

#include "npointingdevice.h"

using namespace pointing;

Napi::FunctionReference NPointingDevice::constructor;

void NPointingDevice::pointingCallback(void *context, TimeStamp::inttime timestamp, int dx, int dy, int buttons){
  NPointingDevice *self = (NPointingDevice *)context;

  PointingReport *pr = new PointingReport();
  pr->timestamp = timestamp;
  pr->dx = dx;
  pr->dy = dy;
  pr->buttons = buttons;

  auto callback = []( Napi::Env env, Napi::Function jsCallback, PointingReport* pr) {
    Napi::Number nTimestamp = Napi::Number::New(env, pr->timestamp);
    Napi::Number nDx = Napi::Number::New(env, pr->dx);
    Napi::Number nDy = Napi::Number::New(env, pr->dy);
    Napi::Number nButtons = Napi::Number::New(env, pr->buttons);

    jsCallback.Call({nTimestamp, nDx, nDy, nButtons});
  };

  self->tsfn.NonBlockingCall(pr, callback);
}

NPointingDevice::~NPointingDevice() {
  // std::cerr << "~NPointingDevice" << std::endl;
  delete input;
}

Napi::Object NPointingDevice::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "PointingDevice", {
    InstanceMethod("setPointingCallback", &NPointingDevice::setPointingCallback)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("PointingDevice", func);

  return exports;
}

NPointingDevice::NPointingDevice(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NPointingDevice>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return;
  }
  std::string uri = info[0].ToString().Utf8Value();
  this->input = PointingDevice::create(uri);

  addProperty<NPointingDevice>(info, "vendorID", &NPointingDevice::getVendorID);
  addProperty<NPointingDevice>(info, "productID", &NPointingDevice::getProductID);
  addProperty<NPointingDevice>(info, "vendor", &NPointingDevice::getVendor);
  addProperty<NPointingDevice>(info, "product", &NPointingDevice::getProduct);
  addProperty<NPointingDevice>(info, "updateFrequency", &NPointingDevice::getUpdateFrequency);
  addProperty<NPointingDevice>(info, "resolution", &NPointingDevice::getResolution);
  addProperty<NPointingDevice>(info, "uri", &NPointingDevice::getURI);
  addProperty<NPointingDevice>(info, "expandedUri", &NPointingDevice::getExpandedURI);
  addProperty<NPointingDevice>(info, "active", &NPointingDevice::isActive);
}

Napi::Object NPointingDevice::NewInstance(Napi::Env env, Napi::Value uri){
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({uri});
  return scope.Escape(napi_value(obj)).ToObject();
}

void NPointingDevice::setPointingCallback(const Napi::CallbackInfo& info){
  this->tsfn = Napi::ThreadSafeFunction::New(
    info.Env(),
    info[0].As<Napi::Function>(),
    "NPointingDevice::pointingCallback",
    0, //Unlimited queue
    1  //1 initial thread
  );
  this->Ref(); //prenvents destructor from being called.
  this->input->setPointingCallback(pointingCallback, this);
}

Napi::Value NPointingDevice::getVendorID(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), this->input->getVendorID());
}

Napi::Value NPointingDevice::getProductID(const Napi::CallbackInfo& info){
  return Napi::Number::New(info.Env(), this->input->getProductID());
}

Napi::Value NPointingDevice::getVendor(const Napi::CallbackInfo& info){
  return Napi::String::New(info.Env(), this->input->getVendor());
}

Napi::Value NPointingDevice::getProduct(const Napi::CallbackInfo& info){
  return Napi::String::New(info.Env(), this->input->getProduct());
}

Napi::Value NPointingDevice::getUpdateFrequency(const Napi::CallbackInfo& info){
  return Napi::Number::New(info.Env(), this->input->getUpdateFrequency());
}

Napi::Value NPointingDevice::getResolution(const Napi::CallbackInfo& info){
  return Napi::Number::New(info.Env(), this->input->getResolution());
}

Napi::Value NPointingDevice::getURI(const Napi::CallbackInfo& info) {
  return Napi::String::New(info.Env(), this->input->getURI().asString());
}

Napi::Value NPointingDevice::getExpandedURI(const Napi::CallbackInfo& info){
  return Napi::String::New(info.Env(), this->input->getURI(true).asString());
}
Napi::Value NPointingDevice::isActive(const Napi::CallbackInfo& info){
  return Napi::Number::New(info.Env(), this->input->isActive());
}
