package com.example.maxbr431.myapplication;

import android.util.Log;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

/**
 * Created by maxbr431 on 2015-11-16.
 */
public class MusicFile implements Serializable {
    public String ARTIST;
    public String ALBUM;
    public String TITLE;
    public String PATH;
    public int TRACK;

    public String artist() {
        if (ARTIST.isEmpty())
            return "ARTIST";
        return ARTIST;
    }

    public String album() {
        if (ALBUM.isEmpty())
            return "ALBUM";
        return ALBUM;
    }

    public String title() {
        if (TITLE.isEmpty())
            return "TITLE";
        return TITLE;
    }

    public String path() {
        if (PATH.isEmpty())
            return "PATH";
        return PATH;
    }

    public int track() {
        return TRACK;
    }

    private void writeObject(ObjectOutputStream o) throws IOException {
        o.writeObject(ARTIST);
        o.writeObject(ALBUM);
        o.writeObject(TITLE);
        o.writeObject(PATH);
        o.writeObject(TRACK);
    }

    private void readObject(ObjectInputStream o) throws IOException, ClassNotFoundException {
        ARTIST = (String) o.readObject();
        ALBUM = (String) o.readObject();
        TITLE = (String) o.readObject();
        PATH = (String) o.readObject();
        TRACK = (int) o.readObject();
    }

    public boolean equals(Object object) {
        if (object instanceof MusicFile) {
            MusicFile mf = (MusicFile) object;
            if (mf.PATH.equals(PATH))
                return true;
        }
        return false;
    }
}
