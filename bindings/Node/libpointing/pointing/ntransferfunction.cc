/* -*- mode: c++ -*-
 *
 * pointing/ntransferfunction.cc
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

#include "ntransferfunction.h"

using namespace pointing;

Napi::FunctionReference NTransferFunction::constructor;

NTransferFunction::~NTransferFunction()
{
  nInput->Reset();
  nOutput->Reset();
  delete func;
}

Napi::Object NTransferFunction::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "TransferFunction", {
    InstanceMethod("applyi", &NTransferFunction::applyi),
    InstanceMethod("applyd", &NTransferFunction::applyd),
    InstanceMethod("clearState", &NTransferFunction::clearState),
    InstanceMethod("setSubPixeling", &NTransferFunction::setSubPixeling),
    InstanceMethod("setHumanResolution", &NTransferFunction::setHumanResolution),
    InstanceMethod("setCardinalitySize", &NTransferFunction::setCardinalitySize)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("TransferFunction", func);

  return exports;
}

NTransferFunction::NTransferFunction(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NTransferFunction>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if ( length <= 0 || length > 3 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String, PointingDevice, DisplayDevice expected").ThrowAsJavaScriptException();
    return;
  }

  std::string uri = info[0].ToString().Utf8Value();
  NPointingDevice *ninput = Napi::ObjectWrap<NPointingDevice>::Unwrap(info[1].As<Napi::Object>());
  NDisplayDevice *noutput = Napi::ObjectWrap<NDisplayDevice>::Unwrap(info[2].As<Napi::Object>());

  this->func = new SubPixelFunction("subpixel:?isOn=false", uri, ninput->input, noutput->output);
  this->nInput = ninput;
  this->nOutput = noutput;

  addProperty(info, "uri", &NTransferFunction::getURI);
  addProperty(info, "subPixeling", &NTransferFunction::getSubPixeling);
  addProperty(info, "humanResolution", &NTransferFunction::getHumanResolution);
  addProperty(info, "cardinality", &NTransferFunction::getCardinality);
  addProperty(info, "widgetSize", &NTransferFunction::getWidgetSize);
}

Napi::Object NTransferFunction::NewInstance(Napi::Env env, Napi::Value uri, Napi::Value input, Napi::Value output){
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({uri, input, output});
  return scope.Escape(napi_value(obj)).ToObject();
}


Napi::Value NTransferFunction::applyi(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();
  Napi::Object obj = Napi::Object::New(env);
  if ( length <= 0 || length > 3 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(env, "3 Numbers expected").ThrowAsJavaScriptException();
    return obj;
  }

  int dxPixel = 0, dyPixel = 0;
  int dx = info[0].As<Napi::Number>().Int32Value();
  int dy = info[1].As<Napi::Number>().Int32Value();
  TimeStamp::inttime timestamp = info[2].As<Napi::Number>().Uint32Value();
  this->func->applyi(dx, dy, &dxPixel, &dyPixel, timestamp);

  obj.Set("dx", dxPixel);
  obj.Set("dy", dyPixel);

  return obj;
}

Napi::Value NTransferFunction::applyd(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();
  Napi::Object obj = Napi::Object::New(env);
  if ( length <= 0 || length > 3 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(env, "3 Numbers expected").ThrowAsJavaScriptException();
    return obj;
  }

  double dxPixel = 0, dyPixel = 0;
  int dx = info[0].As<Napi::Number>().Int32Value();
  int dy = info[1].As<Napi::Number>().Int32Value();
  TimeStamp::inttime timestamp = info[2].As<Napi::Number>().Uint32Value();
  this->func->applyd(dx, dy, &dxPixel, &dyPixel, timestamp);

  obj.Set("dx", dxPixel);
  obj.Set("dy", dyPixel);

  return obj;
}

void NTransferFunction::clearState(const Napi::CallbackInfo& info)
{
  this->func->clearState();
}

void NTransferFunction::setSubPixeling(const Napi::CallbackInfo& info)
{
  bool subpixeling = info[0].As<Napi::Boolean>().Value();
  this->func->setSubPixeling(subpixeling);
}

void NTransferFunction::setHumanResolution(const Napi::CallbackInfo& info){
  int humanResolution = info[0].As<Napi::Number>().Uint32Value();
  this->func->setHumanResolution(humanResolution);
}

void NTransferFunction::setCardinalitySize(const Napi::CallbackInfo& info){
  int cardinality = info[0].As<Napi::Number>().Uint32Value();
  int size = info[1].As<Napi::Number>().Uint32Value();
  this->func->setCardinalitySize(cardinality, size);
}

Napi::Value NTransferFunction::getURI(const Napi::CallbackInfo& info){
  return Napi::String::New(info.Env(), this->func->getInnerURI().asString());
}

Napi::Value NTransferFunction::getSubPixeling(const Napi::CallbackInfo& info){
  return Napi::Boolean::New(info.Env(), this->func->getSubPixeling());
}

Napi::Value NTransferFunction::getHumanResolution(const Napi::CallbackInfo& info){
  return Napi::Number::New(info.Env(), this->func->getHumanResolution());
}

Napi::Value NTransferFunction::getCardinality(const Napi::CallbackInfo& info){
  int cardinality = 0, size = 0;
  this->func->getCardinalitySize(&cardinality, &size);
  return Napi::Number::New(info.Env(), cardinality);
}

Napi::Value NTransferFunction::getWidgetSize(const Napi::CallbackInfo& info){
  int cardinality = 0, size = 0;
  this->func->getCardinalitySize(&cardinality, &size);
  return Napi::Number::New(info.Env(), size);
}
