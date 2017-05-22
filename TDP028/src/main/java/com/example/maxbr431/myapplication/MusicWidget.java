package com.example.maxbr431.myapplication;

import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.util.Log;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RemoteViews;

import java.lang.reflect.Array;
import java.util.ArrayList;

/**
 * Implementation of App Widget functionality.
 */
public class MusicWidget extends AppWidgetProvider {
    public static final short ALL = 0;
    public static final short ONE = 1;
    public static final short NONE = 2;

    public static final short PLAY = 0;
    public static final short PAUSED = 1;

    private static short loopButtonMode;
    private static short playButtonMode;

    private static boolean hasSetPlaylists;

    public static int currentIndex;

    @Override
    public void onUpdate(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds) {
        // There may be multiple widgets active, so update all of them
        for (int i = 0; i < appWidgetIds.length; i++) {
            updateAppWidget(context, appWidgetManager, appWidgetIds[i]);
        }
    }


    @Override
    public void onEnabled(Context context) {
        // Enter relevant functionality for when the first widget is created
        Intent initIntent = new Intent(context, MusicPlayingService.class);
        initIntent.setAction("INIT");
        context.startService(initIntent);
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        switch(intent.getAction()) {
            case "MAINACTIVITY_READY":
                if (/*hasSetPlaylists*/true) break;
                Pair<ArrayList<String>, Integer[]> pair = FileHandler.readPlaylists(context);
                MusicChoosingActivity.playlist = pair.first();
                currentIndex = pair.second()[0];

                Intent playlistIntent = new Intent(context, MusicPlayingService.class);
                playlistIntent.setAction("SETPLAYLISTTOLIST");
                playlistIntent.putExtra("FILES", MusicChoosingActivity.playlist);
                playlistIntent.putExtra("PLAY", false);
                playlistIntent.putExtra("INDEX", currentIndex);
                context.startService(playlistIntent);

                hasSetPlaylists = true;
                loopButtonMode = pair.second()[1].shortValue();
                setLoopButtonImage(context);

                playlistIntent = new Intent(context, MusicPlayingService.class);
                playlistIntent.setAction("LOOPCLICK");
                playlistIntent.putExtra("VALUE", loopButtonMode);
                context.startService(playlistIntent);
                break;
            case "PLAYBUTTON":
                if (playButtonMode == MusicWidget.PLAY)
                    setPlayButton(context, MusicWidget.PAUSED);
                else
                    setPlayButton(context, MusicWidget.PLAY);

                Intent playIntent = new Intent(context, MusicPlayingService.class);
                playIntent.setAction("TOGGLEPLAY");
                context.startService(playIntent);
                break;
            case "LOOPBUTTON":
                loopButtonMode++;
                if (loopButtonMode > MusicWidget.NONE)
                    loopButtonMode = MusicWidget.ALL;
                setLoopButtonImage(context);

                Intent loopIntent = new Intent(context, MusicPlayingService.class);
                loopIntent.setAction("LOOPCLICK");
                loopIntent.putExtra("VALUE", loopButtonMode);
                context.startService(loopIntent);
                FileHandler.savePlaylists(context, MusicChoosingActivity.playlist, currentIndex, loopButtonMode);
                break;
            case "STOPBUTTON":
                setPlayButton(context, MusicWidget.PLAY);
                setPlaytimeText(context, "00:00", null);

                Intent stopIntent = new Intent(context, MusicPlayingService.class);
                stopIntent.setAction("STOP");
                context.startService(stopIntent);
                break;
            case "NEWSONGPLAYING":
                MusicFile mf = MusicChoosingActivity.getMusicFile(intent.getStringExtra("PATH"));
                setSongText(context, mf.artist(), mf.title());

                if (intent.getBooleanExtra("PAUSED", false))
                    setPlayButton(context, MusicWidget.PLAY);
                else
                    setPlayButton(context, MusicWidget.PAUSED);

                setPlaytimeText(context, "00:00", intent.getStringExtra("TOTTIME"));
                currentIndex = intent.getIntExtra("INDEX", -1);
                FileHandler.savePlaylists(context, MusicChoosingActivity.playlist, currentIndex, loopButtonMode);
                break;
            case "NEWPLAYTIME":
                setPlaytimeText(context, intent.getStringExtra("CUR"), null);
                break;
            default:
                super.onReceive(context, intent);
                break;
        }
    }

    public void setSongText(Context context, String artist, String title) {
        RemoteViews remoteViews = new RemoteViews(context.getPackageName(), R.layout.music_widget);
        remoteViews.setTextViewText(R.id.widget_titleText, artist + " - " + title);
        AppWidgetManager.getInstance(context).updateAppWidget(new ComponentName(context, MusicWidget.class), remoteViews);
    }

    public void setPlaytimeText(Context context, String currentPlaytime, String totalPlaytime) {
        RemoteViews remoteViews = new RemoteViews(context.getPackageName(), R.layout.music_widget);
        remoteViews.setTextViewText(R.id.widget_currentPlaytime, currentPlaytime);
        if (totalPlaytime != null)
            remoteViews.setTextViewText(R.id.widget_totalPlaytimeText, "/" + totalPlaytime);
        AppWidgetManager.getInstance(context).updateAppWidget(new ComponentName(context, MusicWidget.class), remoteViews);
    }

    public void setPlayButton(Context context, short mode) {
        RemoteViews remoteViews = new RemoteViews(context.getPackageName(), R.layout.music_widget);
        if (mode == MusicWidget.PAUSED) {
            remoteViews.setImageViewResource(R.id.widget_playpauseButton, R.drawable.widget_pausebutton);
            playButtonMode = MusicWidget.PAUSED;
        } else {
            remoteViews.setImageViewResource(R.id.widget_playpauseButton, R.drawable.widget_playbutton);
            playButtonMode = MusicWidget.PLAY;
        }
        AppWidgetManager.getInstance(context).updateAppWidget(new ComponentName(context, MusicWidget.class), remoteViews);
    }

    public void setLoopButtonImage(Context context) {
        RemoteViews remoteViews = new RemoteViews(context.getPackageName(), R.layout.music_widget);
        if (loopButtonMode == MusicWidget.ALL) {
            remoteViews.setImageViewResource(R.id.widget_loopButton, R.drawable.widget_loopallbutton);
            loopButtonMode = MusicWidget.ALL;
        } else if (loopButtonMode == MusicWidget.ONE) {
            remoteViews.setImageViewResource(R.id.widget_loopButton, R.drawable.widget_looponebutton);
            loopButtonMode = MusicWidget.ONE;
        } else {
            remoteViews.setImageViewResource(R.id.widget_loopButton, R.drawable.widget_loopnonebutton);
            loopButtonMode = MusicWidget.NONE;
        }
        AppWidgetManager.getInstance(context).updateAppWidget(new ComponentName(context, MusicWidget.class), remoteViews);
    }

    @Override
    public void onDisabled(Context context) {
        // Enter relevant functionality for when the last widget is disabled
        Intent closeIntent = new Intent(context, MusicPlayingService.class);
        context.stopService(closeIntent);
    }

    static void updateAppWidget(Context context, AppWidgetManager appWidgetManager,
                                int appWidgetId) {
        loopButtonMode = MusicWidget.ALL;
        playButtonMode = MusicWidget.PLAY;
        hasSetPlaylists = false;

        // Set functionality of all buttons
        RemoteViews views = new RemoteViews(context.getPackageName(), R.layout.music_widget);

        // Looping back to widget
        Intent intent = new Intent(context, MusicWidget.class);
        intent.setAction("LOOPBUTTON");
        PendingIntent pendingIntent = PendingIntent.getBroadcast(context, 0, intent, 0);
        views.setOnClickPendingIntent(R.id.widget_loopButton, pendingIntent);

        intent.setAction("PLAYBUTTON");
        pendingIntent = PendingIntent.getBroadcast(context, 0, intent, 0);
        views.setOnClickPendingIntent(R.id.widget_playpauseButton, pendingIntent);

        intent.setAction("STOPBUTTON");
        pendingIntent = PendingIntent.getBroadcast(context, 0, intent, 0);
        views.setOnClickPendingIntent(R.id.widget_stopButton, pendingIntent);

        // Going to the service
        intent = new Intent(context, MusicPlayingService.class);
        intent.setAction("NEXT");
        pendingIntent = PendingIntent.getService(context, 0, intent, 0);
        views.setOnClickPendingIntent(R.id.widget_nextButton, pendingIntent);

        intent.setAction("PREV");
        pendingIntent = PendingIntent.getService(context, 0, intent, 0);
        views.setOnClickPendingIntent(R.id.widget_previousButton, pendingIntent);

        // Going to activity
        intent = new Intent(context, MusicChoosingActivity.class);
        pendingIntent = PendingIntent.getActivity(context, 0, intent, 0);
        views.setOnClickPendingIntent(R.id.music_widget, pendingIntent);

        appWidgetManager.updateAppWidget(appWidgetId, views);
    }
}

