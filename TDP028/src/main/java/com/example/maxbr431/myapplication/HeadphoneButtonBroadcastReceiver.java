package com.example.maxbr431.myapplication;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;

/**
 * Created by maxbr431 on 2015-12-10.
 */
public class HeadphoneButtonBroadcastReceiver extends BroadcastReceiver {
    static short clickAmount = 0;

    public HeadphoneButtonBroadcastReceiver() {
        super();
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        if (!Intent.ACTION_MEDIA_BUTTON.equals(intent.getAction())) return;

        KeyEvent keyEvent = intent.getParcelableExtra(Intent.EXTRA_KEY_EVENT);
        if (keyEvent == null) return;

        if (keyEvent.getAction() == KeyEvent.ACTION_DOWN) {
            clickAmount++;
            Handler handler = new Handler();
            final Context cont = context;
            Runnable r = new Runnable() {
                @Override
                public void run() {
                    if (clickAmount == 1) {
                        Intent sendIntent = new Intent(cont, MusicWidget.class);
                        sendIntent.setAction("PLAYBUTTON");
                        cont.sendBroadcast(sendIntent);
                    } else if (clickAmount == 2) {
                        Intent sendIntent = new Intent(cont, MusicPlayingService.class);
                        sendIntent.setAction("NEXT");
                        cont.startService(sendIntent);
                    }
                    clickAmount = 0;
                }
            };
            if (clickAmount == 1)
                handler.postDelayed(r, 200);
        }
    }
}
