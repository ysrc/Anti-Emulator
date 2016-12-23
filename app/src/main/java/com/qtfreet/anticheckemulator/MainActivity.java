package com.qtfreet.anticheckemulator;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.qtfreet.anticheckemulator.emulator.Check;
import com.qtfreet.anticheckemulator.emulator.JniAnti;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);

        tv.setText(JniAnti.getApkSign() + "\n\n" + JniAnti.getCpuinfo() + "\n" + JniAnti.getKernelVersion());

        Log.e("qtfreet000", "APK签名：" + JniAnti.getApkSign());
        Log.e("qtfreet000", "CPU信息：" + JniAnti.getCpuinfo());
        Log.e("qtfreet000", "内核信息：" + JniAnti.getKernelVersion());
        Log.e("qtfreet000", "设备ID：" + JniAnti.getDeviceID());
        Log.e("qtfreet000", "已安装App：" + Check.getInstalledApps(this));
        Log.e("qtfreet000", "MAC地址：" + Check.getMacAddress(this));
        Log.e("qtfreet000", "内存大小：" + Check.getMemorySize());
        Log.e("qtfreet000", "存在重力感应器：" + Check.checkGravity(this));
        Log.e("qtfreet000", "设备厂商：" + Check.getModelBrand());
        Log.e("qtfreet000", "设备型号：" + Check.getModelName());
        ;
    }

}
