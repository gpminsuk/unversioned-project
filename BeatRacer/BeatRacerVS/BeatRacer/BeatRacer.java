package com.example.hellojni;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

public class BeatRacer extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mGLView = new BeatRacerGLSurfaceView(this);
        setContentView(mGLView);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }

    private GLSurfaceView mGLView;

    static {
        System.loadLibrary("BeatRacer");
    }
}

class Track
{
	public Track()
	{
		Initialize();
	}

    private static native void Initialize();
    private static native void Draw();
}

class BeatRacerGLSurfaceView extends GLSurfaceView {
    public BeatRacerGLSurfaceView(Context context) {
        super(context);
        mRenderer = new GI();
        setRenderer(mRenderer);
    }

    public boolean onTouchEvent(final MotionEvent event) {
		return false;
    }

    GI mRenderer;
}

class GI implements GLSurfaceView.Renderer {
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Initialize();
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
    }
    
    public void onDrawFrame(GL10 gl) {
        Draw();
    }

    private static native void Initialize();
    private static native void Draw();
}
