/* -*- mode: c++ -*-
 *
 * pointing/ndisplaydevice.h
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

#ifndef NDISPLAYDEVICE_H
#define NDISPLAYDEVICE_H

#include <napi.h>
#include <pointing/pointing.h>

class NDisplayDevice : public Napi::ObjectWrap<NDisplayDevice>
{
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NDisplayDevice(const Napi::CallbackInfo& info);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value uri);
    ~NDisplayDevice();

  private:
    static Napi::FunctionReference constructor;
    Napi::Value getSize(const Napi::CallbackInfo& info);
    Napi::Value getBounds(const Napi::CallbackInfo& info);
    Napi::Value getResolution(const Napi::CallbackInfo& info);
    Napi::Value getRefreshRate(const Napi::CallbackInfo& info);
    Napi::Value getURI(const Napi::CallbackInfo& info);

  	pointing::DisplayDevice *output;

    friend class NTransferFunction;
};

#endif // NDISPLAYDEVICE_H
