package com.smarttoy.mp3;

public class STMp3Util {

	public static final int IMG_UNKNOWN = 0;
	public static final int IMG_PNG = 1;
	public static final int IMG_JPEG = 2;
	
	private String m_file;
	
	public STMp3Util(String filePath) {
		m_file = filePath;
	}
	
	public String getMp3File() {
		return m_file;
	}
	
	public void setMp3File(String path) {
		m_file = path;
	}
	
	public native static String sayHello();
	
	public native boolean init();
	public native void uninit();
	//mp3相关函数
	public native String getTitle();		// 获取 标题
	public native String getArtist();		// 获取艺术家
	public native String getAlbum();		// 获取专辑
	public native String getAlbumArtist();	// 获取专辑艺术家
//	public native String getGenre();		// 获取流派
//	public native String getTrack();		//
//	public native String getYear();			// 
	public native String getComment();		// 获取注释
//	public native String getDiscNumber();
//	public native String getComposer();
	public native byte[] getAlbumCover();	// 获取封面图片信息(有可能返回空)
	public native int getAlbumCoveType();	// 封面图片的类型（当封面图片的获取返回空时，此返回值无意义 ）
	
	static {
		System.loadLibrary("mp3util");
	}
}
