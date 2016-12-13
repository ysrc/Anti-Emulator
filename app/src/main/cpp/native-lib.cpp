#include <jni.h>
#include <string>
#include <sys/stat.h>
#include <sys/system_properties.h>
#include <android/log.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "qtfreet00", __VA_ARGS__)
extern "C" {

//char * antiModels[]={"ChangWan","",""};
char *jstringToChar(JNIEnv *env, jstring jstr) {
    if (jstr == NULL) {
        return NULL;

    }
    char *rtn = new char;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;

    } else {
        rtn = "";

    }

    /**资源清理**/
    env->ReleaseByteArrayElements(barr, ba, 0);
    if (clsstring != NULL) {
        env->DeleteLocalRef(clsstring);
        clsstring = NULL;

    }
    if (strencode != NULL) {
        env->DeleteLocalRef(strencode);
        strencode = NULL;

    }
    mid = NULL;
    return rtn;
}


jobject getApplication(JNIEnv *env) {
    jclass localClass = env->FindClass("android/app/ActivityThread");
    if (localClass != NULL) {
        jmethodID getapplication = env->GetStaticMethodID(localClass, "currentApplication",
                                                          "()Landroid/app/Application;");
        if (getapplication != NULL) {
            jobject application = env->CallStaticObjectMethod(localClass, getapplication);
            return application;
        }
        return NULL;
    }
    return NULL;
}


void verifySign(JNIEnv *env) {
    jobject context = getApplication(env);
    jclass activity = env->GetObjectClass(context);
    // 得到 getPackageManager 方法的 ID
    jmethodID methodID_func = env->GetMethodID(activity, "getPackageManager",
                                               "()Landroid/content/pm/PackageManager;");
    // 获得PackageManager对象
    jobject packageManager = env->CallObjectMethod(context, methodID_func);
    jclass packageManagerclass = env->GetObjectClass(packageManager);
    //得到 getPackageName 方法的 ID
    jmethodID methodID_pack = env->GetMethodID(activity, "getPackageName", "()Ljava/lang/String;");
    //获取包名
    jstring name_str = static_cast<jstring>(env->CallObjectMethod(context, methodID_pack));
    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_pm = env->GetMethodID(packageManagerclass, "getPackageInfo",
                                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    // 获得应用包的信息
    jobject package_info = env->CallObjectMethod(packageManager, methodID_pm, name_str, 64);
    // 获得 PackageInfo 类
    jclass package_infoclass = env->GetObjectClass(package_info);
    // 获得签名数组属性的 ID
    jfieldID fieldID_signatures = env->GetFieldID(package_infoclass, "signatures",
                                                  "[Landroid/content/pm/Signature;");
    // 得到签名数组，待修改
    jobject signatur = env->GetObjectField(package_info, fieldID_signatures);
    jobjectArray signatures = reinterpret_cast<jobjectArray>(signatur);
    // 得到签名
    jobject signature = env->GetObjectArrayElement(signatures, 0);
    // 获得 Signature 类，待修改
    jclass signature_clazz = env->GetObjectClass(signature);
    //获取sign
    jmethodID toCharString = env->GetMethodID(signature_clazz, "toCharsString",
                                              "()Ljava/lang/String;");
    //获取签名字符；或者其他进行验证操作
    jstring signstr = static_cast<jstring>(env->CallObjectMethod(signature, toCharString));
    char *ch = jstringToChar(env, signstr);
    //输入签名字符串，这里可以进行相关验证
    LOGE("the signtures is :%s", ch);
}


jstring getDeviceID(JNIEnv *env) {
    jobject mContext = getApplication(env);
    jclass cls_context = (env)->FindClass("android/content/Context");
    if (cls_context == 0) {
        return (env)->NewStringUTF("unknown");
    }
    jmethodID getSystemService = (env)->GetMethodID(cls_context,
                                                    "getSystemService",
                                                    "(Ljava/lang/String;)Ljava/lang/Object;");
    if (getSystemService == 0) {
        return (env)->NewStringUTF("unknown");
    }
    jfieldID TELEPHONY_SERVICE = (env)->GetStaticFieldID(cls_context,
                                                         "TELEPHONY_SERVICE", "Ljava/lang/String;");
    if (TELEPHONY_SERVICE == 0) {
        return (env)->NewStringUTF("unknown");
    }
    jobject str = (env)->GetStaticObjectField(cls_context, TELEPHONY_SERVICE);
    jobject telephonymanager = (env)->CallObjectMethod(mContext,
                                                       getSystemService, str);
    if (telephonymanager == 0) {
        return (env)->NewStringUTF("unknown");
    }
    jclass cls_tm = (env)->FindClass("android/telephony/TelephonyManager");
    if (cls_tm == 0) {
        return (env)->NewStringUTF("unknown");
    }
    jmethodID getDeviceId = (env)->GetMethodID(cls_tm, "getDeviceId",
                                               "()Ljava/lang/String;");
    if (getDeviceId == 0) {
        return (env)->NewStringUTF("unknown");
    }
    jstring deviceid = static_cast<jstring>((env)->CallObjectMethod(telephonymanager, getDeviceId));
    char *ch = jstringToChar(env, deviceid);
    LOGE("the deviceId is %s", ch);
    return deviceid;
}

void getBattery() {
    char *info = new char[128];

    char *cmd = "dumpsys battery";
    FILE *ptr;
    if ((ptr = popen(cmd, "r")) != NULL) {
        if (fgets(info, 128, ptr) != NULL) {
            LOGE("the status is = %s", info);
        }
    }
}


int anti(char *res) {
    struct stat buf;
    int result = stat(res, &buf) == 0 ? 1 : 0;
    if (result) {
        LOGE("the %s is exist", res);
        //    LOGE("this is a Emulator!!!");
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
        //  LOGE("this is a Emulator!!!");
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

int checkTemp() {
    DIR *dirptr = NULL; //当前手机的温度检测，手机下均有thermal_zone文件
    int i = 0;
    struct dirent *entry;
    if ((dirptr = opendir("/sys/class/thermal/")) != NULL) {
        while (entry = readdir(dirptr)) {
            // LOGE("%s  \n", entry->d_name);
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
            }
            char *tmp = entry->d_name;
            if (strstr(tmp, "thermal_zone") != NULL) {
                i++;
            }
        }
        closedir(dirptr);
    } else {
        LOGE("open thermal fail");
    }
    return i;
}

int check() {
    char buff[PROP_VALUE_MAX];
    memset(buff, 0, PROP_VALUE_MAX);
    int i = 0;
    if (anti("/system/lib/libc_malloc_debug_qemu.so")) {
        //在cm，魔趣等基于aosp改版的系统上会存在libc_malloc_debug_qemu.so这个文件
        if (access("/system/lib/libbluetooth_jni.so", F_OK) != 0) {
            LOGE("the bluetooth is not exist");
            i++;//在误报情况下，再去检测当前设备是否存在蓝牙，不存在则判断为模拟器
        }

    }
    if (anti("/system/lib/libc_malloc_debug_qemu.so-arm")) {
        if (access("/system/lib/libbluetooth_jni.so", F_OK) != 0) {
            i++;
        }
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

    if (anti("/system/lib/libdroid4x.so")) {   //文卓爷
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
    if (checkTemp() == 0) {
        LOGE("can not find the temperature sensor,so this is a Emulator");
    } //获取不到温度感应器则判定为模拟器

    LOGE("the counts is %d", i);
    return i;
}

pthread_t id = NULL;

void checkAndroidServer() {
    DIR *dirptr = NULL;
    int i = 0;
    struct dirent *entry;
    if ((dirptr = opendir("/data")) != NULL) {  //data目录没有读权限，行不通
        while (entry = readdir(dirptr)) {
            // LOGE("%s  \n", entry->d_name);
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
            }
            char *tmp = entry->d_name;
            LOGE("the /data/local/tmp file is %s", tmp);
//            if (DT_DIR == entry->d_type) {
//                char *tmp = entry->d_name;
//                LOGE("the /data/local/tmp file is %s", tmp);
//            }
        }
        closedir(dirptr);
    } else {
        LOGE("open tmp fail");
    }
}

void checkAndroid() {
    if (pthread_create(&id, NULL, (void *(*)(void *)) &checkAndroidServer, NULL) != 0) {
        exit(-1);
    }

}

/*逍遥模拟器
 * 12-13 12:20:58.671 1615-1615/? E/qtfreet00: the /system/bin/microvirt-prop is exist
12-13 12:20:58.671 1615-1615/? E/qtfreet00: the /system/bin/microvirtd is exist
12-13 12:20:58.671 1615-1615/? E/qtfreet00: the init.svc.vbox86-setup result is stopped
12-13 12:20:58.671 1615-1615/? E/qtfreet00: the init.svc.microvirtd result is running*/




jstring
Java_com_qtfreet_anticheckemulator_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    int i = check();
    verifySign(env);
    checkAndroid();
    getDeviceID(env);
    if (i == 0) {
        char *hello = "this is a phone";
        LOGE("%s", hello);
        return env->NewStringUTF(hello);
    } else {
        char *hello2 = "this is a emulator";
        LOGE("%s", hello2);
        return env->NewStringUTF(hello2);
    }
}
}

