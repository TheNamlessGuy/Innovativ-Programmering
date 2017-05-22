package com.example.maxbr431.myapplication.Listeners;

import android.view.MenuItem;
import android.view.View;
import android.widget.PopupMenu;
import android.widget.TextView;

import com.example.maxbr431.myapplication.MusicChoosingActivity;
import com.example.maxbr431.myapplication.R;

/**
 * Created by maxbr431 on 2015-12-07.
 */
public class OnSonglistMenuItemClickListener implements PopupMenu.OnMenuItemClickListener {
    private View parent;
    public OnSonglistMenuItemClickListener(View parent) {
        this.parent = parent;
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
        if (item.getTitle().equals("Add to current playlist")) {
            String path = ((TextView) parent.findViewById(R.id.musicadapter_layout_data)).getText().toString();
            ((MusicChoosingActivity) parent.getContext()).addSongToCurrentPlaylist(path);
        } else if (item.getTitle().equals("Play Song")) {
            String path = ((TextView) parent.findViewById(R.id.musicadapter_layout_data)).getText().toString();
            ((MusicChoosingActivity) parent.getContext()).setCurrentPlaylist(path);
        }
        return true;
    }
}
