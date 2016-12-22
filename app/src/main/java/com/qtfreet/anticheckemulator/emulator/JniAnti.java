package com.qtfreet.anticheckemulator.emulator;

/**
 * Created by qtfreet on 2016/12/22.
 */

public class JniAnti {
    static {
        System.loadLibrary("native-lib");
    }

    public static native String getCpuinfo();

    public static native String stringFromJNI(String str);

    public static native String getKernelVersion();
}
