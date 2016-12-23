package com.qtfreet.anticheckemulator.emulator;

import android.content.Context;
import android.util.AttributeSet;

/**
 * Created by qtfreet on 2016/12/23.
 */

public class GLSurfaceView extends android.opengl.GLSurfaceView {

    public GLSurfaceView(Context context) {
        super(context);
        init();
    }

    public GLSurfaceView(Context context, AttributeSet attributeSet) {
        super(context, attributeSet);
        init();
    }

    private void init() {
        setEGLConfigChooser(false);
        GpuRender gpuRender = new GpuRender();
        setRenderer(gpuRender);
    }
}
