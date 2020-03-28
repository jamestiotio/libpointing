/* -*- mode: c++ -*-
 *
 * pointing/property.hpp
 *
 * Initial software
 * Author: Etienne Orieux
 * Copyright © Inria
 *
 * http://libpointing.org/
 *
 * This software may be used and distributed according to the terms of
 * the GNU General Public License version 2 or any later version.
 *
 */

#ifndef PROPERTY_H
#define PROPERTY_H

#include <napi.h>

/**
 * Adds a read-only property to a {@link Napi::ObjectWrap} subclass.
 *
 * @tparam T the type of subclass
 * @param info the components of the JavaScript request
 * @param name the name of the property
 * @param getter the {@link Napi::InstanceGetterCallback} to call when retrieving the property
 * @author Etienne Orieux
 */
template< class T >
void addProperty(const Napi::CallbackInfo& info,
                 const char* name,
                 Napi::Value(T::*getter)(const Napi::CallbackInfo&))
{
  T* that = Napi::ObjectWrap<T>::Unwrap(info.This().As<Napi::Object>());
  info.This().As<Napi::Object>().DefineProperty(
       Napi::PropertyDescriptor::Accessor(info.Env(),
                                          info.This().As<Napi::Object>(),
                                          name,
                                          [that, getter](const Napi::CallbackInfo& info) -> Napi::Value {
                                            return (that->*getter)(info);
                                          },
                                          napi_enumerable, that));
}

#endif
