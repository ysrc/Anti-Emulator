package com.qtfreet.anticheckemulator.emulator;

/**
 * Created by qtfreet on 2016/12/22.
 */

public class JniAnti {
    static {
        System.loadLibrary("native-lib");
    }

    public static native String getCpuinfo();

    public static native String getApkSign();

    public static native String getKernelVersion();

    public static native String getDeviceID(); //优测测试时提示没有权限读取read_phone_state，这里已经Mainifest注册

    public static native int checkAntiFile();
}
