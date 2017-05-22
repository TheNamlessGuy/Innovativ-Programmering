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
public class OnAlbumlistMenuItemClickListener implements PopupMenu.OnMenuItemClickListener {
    private View parent;
    public OnAlbumlistMenuItemClickListener(View parent) {
        this.parent = parent;
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
        if (item.getTitle().equals("Add to current playlist")) {
            String album = ((TextView) parent.findViewById(R.id.musicadapter_string_layout_text)).getText().toString();
            ((MusicChoosingActivity) parent.getContext()).addAlbumToCurrentPlaylist(album);
        } else if (item.getTitle().equals("Play Album")) {
            String album = ((TextView) parent.findViewById(R.id.musicadapter_string_layout_text)).getText().toString();
            ((MusicChoosingActivity) parent.getContext()).setCurrentPlaylistToAlbum(album);
        }
        return true;
    }
}
