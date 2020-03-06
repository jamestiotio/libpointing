/* -*- mode: c++ -*-
 *
 * pointing/npointingdevicemanager.h
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

#ifndef NPOINTINGDEVICEMANAGER_H
#define NPOINTINGDEVICEMANAGER_H

#include <napi.h>
#include <map>
#include <pointing/input/PointingDeviceManager.h>

class NPointingDeviceManager : public Napi::ObjectWrap<NPointingDeviceManager>
{
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NPointingDeviceManager(const Napi::CallbackInfo& info);
    static Napi::Object NewInstance(Napi::Env env);

  private:
    static Napi::FunctionReference constructor;

    void addDeviceUpdateCallback(const Napi::CallbackInfo& info);
    void removeDeviceUpdateCallback(const Napi::CallbackInfo& info);
    Napi::Value getDeviceList(const Napi::CallbackInfo& info);

  	static void deviceUpdateCallback(void *context, const pointing::PointingDeviceDescriptor &descriptor, bool wasAdded);
};

#endif // NPOINTINGDEVICEMANAGER_H
