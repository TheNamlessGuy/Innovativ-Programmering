package com.example.maxbr431.myapplication;

import android.app.Notification;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.IBinder;
import android.support.v7.app.NotificationCompat;
import android.util.Log;

import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

/**
 * Created by maxbr431 on 2015-11-16.
 */
public class MusicPlayingService extends Service {
    private MediaPlayer mediaPlayer;
    private AudioManager audioManager;
    private Boolean paused;

    private Playlist currentPlaylist;

    private Thread sendProgressionUpdateThread;
    private Runnable sendProgressUpdateRunnable;

    @Override
    public int onStartCommand(Intent intent, int flags, int startID) {
        if (intent != null && intent.getAction() != null) {
            switch (intent.getAction()) {
                case "PLAY":
                    play(intent);
                    break;
                case "TOGGLEPLAY":
                    togglePlay();
                    break;
                case "STOP":
                    stop();
                    break;
                case "NEXT":
                    next();
                    break;
                case "PREV":
                    prev();
                    break;
                case "ADDTOPLAYLIST":
                    addToPlaylist(intent);
                    break;
                case "ADDALBUMTOPLAYLIST":
                    addAlbumToPlaylist(intent);
                    break;
                case "LOOPCLICK":
                    loopClick(intent);
                    break;
                case "GETPLAYLIST":
                    sendPlaylist();
                    break;
                case "CHANGECURRENTLYPLAYING":
                    changeCurrentlyPlaying(intent);
                    break;
                case "REMOVEFROMPLAYLIST":
                    removeFromPlaylist(intent);
                    break;
                case "SETPLAYLISTTOLIST":
                    setPlaylist(intent);
                    break;
                default:
                    break;
            }
        }
        return START_NOT_STICKY;
    }

    public void setPlaylist(Intent intent) {
        currentPlaylist.clear();
        currentPlaylist.add((ArrayList<String>) intent.getSerializableExtra("FILES"));
        currentPlaylist.setCurrent(intent.getIntExtra("INDEX", 0));
        if (intent.getBooleanExtra("PLAY", true)) {
            paused = false;
            playCurrent();
        } else {
            setCurrentSong();
            paused = true;
        }
        sendNewSongToWidget();
    }

    public void removeFromPlaylist(Intent intent) {
        int location = intent.getIntExtra("Value", -1);
        if (currentPlaylist.remove(location))
            stop();
        sendPlaylist();
    }

    public void changeCurrentlyPlaying(Intent intent) {
        currentPlaylist.changeTo(intent.getStringExtra("PATH"));
        paused = false;
        if (intent.getBooleanExtra("PLAY", true))
            playCurrent();
        else
            paused = true;
        sendNewSongToWidget();
    }

    public void sendPlaylist() {
        Intent intent = new Intent();
        intent.setAction("GETPLAYLIST");
        intent.putExtra("value", currentPlaylist.getPlaylist());

        getBaseContext().sendBroadcast(intent);
    }

    public void play(Intent intent) {
        currentPlaylist.clear();
        addToPlaylist(intent);
        playCurrent();
        sendNewSongToWidget();
    }

    public void togglePlay() {
        if (audioManager.isMusicActive() && !paused) { // If playing
            mediaPlayer.pause();
            paused = true;
        } else if (!audioManager.isMusicActive() && paused) { // If paused
            playCurrent();
            paused = false;
        } else if (!audioManager.isMusicActive() && !paused) { // If stopped
            playCurrent();
        }
    }

    public void stop() {
        if (audioManager.isMusicActive()) {
            mediaPlayer.stop();
        }
        paused = false;
    }

    public void playCurrent() {
        if (currentPlaylist.size() == 0) return;
        if (currentPlaylist.current() == "PATH") return;
        if (!paused)
            setCurrentSong();
        mediaPlayer.start();

        sendProgressionUpdateThread = new Thread(sendProgressUpdateRunnable);
        sendProgressionUpdateThread.start();
    }

    public void next() {
        currentPlaylist.next();
        sendNewSongToWidget();
        if (!paused)
            playCurrent();
        else
            setCurrentSong();
    }

    public void prev() {
        currentPlaylist.prev();
        if (paused)
            setCurrentSong();
        else
            playCurrent();
        sendNewSongToWidget();
    }

    public void addToPlaylist(Intent intent) {
        currentPlaylist.add(intent.getStringExtra("PATH"));
    }

    public void addAlbumToPlaylist(Intent intent) {
        ArrayList<String> temp = (ArrayList<String>) intent.getSerializableExtra("FILES");
        currentPlaylist.add(temp);
    }

    public void loopClick(Intent intent) {
        switch (intent.getShortExtra("VALUE", (short) -1)) {
            case MusicWidget.ALL:
                currentPlaylist.loopAll();
                break;
            case MusicWidget.ONE:
                currentPlaylist.loopCurrent();
                break;
            case MusicWidget.NONE:
                currentPlaylist.loopNone();
                break;
            default:
                break;
        }
    }

    public void sendNewSongToWidget() {
        if (currentPlaylist.size() == 0) return;
        Intent intent = new Intent(getBaseContext(), MusicWidget.class);
        intent.setAction("NEWSONGPLAYING");
        intent.putExtra("PATH", currentPlaylist.current());
        intent.putExtra("PAUSED", paused);
        intent.putExtra("INDEX", currentPlaylist.getIndex());

        long millis = mediaPlayer.getDuration();
        int minutes = (int) TimeUnit.MILLISECONDS.toMinutes(millis);
        millis -= TimeUnit.MINUTES.toMillis(minutes);
        int seconds = (int) TimeUnit.MILLISECONDS.toSeconds(millis);

        intent.putExtra("TOTTIME", String.format("%02d:%02d", minutes, seconds));
        getBaseContext().sendBroadcast(intent);
    }

    public void setCurrentSong() {
        try {
            mediaPlayer.reset();
            mediaPlayer.setDataSource(currentPlaylist.current());
            mediaPlayer.prepare();
        } catch (Exception e) {
            Log.e("ERROR", "MusicPlayingService.setCurrentSong.1");
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        mediaPlayer = new MediaPlayer();

        mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            public void onCompletion(MediaPlayer mp) {
                if (paused) return;
                Intent intent = new Intent(getBaseContext(), MusicPlayingService.class);
                intent.setAction("NEXT");
                getBaseContext().startService(intent);
            }
        });

        audioManager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
        paused = false;
        currentPlaylist = new Playlist();

        Notification notification = new NotificationCompat.Builder(this).build();
        startForeground(3141, notification);

        sendProgressUpdateRunnable = new Runnable() {
            @Override
            public void run() {
                while (audioManager.isMusicActive()) {
                    Intent intent = new Intent(getBaseContext(), MusicWidget.class);
                    intent.setAction("NEWPLAYTIME");

                    long millis = mediaPlayer.getCurrentPosition();
                    int minutes = (int) TimeUnit.MILLISECONDS.toMinutes(millis);
                    millis -= TimeUnit.MINUTES.toMillis(minutes);
                    int seconds = (int) TimeUnit.MILLISECONDS.toSeconds(millis);
                    intent.putExtra("CUR", String.format("%02d:%02d", minutes, seconds));

                    getBaseContext().sendBroadcast(intent);

                    try {
                        Thread.sleep(1000);
                    } catch (Exception e) {
                        Log.e("ERROR", "MusicPlaylingService.Thread.1");
                        e.printStackTrace();
                    }
                }
            }
        };

        super.onCreate();
    }

    @Override
    public void onDestroy() {
        if (audioManager.isMusicActive()) {
            mediaPlayer.stop();
            mediaPlayer.release();
        }
        stopForeground(true);
        // Kill thread
        sendProgressionUpdateThread = new Thread(sendProgressUpdateRunnable);
        super.onDestroy();
    }
}
