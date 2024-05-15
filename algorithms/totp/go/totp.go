package main

import (
	"crypto/hmac"
	"crypto/sha1"
	"crypto/sha256"
	"crypto/sha512"
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"hash"
	"math"
	"strconv"
	"time"
)

type HashFn func() hash.Hash

func hmacSha(crypto HashFn, key, message []byte) []byte {
	mac := hmac.New(crypto, key)
	mac.Write(message)
	return mac.Sum(nil)
}

func generateTOTP(crypto HashFn, key string, time []byte, codeDigits int32) string {
	k, _ := hex.DecodeString(key)
	hash := hmacSha(crypto, k, time)

	offset := hash[len(hash)-1] & 0xf
	binary := int64(((int(hash[offset]) & 0x7f) << 24) |
		((int(hash[offset+1]) & 0xff) << 16) |
		((int(hash[offset+2]) & 0xff) << 8) |
		(int(hash[offset+3]) & 0xff))

	otp := binary % int64(math.Pow10(codeDigits))
	result := strconv.FormatInt(10_000_000_000+int64(otp), 10)

	return result[11-codeDigits:]
}

func main() {
	// keys as hex-string from RFC 6238, use `hex.EncodeToString([]byte("plain string"))` to encode a plain string
	seed := "3132333435363738393031323334353637383930"
	seed32 := "3132333435363738393031323334353637383930313233343536373839303132"
	seed64 := "3132333435363738393031323334353637383930" +
		"3132333435363738393031323334353637383930" +
		"3132333435363738393031323334353637383930" +
		"31323334"
	T0 := int64(0)
	X := int64(30)
	testTimes := []int64{59, 1111111109, 1111111111, 1234567890, 2000000000, 20000000000}

	// see: https://datatracker.ietf.org/doc/html/rfc6238#appendix-B
	fmt.Println("+---------------+-----------------------+------------------+----------+--------+")
	fmt.Println("|  Time(sec)    |   Time (UTC format)   | Value of T(Hex)  |   TOTP   | Mode   |")
	fmt.Println("+---------------+-----------------------+------------------+----------+--------+")

	for _, test := range testTimes {
		utcTime := time.UnixMilli(test * 1000).UTC().Format(time.DateTime)
		T := uint64((test - T0) / X)
		msg := make([]byte, 8)

		binary.BigEndian.PutUint64(msg, uint64(T))

		fmt.Printf("|  %11d  |  %s  | %X | %s | SHA1   |\n", test, utcTime, msg, generateTOTP(sha1.New, seed, msg, 8))
		fmt.Printf("|  %11d  |  %s  | %X | %s | SHA256 |\n", test, utcTime, msg, generateTOTP(sha256.New, seed32, msg, 8))
		fmt.Printf("|  %11d  |  %s  | %X | %s | SHA512 |\n", test, utcTime, msg, generateTOTP(sha512.New, seed64, msg, 8))

		fmt.Println("+---------------+-----------------------+------------------+----------+--------+")
	}
}
