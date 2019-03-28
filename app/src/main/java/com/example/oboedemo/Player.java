package com.example.oboedemo;

/**
 * @author yetote QQ:503779938
 * @name OboeDemo
 * @class name：com.example.oboedemo
 * @class describe
 * @time 2019/3/28 11:28
 * @change
 * @chang time
 * @class describe
 */
public class Player {
    static {
        System.loadLibrary("native-lib");
    }

    public native void create(String path);
}
