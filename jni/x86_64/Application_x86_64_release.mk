APP_STL             := c++_static
APP_CPPFLAGS        := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++17 -fsigned-char -Wno-extern-c-compat
APP_LDFLAGS         := -latomic

# APP_OPTIM           := debug
APP_OPTIM           := release # 配布時はreleaseを使う

APP_ABI             := x86_64
APP_PLATFORM        := android-28
APP_BUILD_SCRIPT    := jni/x86_64/Android_x86_64.mk
