package com.example.maxbr431.myapplication;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by maxbr431 on 2015-11-16.
 */
public class MusicAdapter extends ArrayAdapter<String> {
    private View.OnClickListener buttonListener;
    private View.OnClickListener viewListener;

    public MusicAdapter(Context context, ArrayList<String> data) {
        super(context, 0, data);
    }

    public void setOnButtonClickListener(View.OnClickListener listener) {
        buttonListener = listener;
    }

    public void setOnViewClickListener(View.OnClickListener listener) {
        viewListener = listener;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        final String info = getItem(position);

        if (convertView == null)
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.musicadapter_string_layout, parent, false);

        Button button;
        short currentlyShowing = MusicChoosingActivity.currentlyShowing;

        if ((currentlyShowing == MusicChoosingActivity.SONGS) ||
                (currentlyShowing == MusicChoosingActivity.TRACKS) ||
                (currentlyShowing == MusicChoosingActivity.PLAYLIST)) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.musicadapter_layout, parent, false);
            MusicFile mf = ((MusicChoosingActivity)getContext()).getMusicFile(info);

            if (mf == null)
                return convertView;

            TextView tw = (TextView) convertView.findViewById(R.id.musicadapter_layout_artist);
            tw.setText(mf.artist());
            tw = (TextView) convertView.findViewById(R.id.musicadapter_layout_album);
            tw.setText(mf.album());
            tw = (TextView) convertView.findViewById(R.id.musicadapter_layout_title);
            tw.setText(mf.title());
            tw = (TextView) convertView.findViewById(R.id.musicadapter_layout_data);
            tw.setText(mf.path());

            button = (Button) convertView.findViewById(R.id.musicadapter_layout_button);
        } else {
            TextView tw = (TextView) convertView.findViewById(R.id.musicadapter_string_layout_text);
            tw.setText(info);

            button = (Button) convertView.findViewById(R.id.musicadapter_string_layout_button);
            if (currentlyShowing == MusicChoosingActivity.ARTISTS)
                button.setVisibility(View.INVISIBLE);
        }

        if (buttonListener != null)
            button.setOnClickListener(buttonListener);
        if (convertView != null)
            convertView.setOnClickListener(viewListener);

        return convertView;
    }
}
