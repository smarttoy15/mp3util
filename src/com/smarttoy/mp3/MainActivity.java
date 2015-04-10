package com.smarttoy.mp3;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.support.v7.app.ActionBarActivity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ImageView;
import android.widget.TextView;


public class MainActivity extends ActionBarActivity {

	static final String LOG_TAG = "test mp3";
	
	ImageView m_imgAlumb;
	TextView m_txtTitle;
	TextView m_txtAlumb;
	
	STMp3Util m_util;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);        
        m_txtTitle = (TextView)findViewById(R.id.txtTitle);
        m_txtAlumb = (TextView)findViewById(R.id.txtAlumb);
        m_imgAlumb = (ImageView)findViewById(R.id.imgAlumb);

        m_util = new STMp3Util("/storage/sdcard1/thewayyouare.mp3");
        
        if (!m_util.init()) {  // init后要记得找个地方把它给uninit掉，以防内存泄漏
        	Log.e(LOG_TAG, "open mp3 file failed");
        	return;
        }
               
        String title = m_util.getTitle();
        m_txtTitle.setText(title);
        
        String artist = m_util.getArtist();
        m_txtAlumb.setText(artist);
        
        byte[] img = m_util.getAlbumCover();
        
        BitmapFactory.Options bitmapOption = new BitmapFactory.Options();
		bitmapOption.inPreferredConfig = Bitmap.Config.RGB_565;
        Bitmap bitmap = BitmapFactory.decodeByteArray(img, 0, img.length, bitmapOption);
        m_imgAlumb.setImageBitmap(bitmap);
    }


    @Override
	protected void onStop() {
		// TODO Auto-generated method stub
    	m_util.uninit();
		super.onStop();
	}


	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
	}


	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
