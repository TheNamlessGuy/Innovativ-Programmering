package com.example.maxbr431.myapplication;

import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Map;
import java.util.Stack;
import java.util.TreeMap;

public class MusicChoosingActivity extends AppCompatActivity {
    public static final short NONE = -1;
    public static final short SONGS = 0;
    public static final short ALBUMS = 1;
    public static final short PLAYLIST = 2;
    public static final short ARTISTS = 3;
    public static final short TRACKS = 4;

    public static ArrayList<String> playlist;
    static TreeMap<String, MusicFile> songs; // Maps path -> song file
    TreeMap<String, ArrayList<String>> artists; // Maps ARTIST -> ALBUMS
    TreeMap<String, ArrayList<String>> albums; // Maps ALBUMS -> SONGS

    Stack<Pair<String, String>> backtrackingStack;

    public static short currentlyShowing;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        currentlyShowing = MusicChoosingActivity.NONE;

        artists = new TreeMap<>();
        albums = new TreeMap<>();
        backtrackingStack = new Stack<>();
        songs = new TreeMap<>();
        if (playlist == null)
            playlist = new ArrayList<>();
        loadMusic();

        setContentView(R.layout.activity_music_choosing);

        FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
        transaction.add(R.id.list_fragment, new MusicChoosingFragment());
        transaction.add(R.id.button_fragment, new ButtonFragment());
        transaction.commit();
    }

    public void loadMusic() {
        Thread musicLoadingThread = new Thread(new Runnable() {
            @Override
            public void run() {
                scanDirectories();
                isReady();
            }
        });
        musicLoadingThread.start();
    }

    public void scanDirectories() {
        //Only grab music files
        String selection = MediaStore.Audio.Media.IS_MUSIC + " != 0";
        //Get this data from the files "selection" filters through
        final String[] projection = new String[] {
                MediaStore.Audio.Media.ARTIST,
                MediaStore.Audio.Media.ALBUM,
                MediaStore.Audio.Media.TITLE,
                MediaStore.Audio.Media.DATA,
                MediaStore.Audio.Media.TRACK
        };
        //Sort by title. COLLATE = Sort, LOCALIZED = Local keyboard, ASC = Ascending
        final String sortOrder = MediaStore.Audio.AudioColumns.TITLE + " COLLATE LOCALIZED ASC";

        Cursor cursor = null;
        try {
            Uri uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
            cursor = getBaseContext().getContentResolver().query(uri, projection, selection, null, sortOrder);

            if (cursor == null) return;

            cursor.moveToFirst();
            while (!cursor.isAfterLast()) {
                String[] artists = cursor.getString(0).split("/");

                for (String artist : artists) {
                    MusicFile mf = new MusicFile();

                    mf.ARTIST = artist;
                    mf.ALBUM = cursor.getString(1);
                    mf.TITLE = cursor.getString(2);
                    mf.PATH = cursor.getString(3);
                    mf.TRACK = cursor.getInt(4);

                    songs.put(mf.PATH, mf);

                    if (!this.artists.containsKey(mf.artist()))
                        this.artists.put(mf.artist(), new ArrayList<String>());
                    if (!this.artists.get(mf.artist()).contains(mf.album()))
                        this.artists.get(mf.artist()).add(mf.album());

                    if (!this.albums.containsKey(mf.album()))
                        this.albums.put(mf.album(), new ArrayList<String>());
                    this.albums.get(mf.album()).add(mf.path());
                }
                cursor.moveToNext();
            }
        } catch (Exception e) {
            Log.e("ERROR", "MusicChoosingActivity.scanDirectories.1");
            e.printStackTrace();
        }
        if (cursor != null)
            cursor.close();

        // Sort albums after track value
        for (Map.Entry<String, ArrayList<String>> album : albums.entrySet()) {
            Collections.sort(album.getValue(), new Comparator<String>() {
                @Override
                public int compare(String lhs, String rhs) {
                    return Double.compare(getMusicFile(lhs).track(), getMusicFile(rhs).track());
                }
            });
        }
    }

    public void isReady() {
        Intent intent = new Intent(this, MusicWidget.class);
        intent.setAction("MAINACTIVITY_READY");
        sendBroadcast(intent);

        setArtistList();
    }

    public void setSongList() {
        // SHOW ALL SONGS
        if (currentlyShowing == MusicChoosingActivity.SONGS) return;
        currentlyShowing = MusicChoosingActivity.SONGS;
        ArrayList<String> musicList = new ArrayList<>();
        for (Map.Entry<String, MusicFile> entry : songs.entrySet())
            musicList.add(entry.getValue().path());
        Collections.sort(musicList, new Comparator<String>() {
            @Override
            public int compare(String lhs, String rhs) {
                return getMusicFile(lhs).title().compareTo(getMusicFile(rhs).title());
            }
        });

        Fragment fragment = getSupportFragmentManager().findFragmentById(R.id.list_fragment);
        if (fragment instanceof MusicChoosingFragment)
            ((MusicChoosingFragment) fragment).setSongs(musicList);
    }

    public void setPlaylistSongList() {
        // SHOW ALL SONGS IN PLAYLIST
        currentlyShowing = MusicChoosingActivity.TRACKS;
        // Won't be used, but needed to have current pop
        backtrackingStack.add(new Pair<>("", ""));

        Fragment fragment = getSupportFragmentManager().findFragmentById(R.id.list_fragment);
        if (fragment instanceof MusicChoosingFragment)
            ((MusicChoosingFragment) fragment).setPlaylist(playlist);
    }

    public void setAlbumSongList(String album) {
        // SHOW ALL SONGS FOR CERTAIN ALBUM
        if (currentlyShowing == MusicChoosingActivity.TRACKS) return;
        currentlyShowing = MusicChoosingActivity.TRACKS;
        // Won't be used, but needed to have current pop
        backtrackingStack.add(new Pair<>("", ""));

        ArrayList<String> musicList = new ArrayList<>();
        musicList.addAll(albums.get(album));

        Fragment fragment = getSupportFragmentManager().findFragmentById(R.id.list_fragment);
        if (fragment instanceof MusicChoosingFragment)
            ((MusicChoosingFragment) fragment).setSongs(musicList);
    }

    public void setAlbumList() {
        // SHOW ALL ALBUMS
        if (currentlyShowing == MusicChoosingActivity.ALBUMS) return;
        currentlyShowing = MusicChoosingActivity.ALBUMS;
        ArrayList<String> musicList = new ArrayList<>();
        musicList.addAll(albums.keySet());

        Fragment fragment = getSupportFragmentManager().findFragmentById(R.id.list_fragment);
        if (fragment instanceof MusicChoosingFragment)
            ((MusicChoosingFragment) fragment).setAlbumList(musicList);
    }

    public void setArtistsAlbumList(String artist) {
        // SHOW ALL ALBUMS FOR ARTIST
        if (currentlyShowing == MusicChoosingActivity.ALBUMS) return;
        currentlyShowing = MusicChoosingActivity.ALBUMS;

        backtrackingStack.add(new Pair<>("SETARTISTSALBUMLIST", artist));

        ArrayList<String> musicList = new ArrayList<>();
        musicList.addAll(artists.get(artist));

        Fragment fragment = getSupportFragmentManager().findFragmentById(R.id.list_fragment);
        if (fragment instanceof MusicChoosingFragment)
            ((MusicChoosingFragment) fragment).setAlbums(musicList);
    }

    public void setArtistList() {
        // SHOW ALL ARTISTS
        if (currentlyShowing == MusicChoosingActivity.ARTISTS) return;
        currentlyShowing = MusicChoosingActivity.ARTISTS;

        backtrackingStack.clear();
        backtrackingStack.add(new Pair<>("SETARTISTLIST", ""));

        ArrayList<String> musicList = new ArrayList<>();
        musicList.addAll(artists.keySet());

        Fragment fragment = getSupportFragmentManager().findFragmentById(R.id.list_fragment);
        if (fragment instanceof MusicChoosingFragment)
            ((MusicChoosingFragment) fragment).setArtists(musicList);
    }

    public static MusicFile getMusicFile(String path) {
        return songs.get(path);
    }

    public void setCurrentPlaylist(String path) {
        playlist.clear();
        playlist.add(path);

        Intent intent = new Intent(this, MusicPlayingService.class);
        intent.setAction("PLAY");
        intent.putExtra("PATH", path);

        startService(intent);
    }

    public void setCurrentPlaylistToList(ArrayList<String> paths) {
        playlist.clear();
        playlist.addAll(paths);

        Intent intent = new Intent(this, MusicPlayingService.class);
        intent.setAction("SETPLAYLISTTOLIST");
        intent.putExtra("FILES", paths);

        startService(intent);
    }

    public void setCurrentPlaylistToAlbum(String album) {
        setCurrentPlaylistToList(albums.get(album));
    }

    public void addSongToCurrentPlaylist(String path) {
        playlist.add(path);

        Intent intent = new Intent(this, MusicPlayingService.class);
        intent.setAction("ADDTOPLAYLIST");
        intent.putExtra("PATH", path);

        startService(intent);
    }

    public void addAlbumToCurrentPlaylist(String album) {
        playlist.addAll(albums.get(album));

        Intent intent = new Intent(this, MusicPlayingService.class);
        intent.setAction("ADDALBUMTOPLAYLIST");
        intent.putExtra("FILES", albums.get(album));

        startService(intent);
    }

    public void removeFromPlaylist(int location) {
        playlist.remove(location);

        Intent intent = new Intent(this, MusicPlayingService.class);
        intent.setAction("REMOVEFROMPLAYLIST");
        intent.putExtra("Value", location);
        startService(intent);
    }

    @Override
    public void onBackPressed() {
        backtrackingStack.pop(); // Remove the current stackElement
        if (backtrackingStack.isEmpty()) {
            moveTaskToBack(true);
            return;
        }

        Pair<String, String> stackElement = backtrackingStack.pop();
        switch (stackElement.first()) {
            case "SETARTISTSALBUMLIST":
                setArtistsAlbumList(stackElement.second());
                break;
            case "SETARTISTLIST":
                setArtistList();
                break;
            default:
                break;
        }
    }
}
