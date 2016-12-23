package com.qtfreet.anticheckemulator.emulator;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.util.Log;

import com.qtfreet.anticheckemulator.utils.Util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileFilter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.regex.Pattern;

import static android.content.Context.SENSOR_SERVICE;

/**
 * Created by qtfreet on 2016/12/22.
 */

public class Check {
    static String a = "/cpufreq/cpuinfo_max_freq";
    static String b = "/cpufreq/cpuinfo_min_freq";
    static String c = "/cpufreq/scaling_cur_freq";


    public static boolean checkGravity(Context context) {
        boolean z = false;
        List<Sensor> defaultSensor = ((SensorManager) context.getSystemService(SENSOR_SERVICE)).getSensorList(Sensor.TYPE_ALL);
        for (Sensor sensor : defaultSensor) {
            //   Log.e("qtfreet000", sensor.getName());
//            Log.e("qtfreet00",sensor.getVendor());
            if (sensor.getName().equals("Gravity")) {
                z = true;
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

    public static int getCpuCore() {
        try {
            return new File("/sys/devices/system/cpu/").listFiles(new FileFilter() {
                @Override
                public boolean accept(File pathname) {
                    return Pattern.matches("cpu[0-9]", pathname.getName());
                }
            }).length;

        } catch (Exception e) {
            return 0;
        }
    }

    public static String getCpuFrequency() {
        String frequency = Util.convertSize(getCpuMaxFrequency());
        String model = Build.MODEL;
        if (Build.BRAND.equalsIgnoreCase("samsung") && (model.equalsIgnoreCase("sch-i959") || model.equalsIgnoreCase("gt-i9500"))) {
            frequency = frequency + " " + "四核+四核";
            return frequency;
        }
        switch (getCpuCore()) {
            case 1:
                frequency = frequency + " " + "单核";
                break;
            case 2:
                frequency = frequency + " " + "双核";
                break;
            case 4:
                frequency = frequency + " " + "四核";
                break;
            case 6:
                frequency = frequency + " " + "六核";
                break;
            case 8:
                frequency = frequency + " " + "八核";
                break;

        }
        return frequency.trim();
    }

    private static int getCpuMaxFrequency() {
        File file = new File("/sys/devices/system/cpu");
        if (!file.exists()) {
            return 0;
        }
        File[] listFiles = file.listFiles(new FileFilter() {
            @Override
            public boolean accept(File pathname) {
                return Pattern.matches("cpu[0-9]", pathname.getName());
            }
        });
        if (listFiles == null || listFiles.length <= 0) {
            return 0;
        }
        List arrayList = new ArrayList();
        for (File absolutePath : listFiles) {
            String path = absolutePath.getAbsolutePath();
            try {
                int max = Math.max(Math.max(Integer.parseInt(Util.readFile(path + a)), Integer.parseInt(Util.readFile(path + c))), Integer.parseInt(Util.readFile(path + b)));
                if (max > 0) {
                    arrayList.add(Integer.valueOf(max));
                }
            } catch (Throwable th) {
            }
        }
        if (arrayList.isEmpty()) {
            return 0;
        }
        Collections.sort(arrayList);
        return ((Integer) arrayList.get(arrayList.size() - 1)).intValue();
    }


}
