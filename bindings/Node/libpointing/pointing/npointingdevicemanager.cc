/* -*- mode: c++ -*-
 *
 * pointing/npointingdevicemanager.cc
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

#include "npointingdevicemanager.h"

using namespace pointing;

struct PointingDescriptorCallback
{
  PointingDeviceDescriptor desc;
  bool wasAdded;
};

Napi::FunctionReference NPointingDeviceManager::constructor;
typedef std::map<std::string, Napi::ThreadSafeFunction> callbackMap_t;
static callbackMap_t callbackMap = callbackMap_t();

void createDescObject(Napi::Object &obj, const PointingDeviceDescriptor &pdd)
{
  obj.Set("devURI", pdd.devURI.asString());
  obj.Set("vendor", pdd.vendor);
  obj.Set("product", pdd.product);
  obj.Set("vendorID", pdd.vendorID);
  obj.Set("productID", pdd.productID);
}

void NPointingDeviceManager::deviceUpdateCallback(void *context, const PointingDeviceDescriptor &descriptor, bool wasAdded)
{
  PointingDescriptorCallback *pdc = new PointingDescriptorCallback;
  pdc->desc = descriptor;
  pdc->wasAdded = wasAdded;

  callbackMap_t::iterator it = callbackMap.begin();
  for(; it != callbackMap.end(); it++)
  {
    auto callback = []( Napi::Env env, Napi::Function jsCallback, PointingDescriptorCallback* pdc) {
      Napi::Object descObj = Napi::Object::New(env);
      createDescObject(descObj, pdc->desc);
      Napi::Number added = Napi::Number::New(env, pdc->wasAdded);
      jsCallback.Call({descObj, added});
    };

    it->second.BlockingCall(pdc, callback);
  }
}

Napi::Object NPointingDeviceManager::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
  PointingDeviceManager::get()->addDeviceUpdateCallback(deviceUpdateCallback, NULL);

  Napi::Function func = DefineClass(env, "PointingDeviceManager", {
    InstanceMethod("addDeviceUpdateCallback", &NPointingDeviceManager::addDeviceUpdateCallback),
    InstanceMethod("removeDeviceUpdateCallback", &NPointingDeviceManager::removeDeviceUpdateCallback),
    InstanceAccessor("deviceList", &NPointingDeviceManager::getDeviceList, NULL) //NULL = no setter on the deviceList property.
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("PointingDeviceManager", func);

  return exports;
}

NPointingDeviceManager::NPointingDeviceManager(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NPointingDeviceManager>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
};

Napi::Object NPointingDeviceManager::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

void NPointingDeviceManager::addDeviceUpdateCallback(const Napi::CallbackInfo& info) {
  std::string key = info[0].ToString().Utf8Value();

  callbackMap[key] = Napi::ThreadSafeFunction::New(
    info.Env(),
    info[0].As<Napi::Function>(),
    "NDisplayDeviceManager::addDeviceUpdateCallback",
    0, //Unlimited queue
    1  //1 initial thread
  );
}

void NPointingDeviceManager::removeDeviceUpdateCallback(const Napi::CallbackInfo& info) {
  std::string key = info[0].ToString().Utf8Value();
  callbackMap.erase(key);
}

Napi::Value NPointingDeviceManager::getDeviceList(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  PointingDeviceManager *man = PointingDeviceManager::get();
  // Creating javascript array
  Napi::Array result = Napi::Array::New(env, man->size());

  int i = 0;
  for (PointingDescriptorIterator it = man->begin(); it != man->end(); it++)
  {
    PointingDeviceDescriptor pdd = *it;
    Napi::Object descObj = Napi::Object::New(env);
    createDescObject(descObj, pdd);
    result[i++] = descObj;
  }
  return result;
}
