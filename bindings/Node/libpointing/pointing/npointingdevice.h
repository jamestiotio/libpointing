/* -*- mode: c++ -*-
 *
 * pointing/npointingdevice.h
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

#ifndef NPOINTINGDEVICE_H
#define NPOINTINGDEVICE_H

#include <napi.h>
#include <pointing/pointing.h>

class NPointingDevice : public Napi::ObjectWrap<NPointingDevice>
{
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NPointingDevice(const Napi::CallbackInfo& info);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value uri);
    ~NPointingDevice();

  private:
    static Napi::FunctionReference constructor;
    void setPointingCallback(const Napi::CallbackInfo& info);
    Napi::Value getVendorID(const Napi::CallbackInfo& info);
    Napi::Value getProductID(const Napi::CallbackInfo& info);
    Napi::Value getVendor(const Napi::CallbackInfo& info);
    Napi::Value getProduct(const Napi::CallbackInfo& info);
    Napi::Value getUpdateFrequency(const Napi::CallbackInfo& info);
    Napi::Value getResolution(const Napi::CallbackInfo& info);
    Napi::Value getURI(const Napi::CallbackInfo& info);
    Napi::Value getExpandedURI(const Napi::CallbackInfo& info);
    Napi::Value isActive(const Napi::CallbackInfo& info);

    pointing::PointingDevice *input;
    static void pointingCallback(void *context, pointing::TimeStamp::inttime timestamp, int dx, int dy, int buttons);

    Napi::ThreadSafeFunction tsfn;
    struct PointingReport
    {
      pointing::TimeStamp::inttime timestamp;
      int dx;
      int dy;
      int buttons;
    };

    friend class NTransferFunction;
};

#endif // NPOINTINGDEVICE_H
