REDISTRIBUTION
--------------

This binary distribution contains the below components. Components listed under
the "required" section must be redistributed with all applications using JCEF.
Components listed under the "optional" section may be excluded if the related
features will not be used.

Required components:

* Java archives
    jcef.jar
    gluegen-rt.jar
    gluegen-rt-natives-windows-amd64.jar
    jogl-all.jar
    jogl-all-natives-windows-amd64.jar

* CEF JNI library
    jcef.dll

* CEF JNI process helper
    jcef_helper.exe

* CEF core library
    libcef.dll

* Unicode support
    icudtl.dat

Optional components:

* Localized resources
    locales/
  Note: Contains localized strings for WebKit UI controls. A .pak file is loaded
  from this folder based on the CefSettings.locale value. Only configured
  locales need to be distributed. If no locale is configured the default locale
  of "en-US" will be used. Locale file loading can be disabled completely using
  CefSettings.pack_loading_disabled. The locales folder path can be customized
  using CefSettings.locales_dir_path.

* Other resources
    cef.pak
    cef_100_percent.pak
    cef_200_percent.pak
    devtools_resources.pak
  Note: Contains WebKit image and inspector resources. Pack file loading can be
  disabled completely using CefSettings.pack_loading_disabled. The resources
  directory path can be customized using CefSettings.resources_dir_path.

* FFmpeg audio and video support
    ffmpegsumo.dll
  Note: Without this component HTML5 audio and video will not function.

* PDF support
    pdf.dll
  Note: Without this component printing will not function.
