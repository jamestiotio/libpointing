/* -*- mode: c++ -*-
 *
 * pointing/ndisplaydevicemanager.h
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

#ifndef NDISPLAYDEVICEMANAGER_H
#define NDISPLAYDEVICEMANAGER_H

#include <napi.h>
#include <map>
#include <pointing/output/DisplayDeviceManager.h>

class NDisplayDeviceManager : public Napi::ObjectWrap<NDisplayDeviceManager>
{
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NDisplayDeviceManager(const Napi::CallbackInfo& info);
    static Napi::Object NewInstance(Napi::Env env);

  private:
    static Napi::FunctionReference constructor;

    void addDeviceUpdateCallback(const Napi::CallbackInfo& info);
    void removeDeviceUpdateCallback(const Napi::CallbackInfo& info);
    Napi::Value getDeviceList(const Napi::CallbackInfo& info);

  	static void deviceUpdateCallback(void *context, const pointing::DisplayDeviceDescriptor &descriptor, bool wasAdded);
};

#endif // NDISPLAYDEVICEMANAGER_H
