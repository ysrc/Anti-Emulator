package com.qtfreet.anticheckemulator.utils;

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
}
