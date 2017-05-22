package com.example.maxbr431.myapplication;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import java.util.ArrayList;

/**
 * Created by maxbr431 on 2015-11-23.
 */
public class ButtonFragment extends Fragment {
    View view;
    private BroadcastReceiver broadcastReceiver;

    @Override
    public View onCreateView(final LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_buttons, container, false);

        Button curPlaylist = (Button)view.findViewById(R.id.fragment_buttons_playlistButton);
        curPlaylist.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MusicChoosingActivity) getActivity()).setPlaylistSongList();
            }
        });

        Button artists = (Button) view.findViewById(R.id.fragment_buttons_artistsButton);
        artists.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MusicChoosingActivity)getActivity()).setArtistList();
            }
        });

        Button albums = (Button) view.findViewById(R.id.fragment_buttons_albumsButton);
        albums.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MusicChoosingActivity)getActivity()).setAlbumList();
            }
        });

        Button songs = (Button) view.findViewById(R.id.fragment_buttons_songsButton);
        songs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MusicChoosingActivity)getActivity()).setSongList();
            }
        });

        return view;
    }

    @Override
    public void onResume() {
        if (broadcastReceiver == null)
            broadcastReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    if (intent.getAction() == "GETPLAYLIST") {
                        showPlaylist((ArrayList<String>) intent.getSerializableExtra("value"));
                    }
                }
            };
        getActivity().registerReceiver(this.broadcastReceiver, new IntentFilter("GETPLAYLIST"));
        super.onResume();
    }

    @Override
    public void onPause() {
        getActivity().unregisterReceiver(broadcastReceiver);
        super.onPause();
    }

    public void showPlaylist(ArrayList<String> musicList) {
        Fragment fragment = getActivity().getSupportFragmentManager().findFragmentById(R.id.list_fragment);
        if (fragment instanceof MusicChoosingFragment)
            ((MusicChoosingFragment)fragment).setPlaylist(musicList);
    }
}
