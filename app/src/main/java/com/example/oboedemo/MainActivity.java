package com.example.oboedemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    Player player;
    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        player = new Player();
        String path = this.getExternalCacheDir().getPath() + "/res/test.pcm";
        Log.e(TAG, "onCreate: " + path);
        player.create(path);
    }

}
