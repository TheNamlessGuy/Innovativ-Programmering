package com.example.maxbr431.myapplication;
import android.util.Log;

import java.util.ArrayList;

/**
 * Created by maxbr431 on 2015-11-23.
 */
public class Playlist {
    private int current;
    private ArrayList<String> list;
    private boolean loopAll;
    private boolean loopCurrent;

    public Playlist() {
        current = 0;
        list = new ArrayList<String>();
        loopAll = true;
        loopCurrent = false;
    }

    public void add(String newPath) {
        list.add(newPath);
    }

    public void add(ArrayList<String> newList) {
        list.addAll(newList);
    }

    public void changeTo(String path) {
        current = list.indexOf(path);
    }

    public boolean remove(int location) {
        list.remove(location);
        if (current > location)
            current--;
        if (location == current) {
            current = 0;
            return true;
        }
        return false;
    }

    public String current() {
        if (list.size() < current) return null;
        if (0 > current) return null;
        return list.get(current);
    }

    public void next() {
        if (list.size() == 0 || loopCurrent) return; //Loop current, do nothing
        if (loopAll) //Loop the entire playlist
            current = (current + 1) % list.size();
        else if (current != list.size() - 1) { //Loop nothing
            current += 1;
        }
    }

    public void prev() {
        if (list.size() == 0 || loopCurrent) return; //Loop current, do nothing
        //if loop all or no looping, get previous
        current -= 1;
        if (current < 0)
            current = list.size() - 1;
    }

    public int size() {
        return list.size();
    }

    public void clear() {
        list.clear();
        current = 0;
    }

    public void loopAll() {
        loopCurrent = false;
        loopAll = true;
    }

    public void loopCurrent() {
        loopCurrent = true;
        loopAll = false;
    }

    public void loopNone() {
        loopCurrent = false;
        loopAll = false;
    }

    public int getIndex() {
        return current;
    }

    public ArrayList<String> getPlaylist() {
        return list;
    }

    public void setCurrent (int newCurrent) {
        current = newCurrent;
    }
}
