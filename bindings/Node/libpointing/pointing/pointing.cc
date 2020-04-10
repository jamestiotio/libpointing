/* -*- mode: c++ -*-
 *
 * pointing/pointing.cc
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
#include "npointingdevicemanager.h"
#include "ndisplaydevicemanager.h"
#include "nsystempointeracceleration.h"
#include "npointingcursor.h"

#ifdef __APPLE__
#include <uv.h>
#include <pointing/input/PointingDevice.h>

uv_idle_t idler;

void perform_while_idle(uv_idle_t* handle)
{
  pointing::PointingDevice::idle(4);
  // uv_idle_stop(handle);
}

#endif

Napi::Object createNPointingDeviceManager(const Napi::CallbackInfo& info) {
  return NPointingDeviceManager::NewInstance(info.Env());
}

Napi::Object createNDisplayDeviceManager(const Napi::CallbackInfo& info) {
  return NDisplayDeviceManager::NewInstance(info.Env());
}

Napi::Object createNPointingDevice(const Napi::CallbackInfo& info) {
  return NPointingDevice::NewInstance(info.Env(), info[0]);
}

Napi::Object createNDisplayDevice(const Napi::CallbackInfo& info) {
  return NDisplayDevice::NewInstance(info.Env(), info[0]);
}

Napi::Object createNTransferFunction(const Napi::CallbackInfo& info) {
  return NTransferFunction::NewInstance(info.Env(), info[0],  info[1], info[2]);
}

Napi::Object createNSystemPointerAcceleration(const Napi::CallbackInfo& info) {
  return NSystemPointerAcceleration::NewInstance(info.Env());
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  #ifdef __APPLE__
  uv_idle_init(uv_default_loop(), &idler);
  uv_idle_start(&idler, perform_while_idle);
  #endif

  NPointingDevice::Init(env, exports);
  NDisplayDevice::Init(env, exports);
  NTransferFunction::Init(env, exports);
  NPointingDeviceManager::Init(env, exports);
  NDisplayDeviceManager::Init(env, exports);
  NSystemPointerAcceleration::Init(env, exports);
  NPointingCursor::Init(env, exports);

  exports.Set("PointingDeviceManager", Napi::Function::New(env, createNPointingDeviceManager));
  exports.Set("DisplayDeviceManager", Napi::Function::New(env, createNDisplayDeviceManager));
  exports.Set("PointingDevice", Napi::Function::New(env, createNPointingDevice));
  exports.Set("DisplayDevice", Napi::Function::New(env, createNDisplayDevice));
  exports.Set("TransferFunction", Napi::Function::New(env, createNTransferFunction));
  exports.Set("SystemPointerAcceleration", Napi::Function::New(env, createNSystemPointerAcceleration));

  return exports;
}

NODE_API_MODULE(testaddon, InitAll);

