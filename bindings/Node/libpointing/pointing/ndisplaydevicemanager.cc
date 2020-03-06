/* -*- mode: c++ -*-
 *
 * pointing/ndisplaydevicemanager.cc
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

#include "ndisplaydevicemanager.h"

using namespace pointing;

struct DisplayDescriptorCallback
{
  DisplayDeviceDescriptor desc;
  bool wasAdded;
};

Napi::FunctionReference NDisplayDeviceManager::constructor;
typedef std::map<std::string, Napi::ThreadSafeFunction> callbackMap_t;
static callbackMap_t callbackMap = callbackMap_t();

void createDescObject(Napi::Object &obj, const DisplayDeviceDescriptor &ddd)
{
  obj.Set("devURI", ddd.devURI);
  obj.Set("name", ddd.name);
}

void NDisplayDeviceManager::deviceUpdateCallback(void *context, const DisplayDeviceDescriptor &descriptor, bool wasAdded)
{
  DisplayDescriptorCallback *ddc = new DisplayDescriptorCallback;
  ddc->desc = descriptor;
  ddc->wasAdded = wasAdded;

  callbackMap_t::iterator it = callbackMap.begin();
  for(; it != callbackMap.end(); it++)
  {
    auto callback = []( Napi::Env env, Napi::Function jsCallback, DisplayDescriptorCallback* ddc) {
      Napi::Object descObj = Napi::Object::New(env);
      createDescObject(descObj, ddc->desc);
      Napi::Number added = Napi::Number::New(env, ddc->wasAdded);
      jsCallback.Call({descObj, added});
    };

    it->second.BlockingCall(ddc, callback);
  }
}

Napi::Object NDisplayDeviceManager::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
  DisplayDeviceManager::get()->addDeviceUpdateCallback(deviceUpdateCallback, NULL);

  Napi::Function func = DefineClass(env, "DisplayDeviceManager", {
    InstanceMethod("addDeviceUpdateCallback", &NDisplayDeviceManager::addDeviceUpdateCallback),
    InstanceMethod("removeDeviceUpdateCallback", &NDisplayDeviceManager::removeDeviceUpdateCallback),
    InstanceAccessor("deviceList", &NDisplayDeviceManager::getDeviceList, NULL) //NULL = no setter on the deviceList property.
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("DisplayDeviceManager", func);

  return exports;
}

NDisplayDeviceManager::NDisplayDeviceManager(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NDisplayDeviceManager>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
};

Napi::Object NDisplayDeviceManager::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

void NDisplayDeviceManager::addDeviceUpdateCallback(const Napi::CallbackInfo& info) {
  std::string key = info[0].ToString().Utf8Value();

  callbackMap[key] = Napi::ThreadSafeFunction::New(
    info.Env(),
    info[0].As<Napi::Function>(),
    "NDisplayDeviceManager::addDeviceUpdateCallback",
    0, //Unlimited queue
    1  //1 initial thread
  );
}

void NDisplayDeviceManager::removeDeviceUpdateCallback(const Napi::CallbackInfo& info) {
  std::string key = info[0].ToString().Utf8Value();

  callbackMap.erase(key);
}

Napi::Value NDisplayDeviceManager::getDeviceList(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  DisplayDeviceManager *man = DisplayDeviceManager::get();
  // Creating javascript array
  Napi::Array result = Napi::Array::New(env, man->size());

  int i = 0;
  for (DisplayDescriptorIterator it = man->begin(); it != man->end(); it++)
  {
    DisplayDeviceDescriptor pdd = *it;
    Napi::Object descObj = Napi::Object::New(env);
    createDescObject(descObj, pdd);
    result[i++] = descObj;
  }
  return result;
}
