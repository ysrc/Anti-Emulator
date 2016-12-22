package com.qtfreet.anticheckemulator.emulator;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.system.Os;
import android.telephony.TelephonyManager;

import com.qtfreet.anticheckemulator.utils.Util;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import static android.content.Context.SENSOR_SERVICE;

/**
 * Created by qtfreet on 2016/12/22.
 */

public class Check {


    public static boolean checkGravity(Context context) {
        boolean z = true;
        List<Sensor> defaultSensor = ((SensorManager) context.getSystemService(SENSOR_SERVICE)).getSensorList(Sensor.TYPE_ALL);
        for (Sensor sensor : defaultSensor) {
//            Log.e("qtfreet00",sensor.getName());
//            Log.e("qtfreet00",sensor.getVendor());
            if (sensor.getName().equals("Gravity")) {
                z = false;
                break;
            }
        }
        return z;
    }


    public static List<String> getAllSensors(Context context) {
        List<String> list = new ArrayList<>();
        List<Sensor> defaultSensor = ((SensorManager) context.getSystemService(SENSOR_SERVICE)).getSensorList(Sensor.TYPE_ALL);
        for (Sensor sensor : defaultSensor) {
            list.add(sensor.getName());
        }
        return list;
    }


    public static int getVersionCode(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
            if (packageInfo != null) {
                return packageInfo.versionCode;
            }
            return 0;
        } catch (Throwable th) {
            return 0;
        }
    }

    public static String getVersionName(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
            if (packageInfo != null) {
                return packageInfo.versionName;
            }
            return null;
        } catch (Throwable th) {
            return null;
        }
    }


    public static String getInstalledApps(Context context) {
        List<PackageInfo> installedPackages = context.getPackageManager().getInstalledPackages(0);
        HashMap<String, String> map = new HashMap<>();
        for (PackageInfo p : installedPackages) {
            String packageName = p.packageName;
            String versionName = p.versionName;
            map.put(packageName, versionName);
        }
        return Util.hashMapToStringNoSort(map);

    }

    public static String getPackageName(Context context) {
        return context.getPackageName();
    }


    public static boolean checkMultiTouch(Context context) {
        boolean z = false;
        try {
            z = context.getPackageManager().hasSystemFeature("android.hardware.touchscreen.multitouch");
        } catch (Exception e) {
            e.printStackTrace();
        }
        return z;

    }

    public static String getModelName() {
        return Build.MODEL;
    }

    public static String getModelBrand() {
        return Build.BRAND;
    }


    public static String getMacAddress(Context context) {
        String str = "";
        try {
            WifiManager wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
            if (wifiManager != null) {
                WifiInfo connectionInfo = wifiManager.getConnectionInfo();
                return connectionInfo == null ? "" : connectionInfo.getMacAddress();
            }
        } catch (Throwable th) {
        }
        return str;

    }

    public static String getDeviceID(Context context) {
        String str = null;
        TelephonyManager telephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
        if (telephonyManager == null) {
            return str;
        }
        str = telephonyManager.getDeviceId();
        return str;
    }

    public static String getMemorySize() {
        String dir = "/proc/meminfo";

        FileReader fr = null;
        int size = 0;
        try {
            fr = new FileReader(dir);
            BufferedReader br = new BufferedReader(fr, 2048);
            String memoryLine = br.readLine();
            String subMemoryLine = memoryLine.substring(memoryLine.indexOf("MemTotal:"));
            br.close();
            long j = Long.parseLong(subMemoryLine.substring(subMemoryLine.indexOf(58) + 1, subMemoryLine.indexOf("kB")).trim());
            size = (int) (j / 1024);
        } catch (FileNotFoundException e) {
            // e.printStackTrace();
        } catch (IOException e) {
            //  e.printStackTrace();
        }

        if (size < 768) {
            return size + "M";
        }
        if (size < 1024) {
            return "1G";
        }
        return String.format("%.1fG", new Object[]{Float.valueOf(((float) size) / 1024.0f)});
    }

}
