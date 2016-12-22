#include <jni.h>
#include <string>
#include <sys/stat.h>
#include <sys/system_properties.h>
#include <android/log.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h> // sigtrap stuff, duh

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

jstring chartoJstring(JNIEnv *env, const char *pat) {
    jclass strClass = env->FindClass("Ljava/lang/String;");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
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


//char *verifySign(JNIEnv *env) {
//    jobject context = getApplication(env);
//    jclass activity = env->GetObjectClass(context);
//    // 得到 getPackageManager 方法的 ID
//    jmethodID methodID_func = env->GetMethodID(activity, "getPackageManager",
//                                               "()Landroid/content/pm/PackageManager;");
//    // 获得PackageManager对象
//    jobject packageManager = env->CallObjectMethod(context, methodID_func);
//    jclass packageManagerclass = env->GetObjectClass(packageManager);
//    //得到 getPackageName 方法的 ID
//    jmethodID methodID_pack = env->GetMethodID(activity, "getPackageName", "()Ljava/lang/String;");
//    //获取包名
//    jstring name_str = static_cast<jstring>(env->CallObjectMethod(context, methodID_pack));
//    // 得到 getPackageInfo 方法的 ID
//    jmethodID methodID_pm = env->GetMethodID(packageManagerclass, "getPackageInfo",
//                                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
//    // 获得应用包的信息
//    jobject package_info = env->CallObjectMethod(packageManager, methodID_pm, name_str, 64);
//    // 获得 PackageInfo 类
//    jclass package_infoclass = env->GetObjectClass(package_info);
//    // 获得签名数组属性的 ID
//    jfieldID fieldID_signatures = env->GetFieldID(package_infoclass, "signatures",
//                                                  "[Landroid/content/pm/Signature;");
//    // 得到签名数组，待修改
//    jobject signatur = env->GetObjectField(package_info, fieldID_signatures);
//    jobjectArray signatures = reinterpret_cast<jobjectArray>(signatur);
//    // 得到签名
//    jobject signature = env->GetObjectArrayElement(signatures, 0);
//    // 获得 Signature 类，待修改
//    jclass signature_clazz = env->GetObjectClass(signature);
//    //获取sign
//    jmethodID toCharString = env->GetMethodID(signature_clazz, "toCharsString",
//                                              "()Ljava/lang/String;");
//    //获取签名字符；或者其他进行验证操作
//    jstring signstr = static_cast<jstring>(env->CallObjectMethod(signature, toCharString));
//    char *ch = jstringToChar(env, signstr);
//    //输入签名字符串，这里可以进行相关验证
//    LOGE("the signtures is :%s", ch);
//    return ch;
//}


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

char *getCpuInfo() { //获取cpu型号
    char *info = new char[128];
    memset(info, 0, 128);
//    char *res = new char[256];
//    memset(res,0,256);
    char *split = ":";
    char *cmd = "/proc/cpuinfo";
    FILE *ptr;
    if ((ptr = fopen(cmd, "r")) != NULL) {
        while (fgets(info, 128, ptr)) {

//            if (strstr(info, "Processor") != NULL) {
//                strtok(info, split);
//                char *s = strtok(NULL, split);
//                strcat(res, s);
////                strcat(res, "|");
//            }
            if (strstr(info,
                       "Hardware")) {  //真机一般会获取到hardware，示例：Qualcomm MSM 8974 HAMMERHEAD (Flattened Device Tree)
                strtok(info, split);
                char *s = strtok(NULL, split);

                LOGE("the cpu info is %s", s);
                return s;
            } else if (strstr(info,
                              "model name")) { //测试了一个模拟器，取到的是model_name，示例：Intel(R) Core(TM) i5-4590 CPU @ 3.30GHz
                strtok(info, split);
                char *s = strtok(NULL, split);
                if (strstr(s, "Intel(R) Core(TM) i") != NULL) {
                    //     kill(getpid(),SIGKILL);
                }

                LOGE("the cpu info is %s", s);
                return s;
            }
        }
    } else {
        LOGE("NULLLLLLLLL");
    }
}

char *getVersionInfo() {   //获取设备版本，真机示例：Linux version 3.4.0-cyanogenmod (ls@ywk) (gcc version 4.7 (GCC) ) #1 SMP PREEMPT Tue Apr 12 11:38:13 CST 2016
// 海马玩：   Linux version 3.4.0-qemu+ (droid4x@CA) (gcc version 4.6.3 (Ubuntu/Linaro 4.6.3-1ubuntu5) ) #25 SMP PREEMPT Tue Sep 22 15:50:48
    char *info = new char[256];
    memset(info, 0, 256);
    char *cmd = "/proc/version";
    FILE *ptr;
    if ((ptr = fopen(cmd, "r")) != NULL) {
        while (fgets(info, 256, ptr)) {
            LOGE("the version info is %s", info);
            return info;
        }
    } else {
        LOGE("NULLLLLLLLL");
        return NULL;
    }
}

void antiFile(char *res) {
    struct stat buf;
    int result = stat(res, &buf) == 0 ? 1 : 0;
    if (result) {
        LOGE("%s is exsits emulator!", res);
        //     kill(getpid(),SIGKILL);
    }
}

void antiProperty(char *res) {
    char buff[PROP_VALUE_MAX];
    memset(buff, 0, PROP_VALUE_MAX);
    int result =
            __system_property_get(res, (char *) &buff) > 0 ? 1 : 0; //返回命令行内容的长度
    if (result != 0) {
        LOGE("%s %s is exsits emulator!", res, buff);
        //  kill(getpid(),SIGKILL);
    }
}

void getDeviceInfo() {
    char buff[PROP_VALUE_MAX];
    memset(buff, 0, PROP_VALUE_MAX);
    __system_property_get("ro.product.name", (char *) &buff);
    LOGE("the model name is %s", buff);
    if (!strcmp(buff, "ChangWan")) {
        //  kill(getpid(),SIGKILL);
    } else if (!strcmp(buff, "Droid4X")) {                     //非0均为模拟器
        //  kill(getpid(),SIGKILL);
    } else if (!strcmp(buff, "lgshouyou")) {
        // kill(getpid(),SIGKILL);
    } else if (!strcmp(buff, "nox")) {
        //  kill(getpid(),SIGKILL);
    } else if (!strcmp(buff, "ttVM_Hdragon")) {
        //  kill(getpid(),SIGKILL);
    }

}

void checkTemp() {
    DIR *dirptr = NULL; //当前手机的温度检测，手机下均有thermal_zone文件
    //此方法在i9300上测试失败,该方法不稳定
    int i = 0;
    struct dirent *entry;
    if ((dirptr = opendir("/sys/class/thermal/")) != NULL) {
        while (entry = readdir(dirptr)) {
            // LOGE("%s  \n", entry->d_name);
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
            }
            char *tmp = entry->d_name;
            LOGE("thermal name is : %s", tmp);
            if (strstr(tmp, "thermal_zone") != NULL) {
                i++;
            }
        }
        closedir(dirptr);
    } else {
        LOGE("open thermal fail");
        return;
    }
    if (i == 0) {
        LOGE("there is no temp sensor");
        //   kill(getpid(),SIGKILL);
    }
}


void checkBattery() {
    DIR *dirptr = NULL; //当前手机的温度检测，手机下均有thermal_zone文件
    //此方法在i9300上测试失败,该方法不稳定
    int i = 0;
    struct dirent *entry;
    if ((dirptr = opendir("/sys/class/power_supply/")) != NULL) {
        while (entry = readdir(dirptr)) {
            // LOGE("%s  \n", entry->d_name);
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
            }
            char *tmp = entry->d_name;
            LOGE("power_supply name is : %s", tmp);
            if (strstr(tmp, "battery") != NULL) {
                i++;
            }
        }
        closedir(dirptr);
    } else {
        LOGE("open power_supply fail");
        return;
    }
    if (i == 0) {
        LOGE("there is no battery");
        //   kill(getpid(),SIGKILL);
    }
}

void testBluetooth() {
    //此方法在多台设备上也存在问题
    if (access("/system/lib/libbluetooth_jni.so", F_OK) != 0) {
        LOGE("there is no bluetooth");
        //   kill(getpid(), SIGKILL);//在误报情况下，再去检测当前设备是否存在蓝牙，不存在则判断为模拟器
    }
    if (access("/system/lib/libbluetooth_jni.so", F_OK) != 0) {
        //    kill(getpid(), SIGKILL);
        LOGE("there is no bluetooth");
    }

}


void check() {
    antiFile("/system/bin/qemu_props"); //检测原生模拟器
    antiFile("/system/bin/qemud");
    antiFile("/system/bin/androVM-prop");
    antiFile("/system/bin/microvirt-prop");
    antiFile("/system/lib/libdroid4x.so");
    antiFile("/system/bin/windroyed");
    antiFile("/system/bin/microvirtd");
    antiFile("/system/bin/nox-prop"); //夜神
    antiFile("/system/bin/ttVM-prop"); //天天
    antiFile("/system/bin/droid4x-prop");
    antiProperty("init.svc.vbox86-setup");
    antiProperty("init.svc.droid4x");
    antiProperty("init.svc.qemud");
    antiProperty("init.svc.su_kpbs_daemon");
    antiProperty("init.svc.noxd");
    antiProperty("init.svc.ttVM_x86-setup");
    antiProperty("init.svc.xxkmsg");
    antiProperty("init.svc.microvirtd");
    antiProperty("ro.secure'");
    antiProperty("ro.kernel.android.qemud");
    antiProperty("ro.kernel.qemu.gles");

}


char *SocketTest(char *c) {
    struct sockaddr_in serv_addr;
    char buff[1024];
    char res[4096];
    memset(res, 0, 4096);
    memset(buff, 0, 1024);
    memset(&serv_addr, 0, sizeof(serv_addr));

    char *addr = "107.151.180.166";
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        LOGE("create error");
        LOGE("error (errno=%d)", errno);
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6666);
    serv_addr.sin_addr.s_addr = inet_addr(addr);
    if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
        struct hostent *host = gethostbyname(addr);
        if (host == NULL) {
            LOGE("error (errno=%d)", errno);
            exit(1);
        }
        serv_addr.sin_addr.s_addr = ((struct in_addr *) host->h_addr)->s_addr;
    }
    memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));
    int conn = connect(socketfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
    if (conn == -1) {
        LOGE("connect error");
        LOGE("error (errno=%d)", errno);
        exit(1);
    }
    int sen = send(socketfd, c, strlen(c), 0);
    if (sen == -1) {
        LOGE("send errorrr");
        LOGE("error (errno=%d)", errno);
        exit(1);
    }
    while (recv(socketfd, buff, 1023, 0) > 0) {
        LOGE("%s", buff);
        strcpy(res, buff);
    }
    close(socketfd);
    LOGE("send successssss");
    return res;

}

/*逍遥模拟器
 * 12-13 12:20:58.671 1615-1615/? E/qtfreet00: the /system/bin/microvirt-prop is exist
12-13 12:20:58.671 1615-1615/? E/qtfreet00: the /system/bin/microvirtd is exist
12-13 12:20:58.671 1615-1615/? E/qtfreet00: the init.svc.vbox86-setup result is stopped
12-13 12:20:58.671 1615-1615/? E/qtfreet00: the init.svc.microvirtd result is running*/

//void handler_sigtrap(int signo) {
//    exit(-1);
//}
//
//void handler_sigbus(int signo) {
//    exit(-1);
//}
//
//int setupSigTrap() {
//    // BKPT throws SIGTRAP on nexus 5 / oneplus one (and most devices)
//    signal(SIGTRAP, handler_sigtrap);
//    // BKPT throws SIGBUS on nexus 4
//    signal(SIGBUS, handler_sigbus);
//}
//
//// This will cause a SIGSEGV on some QEMU or be properly respected
//int tryBKPT() {
//    __asm__ __volatile__ ("bkpt 255");
//}

JNIEXPORT jstring JNICALL
Java_com_qtfreet_anticheckemulator_emulator_JniAnti_getCpuinfo(JNIEnv *env, jobject instance) {

    char *res = getCpuInfo();

    return env->NewStringUTF(res);
}

JNIEXPORT jstring JNICALL
Java_com_qtfreet_anticheckemulator_emulator_JniAnti_getKernelVersion(JNIEnv *env, jclass type) {

    char *res = getVersionInfo();

    return env->NewStringUTF(res);
}

jstring
Java_com_qtfreet_anticheckemulator_emulator_JniAnti_stringFromJNI(
        JNIEnv *env,
        jobject /* this */, jstring str) {
    //目前已知问题，检测/sys/class/thermal/和bluetooth-jni.so不稳定，存在兼容性问题
    check();
    // testBluetooth();
    char *res = getCpuInfo();
    LOGE("the couuuuu is %s", res);
    getVersionInfo();
    getDeviceInfo();
    //  checkTemp();
    getDeviceID(env);
    checkBattery();
//    setupSigTrap();
//    char *sign = verifySign(env);
//    char *split = ":";
//    char *id = jstringToChar(env, str);
//    int len = strlen(sign) + strlen(id) + strlen(split) + 1;
//    char *s = (char *) malloc(len);
//    memset(s, 0, len);
//    strcat(s, id);
//    strcat(s, split);
//    strcat(s, sign);
//
//    char *test = SocketTest(s);
    return env->NewStringUTF(res);

}
}
