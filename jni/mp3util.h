/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_smarttoy_mp3_STMp3Util */

#ifndef _Included_com_smarttoy_mp3_STMp3Util
#define _Included_com_smarttoy_mp3_STMp3Util
#ifdef __cplusplus
extern "C" {
#endif
#undef com_smarttoy_mp3_STMp3Util_IMG_UNKNOWN
#define com_smarttoy_mp3_STMp3Util_IMG_UNKNOWN 0L
#undef com_smarttoy_mp3_STMp3Util_IMG_PNG
#define com_smarttoy_mp3_STMp3Util_IMG_PNG 1L
#undef com_smarttoy_mp3_STMp3Util_IMG_JPEG
#define com_smarttoy_mp3_STMp3Util_IMG_JPEG 2L
/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    sayHello
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_sayHello
  (JNIEnv *, jclass);

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    init
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_smarttoy_mp3_STMp3Util_init
  (JNIEnv *, jobject);

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    uninit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_smarttoy_mp3_STMp3Util_uninit
  (JNIEnv *, jobject);

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getTitle
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getTitle
  (JNIEnv *, jobject);

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getArtist
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getArtist
  (JNIEnv *, jobject);

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getAlbum
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getAlbum
  (JNIEnv *, jobject);

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getAlbumArtist
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getAlbumArtist
  (JNIEnv *, jobject);

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getGenre
 * Signature: ()Ljava/lang/String;
 */
/*
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getGenre
  (JNIEnv *, jobject);
*/

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getComment
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getComment
  (JNIEnv *, jobject);

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getAlbumCover
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_smarttoy_mp3_STMp3Util_getAlbumCover
  (JNIEnv *, jobject);

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getAlbumCoveType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_smarttoy_mp3_STMp3Util_getAlbumCoveType
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif