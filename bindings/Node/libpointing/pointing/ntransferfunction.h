/* -*- mode: c++ -*-
 *
 * pointing/ntransferfunction.h
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

#ifndef NTRANSFERFUNCTION_H
#define NTRANSFERFUNCTION_H

#include <napi.h>
#include <pointing/transferfunctions/SubPixelFunction.h>
#include "npointingdevice.h"
#include "ndisplaydevice.h"

class NTransferFunction : public Napi::ObjectWrap<NTransferFunction>
{
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NTransferFunction(const Napi::CallbackInfo& info);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value uri, Napi::Value input, Napi::Value output);
    ~NTransferFunction();

  private:
    static Napi::FunctionReference constructor;
    Napi::Value applyi(const Napi::CallbackInfo& info);
    Napi::Value applyd(const Napi::CallbackInfo& info);
    void clearState(const Napi::CallbackInfo& info);
    void setSubPixeling(const Napi::CallbackInfo& info);
    void setHumanResolution(const Napi::CallbackInfo& info);
    void setCardinalitySize(const Napi::CallbackInfo& info);

    Napi::Value getURI(const Napi::CallbackInfo& info);
    Napi::Value getSubPixeling(const Napi::CallbackInfo& info);
    Napi::Value getHumanResolution(const Napi::CallbackInfo& info);
    Napi::Value getCardinality(const Napi::CallbackInfo& info);
    Napi::Value getWidgetSize(const Napi::CallbackInfo& info);

    pointing::SubPixelFunction *func;

    NPointingDevice* nInput;
    NDisplayDevice* nOutput;
};

#endif // NTRANSFERFUNCTION_H
