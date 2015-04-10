#include "mp3util.h"
#include <stdlib.h>
#include "id3v2lib.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "mp3util", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "mp3util", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "mp3util", __VA_ARGS__))

#define IT_UNKNOWN 0
#define IT_PNG 1
#define IT_JPEG 2

static ID3v2_tag* s_globalTag;

static int s_imageType = IT_UNKNOWN;

char* createUtf8StringFromTextContent(ID3v2_frame_text_content* content) {
	char* string = NULL;

	if (content->encoding == UTF_16_ENCODING) { // unicode
		string = parse_bom_utf16_to_utf8((uint16_t*)content->data, content->size / 2);
	}
	else {
		string = (uint8_t*)malloc(sizeof(char) * (content->size + 1));
		memcpy(string, content->data, content->size * sizeof(char));
		string[content->size + 1] = '\0';
	}

	return string;
}

void releaseUtf8StringFromTextContent(char* string) {
	free(string);
}

const char* getFilePathFromJavaObject(JNIEnv* env, jobject object) {
	jclass cls = (*env)->GetObjectClass(env, object);  //获得Java层该对象实例的类引用，即HelloJNI类引用
	jfieldID fileFieldId = (*env)->GetFieldID(env, cls, "m_file" , "Ljava/lang/String;"); //获得属性句柄
	if (!fileFieldId) {
		LOGE("can't get java class filed of \"m_file\"\n");
		return NULL;
	}

	jstring javaPath = (jstring)(*env)->GetObjectField(env, object ,fileFieldId);  // 获得该属性的值
	return (*env)->GetStringUTFChars(env, javaPath , NULL);
}

jstring getJavaStringFromTextFrame(JNIEnv* env, ID3v2_frame* frame) {
	int type = get_frame_type(frame->frame_id);
	if (type != TEXT_FRAME) {
		LOGE("error! title is not a text frame\n");
		return NULL;
	}

	ID3v2_frame_text_content* content = parse_text_frame_content(frame);
	if (!content || content->data == NULL) {
		LOGE("parse music title failed!\n");
		return NULL;
	}

	char* text = createUtf8StringFromTextContent(content);
	jstring sRet = (*env)->NewStringUTF(env, text);
	releaseUtf8StringFromTextContent(text);

	release_text_content(&content);
	return sRet;
}

jstring getJavaStringFromCommentFrame(JNIEnv* env, ID3v2_frame* frame) {
	int type = get_frame_type(frame->frame_id);
	if (type != COMMENT_FRAME) {
		LOGE("error! title is not a comment frame\n");
		return NULL;
	}

	ID3v2_frame_comment_content* content = parse_comment_frame_content(frame);
	if (!content || content->text == NULL) {
		LOGE("parse music comment text failed!\n");
		return NULL;
	}

	char* text = createUtf8StringFromTextContent(content->text);
	jstring sRet = (*env)->NewStringUTF(env, text);
	releaseUtf8StringFromTextContent(text);

	release_comment_content(&content);
	return sRet;
}

JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_sayHello
  (JNIEnv * env, jclass class) {
	return (*env)->NewStringUTF(env, "hello world, ndk is successful!");
}

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    init
 * Signature: ()Z
 */

JNIEXPORT jboolean JNICALL Java_com_smarttoy_mp3_STMp3Util_init
  (JNIEnv* env, jobject object) {
	jboolean jbRet = JNI_FALSE;

	do {
		const char* file = getFilePathFromJavaObject(env, object);
		if (!file) {
			break;
		}

		s_globalTag = load_tag(file);
		if (!s_globalTag) {
			LOGE("parse mp3 file \"%s\" failed!", file);
			break;
		}

		jbRet = JNI_TRUE;
	} while(0);
	return jbRet;
}

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    uninit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_smarttoy_mp3_STMp3Util_uninit
  (JNIEnv* env, jobject object) {
	if (s_globalTag) {
		release_tag(&s_globalTag);
	}
}

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getTitle
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getTitle
  (JNIEnv * env, jobject object) {
	if (!s_globalTag) {
		LOGE("you havn't call init method or initialized failed before!\n");
		return NULL;
	}

	ID3v2_frame* frame = tag_get_title(s_globalTag);

	if (!frame) {
		LOGE("mp3 file havn't include title frame!\n");
		return NULL;
	}

	return getJavaStringFromTextFrame(env, frame);
}

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getArtist
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getArtist
  (JNIEnv * env, jobject object) {
	if (!s_globalTag) {
		LOGE("you havn't call init method or initialized failed before!\n");
		return NULL;
	}

	ID3v2_frame* frame = tag_get_artist(s_globalTag);

	if (!frame) {
		LOGE("mp3 file havn't include artist frame!");
		return NULL;
	}

	return getJavaStringFromTextFrame(env, frame);
}

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getAlbum
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getAlbum
  (JNIEnv * env, jobject object) {
	if (!s_globalTag) {
		LOGE("you havn't call init method or initialized failed before!\n");
		return NULL;
	}

	ID3v2_frame* frame = tag_get_album(s_globalTag);

	if (!frame) {
		LOGE("mp3 file havn't include album frame!\n");
		return NULL;
	}

	return getJavaStringFromTextFrame(env, frame);
}

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getAlbumArtist
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getAlbumArtist
  (JNIEnv * env, jobject object) {
	if (!s_globalTag) {
		LOGE("you havn't call init method or initialized failed before!\n");
		return NULL;
	}

	ID3v2_frame* frame = tag_get_album_artist(s_globalTag);

	if (!frame) {
		LOGE("mp3 file havn't include album artist frame!\n");
		return NULL;
	}

	return getJavaStringFromTextFrame(env, frame);
}

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getGenre
 * Signature: ()Ljava/lang/String;
 */
/*
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getGenre
  (JNIEnv * env, jobject object) {
	if (!s_globalTag) {
		cout << "you havn't call init method or initialized failed before!" << endl;
		return NULL;
	}

	ID3v2_frame* frame = tag_get_genre(s_globalTag);

	if (!frame) {
		cout << "mp3 file havn't include genre frame!" << endl;
		return NULL;
	}

	return NULL;
}
*/

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getComment
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_smarttoy_mp3_STMp3Util_getComment
  (JNIEnv * env, jobject object) {
	if (!s_globalTag) {
		LOGE("you havn't call init method or initialized failed before!\n");
		return NULL;
	}

	ID3v2_frame* frame = tag_get_comment(s_globalTag);
	if (!frame) {
		LOGE("mp3 file havn't include comment frame!\n");
		return NULL;
	}

	return getJavaStringFromCommentFrame(env, frame);
}

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getAlbumCover
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_smarttoy_mp3_STMp3Util_getAlbumCover
  (JNIEnv * env, jobject object) {

	if (!s_globalTag) {
		LOGE("you havn't call init method or initialized failed before!\n");
		return NULL;
	}

	ID3v2_frame* frame = tag_get_album_cover(s_globalTag);
	if (!frame || get_frame_type(frame->frame_id) != APIC_FRAME) {
		LOGE("error: get attached picture of album cover failed!\n");
		return NULL;
	}

	ID3v2_frame_apic_content* content = parse_apic_frame_content(frame);

	if (!content) {
		LOGE("error: pare apic frame failed!\n");
		return NULL;
	}

	jbyteArray jba = (*env)->NewByteArray(env, content->picture_size);
	(*env)->SetByteArrayRegion(env, jba, 0, content->picture_size, (jbyte*)content->data);

	if (strcmp(content->mime_type, JPG_MIME_TYPE) == 0) {
		s_imageType = IT_JPEG;
	} else if (strcmp(content->mime_type, PNG_MIME_TYPE) == 0) {
		s_imageType = IT_PNG;
	} else {
		s_imageType = IT_UNKNOWN;
	}

	release_apic_content(&content);
	return jba;
}

/*
 * Class:     com_smarttoy_mp3_STMp3Util
 * Method:    getAlbumCoveType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_smarttoy_mp3_STMp3Util_getAlbumCoveType
  (JNIEnv * env, jobject object) {
	return (jint)s_imageType;
}
