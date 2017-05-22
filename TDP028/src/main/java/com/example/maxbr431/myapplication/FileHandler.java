package com.example.maxbr431.myapplication;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Map;
import java.util.TreeMap;

/**
 * Created by maxbr431 on 2015-12-10.
 */
public class FileHandler {
    public static void savePlaylists(Context context, ArrayList<String> playlist, int currentIndex, short loopMode) {
        try {
            BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(new File(context.getFilesDir() + "/playlists.txt")));
            bufferedWriter.write(Integer.toString(currentIndex));
            bufferedWriter.newLine();
            bufferedWriter.write(Short.toString(loopMode));
            bufferedWriter.newLine();
            for (String path : playlist) {
                bufferedWriter.write(path);
                bufferedWriter.newLine();
            }
            bufferedWriter.flush();
            bufferedWriter.close();
        } catch (Exception e) {
            Log.e("ERROR", "FileHandler.savePlaylists.1");
            e.printStackTrace();
        }
    }

    public static Pair<ArrayList<String>, Integer[]> readPlaylists(Context context) {
        ArrayList<String> playlist = new ArrayList<>();
        int index = 0;
        int loopMode = 0;

        try {
            BufferedReader bufferedReader = new BufferedReader(new FileReader(new File(context.getFilesDir() + "/playlists.txt")));
            index = Integer.parseInt(bufferedReader.readLine());
            loopMode = Integer.parseInt(bufferedReader.readLine());
            String line = bufferedReader.readLine();

            while (line != null) {
                playlist.add(line);
                line = bufferedReader.readLine();
            }
            bufferedReader.close();
        } catch (Exception e) {
            Log.e("ERROR", "FileHandler.readPlaylists.1");
            e.printStackTrace();
        }

        Integer[] array = new Integer[2];
        array[0] = index;
        array[1] = loopMode;
        Pair<ArrayList<String>, Integer[]> return_value = new Pair<>(playlist, array);
        return return_value;
    }
}
