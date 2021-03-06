#include <string.h>
#include <stdio.h>
#include "../base64.h"

static int assert_enc (char *src, char *dst);
static int assert_dec (char *src, char *dst);
static int assert_roundtrip (char *src);

static int ret = 0;
static char out[100];
static size_t outlen;

int
main ()
{
	/* These are the test vectors from RFC4648: */
	assert_enc("", "");
	assert_enc("f", "Zg==");
	assert_enc("fo", "Zm8=");
	assert_enc("foo", "Zm9v");
	assert_enc("foob", "Zm9vYg==");
	assert_enc("fooba", "Zm9vYmE=");
	assert_enc("foobar", "Zm9vYmFy");

	/* And their inverse: */
	assert_dec("", "");
	assert_dec("Zg==", "f");
	assert_dec("Zm8=", "fo");
	assert_dec("Zm9v", "foo");
	assert_dec("Zm9vYg==", "foob");
	assert_dec("Zm9vYmE=", "fooba");
	assert_dec("Zm9vYmFy", "foobar");

	assert_roundtrip("");
	assert_roundtrip("f");
	assert_roundtrip("fo");
	assert_roundtrip("foo");
	assert_roundtrip("foob");
	assert_roundtrip("fooba");
	assert_roundtrip("foobar");

	assert_roundtrip("");
	assert_roundtrip("Zg==");
	assert_roundtrip("Zm8=");
	assert_roundtrip("Zm9v");
	assert_roundtrip("Zm9vYg==");
	assert_roundtrip("Zm9vYmE=");
	assert_roundtrip("Zm9vYmFy");

	return ret;
}

static int
assert_enc (char *src, char *dst)
{
	size_t srclen = strlen(src);
	size_t dstlen = strlen(dst);

	base64_encode(src, srclen, out, &outlen);

	if (outlen != dstlen) {
		printf("FAIL: encoding of '%s': length expected %d, got %d\n", src, dstlen, outlen);
		ret = 1;
		return 0;
	}
	if (strncmp(dst, out, outlen) != 0) {
		out[outlen] = '\0';
		printf("FAIL: encoding of '%s': expected output '%s', got '%s'\n", src, dst, out);
		ret = 1;
		return 0;
	}
	return 1;
}

static int
assert_dec (char *src, char *dst)
{
	size_t srclen = strlen(src);
	size_t dstlen = strlen(dst);

	if (!base64_decode(src, srclen, out, &outlen)) {
		printf("FAIL: decoding of '%s': decoding error\n", src);
		ret = 1;
		return 0;
	}
	if (outlen != dstlen) {
		printf("FAIL: encoding of '%s': length expected %d, got %d\n", src, dstlen, outlen);
		ret = 1;
		return 0;
	}
	if (strncmp(dst, out, outlen) != 0) {
		out[outlen] = '\0';
		printf("FAIL: decoding of '%s': expected output '%s', got '%s'\n", src, dst, out);
		ret = 1;
		return 0;
	}
	return 1;
}

static int
assert_roundtrip (char *src)
{
	char tmp[100];
	size_t tmplen;
	size_t srclen = strlen(src);

	base64_encode(src, srclen, out, &outlen);

	if (!base64_decode(out, outlen, tmp, &tmplen)) {
		printf("FAIL: decoding of '%s': decoding error\n", out);
		ret = 1;
		return 0;
	}
	/* Check that 'src' is identical to 'tmp': */
	if (srclen != tmplen) {
		printf("FAIL: roundtrip of '%s': length expected %d, got %d\n", src, srclen, tmplen);
		ret = 1;
		return 0;
	}
	if (strncmp(src, tmp, tmplen) != 0) {
		tmp[tmplen] = '\0';
		printf("FAIL: roundtrip of '%s': got '%s'\n", src, tmp);
		ret = 1;
		return 0;
	}
	return 1;
}
