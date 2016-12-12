#include <jni.h>
#include <string>
#include <sys/stat.h>
#include <sys/system_properties.h>
#include <android/log.h>


#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "qtfreet00", __VA_ARGS__)
extern "C" {

//char * antiModels[]={"ChangWan","",""};

int anti(char *res) {
    struct stat buf;
    int result = stat(res, &buf) == 0 ? 1 : 0;
    if (result) {
        LOGE("the %s is exist", res);
        LOGE("this is a Emulator!!!");
    }
    return result;
}

int anti2(char *res) {
    char buff[PROP_VALUE_MAX];
    memset(buff, 0, PROP_VALUE_MAX);
    int result =
            __system_property_get(res, (char *) &buff) > 0 ? 1 : 0; //返回命令行内容的长度
    if (result != 0) {
        LOGE("the %s result is %s", res, buff);
        LOGE("this is a Emulator!!!");
    }
    return result;
}

char *getDeviceInfo(char *res) {
    char buff[PROP_VALUE_MAX];
    memset(buff, 0, PROP_VALUE_MAX);
    __system_property_get(res, (char *) &buff);
    LOGE("the %s result is %s", res, buff);
    return buff;
}

int check() {
    char buff[PROP_VALUE_MAX];
    memset(buff, 0, PROP_VALUE_MAX);
    int i = 0;

    if (anti("/system/lib/libc_malloc_debug_qemu.so")) {
        i++;
    }
    if (anti("/system/lib/libc_malloc_debug_qemu.so-arm")) {
        i++;
    }
    if (anti("/system/bin/qemu_props")) {
        i++;
    }
    if (anti("/system/bin/androVM-prop")) {   //itools模拟器有此特征
        i++;
    }
    if (anti("/system/bin/microvirt-prop")) {
        i++;
    }
    if (anti("/system/bin/windroyed")) {   //文卓爷
        i++;
    }
    if (anti("/system/bin/microvirtd")) {
        i++;
    }
    if (anti("/system/bin/nox-prop")) {  //夜神
        i++;
    }
    if (anti("/system/bin/ttVM-prop")) { //天天模拟器
        i++;
    }
    if (anti("/system/bin/droid4x-prop")) {  //海马玩
        i++;
    }
    if (anti2("init.svc.vbox86-setup")) {
        i++;
    }
    if (anti2("init.svc.droid4x")) {
        i++;
    }
    if (anti2("init.svc.qemud")) {
        i++;
    }
    if (anti2("init.svc.su_kpbs_daemon")) {
        i++;
    }
    if (anti2("init.svc.noxd")) {
        i++;
    }
    if (anti2("init.svc.ttVM_x86-setup")) {
        i++;
    }
    if (anti2("init.svc.xxkmsg")) {
        i++;
    }
    if (anti2("init.svc.microvirtd")) {
        i++;
    }
    getDeviceInfo("ro.product.model");
    char *model = getDeviceInfo("ro.product.name");
    if (!strcmp(model, "ChangWan")) {
        i++;
    } else if (!strcmp(model, "Droid4X")) {                     //非0均为模拟器
        i++;
    } else if (!strcmp(model, "lgshouyou")) {
        i++;
    } else if (!strcmp(model, "nox")) {
        i++;
    } else if (!strcmp(model, "ttVM_Hdragon")) {
        i++;
    }
    char *hardware = getDeviceInfo("ro.hardware");
    if (!strcmp(hardware, "goldfish")) {
        i++;
    }
    char *brand = getDeviceInfo("ro.product.brand");
    if (!strcmp(brand, "xxzs")) {
        i++;
    }

    LOGE("the counts is %d", i);
    return i;
}


jstring
Java_com_qtfreet_anticheckemulator_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    int i = check();
    if (i == 0) {
        char *hello = "this is a phone";
        return env->NewStringUTF(hello);
    } else {
        char *hello2 = "this is a emulator";

        return env->NewStringUTF(hello2);
    }
}
}

