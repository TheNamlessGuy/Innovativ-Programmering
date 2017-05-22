package com.example.maxbr431.myapplication;

import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ListView;
import android.content.Intent;
import android.widget.PopupMenu;
import android.widget.TextView;

import com.example.maxbr431.myapplication.Listeners.OnAlbumlistMenuItemClickListener;
import com.example.maxbr431.myapplication.Listeners.OnPlaylistMenuItemClickListener;
import com.example.maxbr431.myapplication.Listeners.OnSonglistMenuItemClickListener;

import java.util.ArrayList;

/**
 * A placeholder fragment containing a simple view.
 */
public class MusicChoosingFragment extends Fragment {
    View view;

    public MusicChoosingFragment() {}

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_music_choosing, container, false);
        return view;
    }

    public void setArtists(final ArrayList<String> musicList) {
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ListView musicListView = (ListView) view.findViewById(R.id.fragment_music_choosing_listview);
                MusicAdapter musicAdapter = new MusicAdapter(getActivity(), musicList);

                musicAdapter.setOnViewClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        String str = ((TextView) v.findViewById(R.id.musicadapter_string_layout_text)).getText().toString();
                        ((MusicChoosingActivity) getActivity()).setArtistsAlbumList(str);
                    }
                });

                musicListView.setAdapter(musicAdapter);
            }
        });
    }

    public void setAlbums(final ArrayList<String> musicList) {
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ListView musicListView = (ListView) view.findViewById(R.id.fragment_music_choosing_listview);
                MusicAdapter musicAdapter = new MusicAdapter(getActivity(), musicList);

                musicAdapter.setOnViewClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        String str = ((TextView) v.findViewById(R.id.musicadapter_string_layout_text)).getText().toString();
                        ((MusicChoosingActivity) getActivity()).setAlbumSongList(str);
                    }
                });

                musicAdapter.setOnButtonClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Button b = (Button) v;
                        View parent = (View) v.getParent();

                        PopupMenu popupMenu = new PopupMenu(getActivity(), b);
                        popupMenu.getMenuInflater().inflate(R.menu.menu_albumlist_buttonclick, popupMenu.getMenu());

                        popupMenu.setOnMenuItemClickListener(new OnAlbumlistMenuItemClickListener(parent));
                        popupMenu.show();
                    }
                });

                musicListView.setAdapter(musicAdapter);
            }
        });
    }

    public void setAlbumList(final ArrayList<String> musicList) {
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ListView musicListView = (ListView) view.findViewById(R.id.fragment_music_choosing_listview);
                final MusicAdapter musicAdapter = new MusicAdapter(getActivity(), musicList);

                musicAdapter.setOnViewClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        String str = ((TextView) v.findViewById(R.id.musicadapter_string_layout_text)).getText().toString();
                        ((MusicChoosingActivity) getActivity()).setAlbumSongList(str);
                    }
                });

                musicAdapter.setOnButtonClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Button b = (Button) v;
                        View parent = (View) v.getParent();

                        PopupMenu popupMenu = new PopupMenu(getActivity(), b);
                        popupMenu.getMenuInflater().inflate(R.menu.menu_albumlist_buttonclick, popupMenu.getMenu());

                        popupMenu.setOnMenuItemClickListener(new OnAlbumlistMenuItemClickListener(parent));
                        popupMenu.show();
                    }
                });

                musicListView.setAdapter(musicAdapter);
            }
        });
    }

    public void setSongs(final ArrayList<String> musicList) {

        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ListView musicListView = (ListView) view.findViewById(R.id.fragment_music_choosing_listview);
                MusicAdapter musicAdapter = new MusicAdapter(getActivity(), musicList);

                musicAdapter.setOnViewClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (((MusicChoosingActivity) getActivity()).currentlyShowing == MusicChoosingActivity.SONGS) {
                            String str = ((TextView) v.findViewById(R.id.musicadapter_layout_data)).getText().toString();
                            ((MusicChoosingActivity) getActivity()).setCurrentPlaylist(str);
                        } else { // Showing album or playlist
                            // Set the entire list as current playlist, set the item the user clicked on as the currently playing
                            String path = ((TextView) v.findViewById(R.id.musicadapter_layout_data)).getText().toString();
                            String album = ((TextView) v.findViewById(R.id.musicadapter_layout_album)).getText().toString();
                            ((MusicChoosingActivity) getActivity()).setCurrentPlaylistToAlbum(album);

                            Intent intent = new Intent(getContext(), MusicPlayingService.class);
                            intent.setAction("CHANGECURRENTLYPLAYING");
                            intent.putExtra("PATH", path);

                            getContext().startService(intent);
                        }
                    }
                });

                musicAdapter.setOnButtonClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Button b = (Button)v;
                        View parent = (View) v.getParent();

                        PopupMenu popupMenu = new PopupMenu(getActivity(), b);
                        popupMenu.getMenuInflater().inflate(R.menu.menu_songlist_buttonclick, popupMenu.getMenu());

                        popupMenu.setOnMenuItemClickListener(new OnSonglistMenuItemClickListener(parent));
                        popupMenu.show();
                    }
                });

                musicListView.setAdapter(musicAdapter);
            }
        });
    }
    public void setPlaylist(final ArrayList<String> musicList) {
        ((MusicChoosingActivity)getActivity()).currentlyShowing = MusicChoosingActivity.PLAYLIST;
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ListView musicListView = (ListView) view.findViewById(R.id.fragment_music_choosing_listview);
                MusicAdapter musicAdapter = new MusicAdapter(getActivity(), musicList);
                musicAdapter.setOnViewClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Intent intent = new Intent(getContext(), MusicPlayingService.class);
                        intent.setAction("CHANGECURRENTLYPLAYING");

                        String str = ((TextView) v.findViewById(R.id.musicadapter_layout_data)).getText().toString();
                        intent.putExtra("PATH", str);

                        getContext().startService(intent);
                    }
                });

                musicAdapter.setOnButtonClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Button b = (Button) v;
                        View parent = (View) v.getParent();

                        PopupMenu popupMenu = new PopupMenu(getActivity(), b);
                        popupMenu.getMenuInflater().inflate(R.menu.menu_playlist_buttonclick, popupMenu.getMenu());

                        popupMenu.setOnMenuItemClickListener(new OnPlaylistMenuItemClickListener(parent));
                        popupMenu.show();
                    }
                });

                musicListView.setAdapter(musicAdapter);
            }
        });
    }
}
