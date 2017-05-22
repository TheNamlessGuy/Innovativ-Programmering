package com.example.maxbr431.myapplication;

/**
 * Created by maxbr431 on 2015-12-10.
 */
public class Pair<F, S> {
    private F first;
    private S second;

    public Pair(F first, S second) {
        this.first = first;
        this.second = second;
    }

    public F first() {
        return first;
    }

    public S second() {
        return second;
    }
}
