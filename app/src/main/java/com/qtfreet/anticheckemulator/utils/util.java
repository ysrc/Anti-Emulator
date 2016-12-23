package com.qtfreet.anticheckemulator.utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Created by qtfreet on 2016/12/22.
 */

public class Util {
    public static String hashMapToStringNoSort(HashMap<String, String> paramMap) {
        Set<String> listKeys = paramMap.keySet();
        int length = listKeys.size();
        List<String> list = new ArrayList();
        for (String add : listKeys) {
            list.add(add);
        }
        String kvString = "";
        for (int i = 0; i < length; i++) {
            String key = list.get(i);
            if (i == length - 1) {
                kvString = kvString + key + "=" + paramMap.get(key);
            } else {
                kvString = kvString + key + "=" + paramMap.get(key) + "&";
            }
        }
        return kvString;
    }

    public static String hashMapToStringSort(HashMap<String, String> paramMap) {
        Set<String> listKeys = paramMap.keySet();
        int length = listKeys.size();
        List<String> list = new ArrayList();
        for (String add : listKeys) {
            list.add(add);
        }
        Collections.sort(list); //进行排序
        String kvString = "";
        for (int i = 0; i < length; i++) {
            String key = list.get(i);
            if (i == length - 1) {
                kvString = kvString + key + "=" + paramMap.get(key);
            } else {
                kvString = kvString + key + "=" + (paramMap.get(key)) + "&";
            }
        }
        return kvString;
    }

    public static String readFile(String str) {
        File file = new File(str);
        StringBuilder sb = new StringBuilder();
        if (file.exists()) {
            try {
                String line = null;
                FileInputStream fileInputStream = new FileInputStream(file);
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(fileInputStream));
                while ((line = bufferedReader.readLine()) != null) {
                    sb.append(line);
                }
                bufferedReader.close();
                fileInputStream.close();
                return sb.toString();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return "";
    }

    public static String convertSize(int i) {
        int size = i / 1000;
        if (size > 360 && size < 440) {
            return "400M";
        }
        if (size > 460 && size < 540) {
            return "500M";
        }
        if (size > 560 && size < 640) {
            return "600M";
        }
        if (size > 660 && size < 740) {
            return "700M";
        }
        if (size > 760 && size < 840) {
            return "800M";
        }
        if (size > 860 && size < 940) {
            return "900M";
        }
        if (size > 960 && size < 1040) {
            return "1G";
        }
        if (size < 1000) {
            return String.format("%dM", new Object[]{Integer.valueOf(size)});
        }
        return String.format("%.1fG", new Object[]{Float.valueOf(((float) size) / 1000.0f)});
    }

    public static String tempToStr(float temp, int tempSetting) {
        if (temp <= 0.0f) {
            return "";
        }
        if (tempSetting == 2) {
            return String.format("%.1f°F", new Object[]{Float.valueOf(((9.0f * temp) + 160.0f) / 5.0f)});
        }
        return String.format("%.1f°C", new Object[]{Float.valueOf(temp)});
    }

}
