package com.qtfreet.anticheckemulator.emulator;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by qtfreet on 2016/12/23.
 */

public class GpuRender implements GLSurfaceView.Renderer {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        String vendor = gl.glGetString(GL10.GL_VENDOR);
        String renderer = gl.glGetString(GL10.GL_RENDERER);
        Log.e("qtfreet000", "显卡信息：" + vendor +" "+ renderer);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }
}
