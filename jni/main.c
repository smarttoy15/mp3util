#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "id3v2lib.h"

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

void printTextFrame(ID3v2_frame* frame, const char* message) {
	if (!frame) {
		printf("frame not exist!\n");
		return;
	}

	if (get_frame_type(frame->frame_id) != TEXT_FRAME) {
		printf("error: frame is not a text frame\n");
		return;
	}
	ID3v2_frame_text_content* content = parse_text_frame_content(frame);
	if (!content) {
		printf("error: parse title content failed!\n");
		return;
	}
	char* title = createUtf8StringFromTextContent(content);
	printf(message, title);
	releaseUtf8StringFromTextContent(title);

	release_text_content(&content);
}

void printCommentFrame(ID3v2_frame* frame, const char* message) {
	if (!frame) {
		printf("frame not exist!\n");
		return;
	}

	if (get_frame_type(frame->frame_id) != COMMENT_FRAME) {
		printf("error: frame is not a comment frame\n");
		return;
	}

	ID3v2_frame_comment_content* content = parse_comment_frame_content(frame);

	if (!content->text) {
		printf("comment frame has not text content!");
	}
	char* string = createUtf8StringFromTextContent(content->text);
	printf(message, string);
	releaseUtf8StringFromTextContent(string);

	release_comment_content(&content);
}

void checkApic(ID3v2_frame* frame) {
	if (!frame) {
		printf("frame not exist!\n");
		return;
	}

	if (get_frame_type(frame->frame_id) != APIC_FRAME) {
		printf("error: frame is not a attached picture frame\n");
		return;
	}

	ID3v2_frame_apic_content* apic = parse_apic_frame_content(frame);

	printf("apic encoding: %d\n", apic->encoding);
	printf("apic mime type: %s\n", apic->mime_type);
	printf("apic picture type: %d\n", apic->picture_type);
	printf("apic discription: %s\n", apic->description);
	printf("apic picture size: %d\n", apic->picture_size);
	printf("apic picture data: %x\n", apic->data);

	release_apic_content(&apic);
}

void usage() {
	printf("usage:\n");
	printf("\tmp3lib [mp3file_path]\n");
	printf("Dump out mp3 info. The charactor encode default is utf8.");
}

int main(int argc, char** argv) {

	if (argc < 2) {
		usage();
		return 1;
	}

	int bRet = 0;
	ID3v2_tag* tag = load_tag(argv[1]);

	if (!tag) {
		printf("error: open file \"%s\" failed", argv[1]);
		goto End;
	}

	printTextFrame(tag_get_title(tag), "title: %s\n");
	printTextFrame(tag_get_artist(tag), "artist: %s\n");
	printTextFrame(tag_get_album(tag), "album: %s\n");
	printTextFrame(tag_get_album_artist(tag), "album artist: %s\n");
	printCommentFrame(tag_get_comment(tag), "comment: %s\n");

	checkApic(tag_get_album_cover(tag));

	bRet = 1;
Error:
	release_tag(&tag);
End:
	return bRet;
}
