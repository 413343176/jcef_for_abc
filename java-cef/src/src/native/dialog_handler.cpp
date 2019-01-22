// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "dialog_handler.h"
#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

DialogHandler::DialogHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

DialogHandler::~DialogHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool DialogHandler::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                 FileDialogMode mode,
                                 const CefString &title,
                                 const CefString &default_file_name,
                                 const std::vector<CefString> &accept_types,
                                 CefRefPtr<CefFileDialogCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jmode = NULL;
  switch (mode) {
    default:
    JNI_CASE(env, "org/cef/handler/CefDialogHandler$FileDialogMode", FILE_DIALOG_OPEN, jmode);
    JNI_CASE(env, "org/cef/handler/CefDialogHandler$FileDialogMode", FILE_DIALOG_OPEN_MULTIPLE, jmode);
    JNI_CASE(env, "org/cef/handler/CefDialogHandler$FileDialogMode", FILE_DIALOG_SAVE, jmode);
  }

  jobject jcallback =  NewJNIObject(env, "org/cef/callback/CefFileDialogCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefFileDialogCallback");

  jboolean jreturn = JNI_FALSE;
  jstring jtitle = NewJNIString(env, title);
  jstring jdefault_file_name = NewJNIString(env, default_file_name);
  jobject jaccept_types = NewJNIStringVector(env, accept_types);
  JNI_CALL_METHOD(env, jhandler_,
                  "onFileDialog",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/CefDialogHandler$FileDialogMode;Ljava/lang/String;Ljava/lang/String;Ljava/util/Vector;Lorg/cef/callback/CefFileDialogCallback;)Z",
                  Boolean,
                  jreturn,
                  GetJNIBrowser(browser),
                  jmode,
                  NewJNIString(env, title),
                  NewJNIString(env, default_file_name),
                  NewJNIStringVector(env, accept_types),
                  jcallback);
  env->DeleteLocalRef(jtitle);
  env->DeleteLocalRef(jdefault_file_name);
  env->DeleteLocalRef(jaccept_types);
  env->DeleteLocalRef(jcallback);
  return (jreturn != JNI_FALSE);
}
