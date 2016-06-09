﻿/***********************************************************************

CoreLib - The MIT License (MIT)
Copyright (c) 2016, Yong He

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.

***********************************************************************/

/***********************************************************************
WARNING: This is an automatically generated file.
***********************************************************************/
#include "Basic.h"

/***********************************************************************
LIBIO.CPP
***********************************************************************/
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif
namespace CoreLib
{
	namespace IO
	{
		using namespace CoreLib::Basic;

		bool File::Exists(const String & fileName)
		{
			struct stat sts;
			return stat(((String)fileName).ToMultiByteString(), &sts) != -1;
		}

		String Path::TruncateExt(const String & path)
		{
			int dotPos = path.LastIndexOf(L'.');
			if (dotPos != -1)
				return path.SubString(0, dotPos);
			else
				return path;
		}
		String Path::ReplaceExt(const String & path, const wchar_t * newExt)
		{
			StringBuilder sb(path.Length()+10);
			int dotPos = path.LastIndexOf(L'.');
			if (dotPos == -1)
				dotPos = path.Length();
			sb.Append(path.Buffer(), dotPos);
			sb.Append(L'.');
			sb.Append(newExt);
			return sb.ProduceString();
		}
		String Path::GetFileName(const String & path)
		{
			int pos = path.LastIndexOf(L'/');
			pos = Math::Max(path.LastIndexOf(L'\\'), pos) + 1;
			return path.SubString(pos, path.Length()-pos);
		}
		String Path::GetFileNameWithoutEXT(const String & path)
		{
			int pos = path.LastIndexOf(L'/');
			pos = Math::Max(path.LastIndexOf(L'\\'), pos) + 1;
			int dotPos = path.LastIndexOf(L'.');
			if (dotPos <= pos)
				dotPos = path.Length();
			return path.SubString(pos, dotPos - pos);
		}
		String Path::GetFileExt(const String & path)
		{
			int dotPos = path.LastIndexOf(L'.');
			if (dotPos != -1)
				return path.SubString(dotPos+1, path.Length()-dotPos-1);
			else
				return L"";
		}
		String Path::GetDirectoryName(const String & path)
		{
			int pos = path.LastIndexOf(L'/');
			pos = Math::Max(path.LastIndexOf(L'\\'), pos);
			if (pos != -1)
				return path.SubString(0, pos);
			else
				return L"";
		}
		String Path::Combine(const String & path1, const String & path2)
		{
			if (path1.Length() == 0) return path2;
			StringBuilder sb(path1.Length()+path2.Length()+2);
			sb.Append(path1);
			if (!path1.EndsWith(L'\\') && !path1.EndsWith(L'/'))
				sb.Append(PathDelimiter);
			sb.Append(path2);
			return sb.ProduceString();
		}
		String Path::Combine(const String & path1, const String & path2, const String & path3)
		{
			StringBuilder sb(path1.Length()+path2.Length()+path3.Length()+3);
			sb.Append(path1);
			if (!path1.EndsWith(L'\\') && !path1.EndsWith(L'/'))
				sb.Append(PathDelimiter);
			sb.Append(path2);
			if (!path2.EndsWith(L'\\') && !path2.EndsWith(L'/'))
				sb.Append(PathDelimiter);
			sb.Append(path3);
			return sb.ProduceString();
		}
#ifdef CreateDirectory
#undef CreateDirectory
#endif
		bool Path::CreateDirectory(const String & path)
		{
#if defined(_WIN32)
			return _mkdir(path.ToMultiByteString()) == 0;
#else 
			return mkdir(path.ToMultiByteString(), 0777) == 0;
#endif
		}

		CoreLib::Basic::String File::ReadAllText(const CoreLib::Basic::String & fileName)
		{
			StreamReader reader(new FileStream(fileName, FileMode::Open, FileAccess::Read, FileShare::ReadWrite));
			return reader.ReadToEnd();
		}

		void File::WriteAllText(const CoreLib::Basic::String & fileName, const CoreLib::Basic::String & text)
		{
			StreamWriter writer(new FileStream(fileName, FileMode::Create));
			writer.Write(text);
		}
	}
}

/***********************************************************************
LIBMATH.CPP
***********************************************************************/

namespace CoreLib
{
	namespace Basic
	{
		const float Math::Pi = 3.141592654f;
	}
}

/***********************************************************************
LIBSTRING.CPP
***********************************************************************/

namespace CoreLib
{
	namespace Basic
	{
		_EndLine EndLine;
		String StringConcat(const wchar_t * lhs, int leftLen, const wchar_t * rhs, int rightLen)
		{
			String res;
			res.length = leftLen + rightLen;
			res.buffer = new wchar_t[res.length + 1];
			wcscpy_s(res.buffer.Ptr(), res.length + 1, lhs);
			wcscpy_s(res.buffer + leftLen, res.length + 1 - leftLen, rhs);
			return res;
		}
		String operator+(const wchar_t * op1, const String & op2)
		{
			if(!op2.buffer)
				return String(op1);

			return StringConcat(op1, (int)wcslen(op1), op2.buffer.Ptr(), op2.length);
		}

		String operator+(const String & op1, const wchar_t*op2)
		{
			if(!op1.buffer)
				return String(op2);

			return StringConcat(op1.buffer.Ptr(), op1.length, op2, (int)wcslen(op2));
		}

		String operator+(const String & op1, const String & op2)
		{
			if(!op1.buffer && !op2.buffer)
				return String();
			else if(!op1.buffer)
				return String(op2);
			else if(!op2.buffer)
				return String(op1);

			return StringConcat(op1.buffer.Ptr(), op1.length, op2.buffer.Ptr(), op2.length);
		}

		int StringToInt(const String & str)
		{
			return (int)wcstol(str.Buffer(), NULL, 10);
			//return (int)_wcstoi64(str.Buffer(), NULL, 10);
		}
		double StringToDouble(const String & str)
		{
			return (double)wcstod(str.Buffer(), NULL);
		}

		String String::MD5() const
		{
			unsigned char result[16];
			MD5_CTX ctx;
			MD5_Init(&ctx);
			MD5_Update(&ctx, buffer.Ptr(), length * sizeof(wchar_t));
			MD5_Final(result, &ctx);
			StringBuilder strResult;
			for (int i = 0; i < 16; i++)
			{
				auto ch = String((int)result[i], 16);
				if (ch.length == 1)
					strResult << L'0';
				else
					strResult << ch;
			}
			return strResult.ProduceString();
		}

		String String::PadLeft(wchar_t ch, int pLen)
		{
			StringBuilder sb;
			for (int i = 0; i < pLen - this->length; i++)
				sb << ch;
			for (int i = 0; i < this->length; i++)
				sb << buffer[i];
			return sb.ProduceString();
		}

		String String::PadRight(wchar_t ch, int pLen)
		{
			StringBuilder sb;
			for (int i = 0; i < this->length; i++)
				sb << buffer[i];
			for (int i = 0; i < pLen - this->length; i++)
				sb << ch;
			return sb.ProduceString();
		}
	}
}

/***********************************************************************
MD5.CPP
***********************************************************************/
/*
* This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
* MD5 Message-Digest Algorithm (RFC 1321).
*
* Homepage:
* http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
*
* Author:
* Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
*
* This software was written by Alexander Peslyak in 2001.  No copyright is
* claimed, and the software is hereby placed in the public domain.
* In case this attempt to disclaim copyright and place the software in the
* public domain is deemed null and void, then the software is
* Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
* general public under the following terms:
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*
* There's ABSOLUTELY NO WARRANTY, express or implied.
*
* (This is a heavily cut-down "BSD license".)
*
* This differs from Colin Plumb's older public domain implementation in that
* no exactly 32-bit integer data type is required (any 32-bit or wider
* unsigned integer data type will do), there's no compile-time endianness
* configuration, and the function prototypes match OpenSSL's.  No code from
* Colin Plumb's implementation has been reused; this comment merely compares
* the properties of the two independent implementations.
*
* The primary goals of this implementation are portability and ease of use.
* It is meant to be fast, but not as fast as possible.  Some known
* optimizations are not included to reduce source code size and avoid
* compile-time configuration.
*/

#ifndef HAVE_OPENSSL



/*
* The basic MD5 functions.
*
* F and G are optimized compared to their RFC 1321 definitions for
* architectures that lack an AND-NOT instruction, just like in Colin Plumb's
* implementation.
*/
#define F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)			((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)			(((x) ^ (y)) ^ (z))
#define H2(x, y, z)			((x) ^ ((y) ^ (z)))
#define I(x, y, z)			((y) ^ ((x) | ~(z)))

/*
* The MD5 transformation for all four rounds.
*/
#define STEP(f, a, b, c, d, x, t, s) \
	(a) += f((b), (c), (d)) + (x)+(t); \
	(a) = (((a) << (s)) | (((a)& 0xffffffff) >> (32 - (s)))); \
	(a) += (b);

/*
* SET reads 4 input bytes in little-endian byte order and stores them
* in a properly aligned word in host byte order.
*
* The check for little-endian architectures that tolerate unaligned
* memory accesses is just an optimization.  Nothing will break if it
* doesn't work.
*/
#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#define SET(n) \
	(*(MD5_u32plus *)&ptr[(n)* 4])
#define GET(n) \
	SET(n)
#else
#define SET(n) \
	(ctx->block[(n)] = \
	(MD5_u32plus)ptr[(n)* 4] | \
	((MD5_u32plus)ptr[(n)* 4 + 1] << 8) | \
	((MD5_u32plus)ptr[(n)* 4 + 2] << 16) | \
	((MD5_u32plus)ptr[(n)* 4 + 3] << 24))
#define GET(n) \
	(ctx->block[(n)])
#endif

/*
* This processes one or more 64-byte data blocks, but does NOT update
* the bit counters.  There are no alignment requirements.
*/
static const void *body(MD5_CTX *ctx, const void *data, unsigned long size)
{
	const unsigned char *ptr;
	MD5_u32plus a, b, c, d;
	MD5_u32plus saved_a, saved_b, saved_c, saved_d;

	ptr = (const unsigned char *)data;

	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;

	do {
		saved_a = a;
		saved_b = b;
		saved_c = c;
		saved_d = d;

		/* Round 1 */
		STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7)
			STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12)
			STEP(F, c, d, a, b, SET(2), 0x242070db, 17)
			STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22)
			STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7)
			STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12)
			STEP(F, c, d, a, b, SET(6), 0xa8304613, 17)
			STEP(F, b, c, d, a, SET(7), 0xfd469501, 22)
			STEP(F, a, b, c, d, SET(8), 0x698098d8, 7)
			STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12)
			STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17)
			STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22)
			STEP(F, a, b, c, d, SET(12), 0x6b901122, 7)
			STEP(F, d, a, b, c, SET(13), 0xfd987193, 12)
			STEP(F, c, d, a, b, SET(14), 0xa679438e, 17)
			STEP(F, b, c, d, a, SET(15), 0x49b40821, 22)

			/* Round 2 */
			STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)
			STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)
			STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
			STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
			STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)
			STEP(G, d, a, b, c, GET(10), 0x02441453, 9)
			STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
			STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
			STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
			STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)
			STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
			STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)
			STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
			STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
			STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)
			STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

			/* Round 3 */
			STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)
			STEP(H2, d, a, b, c, GET(8), 0x8771f681, 11)
			STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
			STEP(H2, b, c, d, a, GET(14), 0xfde5380c, 23)
			STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)
			STEP(H2, d, a, b, c, GET(4), 0x4bdecfa9, 11)
			STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
			STEP(H2, b, c, d, a, GET(10), 0xbebfbc70, 23)
			STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
			STEP(H2, d, a, b, c, GET(0), 0xeaa127fa, 11)
			STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
			STEP(H2, b, c, d, a, GET(6), 0x04881d05, 23)
			STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
			STEP(H2, d, a, b, c, GET(12), 0xe6db99e5, 11)
			STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
			STEP(H2, b, c, d, a, GET(2), 0xc4ac5665, 23)

			/* Round 4 */
			STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)
			STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)
			STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
			STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)
			STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)
			STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
			STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
			STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)
			STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
			STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
			STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)
			STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
			STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)
			STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
			STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
			STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)

			a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} while (size -= 64);

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	return ptr;
}

void MD5_Init(MD5_CTX *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;

	ctx->lo = 0;
	ctx->hi = 0;
}

void MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size)
{
	MD5_u32plus saved_lo;
	unsigned long used, available;

	saved_lo = ctx->lo;
	if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
		ctx->hi++;
	ctx->hi += size >> 29;

	used = saved_lo & 0x3f;

	if (used) {
		available = 64 - used;

		if (size < available) {
			memcpy(&ctx->buffer[used], data, size);
			return;
		}

		memcpy(&ctx->buffer[used], data, available);
		data = (const unsigned char *)data + available;
		size -= available;
		body(ctx, ctx->buffer, 64);
	}

	if (size >= 64) {
		data = body(ctx, data, size & ~(unsigned long)0x3f);
		size &= 0x3f;
	}

	memcpy(ctx->buffer, data, size);
}

void MD5_Final(unsigned char *result, MD5_CTX *ctx)
{
	unsigned long used, available;

	used = ctx->lo & 0x3f;

	ctx->buffer[used++] = 0x80;

	available = 64 - used;

	if (available < 8) {
		memset(&ctx->buffer[used], 0, available);
		body(ctx, ctx->buffer, 64);
		used = 0;
		available = 64;
	}

	memset(&ctx->buffer[used], 0, available - 8);

	ctx->lo <<= 3;
	ctx->buffer[56] = (unsigned char)(ctx->lo);
	ctx->buffer[57] = (unsigned char)(ctx->lo >> 8);
	ctx->buffer[58] = (unsigned char)(ctx->lo >> 16);
	ctx->buffer[59] = (unsigned char)(ctx->lo >> 24);
	ctx->buffer[60] = (unsigned char)(ctx->hi);
	ctx->buffer[61] = (unsigned char)(ctx->hi >> 8);
	ctx->buffer[62] = (unsigned char)(ctx->hi >> 16);
	ctx->buffer[63] = (unsigned char)(ctx->hi >> 24);

	body(ctx, ctx->buffer, 64);

	result[0] = (unsigned char)(ctx->a);
	result[1] = (unsigned char)(ctx->a >> 8);
	result[2] = (unsigned char)(ctx->a >> 16);
	result[3] = (unsigned char)(ctx->a >> 24);
	result[4] = (unsigned char)(ctx->b);
	result[5] = (unsigned char)(ctx->b >> 8);
	result[6] = (unsigned char)(ctx->b >> 16);
	result[7] = (unsigned char)(ctx->b >> 24);
	result[8] = (unsigned char)(ctx->c);
	result[9] = (unsigned char)(ctx->c >> 8);
	result[10] = (unsigned char)(ctx->c >> 16);
	result[11] = (unsigned char)(ctx->c >> 24);
	result[12] = (unsigned char)(ctx->d);
	result[13] = (unsigned char)(ctx->d >> 8);
	result[14] = (unsigned char)(ctx->d >> 16);
	result[15] = (unsigned char)(ctx->d >> 24);

	memset(ctx, 0, sizeof(*ctx));
}

#endif

/***********************************************************************
PARSER.CPP
***********************************************************************/

using namespace CoreLib::Basic;

namespace CoreLib
{
	namespace Text
	{
		RefPtr<MetaLexer> Parser::metaLexer;
		MetaLexer * Parser::GetTextLexer()
		{
			if (!metaLexer)
			{
				metaLexer = new MetaLexer();
				metaLexer->SetLexProfile(
					L"#WhiteSpace = {\\s+}\n"\
					L"#SingleLineComment = {//[^\\n]*\\n}\n"\
					L"#MultiLineComment = {/\\*([^*]|\\*[^/])*\\*/}\n"\
					L"Identifier = {[a-zA-Z_]\\w*}\n"\
					L"IntConstant = {\\d+}\n"\
					L"FloatConstant = {\\d*.\\d+|\\d+(.\\d+)?(e(-)?\\d+)?}\n"\
					L"StringConstant = {\"([^\\\\\"]|\\\\\\.)*\"}\n"\
					L"CharConstant = {'[^\\n\\r]*'}\n"\
					L"LParent = {\\(}\n"\
					L"RParent = {\\)}\n"\
					L"LBrace = {{}\n"\
					L"RBrace = {}}\n"\
					L"LBracket = {\\[}\n"\
					L"RBracket = {\\]}\n"\
					L"Dot = {.}\n"\
					L"Semicolon = {;}\n"\
					L"Comma = {,}\n"\
					L"Colon = {:}\n"\
					L"OpAdd = {\\+}\n"\
					L"OpSub = {-}\n"\
					L"OpDiv = {/}\n"\
					L"OpMul = {\\*}\n"\
					L"OpMod = {%}\n"\
					L"OpExp = {^}\n"\
					L"OpGreater = {>}\n"\
					L"OpLess = {<}\n"\
					L"OpEqual = {==}\n"\
					L"OpGEqual = {>=}\n"\
					L"OpLEqual = {<=}\n"\
					L"OpNEqual = {!=}\n"\
					L"OpAnd = {&}\n"\
					L"OpOr = {\\|}\n"\
					L"OpNot = {!}\n"\
					L"OpAssign = {=}\n"\
					L"OpDollar = {$}\n"
					);
			}
			return metaLexer.Ptr();
		}
		void Parser::DisposeTextLexer()
		{
			metaLexer = nullptr;
		}
		Basic::List<Basic::String> Parser::SplitString(Basic::String str, wchar_t ch)
		{
			List<String> result;
			StringBuilder currentBuilder;
			for (int i = 0; i < str.Length(); i++)
			{
				if (str[i] == ch)
				{
					result.Add(currentBuilder.ToString());
					currentBuilder.Clear();
				}
				else
					currentBuilder.Append(str[i]);
			}
			result.Add(currentBuilder.ToString());
			return result;
		}
		Parser::Parser(String text)
		{
			this->text = text;
			
			stream = GetTextLexer()->Parse(text);
			for (auto token : stream)
			{
				if (token.TypeID != -1)
					tokens.Add(token);
			}
			tokenPtr = 0;
		}


		List<String> Split(String text, wchar_t c)
		{
			List<String> result;
			StringBuilder sb;
			for (int i = 0; i < text.Length(); i++)
			{
				if (text[i] == c)
				{
					auto str = sb.ToString();
					if (str.Length() != 0)
						result.Add(str);
					sb.Clear();
				}
				else
					sb << text[i];
			}
			auto lastStr = sb.ToString();
			if (lastStr.Length())
				result.Add(lastStr);
			return result;
		}

	}
}

/***********************************************************************
PERFORMANCECOUNTER.CPP
***********************************************************************/

using namespace std::chrono;

namespace CoreLib
{
	namespace Diagnostics
	{
		TimePoint PerformanceCounter::Start()
		{
			return high_resolution_clock::now();
		}

		Duration PerformanceCounter::End(TimePoint counter)
		{
			return high_resolution_clock::now()-counter;
		}

		float PerformanceCounter::EndSeconds(TimePoint counter)
		{
			return (float)ToSeconds(high_resolution_clock::now() - counter);
		}

		double PerformanceCounter::ToSeconds(Duration counter)
		{
			auto rs = duration_cast<duration<double>>(counter);
			return *(double*)&rs;
		}
	}
}

/***********************************************************************
STREAM.CPP
***********************************************************************/
#ifdef _WIN32
#include <share.h>
#endif

namespace CoreLib
{
	namespace IO
	{
		using namespace CoreLib::Basic;
		FileStream::FileStream(const CoreLib::Basic::String & fileName, FileMode fileMode)
		{
			Init(fileName, fileMode, fileMode==FileMode::Open?FileAccess::Read:FileAccess::Write, FileShare::None);
		}
		FileStream::FileStream(const CoreLib::Basic::String & fileName, FileMode fileMode, FileAccess access, FileShare share)
		{
			Init(fileName, fileMode, access, share);
		}
		void FileStream::Init(const CoreLib::Basic::String & fileName, FileMode fileMode, FileAccess access, FileShare share)
		{
			const wchar_t * mode = L"rt";
			const char* modeMBCS = "rt";
			switch (fileMode)
			{
			case CoreLib::IO::FileMode::Create:
				if (access == FileAccess::Read)
					throw ArgumentException(L"Read-only access is incompatible with Create mode.");
				else if (access == FileAccess::ReadWrite)
				{
					mode = L"w+b";
					modeMBCS = "w+b";
					this->fileAccess = FileAccess::ReadWrite;
				}
				else
				{
					mode = L"wb";
					modeMBCS = "wb";
					this->fileAccess = FileAccess::Write;
				}
				break;
			case CoreLib::IO::FileMode::Open:
				if (access == FileAccess::Read)
				{
					mode = L"rb";
					modeMBCS = "rb";
					this->fileAccess = FileAccess::Read;
				}
				else if (access == FileAccess::ReadWrite)
				{
					mode = L"r+b";
					modeMBCS = "r+b";
					this->fileAccess = FileAccess::ReadWrite;
				}
				else
				{
					mode = L"wb";
					modeMBCS = "wb";
					this->fileAccess = FileAccess::Write;
				}
				break;
			case CoreLib::IO::FileMode::CreateNew:
				if (File::Exists(fileName))
				{
					throw IOException(L"Failed opening '" + fileName + L"', file already exists.");
				}
				if (access == FileAccess::Read)
					throw ArgumentException(L"Read-only access is incompatible with Create mode.");
				else if (access == FileAccess::ReadWrite)
				{
					mode = L"w+b";
					this->fileAccess = FileAccess::ReadWrite;
				}
				else
				{
					mode = L"wb";
					this->fileAccess = FileAccess::Write;
				}
				break;
			case CoreLib::IO::FileMode::Append:
				if (access == FileAccess::Read)
					throw ArgumentException(L"Read-only access is incompatible with Append mode.");
				else if (access == FileAccess::ReadWrite)
				{
					mode = L"a+b";
					this->fileAccess = FileAccess::ReadWrite;
				}
				else
				{
					mode = L"ab";
					this->fileAccess = FileAccess::Write;
				}
				break;
			default:
				break;
			}
			int shFlag;
#ifdef _WIN32
			switch (share)
			{
			case CoreLib::IO::FileShare::None:
				shFlag = _SH_DENYRW;
				break;
			case CoreLib::IO::FileShare::ReadOnly:
				shFlag = _SH_DENYWR;
				break;
			case CoreLib::IO::FileShare::WriteOnly:
				shFlag = _SH_DENYRD;
				break;
			case CoreLib::IO::FileShare::ReadWrite:
				shFlag = _SH_DENYNO;
				break;
			default:
				throw ArgumentException(L"Invalid file share mode.");
				break;
			}
			handle = _wfsopen(fileName.Buffer(), mode, shFlag);
#else
			handle = fopen(fileName.ToMultiByteString(), modeMBCS);
#endif
			if (!handle)
			{
				throw IOException(L"Cannot open file '" + fileName + L"'");
			}
		}
		FileStream::~FileStream()
		{
			Close();
		}
		Int64 FileStream::GetPosition()
		{
#ifdef _WIN32
			fpos_t pos;
			fgetpos(handle, &pos);
			return pos;
#else
			fpos64_t pos;
			fgetpos64(handle, &pos);
			return *(Int64*)(&pos);
#endif
		}
		void FileStream::Seek(SeekOrigin origin, Int64 offset)
		{
			int _origin;
			switch (origin)
			{
			case CoreLib::IO::SeekOrigin::Start:
				_origin = SEEK_SET;
				endReached = false;
				break;
			case CoreLib::IO::SeekOrigin::End:
				_origin = SEEK_END;
				endReached = true;
				break;
			case CoreLib::IO::SeekOrigin::Current:
				_origin = SEEK_CUR;
				endReached = false;
				break;
			default:
				throw NotSupportedException(L"Unsupported seek origin.");
				break;
			}
#ifdef _WIN32
			int rs = _fseeki64(handle, offset, _origin);
#else
			int rs = fseek(handle, (int)offset, _origin);
#endif
			if (rs != 0)
			{
				throw IOException(L"FileStream seek failed.");
			}
		}
		Int64 FileStream::Read(void * buffer, Int64 length)
		{
			auto bytes = fread_s(buffer, (size_t)length, 1, (size_t)length, handle);
			if (bytes == 0 && length > 0)
			{
				if (!feof(handle))
					throw IOException(L"FileStream read failed.");
				else if (endReached)
					throw EndOfStreamException(L"End of file is reached.");
				endReached = true;
			}
			return (int)bytes;
		}
		Int64 FileStream::Write(const void * buffer, Int64 length)
		{
			auto bytes = (Int64)fwrite(buffer, 1, (size_t)length, handle);
			if (bytes < length)
			{
				throw IOException(L"FileStream write failed.");
			}
			return bytes;
		}
		bool FileStream::CanRead()
		{
			return ((int)fileAccess & (int)FileAccess::Read) != 0;
		}
		bool FileStream::CanWrite()
		{
			return ((int)fileAccess & (int)FileAccess::Write) != 0;
		}
		void FileStream::Close()
		{
			if (handle)
			{
				fclose(handle);
				handle = 0;
			}
		}
		bool FileStream::IsEnd()
		{
			return endReached;
		}
	}
}

/***********************************************************************
TEXTIO.CPP
***********************************************************************/
#ifdef _WIN32
#include <Windows.h>
#define CONVERT_END_OF_LINE
#endif

namespace CoreLib
{
	namespace IO
	{
		using namespace CoreLib::Basic;

		class UnicodeEncoding : public Encoding //UTF8
		{
		public:
			virtual void GetBytes(List<char> & result, const String & str) override
			{
				for (int i = 0; i < str.Length(); i++)
				{
					unsigned int codePoint = str[i];
					if (codePoint >= 0xD800 && codePoint <= 0xDBFF && i < str.Length() - 1) // surrogate
					{
						codePoint -= 0xD800;
						codePoint <<= 10;
						i++;
						codePoint += str[i] - 0xDC00;
						codePoint += 0x10000;
					}
					// encode codePoint as UTF8
					if (codePoint <= 0x7F)
						result.Add((char)codePoint);
					else if (codePoint <= 0x7FF)
					{
						unsigned char byte = (unsigned char)(0b11000000 + (codePoint >> 6));
						result.Add((char)byte);
						byte = 0b10000000 + (codePoint & 0b111111);
						result.Add((char)byte);
					}
					else if (codePoint <= 0xFFFF)
					{
						unsigned char byte = (unsigned char)(0b11100000 + (codePoint >> 12));
						result.Add((char)byte);
						byte = (unsigned char)(0b10000000 + ((codePoint >> 6) & (0b111111)));
						result.Add((char)byte);
						byte = (unsigned char)(0b10000000 + (codePoint & 0b111111));
						result.Add((char)byte);
					}
					else
					{
						unsigned char byte = (unsigned char)(0b11110000 + (codePoint >> 18));
						result.Add((char)byte);
						byte = (unsigned char)(0b10000000 + ((codePoint >> 12) & 0b111111));
						result.Add((char)byte);
						byte = (unsigned char)(0b10000000 + ((codePoint >> 6) & 0b111111));
						result.Add((char)byte);
						byte = (unsigned char)(0b10000000 + (codePoint & 0b111111));
						result.Add((char)byte);
					}
				}
			}
		};

		class Utf16Encoding : public Encoding //UTF16
		{
		private:
			bool reverseOrder = false;
		public:
			Utf16Encoding(bool pReverseOrder)
				: reverseOrder(pReverseOrder)
			{}
			virtual void GetBytes(List<char> & result, const String & str) override
			{
				auto addChar = [&](unsigned short ch)
				{
					if (reverseOrder)
					{
						unsigned char firstByte = ch >> 8;
						unsigned char lastByte = ch & 0b1111'1111;
						result.Add((char)firstByte);
						result.Add((char)lastByte);
					}
					else
						result.AddRange((char*)&ch, 2);
				};
#ifdef _WIN32
				if (reverseOrder)
				{
					for (int i = 0; i < str.Length(); i++)
					{
						unsigned short ch = (unsigned short)str[i];
						addChar(ch);
					}
				}
				else
					result.AddRange((char*)str.Buffer(), str.Length() * sizeof(wchar_t));
#else
				for (int i = 0; i < str.Length(); i++)
				{
					unsigned int codePoint = str[i];
					if (codePoint <= 0xD7FF || codePoint >= 0xE000 && codePoint <= 0xFFFF)
					{
						unsigned short toWrite = (unsigned short)codePoint;
						addChar(toWrite);
					}
					else
					{
						int sub = codePoint - 0x10000;
						unsigned short high = (unsigned short)((sub >> 10) + 0xD800);
						unsigned short low = (unsigned short)((sub & 0x3FF) + 0xDC00);
						addChar(high);
						addChar(low);
					}
				}
#endif
			}
		};


		class AnsiEncoding : public Encoding
		{
		private:
			static char * WideCharToAnsi(wchar_t * buffer, int length)
			{
				return WideCharToMByte(buffer, length);
			}
		public:
			virtual void GetBytes(List<char> & result, const String & str) override
			{
				String cpy = str;
				int len;
				char * buffer = cpy.ToMultiByteString(&len);
				result.AddRange(buffer, len);
			}
		};

		UnicodeEncoding __unicodeEncoding;
		Utf16Encoding __utf16Encoding(false);
		Utf16Encoding __utf16EncodingReversed(true);
		AnsiEncoding __ansiEncoding;

		Encoding * Encoding::UTF8 = &__unicodeEncoding;
		Encoding * Encoding::UTF16 = &__utf16Encoding;
		Encoding * Encoding::UTF16Reversed = &__utf16EncodingReversed;
		Encoding * Encoding::Ansi = &__ansiEncoding;

		const unsigned short Utf16Header = 0xFEFF;
		const unsigned short Utf16ReversedHeader = 0xFFFE;


		const unsigned char Utf8Header[] = { 0xEF,0xBB,0xBF };

		StreamWriter::StreamWriter(const String & path, Encoding * encoding)
		{
			this->stream = new FileStream(path, FileMode::Create);
			this->encoding = encoding;
			if (encoding == Encoding::UTF16)
			{
				this->stream->Write(&Utf16Header, 2);
			}
			else if (encoding == Encoding::UTF16Reversed)
			{
				this->stream->Write(&Utf16ReversedHeader, 2);
			}
		}
		StreamWriter::StreamWriter(RefPtr<Stream> stream, Encoding * encoding)
		{
			this->stream = stream;
			this->encoding = encoding;
			if (encoding == Encoding::UTF16)
			{
				this->stream->Write(&Utf16Header, 2);
			}
			else if (encoding == Encoding::UTF16Reversed)
			{
				this->stream->Write(&Utf16ReversedHeader, 2);
			}
		}
		void StreamWriter::Write(const String & str)
		{
			encodingBuffer.Clear();
			encoding->GetBytes(encodingBuffer, str);
			stream->Write(encodingBuffer.Buffer(), encodingBuffer.Count());
		}
		void StreamWriter::Write(const wchar_t * str)
		{
			encodingBuffer.Clear();
			encoding->GetBytes(encodingBuffer, String(str));
			stream->Write(encodingBuffer.Buffer(), encodingBuffer.Count());
		}
		void StreamWriter::Write(const char * str)
		{
			encodingBuffer.Clear();
			encoding->GetBytes(encodingBuffer, String(str));
			stream->Write(encodingBuffer.Buffer(), encodingBuffer.Count());
		}

		StreamReader::StreamReader(const String & path)
		{
			stream = new FileStream(path, FileMode::Open);
			ReadBuffer();
			encoding = DetermineEncoding();
			if (encoding == 0)
				encoding = Encoding::Ansi;
		}
		StreamReader::StreamReader(RefPtr<Stream> stream, Encoding * encoding)
		{
			this->stream = stream;
			this->encoding = encoding;
			ReadBuffer();
			auto determinedEncoding = DetermineEncoding();
			if (this->encoding == nullptr)
				this->encoding = determinedEncoding;
		}

		Encoding * StreamReader::DetermineEncoding()
		{
			if (buffer.Count() >= 3 && (unsigned char)(buffer[0]) == 0xEF && (unsigned char)(buffer[1]) == 0xBB && (unsigned char)(buffer[2]) == 0xBF)
			{
				ptr += 3;
				return Encoding::UTF8;
			}
			else if (*((unsigned short*)(buffer.Buffer())) == 0xFEFF)
			{
				ptr += 2;
				return Encoding::UTF16;
			}
			else if (*((unsigned short*)(buffer.Buffer())) == 0xFFFE)
			{
				ptr += 2;
				return Encoding::UTF16Reversed;
			}
			else
			{
#ifdef _WIN32
				int flag = IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_REVERSE_SIGNATURE | IS_TEXT_UNICODE_STATISTICS | IS_TEXT_UNICODE_ASCII16;
				int rs = IsTextUnicode(buffer.Buffer(), buffer.Count(), &flag);
				if (rs)
				{
					if (flag & (IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_STATISTICS))
						return Encoding::UTF16;
					else if (flag & (IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_STATISTICS))
						return Encoding::UTF16Reversed;
					else if (flag & IS_TEXT_UNICODE_ASCII16)
						return Encoding::Ansi;
				}
#endif 
				return Encoding::UTF8;
			}
		}
		
		void StreamReader::ReadBuffer()
		{
			buffer.SetSize(4096);
			auto len = stream->Read(buffer.Buffer(), buffer.Count());
			buffer.SetSize((int)len);
			ptr = 0;
		}

		char StreamReader::ReadBufferChar()
		{
			if (ptr<buffer.Count())
			{
				return buffer[ptr++];
			}
			if (!stream->IsEnd())
				ReadBuffer();
			if (ptr<buffer.Count())
			{
				return buffer[ptr++];
			}
			return 0;
		}
		int TextReader::Read(wchar_t * destBuffer, int length)
		{
			int i = 0;
			for (i = 0; i<length; i++)
			{
				try
				{
					auto ch = Read();
					if (IsEnd())
						break;
					if (ch == L'\r')
					{
						if (Peak() == L'\n')
							Read();
						break;
					}
					else if (ch == L'\n')
					{
						break;
					}
					destBuffer[i] = ch;
				}
				catch (EndOfStreamException)
				{
					break;
				}
			}
			return i;
		}
		String StreamReader::ReadLine()
		{
			StringBuilder sb(256);
			while (!IsEnd())
			{
				try
				{
					auto ch = Read();
					if (IsEnd())
						break;
					if (ch == L'\r')
					{
						if (Peak() == L'\n')
							Read();
						break;
					}
					else if (ch == L'\n')
					{
						break;
					}
					sb.Append(ch);
				}
				catch (EndOfStreamException)
				{
					break;
				}
			}
			return sb.ProduceString();
		}
		String StreamReader::ReadToEnd()
		{
			StringBuilder sb(16384);
			while (!IsEnd())
			{
				try
				{
					auto ch = Read();
					if (IsEnd())
						break;
					if (ch == L'\r')
					{
						sb.Append(L'\n');
						if (Peak() == L'\n')
							Read();
					}
					else
						sb.Append(ch);
				}
				catch (EndOfStreamException)
				{
					break;
				}
			}
			return sb.ProduceString();
		}
	}
}

/***********************************************************************
THREADING.CPP
***********************************************************************/

#ifdef _WIN32
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

namespace CoreLib
{
	namespace Threading
	{
		unsigned int __stdcall ThreadProcedure(const ThreadParam& param)
		{
			if (param.thread->paramedThreadProc)
				param.thread->paramedThreadProc->Invoke(param.threadParam);
			else
				param.thread->threadProc->Invoke();
			return 0;
		}

		int ParallelSystemInfo::GetProcessorCount()
		{
		#ifdef _WIN32
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			return sysinfo.dwNumberOfProcessors;
		#elif MACOS
			int nm[2];
			size_t len = 4;
			uint32_t count;

			nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
			sysctl(nm, 2, &count, &len, NULL, 0);

			if(count < 1) {
				nm[1] = HW_NCPU;
				sysctl(nm, 2, &count, &len, NULL, 0);
				if(count < 1) { count = 1; }
			}
			return count;
		#else
			return sysconf(_SC_NPROCESSORS_ONLN);
		#endif
		}
	}
}

/***********************************************************************
VECTORMATH.CPP
***********************************************************************/

namespace VectorMath
{
	const __m128 Matrix4_M128::VecOne = _mm_set_ps1(1.0f);
	void Matrix4::Rotation(Matrix4 & rs, const Vec3 & axis, float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		float t = 1.0f - c;

		Vec3 nAxis;
		Vec3::Normalize(nAxis, axis);
		float x = nAxis.x;
		float y = nAxis.y;
		float z = nAxis.z;

		rs.m[0][0] = 1 + t*(x*x-1);
		rs.m[1][0] = z*s+t*x*y;
		rs.m[2][0] = -y*s+t*x*z;
		rs.m[3][0] = 0.0f;

		rs.m[0][1] = -z*s+t*x*y;
		rs.m[1][1] = 1+t*(y*y-1);
		rs.m[2][1] = x*s+t*y*z;
		rs.m[3][1] = 0.0f;

		rs.m[0][2] = y*s+t*x*z;
		rs.m[1][2] = -x*s+t*y*z;
		rs.m[2][2] = 1+t*(z*z-1);
		rs.m[3][2] = 0.0f;

		rs.m[0][3] = 0.0f;
		rs.m[1][3] = 0.0f;
		rs.m[2][3] = 0.0f;
		rs.m[3][3] = 1.0f;
	}
	void Matrix4::Rotation(Matrix4 & rs, float yaw, float pitch, float roll)
	{
		Matrix4 mat;
		Matrix4::RotationY(rs, yaw);
		Matrix4::RotationX(mat, pitch);
		Matrix4::Multiply(rs, rs, mat);
		Matrix4::RotationZ(mat, roll);
		Matrix4::Multiply(rs, rs, mat);
	}

	void Matrix4::GetNormalMatrix(Matrix4 & mOut) const
	{
		float fDet = (mi._11 * (mi._22 * mi._33 - mi._23 * mi._32) -
			mi._12 * (mi._21 * mi._33 - mi._23 * mi._31) +
			mi._13 * (mi._21 * mi._32 - mi._22 * mi._31));
		float fDetInv = 1.0f / fDet;

		mOut.mi._11 = fDetInv * (mi._22 * mi._33 - mi._23 * mi._32);
		mOut.mi._21 = -fDetInv * (mi._12 * mi._33 - mi._13 * mi._32);
		mOut.mi._31 = fDetInv * (mi._12 * mi._23 - mi._13 * mi._22);

		mOut.mi._12 = -fDetInv * (mi._21 * mi._33 - mi._23 * mi._31);
		mOut.mi._22 = fDetInv * (mi._11 * mi._33 - mi._13 * mi._31);
		mOut.mi._32 = -fDetInv * (mi._11 * mi._23 - mi._13 * mi._21);

		mOut.mi._13 = fDetInv * (mi._21 * mi._32 - mi._22 * mi._31);
		mOut.mi._23 = -fDetInv * (mi._11 * mi._32 - mi._12 * mi._31);
		mOut.mi._33 = fDetInv * (mi._11 * mi._22 - mi._12 * mi._21);

		mOut.mi._14 = 0.0f;
		mOut.mi._24 = 0.0f;
		mOut.mi._34 = 0.0f;
		mOut.mi._41 = 0.0f;
		mOut.mi._42 = 0.0f;
		mOut.mi._43 = 0.0f;
		mOut.mi._44 = 1.0f;
	}
	
	float Matrix4::Inverse3D(Matrix4 & mOut_d) const
	{
		if(fabs(mi._44 - 1.0f) > 0.001f)
			return 0.0f;
		if(fabs(mi._14)>0.001f || fabs(mi._24)>0.001f || fabs(mi._34)>0.001f)
			return 0.0f;

		float fDet = (mi._11 * (mi._22 * mi._33 - mi._23 * mi._32) - 
		mi._12 * (mi._21 * mi._33 - mi._23 * mi._31) +
		mi._13 * (mi._21 * mi._32 - mi._22 * mi._31));
		float fDetInv = 1.0f / fDet;

		mOut_d.mi._11 = fDetInv * (mi._22 * mi._33 - mi._23 * mi._32);
		mOut_d.mi._12 = -fDetInv * (mi._12 * mi._33 - mi._13 * mi._32);
		mOut_d.mi._13 = fDetInv * (mi._12 * mi._23 - mi._13 * mi._22);
		mOut_d.mi._14 = 0.0f;

		mOut_d.mi._21 = -fDetInv * (mi._21 * mi._33 - mi._23 * mi._31);
		mOut_d.mi._22 = fDetInv * (mi._11 * mi._33 - mi._13 * mi._31);
		mOut_d.mi._23 = -fDetInv * (mi._11 * mi._23 - mi._13 * mi._21);
		mOut_d.mi._24 = 0.0f;

		mOut_d.mi._31 = fDetInv * (mi._21 * mi._32 - mi._22 * mi._31);
		mOut_d.mi._32 = -fDetInv * (mi._11 * mi._32 - mi._12 * mi._31);
		mOut_d.mi._33 = fDetInv * (mi._11 * mi._22 - mi._12 * mi._21);
		mOut_d.mi._34 = 0.0f;

		mOut_d.mi._41 = -(mi._41 * mOut_d.mi._11 + mi._42 * mOut_d.mi._21 + mi._43 * mOut_d.mi._31);
		mOut_d.mi._42 = -(mi._41 * mOut_d.mi._12 + mi._42 * mOut_d.mi._22 + mi._43 * mOut_d.mi._32);
		mOut_d.mi._43 = -(mi._41 * mOut_d.mi._13 + mi._42 * mOut_d.mi._23 + mi._43 * mOut_d.mi._33);
		mOut_d.mi._44 = 1.0f;

		return fDet;
	}
		
	float Matrix4::InverseFPU(Matrix4 &mOut_d) const
	{
		float succ = Inverse3D(mOut_d);
		if (succ != 0.0f)
			return succ;
		double Result[4][4];
		double tmp[12];
		double src[16];
		double det;
		for (int i = 0; i < 4; i++)
		{
			src[i+0] = m[i][0];
			src[i+4] = m[i][1];
			src[i+8] = m[i][2];
			src[i+12] = m[i][3];
		}
		tmp[0] = src[10] * src[15];
		tmp[1] = src[11] * src[14];
		tmp[2] = src[9] * src[15];
		tmp[3] = src[11] * src[13];
		tmp[4] = src[9] * src[14];
		tmp[5] = src[10] * src[13];
		tmp[6] = src[8] * src[15];
		tmp[7] = src[11] * src[12];
		tmp[8] = src[8] * src[14];
		tmp[9] = src[10] * src[12];
		tmp[10] = src[8] * src[13];
		tmp[11] = src[9] * src[12];
		Result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
		Result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
		Result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
		Result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
		Result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
		Result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
		Result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
		Result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
		Result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
		Result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
		Result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
		Result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
		Result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
		Result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
		Result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
		Result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
		tmp[0] = src[2]*src[7];
		tmp[1] = src[3]*src[6];
		tmp[2] = src[1]*src[7];
		tmp[3] = src[3]*src[5];
		tmp[4] = src[1]*src[6];
		tmp[5] = src[2]*src[5];
		tmp[6] = src[0]*src[7];
		tmp[7] = src[3]*src[4];
		tmp[8] = src[0]*src[6];
		tmp[9] = src[2]*src[4];
		tmp[10] = src[0]*src[5];
		tmp[11] = src[1]*src[4];
		Result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
		Result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
		Result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
		Result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
		Result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
		Result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
		Result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
		Result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
		Result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
		Result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
		Result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
		Result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
		Result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
		Result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
		Result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
		Result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
		det=src[0]*Result[0][0]+src[1]*Result[0][1]+src[2]*Result[0][2]+src[3]*Result[0][3];
		det = 1.0f / det;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				mOut_d.m[i][j] = (float)(Result[i][j] * det);
			}
		}
		return (float)det;
	}
	
	void Matrix4::LookAt(Matrix4 & rs, const Vec3 & pos, const Vec3 & center, const Vec3 & up)
	{
		Vec3 xAxis, yAxis, zAxis;
		Vec3::Subtract(zAxis, pos, center);
		Vec3::Normalize(zAxis, zAxis);
		Vec3::Cross(xAxis, up, zAxis);
		Vec3::Normalize(xAxis, xAxis);
		Vec3::Cross(yAxis, zAxis, xAxis);
		Vec3::Normalize(yAxis, yAxis);

		rs.m[0][0] = xAxis.x;
		rs.m[0][1] = yAxis.x;
		rs.m[0][2] = zAxis.x;
		rs.m[0][3] = 0.0f;

		rs.m[1][0] = xAxis.y;
		rs.m[1][1] = yAxis.y;
		rs.m[1][2] = zAxis.y;
		rs.m[1][3] = 0.0f;

		rs.m[2][0] = xAxis.z;
		rs.m[2][1] = yAxis.z;
		rs.m[2][2] = zAxis.z;
		rs.m[2][3] = 0.0f;

		rs.m[3][0] = -Vec3::Dot(xAxis, pos);
		rs.m[3][1] = -Vec3::Dot(yAxis, pos);
		rs.m[3][2] = -Vec3::Dot(zAxis, pos);
		rs.m[3][3] = 1.0f;
	}
	
	float Matrix4_M128::Inverse(Matrix4_M128 &mOut) const
	{
		__m128 Fac0;
		{
			__m128 Swp0a = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Swp0b = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(2, 2, 2, 2));

			__m128 Swp00 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(2, 2, 2, 2));
			__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp03 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(3, 3, 3, 3));

			__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
			__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
			Fac0 = _mm_sub_ps(Mul00, Mul01);
		}

		__m128 Fac1;
		{
			__m128 Swp0a = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Swp0b = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(1, 1, 1, 1));

			__m128 Swp00 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(1, 1, 1, 1));
			__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp03 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(3, 3, 3, 3));

			__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
			__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
			Fac1 = _mm_sub_ps(Mul00, Mul01);
		}

		__m128 Fac2;
		{
			__m128 Swp0a = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(2, 2, 2, 2));
			__m128 Swp0b = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(1, 1, 1, 1));

			__m128 Swp00 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(1, 1, 1, 1));
			__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp03 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(2, 2, 2, 2));

			__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
			__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
			Fac2 = _mm_sub_ps(Mul00, Mul01);
		}

		__m128 Fac3;
		{
			__m128 Swp0a = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Swp0b = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(0, 0, 0, 0));

			__m128 Swp00 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp03 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(3, 3, 3, 3));

			__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
			__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
			Fac3 = _mm_sub_ps(Mul00, Mul01);
		}

		__m128 Fac4;
		{
			__m128 Swp0a = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(2, 2, 2, 2));
			__m128 Swp0b = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(0, 0, 0, 0));

			__m128 Swp00 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp03 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(2, 2, 2, 2));

			__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
			__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
			Fac4 = _mm_sub_ps(Mul00, Mul01);
		}

		__m128 Fac5;
		{
			__m128 Swp0a = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(1, 1, 1, 1));
			__m128 Swp0b = _mm_shuffle_ps(C4, C3, _MM_SHUFFLE(0, 0, 0, 0));

			__m128 Swp00 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
			__m128 Swp03 = _mm_shuffle_ps(C3, C2, _MM_SHUFFLE(1, 1, 1, 1));

			__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
			__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
			Fac5 = _mm_sub_ps(Mul00, Mul01);
		}

		__m128 SignA = _mm_set_ps( 1.0f,-1.0f, 1.0f,-1.0f);
		__m128 SignB = _mm_set_ps(-1.0f, 1.0f,-1.0f, 1.0f);

		__m128 Temp0 = _mm_shuffle_ps(C2, C1, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 Vec0 = _mm_shuffle_ps(Temp0, Temp0, _MM_SHUFFLE(2, 2, 2, 0));

		__m128 Temp1 = _mm_shuffle_ps(C2, C1, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 Vec1 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(2, 2, 2, 0));

		__m128 Temp2 = _mm_shuffle_ps(C2, C1, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 Vec2 = _mm_shuffle_ps(Temp2, Temp2, _MM_SHUFFLE(2, 2, 2, 0));

		__m128 Temp3 = _mm_shuffle_ps(C2, C1, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 Vec3 = _mm_shuffle_ps(Temp3, Temp3, _MM_SHUFFLE(2, 2, 2, 0));

		__m128 Mul00 = _mm_mul_ps(Vec1, Fac0);
		__m128 Mul01 = _mm_mul_ps(Vec2, Fac1);
		__m128 Mul02 = _mm_mul_ps(Vec3, Fac2);
		__m128 Sub00 = _mm_sub_ps(Mul00, Mul01);
		__m128 Add00 = _mm_add_ps(Sub00, Mul02);
		__m128 Inv0 = _mm_mul_ps(SignB, Add00);

		__m128 Mul03 = _mm_mul_ps(Vec0, Fac0);
		__m128 Mul04 = _mm_mul_ps(Vec2, Fac3);
		__m128 Mul05 = _mm_mul_ps(Vec3, Fac4);
		__m128 Sub01 = _mm_sub_ps(Mul03, Mul04);
		__m128 Add01 = _mm_add_ps(Sub01, Mul05);
		__m128 Inv1 = _mm_mul_ps(SignA, Add01);

		__m128 Mul06 = _mm_mul_ps(Vec0, Fac1);
		__m128 Mul07 = _mm_mul_ps(Vec1, Fac3);
		__m128 Mul08 = _mm_mul_ps(Vec3, Fac5);
		__m128 Sub02 = _mm_sub_ps(Mul06, Mul07);
		__m128 Add02 = _mm_add_ps(Sub02, Mul08);
		__m128 Inv2 = _mm_mul_ps(SignB, Add02);

		__m128 Mul09 = _mm_mul_ps(Vec0, Fac2);
		__m128 Mul10 = _mm_mul_ps(Vec1, Fac4);
		__m128 Mul11 = _mm_mul_ps(Vec2, Fac5);
		__m128 Sub03 = _mm_sub_ps(Mul09, Mul10);
		__m128 Add03 = _mm_add_ps(Sub03, Mul11);
		__m128 Inv3 = _mm_mul_ps(SignA, Add03);

		__m128 Row0 = _mm_shuffle_ps(Inv0, Inv1, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 Row1 = _mm_shuffle_ps(Inv2, Inv3, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 Row2 = _mm_shuffle_ps(Row0, Row1, _MM_SHUFFLE(2, 0, 2, 0));

		// Det0 = dot(C1, Row2)
		__m128 mul0 = _mm_mul_ps(C1, Row2);
		__m128 swp0 = _mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(2, 3, 0, 1));
		__m128 add0 = _mm_add_ps(mul0, swp0);
		__m128 swp1 = _mm_shuffle_ps(add0, add0, _MM_SHUFFLE(0, 1, 2, 3));
		__m128 Det0 = _mm_add_ps(add0, swp1);

		__m128 Rcp0 = _mm_div_ps(VecOne, Det0);

		mOut.C1 = _mm_mul_ps(Inv0, Rcp0);
		mOut.C2 = _mm_mul_ps(Inv1, Rcp0);
		mOut.C3 = _mm_mul_ps(Inv2, Rcp0);
		mOut.C4 = _mm_mul_ps(Inv3, Rcp0);

		float retVal;
		_mm_store_ss(&retVal, Det0);
		return retVal;
	}

}

/***********************************************************************
WIDECHAR.CPP
***********************************************************************/
#include <locale.h>

#define _CRT_SECUIRE_NO_WARNINGS

class DefaultLocaleSetter
{
public:
	DefaultLocaleSetter()
	{
		setlocale(LC_ALL, ""); 
	};
};


char * WideCharToMByte(const wchar_t * buffer, int length)
{
	size_t requiredBufferSize;
#ifdef _MSC_VER
	wcstombs_s(&requiredBufferSize, nullptr, 0, buffer, length);
#else
	requiredBufferSize = std::wcstombs(nullptr, buffer, 0);
#endif
	if (requiredBufferSize > 0)
	{
		char * multiByteBuffer = new char[requiredBufferSize + 1];
#ifdef _MSC_VER
		wcstombs_s(&requiredBufferSize, multiByteBuffer, requiredBufferSize, buffer, length);
		auto pos = requiredBufferSize;
#else
		auto pos = std::wcstombs(multiByteBuffer, buffer, requiredBufferSize + 1);
#endif
		if (pos <= requiredBufferSize && pos >= 0)
			multiByteBuffer[pos] = 0;
		return multiByteBuffer;
	}
	else
		return 0;
}

wchar_t * MByteToWideChar(const char * buffer, int length)
{
	// regard as ansi
#ifdef _MSC_VER
	size_t bufferSize;
	mbstowcs_s((size_t*)&bufferSize, nullptr, 0, buffer, length);
#else
	size_t bufferSize = std::mbstowcs(nullptr, buffer, 0);
#endif
	if (bufferSize > 0)
	{
		wchar_t * rbuffer = new wchar_t[bufferSize +1];
		size_t pos;
#ifdef _MSC_VER
		mbstowcs_s(&pos, rbuffer, bufferSize, buffer, length);
#else
		pos = std::mbstowcs(rbuffer, buffer, bufferSize + 1);
#endif
		if (pos <= bufferSize && pos >= 0)
			rbuffer[pos] = 0;
		return rbuffer;
	}
	else
		return 0;
}

void MByteToWideChar(wchar_t * buffer, int bufferSize, const char * str, int length)
{
#ifdef _MSC_VER
	size_t pos;
	mbstowcs_s(&pos, buffer, bufferSize, str, length);
#else
	std::mbstowcs(buffer, str, bufferSize);
#endif
}

/***********************************************************************
REGEX\METALEXER.CPP
***********************************************************************/


namespace CoreLib
{
namespace Text
{
	MetaLexer::MetaLexer()
	{
	}

	MetaLexer::MetaLexer(String profile)
	{
		SetLexProfile(profile);
	}

	String MetaLexer::GetTokenName(int id)
	{
		return TokenNames[id];
	}

	int MetaLexer::GetRuleCount()
	{
		return TokenNames.Count();
	}

	void MetaLexer::SetLexProfile(String lex)
	{
		Errors.Clear();
		ParseLexProfile(lex);
		if (!Errors.Count())
			ConstructDFA();
		else
			dfa = 0;
	}

	bool IsWhiteSpace(wchar_t ch)
	{
		return (ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r' || ch == L'\v');
	}

	bool IsIdent(wchar_t ch)
	{
		return (ch >=L'A' && ch <= L'Z' || ch >= L'a' && ch<=L'z' || ch>=L'0' && ch<=L'9'
			|| ch == L'_' || ch==L'#');
	}

	bool IsLetter(wchar_t ch)
	{
		return (ch >=L'A' && ch <= L'Z' || ch >= L'a' && ch<=L'z' || ch == L'_' || ch==L'#');
	}

	bool MetaLexer::ParseLexProfile(const CoreLib::String & lex)
	{
		LinkedList<LexProfileToken> tokens;
		int ptr = 0;
		int state = 0;
		StringBuilder curToken;
		while (ptr < lex.Length())
		{
			wchar_t curChar = lex[ptr];
			wchar_t nextChar = 0;
			if (ptr+1<lex.Length())
				nextChar = lex[ptr+1];
			switch (state)
			{
			case 0:
				{
					if (IsLetter(curChar))
						state = 1;
					else if (IsWhiteSpace(curChar))
						ptr ++;
					else if (curChar == L'{')
					{
						state = 2;
						ptr ++;
					}
					else if (curChar == L'=')
						state = 3;
					else if (curChar == L'/' && nextChar == L'/')
						state = 4;
					else
					{
						LexerError err;
						err.Position = ptr;
						err.Text = String(L"[Profile Error] Illegal character \'") + curChar + L"\'";
						Errors.Add(err);
						ptr ++;
					}
					curToken.Clear();
				}
				break;
			case 1:
				{
					if (IsIdent(curChar))
					{
						curToken.Append(curChar);
						ptr ++;
					}
					else
					{
						LexProfileToken tk;
						tk.str = curToken.ToString();
						tk.type = LexProfileToken::Identifier;
						tokens.AddLast(tk);
						state = 0;
					}
				}
				break;
			case 2:
				{
					if (curChar == L'}' && (nextChar == L'\r' || nextChar == L'\n' || nextChar == 0) )
					{
						LexProfileToken tk;
						tk.str = curToken.ToString();
						tk.type = LexProfileToken::Regex;
						tokens.AddLast(tk);
						ptr ++;
						state = 0;
					}
					else
					{
						curToken.Append(curChar);
						ptr ++;
					}
				}
				break;
			case 3:
				{
					LexProfileToken tk;
					tk.str = curChar;
					tk.type = LexProfileToken::Equal;
					tokens.AddLast(tk);
					ptr ++;
					state = 0;
				}
				break;
			case 4:
				{
					if (curChar == L'\n')
						state = 0;
					else
						ptr ++;
				}
			}
		}

		// Parse tokens
		LinkedNode<LexProfileToken> * l = tokens.FirstNode();
		state = 0;
		String curName, curRegex;
		try
		{
			TokenNames.Clear();
			Regex.Clear();
			while (l)
			{
				curName = ReadProfileToken(l, LexProfileToken::Identifier);
				l = l->GetNext();
				ReadProfileToken(l, LexProfileToken::Equal);
				l = l->GetNext();
				curRegex = ReadProfileToken(l, LexProfileToken::Regex);
				l = l->GetNext();
				TokenNames.Add(curName);
				Regex.Add(curRegex);
				if (curName[0] == L'#')
					Ignore.Add(true);
				else
					Ignore.Add(false);
			}
		}
		catch(int)
		{
			return false;
		}
		return true;
	}

	String MetaLexer::ReadProfileToken(LexProfileTokenNode*n, LexProfileToken::LexProfileTokenType type)
	{
		if (n && n->Value.type == type)
		{
			return n->Value.str;
		}
		else
		{
			String name = L"[Profile Error] ";
			switch (type)
			{
			case LexProfileToken::Equal:
				name = L"\'=\'";
				break;
			case LexProfileToken::Identifier:
				name = L"Token identifier";
				break;
			case LexProfileToken::Regex:
				name = L"Regular expression";
				break;
			}
			name = name + L" expected.";
			LexerError err;
			err.Text = name;
			err.Position = 0;
			Errors.Add(err);
			throw 0;
		}
	}

	DFA_Table * MetaLexer::GetDFA()
	{
		return dfa.operator->();
	}

	void MetaLexer::ConstructDFA()
	{
		RegexParser parser;
		NFA_Graph nfa;
		NFA_Node * node = nfa.CreateNode();
		nfa.SetStartNode(node);
		for (int i=0; i<Regex.Count(); i++)
		{
			RefPtr<RegexNode> tree = parser.Parse(Regex[i]);
			if (tree)
			{
				NFA_Graph cNfa;
				cNfa.GenerateFromRegexTree(tree.operator->(), true);
				cNfa.SetTerminalIdentifier(i);
				nfa.CombineNFA(&cNfa);
				NFA_Translation * trans = nfa.CreateTranslation();
				trans->NodeDest = cNfa.GetStartNode();
				trans->NodeSrc = node;
				trans->NodeDest->PrevTranslations.Add(trans);
				trans->NodeSrc->Translations.Add(trans);
			}
			else
			{
				LexerError err;
				err.Position = 0;
				err.Text = L"Illegal regex for \"" + String(TokenNames[i]) + L"\"";
				Errors.Add(err);
				return;
			}
		}
		nfa.PostGenerationProcess();
		DFA_Graph dfaGraph;
		dfaGraph.Generate(&nfa);
		dfa = new DFA_Table();
		dfaGraph.ToDfaTable(dfa.operator ->());
	}

	LazyLexStream::Iterator & LazyLexStream::Iterator::operator ++()
	{
		auto &str = stream->InputText;
		auto sDfa = stream->GetDFA();
		auto & ignore = stream->GetIgnoreSet();
		if (lastTokenPtr == str.Length())
		{
			lastTokenPtr = -1;
			return *this;
		}

		int lastAcceptState = -1;
		int lastAcceptPtr = -1;
		while (ptr < str.Length())
		{
			if (sDfa->Tags[state]->IsFinal)
			{
				lastAcceptState = state;
				lastAcceptPtr = ptr;
			}
			Word charClass = (*sDfa->CharTable)[str[ptr]];
			if (charClass == 0xFFFF)
			{
				ptr++;
				continue;
			}
			int nextState = sDfa->DFA[state][charClass];
			if (nextState >= 0)
			{
				state = nextState;
				ptr++;
			}
			else
			{
				if (lastAcceptState != -1)
				{
					state = lastAcceptState;
					ptr = lastAcceptPtr;
					
					
					if (!ignore[sDfa->Tags[state]->TerminalIdentifiers[0]])
					{
						currentToken.Length = ptr - lastTokenPtr;
						currentToken.TypeID = sDfa->Tags[state]->TerminalIdentifiers[0];
						currentToken.Position = lastTokenPtr;
						state = sDfa->StartState;
						lastTokenPtr = ptr;
						lastAcceptState = -1;
						lastAcceptPtr = -1;
						break;
					}
					state = sDfa->StartState;
					lastTokenPtr = ptr;
					lastAcceptState = -1;
					lastAcceptPtr = -1;
				}
				else
				{
					ptr++;
					lastAcceptState = lastAcceptPtr = -1;
					lastTokenPtr = ptr;
					state = sDfa->StartState;
					continue;
				}
			}
		}
		if (ptr == str.Length())
		{
			if (sDfa->Tags[state]->IsFinal &&
				!ignore[sDfa->Tags[state]->TerminalIdentifiers[0]])
			{
				currentToken.Length = ptr - lastTokenPtr;
				currentToken.TypeID = sDfa->Tags[state]->TerminalIdentifiers[0];
				currentToken.Position = lastTokenPtr;
			}
			else
			{
				currentToken.Length = 0;
				currentToken.TypeID = -1;
				currentToken.Position = lastTokenPtr;
			}
			lastTokenPtr = ptr;
		}
		
		return *this;
	}

	bool MetaLexer::Parse(String str, LexStream & stream)
	{
		TokensParsed = 0;
		if (!dfa)
			return false;
		int ptr = 0;
		int lastAcceptState = -1;
		int lastAcceptPtr = -1;
		int lastTokenPtr = 0;
		int state = dfa->StartState;
		while (ptr<str.Length())
		{
			if (dfa->Tags[state]->IsFinal)
			{
				lastAcceptState = state;
				lastAcceptPtr = ptr;
			}
			Word charClass = (*dfa->CharTable)[str[ptr]];
			if (charClass == 0xFFFF)
			{
				LexerError err;
				err.Text = String(L"Illegal character \'") + str[ptr] + L"\'";
				err.Position = ptr;
				Errors.Add(err);
				ptr++;
				continue;
			}
			int nextState = dfa->DFA[state][charClass];
			if (nextState >= 0)
			{
				state = nextState;
				ptr++;
			}
			else
			{
				if (lastAcceptState != -1)
				{
					state = lastAcceptState;
					ptr = lastAcceptPtr;
					if (!Ignore[dfa->Tags[state]->TerminalIdentifiers[0]])
					{
						LexToken tk;
						tk.Str = str.SubString(lastTokenPtr, ptr-lastTokenPtr);
						tk.TypeID = dfa->Tags[state]->TerminalIdentifiers[0];
						tk.Position = lastTokenPtr;
						stream.AddLast(tk);
					}
					TokensParsed ++;
					lastTokenPtr = ptr;
					state = dfa->StartState;
					lastAcceptState = -1;
					lastAcceptPtr = -1;
				}
				else
				{
					LexerError err;
					err.Text = L"Illegal token \'" +
						str.SubString(lastTokenPtr, ptr-lastTokenPtr) + L"\'";
					err.Position = ptr;
					Errors.Add(err);
					ptr++;
					lastAcceptState = lastAcceptPtr = -1;
					lastTokenPtr = ptr;
					state = dfa->StartState;
					continue;
				}
			}
		}

		if (dfa->Tags[state]->IsFinal &&
			!Ignore[dfa->Tags[state]->TerminalIdentifiers[0]])
		{
			LexToken tk;
			tk.Str = str.SubString(lastTokenPtr, ptr-lastTokenPtr);
			tk.TypeID = dfa->Tags[state]->TerminalIdentifiers[0];
			stream.AddLast(tk);
			TokensParsed ++;
		}
		return (Errors.Count() == 0);
	}
}
}

/***********************************************************************
REGEX\REGEX.CPP
***********************************************************************/

namespace CoreLib
{
namespace Text
{
	RegexMatcher::RegexMatcher(DFA_Table * table)
		:dfa(table)
	{
	}

	int RegexMatcher::Match(const String & str, int startPos)
	{
		int state = dfa->StartState;
		if (state == -1)
			return -1;
		for (int i=startPos; i<str.Length(); i++)
		{
			Word charClass = (*dfa->CharTable)[str[i]];
			if (charClass == 0xFFFF)
				return -1;
			int nextState = dfa->DFA[state][charClass];
			if (nextState == -1)
			{
				if (dfa->Tags[state]->IsFinal)
					return i-startPos;
				else
					return -1;
			}
			else
				state = nextState;
		}
		if (dfa->Tags[state]->IsFinal)
			return str.Length()-startPos;
		else
			return -1;
	}

	DFA_Table * PureRegex::GetDFA()
	{
		return dfaTable.operator->();
	}

	PureRegex::PureRegex(const String & regex)
	{
		RegexParser p;
		RefPtr<RegexNode> tree = p.Parse(regex);
		if (tree)
		{
			NFA_Graph nfa;
			nfa.GenerateFromRegexTree(tree.operator ->());
			DFA_Graph dfa;
			dfa.Generate(&nfa);
			dfaTable = new DFA_Table();
			dfa.ToDfaTable(dfaTable.operator->());
		}
		else
		{
			IllegalRegexException ex;
			if (p.Errors.Count())
				ex.Message = p.Errors[0].Text;
			throw ex;
		}
	}

	bool PureRegex::IsMatch(const String & str)
	{
		RegexMatcher matcher(dfaTable.operator->());
		return (matcher.Match(str, 0)==str.Length());
	}

	PureRegex::RegexMatchResult PureRegex::Search(const String & str, int startPos)
	{
		RegexMatcher matcher(dfaTable.operator ->());
		for (int i=startPos; i<str.Length(); i++)
		{
			int len = matcher.Match(str, i);
			if (len >= 0)
			{
				RegexMatchResult rs;
				rs.Start = i;
				rs.Length = len;
				return rs;
			}
		}
		RegexMatchResult rs;
		rs.Start = 0;
		rs.Length = -1;
		return rs;
	}
}
}

/***********************************************************************
REGEX\REGEXDFA.CPP
***********************************************************************/

namespace CoreLib
{
namespace Text
{
	CharTableGenerator::CharTableGenerator(RegexCharTable * _table)
		: table(_table)
	{
		table->SetSize(65536);
		memset(table->Buffer(),0,sizeof(Word)*table->Count());
	}

	DFA_Table_Tag::DFA_Table_Tag()
	{
		IsFinal = false;
	}

	int CharTableGenerator::AddSet(String set)
	{
		int fid = sets.IndexOf(set);
		if (fid != -1)
			return fid;
		else
		{
			sets.Add(set);
			return sets.Count()-1;
		}
	}

	int CharTableGenerator::Generate(List<RegexCharSet *> & charSets)
	{
		/*List<RegexCharSet *> cs;
		cs.SetCapacity(charSets.Count());
		String str;
		str.Alloc(1024);
		for (int i=1; i<65536; i++)
		{
			str = L"";
			cs.Clear();
			for (int j=0; j<charSets.Count(); j++)
			{
				if (charSets[j]->Contains(i))
				{
					str += (wchar_t)(j+1);
					cs.Add(charSets[j]);
				}
			}
			int lastCount = sets.Count();
			if (str.Length())
			{
				int id = AddSet(str);
				if (id == lastCount)
				{
					for (int j=0; j<cs.Count(); j++)
						cs[j]->Elements.Add(id);
				}
				(*table)[i] = id;
			}
			else
				(*table)[i] = 0xFFFF;
		}
		return sets.Count();*/
		
		RegexCharSet::CalcCharElements(charSets, elements);
		for (int i=0; i<table->Count(); i++)
			(*table)[i] = 0xFFFF;
		Word* buf = table->Buffer();
		for (int i=0; i<elements.Count(); i++)
		{
			for (int k=elements[i].Begin; k<=elements[i].End; k++)	
			{
#ifdef _DEBUG
				if ((*table)[k] != 0xFFFF)
				{
					throw L"Illegal subset generation."; // This indicates a bug.
				}
#endif
				buf[k] = (Word)i;
			}
		}
		return elements.Count();
	}

	DFA_Node::DFA_Node(int elements)
	{
		Translations.SetSize(elements);
		for (int i=0; i<elements; i++)
			Translations[i] = 0;
	}

	void DFA_Graph::CombineCharElements(NFA_Node * node, List<Word> & elem)
	{
		for (int i=0; i<node->Translations.Count(); i++)
		{
			for (int j=0; j<node->Translations[i]->CharSet->Elements.Count(); j++)
			{
				if (elem.IndexOf(node->Translations[i]->CharSet->Elements[j]) == -1)
					elem.Add(node->Translations[i]->CharSet->Elements[j]);
			}
		}
	}

	void DFA_Graph::Generate(NFA_Graph * nfa)
	{
		table = new RegexCharTable();
		List<RegexCharSet * > charSets;
		for (int i=0; i<nfa->translations.Count(); i++)
		{
			if (nfa->translations[i]->CharSet && nfa->translations[i]->CharSet->Ranges.Count())
				charSets.Add(nfa->translations[i]->CharSet.operator->());
		}
		CharTableGenerator gen(table.operator ->());
		int elements = gen.Generate(charSets);
		CharElements = gen.elements;
		List<DFA_Node *> L,D;
		startNode = new DFA_Node(elements);
		startNode->ID = 0;
		startNode->Nodes.Add(nfa->start);
		L.Add(startNode);
		nodes.Add(startNode);
		List<Word> charElem;
		do
		{
			DFA_Node * node = L.Last();
			L.RemoveAt(L.Count()-1);
			charElem.Clear();
			node->IsFinal = false;
			for (int i=0; i<node->Nodes.Count(); i++)
			{
				CombineCharElements(node->Nodes[i], charElem);
				if (node->Nodes[i]->IsFinal)
					node->IsFinal = true;
			}
			for (int i=0; i<charElem.Count(); i++)
			{
				DFA_Node * n = new DFA_Node(0);
				for (int j=0; j<node->Nodes.Count(); j++)
				{
					for (int k=0; k<node->Nodes[j]->Translations.Count(); k++)
					{
						NFA_Translation * trans = node->Nodes[j]->Translations[k];
						if (trans->CharSet->Elements.Contains(charElem[i]))
						{
							if (!n->Nodes.Contains(node->Nodes[j]->Translations[k]->NodeDest))
								n->Nodes.Add(node->Nodes[j]->Translations[k]->NodeDest);
						}
					}
				}
				int fid = -1;
				for (int j=0; j<nodes.Count(); j++)
				{
					if ((*nodes[j]) == *n)
					{
						fid = j;
						break;
					}
				}
				if (fid == -1)
				{
					n->Translations.SetSize(elements);
					for (int m=0; m<elements; m++)
						n->Translations[m] = 0;
					n->ID = nodes.Count();
					L.Add(n);
					nodes.Add(n);
					fid = nodes.Count()-1;
				}
				else
					delete n;
				n = nodes[fid].operator ->();
				node->Translations[charElem[i]] = n;
			}
		}
		while (L.Count());

		// Set Terminal Identifiers
		HashSet<int> terminalIdentifiers;
		for (int i=0; i<nodes.Count(); i++)
		{
			terminalIdentifiers.Clear();
			for (int j=0; j<nodes[i]->Nodes.Count(); j++)
			{
				if (nodes[i]->Nodes[j]->IsFinal && 
					!terminalIdentifiers.Contains(nodes[i]->Nodes[j]->TerminalIdentifier))
				{
					nodes[i]->IsFinal = true;
					terminalIdentifiers.Add(nodes[i]->Nodes[j]->TerminalIdentifier);
					nodes[i]->TerminalIdentifiers.Add(nodes[i]->Nodes[j]->TerminalIdentifier);
				}
			}
			nodes[i]->TerminalIdentifiers.Sort();
		}
	}

	bool DFA_Node::operator == (const DFA_Node & node)
	{
		if (Nodes.Count() != node.Nodes.Count())
			return false;
		for (int i=0; i<node.Nodes.Count(); i++)
		{
			if (node.Nodes[i] != Nodes[i])
				return false;
		}
		return true;
	}

	String DFA_Graph::Interpret()
	{
		StringBuilder sb(4096000);
		for (int i=0; i<nodes.Count(); i++)
		{
			if (nodes[i]->IsFinal)
				sb.Append(L'#');
			else if (nodes[i] == startNode)
				sb.Append(L'*');
			sb.Append(String(nodes[i]->ID));
			sb.Append(L'(');
			for (int j=0; j<nodes[i]->Nodes.Count(); j++)
			{
				sb.Append(String(nodes[i]->Nodes[j]->ID));
				sb.Append(L" ");
			}
			sb.Append(L")\n");
			for (int j=0; j<nodes[i]->Translations.Count(); j++)
			{
				if (nodes[i]->Translations[j])
				{
					sb.Append(L"\tOn ");
					sb.Append(String(j));
					sb.Append(L": ");
					sb.Append(String(nodes[i]->Translations[j]->ID));
					sb.Append(L'\n');
				}
			}
		}

		sb.Append(L"\n\n==================\n");
		sb.Append(L"Char Set Table:\n");
		for (int i=0; i<CharElements.Count(); i++)
		{
			sb.Append(L"Class ");
			sb.Append(String(i));
			sb.Append(L": ");
			RegexCharSet s;
			s.Ranges.Add(CharElements[i]);
			sb.Append(s.Reinterpret());
			sb.Append(L"\n");
		}
		return sb.ProduceString();
	}

	void DFA_Graph::ToDfaTable(DFA_Table * dfa)
	{
		dfa->CharTable = table;
		dfa->DFA = new int*[nodes.Count()];
		dfa->Tags.SetSize(nodes.Count());
		for (int i=0; i<nodes.Count(); i++)
			dfa->Tags[i] = new DFA_Table_Tag();
		dfa->StateCount = nodes.Count();
		dfa->AlphabetSize = CharElements.Count();
		for (int i=0; i<nodes.Count(); i++)
		{
			dfa->DFA[i] = new int[table->Count()];
			for (int j=0; j<nodes[i]->Translations.Count(); j++)
			{
				if (nodes[i]->Translations[j])
					dfa->DFA[i][j] = nodes[i]->Translations[j]->ID;
				else
					dfa->DFA[i][j] = -1;
			}
			if (nodes[i] == startNode)
				dfa->StartState = i;
			if (nodes[i]->IsFinal)
			{
				dfa->Tags[i]->IsFinal = true;
				dfa->Tags[i]->TerminalIdentifiers = nodes[i]->TerminalIdentifiers;
			}
		}
	}

	DFA_Table::DFA_Table()
	{
		DFA = 0;
		StateCount = 0;
		AlphabetSize = 0;
		StartState = -1;
	}
	
	DFA_Table::~DFA_Table()
	{
		if (DFA)
		{
			for (int i=0; i<StateCount; i++)
				delete [] DFA[i];
			delete [] DFA;
		}
	}
}
}

/***********************************************************************
REGEX\REGEXNFA.CPP
***********************************************************************/

namespace CoreLib
{
namespace Text
{
	int NFA_Node::HandleCount = 0;

	NFA_Translation::NFA_Translation(NFA_Node * src, NFA_Node * dest, RefPtr<RegexCharSet> charSet)
		: NodeSrc(src), NodeDest(dest), CharSet(charSet)
	{}

	NFA_Translation::NFA_Translation()
	{
		NodeSrc = NodeDest = 0;
	}

	NFA_Translation::NFA_Translation(NFA_Node * src, NFA_Node * dest)
		: NodeSrc(src), NodeDest(dest)
	{
	}

	NFA_Node::NFA_Node()
		: Flag(false), IsFinal(false), TerminalIdentifier(0)
	{
		HandleCount ++;
		ID = HandleCount;
	}

	void NFA_Node::RemoveTranslation(NFA_Translation * trans)
	{
		int fid = Translations.IndexOf(trans);
		if (fid != -1)
			Translations.RemoveAt(fid);
	}

	void NFA_Node::RemovePrevTranslation(NFA_Translation * trans)
	{
		int fid = PrevTranslations.IndexOf(trans);
		if (fid != -1)
			PrevTranslations.RemoveAt(fid);
	}

	NFA_Node * NFA_Graph::CreateNode()
	{
		NFA_Node * nNode = new NFA_Node();
		nodes.Add(nNode);
		return nNode;
	}

	NFA_Translation * NFA_Graph::CreateTranslation()
	{
		NFA_Translation * trans = new NFA_Translation();
		translations.Add(trans);
		return trans;
	}

	void NFA_Graph::ClearNodes()
	{
		for (int i=0; i<nodes.Count(); i++)
			nodes[i] = 0;
		for (int i=0; i<translations.Count(); i++)
			translations[i] = 0;
		nodes.Clear();
		translations.Clear();
	}

	void NFA_Graph::GenerateFromRegexTree(RegexNode * tree, bool elimEpsilon)
	{
		NFA_StatePair s;
		tree->Accept(this);
		s = PopState();
		start = s.start;
		end = s.end;
		end->IsFinal = true;

		if (elimEpsilon)
		{
			PostGenerationProcess();
		}

	}

	void NFA_Graph::PostGenerationProcess()
	{
		EliminateEpsilon();
		for (int i=0; i<translations.Count(); i++)
		{
			if (translations[i]->CharSet)
				translations[i]->CharSet->Normalize();
			else
			{
				translations[i] = 0;
				translations.RemoveAt(i);
				i--;
			}
		}
	}

	NFA_Node * NFA_Graph::GetStartNode()
	{
		return start;
	}

	void NFA_Graph::PushState(NFA_StatePair s)
	{
		stateStack.Add(s);
	}

	NFA_Graph::NFA_StatePair NFA_Graph::PopState()
	{
		NFA_StatePair s = stateStack.Last();
		stateStack.RemoveAt(stateStack.Count()-1);
		return s;
	}

	void NFA_Graph::VisitCharSetNode(RegexCharSetNode * node)
	{
		NFA_StatePair s;
		s.start = CreateNode();
		s.end = CreateNode();
		NFA_Translation * trans = CreateTranslation();
		trans->CharSet = node->CharSet;
		trans->NodeSrc = s.start;
		trans->NodeDest = s.end;
		s.start->Translations.Add(trans);
		s.end->PrevTranslations.Add(trans);
		PushState(s);
	}

	void NFA_Graph::VisitRepeatNode(RegexRepeatNode * node)
	{
		NFA_StatePair sr;
		sr.start = sr.end = nullptr;
		node->Child->Accept(this);
		NFA_StatePair s = PopState();
		if (node->RepeatType == RegexRepeatNode::rtArbitary)
		{
			sr.start = CreateNode();
			sr.end = CreateNode();

			NFA_Translation * trans = CreateTranslation();
			trans->NodeSrc = sr.start;
			trans->NodeDest = sr.end;
			sr.start->Translations.Add(trans);
			sr.end->PrevTranslations.Add(trans);
			
			NFA_Translation * trans1 = CreateTranslation();
			trans1->NodeSrc = sr.end;
			trans1->NodeDest = s.start;
			sr.end->Translations.Add(trans1);
			s.start->PrevTranslations.Add(trans1);

			NFA_Translation * trans2 = CreateTranslation();
			trans2->NodeSrc = s.end;
			trans2->NodeDest = sr.end;
			s.end->Translations.Add(trans2);
			sr.end->PrevTranslations.Add(trans2);
		}
		else if (node->RepeatType == RegexRepeatNode::rtOptional)
		{
			sr = s;

			NFA_Translation * trans = CreateTranslation();
			trans->NodeSrc = sr.start;
			trans->NodeDest = sr.end;
			sr.start->Translations.Add(trans);
			sr.end->PrevTranslations.Add(trans);
		}
		else if (node->RepeatType == RegexRepeatNode::rtMoreThanOnce)
		{
			sr = s;

			NFA_Translation * trans = CreateTranslation();
			trans->NodeSrc = sr.end;
			trans->NodeDest = sr.start;
			sr.start->PrevTranslations.Add(trans);
			sr.end->Translations.Add(trans);
		}
		else if (node->RepeatType == RegexRepeatNode::rtSpecified)
		{
			if (node->MinRepeat == 0)
			{
				if (node->MaxRepeat > 0)
				{
					for (int i=1; i<node->MaxRepeat; i++)
					{
						node->Child->Accept(this);
						NFA_StatePair s1 = PopState();
						NFA_Translation * trans = CreateTranslation();
						trans->NodeDest = s1.start;
						trans->NodeSrc = s.end;
						trans->NodeDest->PrevTranslations.Add(trans);
						trans->NodeSrc->Translations.Add(trans);

						trans = CreateTranslation();
						trans->NodeDest = s1.start;
						trans->NodeSrc = s.start;
						trans->NodeDest->PrevTranslations.Add(trans);
						trans->NodeSrc->Translations.Add(trans);

						s.end = s1.end;
					}
					NFA_Translation * trans = CreateTranslation();
					trans->NodeDest = s.end;
					trans->NodeSrc = s.start;
					trans->NodeDest->PrevTranslations.Add(trans);
					trans->NodeSrc->Translations.Add(trans);
					sr = s;
				}
				else if (node->MaxRepeat == 0)
				{
					sr.start = CreateNode();
					sr.end = CreateNode();
					NFA_Translation * trans = CreateTranslation();
					trans->NodeDest = sr.end;
					trans->NodeSrc = sr.start;
					trans->NodeDest->PrevTranslations.Add(trans);
					trans->NodeSrc->Translations.Add(trans);
				}
				else
				{
					// Arbitary repeat
					sr.start = CreateNode();
					sr.end = CreateNode();

					NFA_Translation * trans = CreateTranslation();
					trans->NodeSrc = sr.start;
					trans->NodeDest = sr.end;
					sr.start->Translations.Add(trans);
					sr.end->PrevTranslations.Add(trans);
					
					NFA_Translation * trans1 = CreateTranslation();
					trans1->NodeSrc = sr.end;
					trans1->NodeDest = s.start;
					sr.end->Translations.Add(trans1);
					s.start->PrevTranslations.Add(trans1);

					NFA_Translation * trans2 = CreateTranslation();
					trans2->NodeSrc = s.end;
					trans2->NodeDest = sr.end;
					s.end->Translations.Add(trans2);
					sr.end->PrevTranslations.Add(trans2);
				}
			}
			else
			{
				NFA_Node * lastBegin = s.start;
				for (int i=1; i<node->MinRepeat; i++)
				{
					node->Child->Accept(this);
					NFA_StatePair s1 = PopState();
					NFA_Translation * trans = CreateTranslation();
					trans->NodeDest = s1.start;
					trans->NodeSrc = s.end;
					trans->NodeDest->PrevTranslations.Add(trans);
					trans->NodeSrc->Translations.Add(trans);
					s.end = s1.end;
					lastBegin = s1.start;
				}
				if (node->MaxRepeat == -1)
				{
					NFA_Translation * trans = CreateTranslation();
					trans->NodeDest = lastBegin;
					trans->NodeSrc = s.end;
					trans->NodeDest->PrevTranslations.Add(trans);
					trans->NodeSrc->Translations.Add(trans);
				}
				else if (node->MaxRepeat > node->MinRepeat)
				{
					lastBegin = s.end;
					for (int i=node->MinRepeat; i<node->MaxRepeat; i++)
					{
						node->Child->Accept(this);
						NFA_StatePair s1 = PopState();
						NFA_Translation * trans = CreateTranslation();
						trans->NodeDest = s1.start;
						trans->NodeSrc = s.end;
						trans->NodeDest->PrevTranslations.Add(trans);
						trans->NodeSrc->Translations.Add(trans);

						trans = CreateTranslation();
						trans->NodeDest = s1.start;
						trans->NodeSrc = lastBegin;
						trans->NodeDest->PrevTranslations.Add(trans);
						trans->NodeSrc->Translations.Add(trans);

						s.end = s1.end;
					}

					NFA_Translation * trans = CreateTranslation();
					trans->NodeDest = s.end;
					trans->NodeSrc = lastBegin;
					trans->NodeDest->PrevTranslations.Add(trans);
					trans->NodeSrc->Translations.Add(trans);
				}

				sr = s;
			}
			
		}
		PushState(sr);
	}

	void NFA_Graph::VisitSelectionNode(RegexSelectionNode * node)
	{
		NFA_StatePair s, s1, sr;
		sr.start = CreateNode();
		sr.end = CreateNode();
		s.start = sr.start;
		s.end = sr.end;
		s1.start = sr.start;
		s1.end = sr.end;
		if (node->LeftChild)
		{
			node->LeftChild->Accept(this);
			s = PopState();
		}
		if (node->RightChild)
		{
			node->RightChild->Accept(this);
			s1 = PopState();
		}
		
		NFA_Translation * trans;
		trans = CreateTranslation();
		trans->NodeSrc = sr.start;
		trans->NodeDest = s.start;
		sr.start->Translations.Add(trans);
		s.start->PrevTranslations.Add(trans);

		trans = CreateTranslation();
		trans->NodeSrc = sr.start;
		trans->NodeDest = s1.start;
		sr.start->Translations.Add(trans);
		s1.start->PrevTranslations.Add(trans);

		trans = CreateTranslation();
		trans->NodeSrc = s.end;
		trans->NodeDest = sr.end;
		s.end->Translations.Add(trans);
		sr.end->PrevTranslations.Add(trans);

		trans = CreateTranslation();
		trans->NodeSrc = s1.end;
		trans->NodeDest = sr.end;
		s1.end->Translations.Add(trans);
		sr.end->PrevTranslations.Add(trans);

		PushState(sr);
	}

	void NFA_Graph::VisitConnectionNode(RegexConnectionNode * node)
	{
		NFA_StatePair s, s1;
		node->LeftChild->Accept(this);
		s = PopState();
		node->RightChild->Accept(this);
		s1 = PopState();
		NFA_Translation * trans = CreateTranslation();
		trans->NodeDest = s1.start;
		trans->NodeSrc = s.end;
		s.end->Translations.Add(trans);
		s1.start->PrevTranslations.Add(trans);
		s.end = s1.end;
		PushState(s);
		
	}

	void NFA_Graph::ClearNodeFlags()
	{
		for (int i=0; i<nodes.Count(); i++)
			nodes[i]->Flag = false;
	}

	void NFA_Graph::GetValidStates(List<NFA_Node *> & states)
	{
		RefPtr<List<NFA_Node *>> list1 = new List<NFA_Node *>();
		RefPtr<List<NFA_Node *>> list2 = new List<NFA_Node *>();
		list1->Add(start);
		states.Add(start);
		ClearNodeFlags();
		while (list1->Count())
		{
			list2->Clear();
			for (int i=0; i<list1->Count(); i++)
			{
				bool isValid = false;
				NFA_Node * curNode = (*list1)[i];
				curNode->Flag = true;
				for (int j=0; j<curNode->PrevTranslations.Count(); j++)
				{
					if (curNode->PrevTranslations[j]->CharSet)
					{
						isValid = true;
						break;
					}
					
				}
				if (isValid)
					states.Add(curNode);
				for (int j=0; j<curNode->Translations.Count(); j++)
				{
					if (!curNode->Translations[j]->NodeDest->Flag)
					{
						list2->Add(curNode->Translations[j]->NodeDest);
					}
				}
			}
			RefPtr<List<NFA_Node *>> tmp = list1;
			list1 = list2;
			list2 = tmp;
		}
	}

	void NFA_Graph::GetEpsilonClosure(NFA_Node * node, List<NFA_Node *> & states)
	{
		RefPtr<List<NFA_Node *>> list1 = new List<NFA_Node *>();
		RefPtr<List<NFA_Node *>> list2 = new List<NFA_Node *>();
		list1->Add(node);
		ClearNodeFlags();
		while (list1->Count())
		{
			list2->Clear();
			for (int m=0; m<list1->Count(); m++)
			{
				NFA_Node * curNode = (*list1)[m];
				for (int i=0; i<curNode->Translations.Count(); i++)
				{
					
					if (!curNode->Translations[i]->CharSet)
					{
						if (!curNode->Translations[i]->NodeDest->Flag)
						{
							states.Add(curNode->Translations[i]->NodeDest);
							list2->Add(curNode->Translations[i]->NodeDest);
							curNode->Translations[i]->NodeDest->Flag = true;
						}
					}
				}
			}
			RefPtr<List<NFA_Node *>> tmp = list1;
			list1 = list2;
			list2 = tmp;
		}
	}

	void NFA_Graph::EliminateEpsilon()
	{
		List<NFA_Node *> validStates;
		GetValidStates(validStates);
		for (int i=0; i<validStates.Count(); i++)
		{
			NFA_Node * curState = validStates[i];
			List<NFA_Node *> closure;
			GetEpsilonClosure(curState, closure);
			// Add translations from epsilon closures
			for (int j=0; j<closure.Count(); j++)
			{
				NFA_Node * curNode = closure[j];
				for (int k=0; k<curNode->Translations.Count(); k++)
				{
					if (curNode->Translations[k]->CharSet)
					{
						// Generate a translation from curState to curNode->Dest[k]
						NFA_Translation * trans = CreateTranslation();
						trans->CharSet = curNode->Translations[k]->CharSet;
						trans->NodeSrc = curState;
						trans->NodeDest = curNode->Translations[k]->NodeDest;
						curState->Translations.Add(trans);
						trans->NodeDest->PrevTranslations.Add(trans);
					}
				}
				if (curNode == end)
				{
					curState->IsFinal = true;
					curState->TerminalIdentifier = end->TerminalIdentifier;
				}
			}
		}
		// Remove epsilon-translations and invalid states
		ClearNodeFlags();
		for (int i=0; i<validStates.Count(); i++)
		{
			validStates[i]->Flag = true;
		}
		for (int i=0; i<nodes.Count(); i++)
		{
			if (!nodes[i]->Flag)
			{
				// Remove invalid state
				for (int j=0; j<nodes[i]->PrevTranslations.Count(); j++)
				{
					NFA_Translation * trans = nodes[i]->PrevTranslations[j];
					trans->NodeSrc->RemoveTranslation(trans);
					int fid = translations.IndexOf(trans);
					if (fid != -1)
					{
						translations[fid] = 0;
						translations.RemoveAt(fid);
					}
				}
				for (int j=0; j<nodes[i]->Translations.Count(); j++)
				{
					NFA_Translation * trans = nodes[i]->Translations[j];
					trans->NodeDest->RemovePrevTranslation(trans);
					int fid = translations.IndexOf(trans);
					if (fid != -1)
					{
						translations[fid] = 0;
						translations.RemoveAt(fid);
					}
				}
			}
		}

		for (int i=0; i<validStates.Count(); i++)
		{
			for (int j=0; j<validStates[i]->Translations.Count(); j++)
			{
				NFA_Translation * trans = validStates[i]->Translations[j];
				if (!trans->CharSet)
				{
					validStates[i]->RemoveTranslation(trans);
					trans->NodeDest->RemovePrevTranslation(trans);
					int fid = translations.IndexOf(trans);
					if (fid != -1)
					{
						translations[fid] = 0;
						translations.RemoveAt(fid);
					}
				}
			}
		}

		int ptr = 0;
		while (ptr < nodes.Count())
		{
			if (!nodes[ptr]->Flag)
			{
				nodes[ptr] = 0;
				nodes.RemoveAt(ptr);
			}
			else
				ptr ++;
		}
	}

	String NFA_Graph::Interpret()
	{
		StringBuilder sb(4096);
		for (int i=0; i<nodes.Count(); i++)
		{
			sb.Append(L"State: ");
			if (nodes[i]->IsFinal)
				sb.Append(L"[");
			if (nodes[i] == start)
				sb.Append(L"*");
			sb.Append(String(nodes[i]->ID));
			if (nodes[i]->IsFinal)
				sb.Append(L"]");
			sb.Append(L'\n');
			for (int j=0; j<nodes[i]->Translations.Count(); j++)
			{
				sb.Append(L"\t");
				if (nodes[i]->Translations[j]->CharSet)
					sb.Append(nodes[i]->Translations[j]->CharSet->Reinterpret());
				else
					sb.Append(L"<epsilon>");
				sb.Append(L":");
				sb.Append(String(nodes[i]->Translations[j]->NodeDest->ID));
				sb.Append(L"\n");
			}
		}
		return sb.ProduceString();
		
	}

	void NFA_Graph::SetStartNode(NFA_Node *node)
	{
		start = node;
	}

	void NFA_Graph::CombineNFA(NFA_Graph * graph)
	{
		for (int i=0; i<graph->nodes.Count(); i++)
		{
			nodes.Add(graph->nodes[i]);
		}
		for (int i=0; i<graph->translations.Count(); i++)
		{
			translations.Add(graph->translations[i]);
		}

	}

	void NFA_Graph::SetTerminalIdentifier(int id)
	{
		for (int i=0; i<nodes.Count(); i++)
		{
			if (nodes[i]->IsFinal)
			{
				nodes[i]->TerminalIdentifier = id;
			}
		}
	}
}
}

/***********************************************************************
REGEX\REGEXPARSER.CPP
***********************************************************************/

namespace CoreLib
{
namespace Text
{
	RegexCharSetNode::RegexCharSetNode()
	{
		CharSet = new RegexCharSet();
	}

	RefPtr<RegexNode> RegexParser::Parse(const String &regex)
	{
		src = regex;
		ptr = 0;
		try
		{
			return ParseSelectionNode();
		}
		catch (...)
		{
			return 0;
		}
	}

	RegexNode * RegexParser::ParseSelectionNode()
	{
		if (ptr >= src.Length())
			return 0;
		RefPtr<RegexNode> left = ParseConnectionNode();
		while (ptr < src.Length() && src[ptr] == L'|')
		{
			ptr ++;
			RefPtr<RegexNode> right = ParseConnectionNode();
			RegexSelectionNode * rs = new RegexSelectionNode();
			rs->LeftChild = left;
			rs->RightChild = right;
			left = rs;
		}
		return left.Release();
	}

	RegexNode * RegexParser::ParseConnectionNode()
	{
		if (ptr >= src.Length())
		{
			return 0;
		}
		RefPtr<RegexNode> left = ParseRepeatNode();
		while (ptr < src.Length() && src[ptr] != L'|' && src[ptr] != L')')
		{
			RefPtr<RegexNode> right = ParseRepeatNode();
			if (right)
			{
				RegexConnectionNode * reg = new RegexConnectionNode();
				reg->LeftChild = left;
				reg->RightChild = right;
				left = reg;
			}
			else
				break;
		}
		return left.Release();
	}

	RegexNode * RegexParser::ParseRepeatNode()
	{
		if (ptr >= src.Length() || src[ptr] == L')' || src[ptr] == L'|')
			return 0;
		
		RefPtr<RegexNode> content;
		if (src[ptr] == L'(')
		{
			RefPtr<RegexNode> reg;
			ptr ++;
			reg = ParseSelectionNode();
			if (src[ptr] != L')')
			{
				SyntaxError err;
				err.Position = ptr;
				err.Text = L"\')\' expected.";
				Errors.Add(err);
				throw 0;
			}
			ptr ++;
			content = reg.Release();
		}
		else
		{
			RefPtr<RegexCharSetNode> reg;
			if (src[ptr] == L'[')
			{
				reg = new RegexCharSetNode();
				ptr ++;
				reg->CharSet = ParseCharSet();
				
				if (src[ptr] != L']')
				{
					SyntaxError err;
					err.Position = ptr;
					err.Text = L"\']\' expected.";
					Errors.Add(err);
					throw 0;
				}
				ptr ++;
			}
			else if (src[ptr] == L'\\')
			{
				ptr ++;
				reg = new RegexCharSetNode();
				reg->CharSet = new RegexCharSet();
				switch (src[ptr])
				{
				case L'.':
					{
						reg->CharSet->Neg = true;
						break;
					}
				case L'w':
				case L'W':
					{
						RegexCharSet::RegexCharRange range;
						reg->CharSet->Neg = false;
						range.Begin = L'a';
						range.End = L'z';
						reg->CharSet->Ranges.Add(range);
						range.Begin = L'A';
						range.End = L'Z';
						reg->CharSet->Ranges.Add(range);
						range.Begin = L'_';
						range.End = L'_';
						reg->CharSet->Ranges.Add(range);
						range.Begin = L'0';
						range.End = L'9';
						reg->CharSet->Ranges.Add(range);
						if (src[ptr] == L'W')
							reg->CharSet->Neg = true;
						break;
					}
				case L's':
				case 'S':
					{
						RegexCharSet::RegexCharRange range;
						reg->CharSet->Neg = false;
						range.Begin = L' ';
						range.End = L' ';
						reg->CharSet->Ranges.Add(range);
						range.Begin = L'\t';
						range.End = L'\t';
						reg->CharSet->Ranges.Add(range);
						range.Begin = L'\r';
						range.End = L'\r';
						reg->CharSet->Ranges.Add(range);
						range.Begin = L'\n';
						range.End = L'\n';
						reg->CharSet->Ranges.Add(range);
						if (src[ptr] == L'S')
							reg->CharSet->Neg = true;
						break;
					}
				case L'd':
				case L'D':
					{
						RegexCharSet::RegexCharRange range;
						reg->CharSet->Neg = false;
						range.Begin = L'0';
						range.End = L'9';
						reg->CharSet->Ranges.Add(range);
						if (src[ptr] == L'D')
							reg->CharSet->Neg = true;
						break;
					}
				case L'n':
					{
						RegexCharSet::RegexCharRange range;
						reg->CharSet->Neg = false;
						range.Begin = L'\n';
						range.End = L'\n';
						reg->CharSet->Ranges.Add(range);
						break;
					}
				case L't':
					{
						RegexCharSet::RegexCharRange range;
						reg->CharSet->Neg = false;
						range.Begin = L'\t';
						range.End = L'\t';
						reg->CharSet->Ranges.Add(range);
						break;
					}
				case L'r':
					{
						RegexCharSet::RegexCharRange range;
						reg->CharSet->Neg = false;
						range.Begin = L'\r';
						range.End = L'\r';
						reg->CharSet->Ranges.Add(range);
						break;
					}
				case L'v':
					{
						RegexCharSet::RegexCharRange range;
						reg->CharSet->Neg = false;
						range.Begin = L'\v';
						range.End = L'\v';
						reg->CharSet->Ranges.Add(range);
						break;
					}
				case L'f':
					{
						RegexCharSet::RegexCharRange range;
						reg->CharSet->Neg = false;
						range.Begin = L'\f';
						range.End = L'\f';
						reg->CharSet->Ranges.Add(range);
						break;
					}
				case L'*':
				case L'|':
				case L'(':
				case L')':
				case L'?':
				case L'+':
				case L'[':
				case L']':
				case L'\\':
					{
						RegexCharSet::RegexCharRange range;
						reg->CharSet->Neg = false;
						range.Begin = src[ptr];
						range.End = range.Begin;
						reg->CharSet->Ranges.Add(range);
						break;
					}
				default:
					{
						SyntaxError err;
						err.Position = ptr;
						err.Text = String(L"Illegal escape sequence \'\\") + src[ptr] + L"\'";
						Errors.Add(err);
						throw 0;
					}
				}
				ptr ++;
			}
			else if (!IsOperator())
			{
				RegexCharSet::RegexCharRange range;
				reg = new RegexCharSetNode();
				reg->CharSet->Neg = false;
				range.Begin = src[ptr];
				range.End = range.Begin;
				ptr ++;
				reg->CharSet->Ranges.Add(range);
			}
			else
			{
				SyntaxError err;
				err.Position = ptr;
				err.Text = String(L"Unexpected \'") + src[ptr] + L'\'';
				Errors.Add(err);
				throw 0;
			}
			content = reg.Release();
		}
		if (ptr < src.Length())
		{
			if (src[ptr] == L'*')
			{
				RefPtr<RegexRepeatNode> node = new RegexRepeatNode();
				node->Child = content;
				node->RepeatType = RegexRepeatNode::rtArbitary;
				ptr ++;
				return node.Release();
			}
			else if (src[ptr] == L'?')
			{
				RefPtr<RegexRepeatNode> node = new RegexRepeatNode();
				node->Child = content;
				node->RepeatType = RegexRepeatNode::rtOptional;
				ptr ++;
				return node.Release();
			}
			else if (src[ptr] == L'+')
			{
				RefPtr<RegexRepeatNode> node = new RegexRepeatNode();
				node->Child = content;
				node->RepeatType = RegexRepeatNode::rtMoreThanOnce;
				ptr ++;
				return node.Release();
			}
			else if (src[ptr] == L'{')
			{
				ptr++;
				RefPtr<RegexRepeatNode> node = new RegexRepeatNode();
				node->Child = content;
				node->RepeatType = RegexRepeatNode::rtSpecified;
				node->MinRepeat = ParseInteger();
				if (src[ptr] == L',')
				{
					ptr ++;
					node->MaxRepeat = ParseInteger();
				}
				else
					node->MaxRepeat = node->MinRepeat;
				if (src[ptr] == L'}')
					ptr++;
				else
				{
					SyntaxError err;
					err.Position = ptr;
					err.Text = L"\'}\' expected.";
					Errors.Add(err);
					throw 0;
				}
				if (node->MinRepeat < 0)
				{
					SyntaxError err;
					err.Position = ptr;
					err.Text = L"Minimun repeat cannot be less than 0.";
					Errors.Add(err);
					throw 0;
				}
				if (node->MaxRepeat != -1 && node->MaxRepeat < node->MinRepeat)
				{
					SyntaxError err;
					err.Position = ptr;
					err.Text = L"Max repeat cannot be less than min repeat.";
					Errors.Add(err);
					throw 0;
				}
				return node.Release();
			}
		}
		return content.Release();
	}

	bool IsDigit(wchar_t ch)
	{
		return ch>=L'0'&& ch <=L'9';
	}

	int RegexParser::ParseInteger()
	{
		StringBuilder number;
		while (IsDigit(src[ptr]))
		{
			number.Append(src[ptr]);
			ptr ++;
		}
		if (number.Length() == 0)
			return -1;
		else
			return StringToInt(number.ProduceString());
	}

	bool RegexParser::IsOperator()
	{
		return (src[ptr] == L'|' || src[ptr] == L'*' || src[ptr] == L'(' || src[ptr] == L')'
				|| src[ptr] == L'?' || src[ptr] == L'+');
	}

	wchar_t RegexParser::ReadNextCharInCharSet()
	{
		if (ptr < src.Length() && src[ptr] != L']')
		{
			if (src[ptr] == L'\\')
			{
				ptr ++;
				if (ptr >= src.Length())
				{
					SyntaxError err;
					err.Position = ptr;
					err.Text = String(L"Unexpected end of char-set when looking for escape sequence.");
					Errors.Add(err);
					throw 0;
				}
				wchar_t rs = 0;
				if (src[ptr] == L'\\')
					rs = L'\\';
				else if (src[ptr] == L'^')
					rs = L'^';
				else if (src[ptr] == L'-')
					rs = L'-';
				else if (src[ptr] == L']')
					rs = L']';
				else if (src[ptr] == L'n')
					rs = L'\n';
				else if (src[ptr] == L't')
					rs = L'\t';
				else if (src[ptr] == L'r')
					rs = L'\r';
				else if (src[ptr] == L'v')
					rs = L'\v';
				else if (src[ptr] == L'f')
					rs = L'\f';
				else
				{
					SyntaxError err;
					err.Position = ptr;
					err.Text = String(L"Illegal escape sequence inside charset definition \'\\") + src[ptr] + L"\'";
					Errors.Add(err);
					throw 0;
				}
				ptr ++;
				return rs;
			}
			else
				return src[ptr++];
		}
		else
		{
			SyntaxError err;
			err.Position = ptr;
			err.Text = String(L"Unexpected end of char-set.");
			Errors.Add(err);
			throw 0;
		}
	}

	RegexCharSet * RegexParser::ParseCharSet()
	{
		RefPtr<RegexCharSet> rs = new RegexCharSet();
		if (src[ptr] == L'^')
		{
			rs->Neg = true;
			ptr ++;
		}
		else
			rs->Neg = false;
		RegexCharSet::RegexCharRange range;
		while (ptr < src.Length() && src[ptr] != L']')
		{
			range.Begin = ReadNextCharInCharSet();
			//ptr ++;
			
			if (ptr >= src.Length())
			{
				break;
			}
			if (src[ptr] == L'-')
			{
				ptr ++;
				range.End = ReadNextCharInCharSet();	
			}
			else
			{
				range.End = range.Begin;
			}
			rs->Ranges.Add(range);
		
		}
		if (ptr >=src.Length() || src[ptr] != L']')
		{
			SyntaxError err;
			err.Position = ptr;
			err.Text = String(L"Unexpected end of char-set.");
			Errors.Add(err);
			throw 0;
		}
		return rs.Release();
	}
}
}

/***********************************************************************
REGEX\REGEXTREE.CPP
***********************************************************************/

namespace CoreLib
{
namespace Text
{
	void RegexNodeVisitor::VisitCharSetNode(RegexCharSetNode * )
	{
	}

	void RegexNodeVisitor::VisitRepeatNode(RegexRepeatNode * )
	{

	}

	void RegexNodeVisitor::VisitSelectionNode(RegexSelectionNode * )
	{
	}

	void RegexNodeVisitor::VisitConnectionNode(RegexConnectionNode * )
	{

	}

	void RegexCharSetNode::Accept(RegexNodeVisitor * visitor)
	{
		visitor->VisitCharSetNode(this);
	}

	void RegexSelectionNode::Accept(RegexNodeVisitor * visitor)
	{
		visitor->VisitSelectionNode(this);
	}

	void RegexConnectionNode::Accept(RegexNodeVisitor * visitor)
	{
		visitor->VisitConnectionNode(this);
	}

	void RegexRepeatNode::Accept(RegexNodeVisitor *visitor)
	{
		visitor->VisitRepeatNode(this);
	}

	String RegexConnectionNode::Reinterpret()
	{
		return LeftChild->Reinterpret() + RightChild->Reinterpret();
	}

	String RegexSelectionNode::Reinterpret()
	{
		return LeftChild->Reinterpret() + L"|" + RightChild->Reinterpret();
	}

	String RegexRepeatNode::Reinterpret()
	{
		wchar_t t;
		if (RepeatType == RegexRepeatNode::rtArbitary)
			t = L'*';
		else if (RepeatType == rtOptional)
			t = L'?';
		else
			t = L'+';
		return String(L"(") + Child->Reinterpret() + L")" + t;
	}

	String RegexCharSet::Reinterpret()
	{
		if (Ranges.Count()== 1 && Ranges[0].Begin == Ranges[0].End &&
			!Neg)
		{
			return (Ranges[0].Begin>=28 && Ranges[0].Begin <127)? String((wchar_t)Ranges[0].Begin):
				String(L"<") + String((int)Ranges[0].Begin) + String(L">");
		}
		else
		{
			StringBuilder rs;
			rs.Append(L"[");
			if (Neg)
				rs.Append(L'^');
			for (int i=0; i<Ranges.Count(); i++)
			{
				if (Ranges[i].Begin == Ranges[i].End)
					rs.Append(Ranges[i].Begin);
				else
				{
					rs.Append(Ranges[i].Begin>=28 && Ranges[i].Begin<128?Ranges[i].Begin:
						String(L"<") + String((int)Ranges[i].Begin) + L">");
					rs.Append(L'-');
					rs.Append(Ranges[i].End>=28 && Ranges[i].End<128?Ranges[i].End:
						String(L"<") + String((int)Ranges[i].End)+ L">");
				}
			}
			rs.Append(L']');
			return rs.ProduceString();
		}
	}

	String RegexCharSetNode::Reinterpret()
	{
		return CharSet->Reinterpret();
	}

	void RegexCharSet::Sort()
	{
		for (int i=0; i<Ranges.Count()-1; i++)
		{
			for (int j=i+1; j<Ranges.Count(); j++)
			{
				RegexCharRange ri,rj;
				ri = Ranges[i];
				rj = Ranges[j];
				if (Ranges[i].Begin > Ranges[j].Begin)
				{
					RegexCharRange range = Ranges[i];
					Ranges[i] = Ranges[j];
					Ranges[j] = range;
				}
			}
		}
	}

	void RegexCharSet::Normalize()
	{
		for (int i=0; i<Ranges.Count()-1; i++)
		{
			for (int j=i+1; j<Ranges.Count(); j++)
			{
				if ((Ranges[i].Begin >= Ranges[j].Begin && Ranges[i].Begin <= Ranges[j].End) ||
					(Ranges[j].Begin >= Ranges[i].Begin && Ranges[j].Begin <= Ranges[i].End) )
				{
					Ranges[i].Begin = Math::Min(Ranges[i].Begin, Ranges[j].Begin);
					Ranges[i].End = Math::Max(Ranges[i].End, Ranges[j].End);
					Ranges.RemoveAt(j);
					j--;
				}
			}
		}
		Sort();
		if (Neg)
		{
			List<RegexCharRange> nranges;
			nranges.AddRange(Ranges);
			Ranges.Clear();
			RegexCharRange range;
			range.Begin = 1;
			for (int i=0; i<nranges.Count(); i++)
			{
				range.End = nranges[i].Begin-1;
				Ranges.Add(range);
				range.Begin = nranges[i].End+1;
			}
			range.End = 65530;
			Ranges.Add(range);
			Neg = false;
		}
	}

	bool RegexCharSet::Contains(RegexCharRange r)
	{
		for (int i=0; i<Ranges.Count(); i++)
		{
			if (r.Begin >= Ranges[i].Begin && r.End <= Ranges[i].End)
				return true;
		}
		return false;
	}

	void RegexCharSet::RangeIntersection(RegexCharRange r1, RegexCharRange r2, RegexCharSet & rs)
	{
		RegexCharRange r;
		r.Begin = Math::Max(r1.Begin,r2.Begin);
		r.End = Math::Min(r1.End, r2.End);
		if (r.Begin <= r.End)
			rs.Ranges.Add(r);
	}
	
	void RegexCharSet::RangeMinus(RegexCharRange r1, RegexCharRange r2, RegexCharSet & rs)
	{
		if (r2.Begin <= r1.Begin && r2.End>= r1.Begin && r2.End <= r1.End)
		{
			RegexCharRange r;
			r.Begin = ((int)r2.End + 1)>0xFFFF?0xFFFF:r2.End+1;
			r.End = r1.End;
			if (r.Begin <= r.End && !(r.Begin == r.End && r.Begin == 65530))
				rs.Ranges.Add(r);
		}
		else if (r2.Begin >= r1.Begin && r2.Begin <= r1.End && r2.End >= r1.End)
		{
			RegexCharRange r;
			r.Begin = r1.Begin;
			r.End = r2.Begin == 1? 1: r2.Begin - 1;
			if (r.Begin <= r.End && !(r.Begin == r.End == 1))
				rs.Ranges.Add(r);
		}
		else if (r2.Begin >= r1.Begin && r2.End <= r1.End)
		{
			RegexCharRange r;
			r.Begin = r1.Begin;
			r.End = r2.Begin == 1? 1: r2.Begin - 1;
			if (r.Begin <= r.End && !(r.Begin == r.End && r.Begin  == 1))
				rs.Ranges.Add(r);
			r.Begin = r2.End == 0xFFFF? r2.End : r2.End + 1;
			r.End = r1.End;
			if (r.Begin <= r.End && !(r.Begin == r.End && r.Begin  == 65530))
				rs.Ranges.Add(r);
		}
		else if (r2.End<r1.Begin || r1.End < r2.Begin)
		{
			rs.Ranges.Add(r1);
		}
	}

	void RegexCharSet::CharSetMinus(RegexCharSet & s1, RegexCharSet & s2)
	{
		RegexCharSet s;
		for (int i=0; i<s1.Ranges.Count(); i++)
		{
			for (int j=0; j<s2.Ranges.Count(); j++)
			{
				if (i>=s1.Ranges.Count() || i<0)
					return;
				s.Ranges.Clear();
				RangeMinus(s1.Ranges[i], s2.Ranges[j], s);
				if (s.Ranges.Count() == 1)
					s1.Ranges[i] = s.Ranges[0];
				else if (s.Ranges.Count() == 2)
				{
					s1.Ranges[i] = s.Ranges[0];
					s1.Ranges.Add(s.Ranges[1]);
				}
				else
				{
					s1.Ranges.RemoveAt(i);
					i--;
				}
			}
		}
	}

	RegexCharSet & RegexCharSet::operator = (const RegexCharSet & set)
	{
		CopyCtor(set);
		return *this;
	}

	bool RegexCharSet::RegexCharRange::operator == (const RegexCharRange & r)
	{
		return r.Begin == Begin && r.End == End;
	}

	void RegexCharSet::AddRange(RegexCharRange newR)
	{
		//RegexCharSet set;
		//set.Ranges.Add(r);
		//for (int i=0; i<Ranges.Count(); i++)
		//{
		//	if (Ranges[i].Begin < r.Begin && Ranges[i].End > r.Begin)
		//	{
		//		RegexCharRange nrange;
		//		nrange.Begin = r.Begin;
		//		nrange.End = Ranges[i].End;
		//		Ranges[i].End = r.Begin == 1? 1:r.Begin-1;
		//		if (!Ranges.Contains(nrange))
		//			Ranges.Add(nrange);
		//	}
		//	if (r.End > Ranges[i].Begin && r.End < Ranges[i].End)
		//	{
		//		RegexCharRange nrange;
		//		nrange.Begin = r.End == 0xFFFF ? 0xFFFF : r.End+1;
		//		nrange.End = Ranges[i].End;
		//		Ranges[i].End = r.End;
		//		if (!Ranges.Contains(nrange))
		//			Ranges.Add(nrange);
		//	}
		//	if (r.Begin == Ranges[i].Begin && r.End == Ranges[i].End)
		//		return;
		//}
		//for (int i=0; i<Ranges.Count(); i++)
		//	set.SubtractRange(Ranges[i]);
		//for (int i=0; i<set.Ranges.Count(); i++)
		//{
		//	for (int j=0; j<Ranges.Count(); j++)
		//		if (Ranges[j].Begin == set.Ranges[i].Begin ||
		//			Ranges[j].Begin == set.Ranges[i].End ||
		//			Ranges[j].End == set.Ranges[i].End||
		//			Ranges[j].End == set.Ranges[i].Begin)
		//		{
		//			RegexCharRange sr = set.Ranges[i];
		//			RegexCharRange r = Ranges[j];
		//			throw 0;
		//		}
		//	if (!Ranges.Contains(set.Ranges[i]))
		//		Ranges.Add(set.Ranges[i]);
		//}
		//Normalize();
		if (newR.Begin > newR.End)
			return;
		Sort();
		int rangeCount = Ranges.Count();
		for (int i=0; i<rangeCount; i++)
		{
			RegexCharRange & oriR = Ranges[i];
			if (newR.Begin > oriR.Begin)
			{
				if (newR.Begin > oriR.End)
				{

				}
				else if (newR.End > oriR.End)
				{
					RegexCharRange nRange;
					nRange.Begin = newR.Begin;
					nRange.End = oriR.End;
					wchar_t newR_begin = newR.Begin;
					newR.Begin = oriR.End + 1;
					oriR.End = newR_begin-1;
					Ranges.Add(nRange);
				}
				else if (newR.End == oriR.End)
				{
					oriR.End = newR.Begin - 1;
					Ranges.Add(newR);
					return;
				}
				else if (newR.End < oriR.End)
				{
					RegexCharRange nRange;
					nRange.Begin = newR.End + 1;
					nRange.End = oriR.End;
					oriR.End = newR.Begin - 1;
					Ranges.Add(newR);
					Ranges.Add(nRange);
					return;
				}
			}
			else if (newR.Begin == oriR.Begin)
			{
				if (newR.End > oriR.End)
				{
					newR.Begin = oriR.End + 1;
				}
				else if (newR.End == oriR.End)
				{
					return;
				}
				else
				{
					wchar_t oriR_end = oriR.End;
					oriR.End = newR.End;
					newR.End = oriR_end;
					newR.Begin = oriR.End + 1;
					Ranges.Add(newR);
					return;
				}
			}
			else if (newR.Begin < oriR.Begin)
			{
				if (newR.End > oriR.End)
				{
					RegexCharRange nRange;
					nRange.Begin = newR.Begin;
					nRange.End = oriR.Begin-1;
					Ranges.Add(nRange);
					newR.Begin = oriR.Begin;
					i--;
				}
				else if (newR.End == oriR.End)
				{
					RegexCharRange nRange;
					nRange.Begin = newR.Begin;
					nRange.End = oriR.Begin-1;
					Ranges.Add(nRange);
					return;
				}
				else if (newR.End < oriR.End && newR.End >= oriR.Begin)
				{
					RegexCharRange nRange;
					nRange.Begin = newR.Begin;
					nRange.End = oriR.Begin-1;
					Ranges.Add(nRange);
					nRange.Begin = newR.End+1;
					nRange.End = oriR.End;
					Ranges.Add(nRange);
					oriR.End = newR.End;
					return;
				}
				else
					break;
			}
		}
		Ranges.Add(newR);
		
	}

	void RegexCharSet::SubtractRange(RegexCharRange r)
	{
		
		int rc = Ranges.Count();
		for (int i=0; i<rc; i++)
		{
			RegexCharSet rs;
			RangeMinus(Ranges[i], r, rs);
			if (rs.Ranges.Count() == 1)
				Ranges[i] = rs.Ranges[0];
			else if (rs.Ranges.Count() == 0)
			{
				Ranges.RemoveAt(i);
				i--;
				rc--;
			}
			else
			{
				Ranges[i] = rs.Ranges[0];
				Ranges.Add(rs.Ranges[1]);
			}
		}
		Normalize();
	}

	void RegexCharSet::CalcCharElementFromPair(RegexCharSet * c1, RegexCharSet * c2, RegexCharSet & AmB, RegexCharSet & BmA, RegexCharSet & AnB)
	{
		AmB = *c1;
		BmA = *c2;
		CharSetMinus(AmB, *c2);
		CharSetMinus(BmA, *c1);
		for (int i=0; i<c1->Ranges.Count(); i++)
		{
			if (c2->Ranges.Count())
			{
				for (int j=0; j<c2->Ranges.Count(); j++)
				{
					RangeIntersection(c1->Ranges[i], c2->Ranges[j], AnB);
				}
			}
		}
		AmB.Normalize();
		BmA.Normalize();
		AnB.Normalize();
	}

	bool RegexCharSet::operator ==(const RegexCharSet & set)
	{
		if (Ranges.Count() != set.Ranges.Count())
			return false;
		for (int i=0; i<Ranges.Count(); i++)
		{
			if (Ranges[i].Begin != set.Ranges[i].Begin ||
				Ranges[i].End != set.Ranges[i].End)
				return false;
		}
		return true;
	}

	void RegexCharSet::InsertElement(List<RefPtr<RegexCharSet>> &L, RefPtr<RegexCharSet> & elem)
	{
		bool find = false;
		for (int i=0; i<L.Count(); i++)
		{
			if ((*L[i]) == *elem)
			{
				for (int k=0; k<elem->OriSet.Count(); k++)
				{
					if (!L[i]->OriSet.Contains(elem->OriSet[k]))
						L[i]->OriSet.Add(elem->OriSet[k]);
				}
				find = true;
				break;
			}
		}
		if (!find)
			L.Add(elem);
	}

	void RegexCharSet::CalcCharElements(List<RegexCharSet *> &sets, List<RegexCharRange> & elements)
	{
		RegexCharSet set;
		for (int i=0; i<sets.Count(); i++)
			for (int j=0; j<sets[i]->Ranges.Count(); j++)
				set.AddRange(sets[i]->Ranges[j]);
		for (int j=0; j<set.Ranges.Count(); j++)
		{
			for (int i=0; i<sets.Count(); i++)
			{
				if (sets[i]->Contains(set.Ranges[j]))
					sets[i]->Elements.Add((unsigned short)j);
			}
			elements.Add(set.Ranges[j]);
		}
		/*
		List<RefPtr<RegexCharSet>> L;
		if (!sets.Count())
			return;
		int lastSetCount = sets.Count();
		for (int i=0; i<sets.Count(); i++)
			sets[i]->OriSet.Add(sets[i]);
		L.Add(new RegexCharSet(*(sets[0])));
		for (int i=1; i<sets.Count(); i++)
		{
			RefPtr<RegexCharSet> bma = new RegexCharSet(*sets[i]);
			bma->OriSet = sets[i]->OriSet;
			for (int j=L.Count()-1; j>=0; j--)
			{
				RefPtr<RegexCharSet> bma2 = new RegexCharSet();
				RefPtr<RegexCharSet> amb = new RegexCharSet();
				RefPtr<RegexCharSet> anb = new RegexCharSet();
				CalcCharElementFromPair(L[j].operator ->(), sets[i], *amb, *bma2, *anb);
				CharSetMinus(*bma, *L[j]);
				L[j]->Normalize();
				amb->OriSet = L[j]->OriSet;
				anb->OriSet = amb->OriSet;
				for (int k=0; k<bma->OriSet.Count(); k++)
				{
					if (!anb->OriSet.Contains(bma->OriSet[k]))
						anb->OriSet.Add(bma->OriSet[k]);
				}
				if (amb->Ranges.Count())
				{
					L[j] = amb;
				}
				else
				{
					L[j] = 0;
					L.RemoveAt(j);
				}
				if (anb->Ranges.Count())
				{
					InsertElement(L,anb);
				}
			}
			if (bma->Ranges.Count())
			{
				InsertElement(L,bma);
			}

		}
		for (int i=0; i<L.Count(); i++)
		{
			for (int j=0; j<L[i]->OriSet.Count(); j++)
			{
				L[i]->OriSet[j]->Elements.Add(i);
			}
			elements.Add(L[i].Release());
		}
		for (int i=lastSetCount; i<sets.Count(); i++)
			RemoveAt sets[i];
		sets.SetSize(lastSetCount);

		*/
	}

	void RegexCharSet::CopyCtor(const RegexCharSet & set)
	{
		Ranges = set.Ranges;
		Elements = set.Elements;
		Neg = set.Neg;
		OriSet = set.OriSet;
	}

	RegexCharSet::RegexCharSet(const RegexCharSet & set)
	{
		CopyCtor(set);
	}


}
}