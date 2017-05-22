package com.example.maxbr431.myapplication.Listeners;

import android.view.MenuItem;
import android.view.View;
import android.widget.ListView;
import android.widget.PopupMenu;

import com.example.maxbr431.myapplication.MusicChoosingActivity;

/**
 * Created by maxbr431 on 2015-12-04.
 */
public class OnPlaylistMenuItemClickListener implements PopupMenu.OnMenuItemClickListener {
    private View parent;
    public OnPlaylistMenuItemClickListener(View parent) {
        this.parent = parent;
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
        ListView listView = (ListView) parent.getParent();
        int location = listView.getPositionForView(parent);

        ((MusicChoosingActivity) parent.getContext()).removeFromPlaylist(location);
        return true;
    }
}
