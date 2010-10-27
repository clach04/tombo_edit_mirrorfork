
#include <stdlib.h>
#include <time.h>
#include "wintypes.h"
#include "CryptManager.h"
#include "File.h"
#include <string.h>

// CryptManager‚É‚æ‚éˆÃ�†‰»ƒtƒ@ƒCƒ‹‚ÌƒtƒH�[ƒ}ƒbƒg
// '*'‚ÍˆÃ�†‰»‚³‚ê‚½ƒf�[ƒ^
// 
// 0-3  : BF01(4 bytes)
// 4-7  : ƒf�[ƒ^’·(ŠÜ‚Þ rand + md5sum)(4 bytes)
// 8-15 :* random data(8 bytes)
//16-31 :* •½•¶‚Ìmd5sum(16 bytes)
//32-   :* data


void WipeOutAndDelete(char *p, DWORD len);
#ifdef _WIN32_WCE
void WipeOutAndDelete(LPTSTR p, DWORD len);
#endif

BOOL CryptManager::Init(const char *pKey)
{
	//SYSTEMTIME st;
	//GetSystemTime(&st);
	//FILETIME ft;
	//SystemTimeToFileTime(&st, &ft);
	//srand(ft.dwLowDateTime);
	srand((DWORD)time(NULL));

	getMD5Sum(md5key, (byte*)pKey, (int) strlen(pKey));
	return TRUE;
}

//////////////////////////////////////////////////
// ƒfƒXƒgƒ‰ƒNƒ^
//////////////////////////////////////////////////
// ƒZƒLƒ…ƒŠƒeƒB�ã�A•ÛŽ�‚µ‚Ä‚¢‚½md5key‚ð�Á‹Ž‚·‚é�B

CryptManager::~CryptManager()
{
	for (DWORD i = 0; i < 16; i++) {
		md5key[i] = 0;
	}
}

//////////////////////////////////////////////////
// ƒf�[ƒ^‚ÌˆÃ�†‰»
//////////////////////////////////////////////////

BOOL CryptManager::Encrypt(LPBYTE pBuf, int len)
{
	if (len == 0) return FALSE;

	if (!crypt.ResetStream(md5key, 16)) return FALSE;
	BYTE buf[8];
	LPBYTE p = pBuf;
	int n = len;
	int i;
	while (n > 8) {
		for (i = 0; i < 8; i++) {
			buf[i] = p[i];
		}
		crypt.Encrypt(p, buf, 8);
		p += 8;
		n -= 8;
	}
	if (n > 0) {
		for (i = 0; i < n; i++) {
			buf[i] = p[i];
		}
		crypt.Encrypt(p, buf, n);
	}

	for (i = 0; i < 8; i++) buf[i] = 0;
	return TRUE;
}

//////////////////////////////////////////////////
// ƒf�[ƒ^‚Ì•œ�†
//////////////////////////////////////////////////

BOOL CryptManager::Decrypt(LPBYTE pBuf, int len)
{
	if (len == 0) return FALSE;

	if (!crypt.ResetStream(md5key, 16)) return FALSE;

	BYTE buf[8];
	LPBYTE p = pBuf;
	int n = len;
	int i;
	while (n >= 8) {
		for (i = 0; i < 8; i++) {
			buf[i] = p[i];
		}
		crypt.Decrypt(p, buf);
		p += 8;
		n -= 8;
	}

	for (i = 0; i < 8; i++) buf[i] = 0;
	return TRUE;
}

//////////////////////////////////////////////////
// ƒf�[ƒ^‚ÌˆÃ�†‰»‚Æƒtƒ@ƒCƒ‹‚Ö‚Ì•Û‘¶
//////////////////////////////////////////////////

BOOL CryptManager::EncryptAndStore(const LPBYTE pData, int nSize, LPCTSTR pFileName)
{
	// —ÌˆæŠm•Û�EˆÃ�†‰»

	int i=0;
	int len = ((nSize >> 3) + 1) * 8;
	len += 24;

	LPBYTE pBuf = new BYTE[len];
	if (pBuf == NULL) {
		FatalError(ERROR_NOT_ENOUGH_MEMORY, TEXT("CryptManager::EncryptAndStore memory allocation failed"));
		return FALSE;
	}

	// —��”‚Ì–„‚ß�ž‚Ý
	for (i = 0; i < 8; i++) {
		pBuf[i] = (BYTE)(rand() & 0xFF);
	}

	// •½•¶‚ÌMD5SUM‚ðŽæ“¾
	getMD5Sum(pBuf + 8, pData, nSize);

	// •½•¶‚ÌƒRƒs�[
	LPBYTE p = pBuf + 24;
	const BYTE *q = pData;
	for (i = 0; i < nSize; i++) {
		*p++ = *q++;
	}

	// ˆÃ�†‰»
	if (!Encrypt(pBuf, nSize + 24)) {
		for (i = 0; i < len; i++) pBuf[i] = 0;
		WipeOutAndDelete((char*)pBuf, len);
		
		FatalError(0, TEXT("CryptManager::EncryptAndStore : Encrypt failed"));
		return FALSE;
	}

	// ƒtƒ@ƒCƒ‹‚Ö‚Ì•Û‘¶
	File outf;

	if (!outf.Open(pFileName, true)) {
		for (i = 0; i < len; i++) pBuf[i] = 0;
		WipeOutAndDelete((char*)pBuf, len);

		TCHAR buf[1024];
		wsprintf(buf, TEXT("CryptManager::EncryptAndStore : Open File failed %s"), pFileName);
		FatalError(0, buf);
		return FALSE;
	}
	if (!outf.Write((LPBYTE)"BF01", 4) ||				// ƒo�[ƒWƒ‡ƒ“ƒwƒbƒ_
		!outf.Write((const LPBYTE)&nSize, sizeof(nSize)) ||	// •½•¶ƒf�[ƒ^’·
		!outf.Write(pBuf, len)) {							// ƒf�[ƒ^
		TCHAR buf[1024];
		wsprintf(buf, TEXT("CryptManager::EncryptAndStore write failed"));
		FatalError(0, buf);
	}
	if (!outf.SetEOF()) return FALSE;
	outf.Close();

	WipeOutAndDelete((char*)pBuf, len);
	return TRUE;
}

LPBYTE CryptManager::LoadAndDecrypt(LPDWORD pSize, LPCTSTR pFileName)
{
	File inf;
	if (!inf.Open(pFileName, false)) {
		return NULL;
	}

	DWORD nFileSize = inf.FileSize();
	char version[5];
	DWORD n;
	DWORD nDataSize;

	// ƒo�[ƒWƒ‡ƒ“ƒwƒbƒ_
	n = 4;
	inf.Read((LPBYTE)version, &n);
	version[4] = '\0';
	if (strcmp(version, "BF01") != 0) {
		SetLastError(ERROR_INVALID_DATA);
		return NULL;
	}

	// ƒf�[ƒ^’·
	n = sizeof(nDataSize);
	inf.Read((LPBYTE)&nDataSize, &n);

	LPBYTE pBuf = new BYTE[nFileSize + 1];
	n = nFileSize - 4 - sizeof(nDataSize);
	inf.Read(pBuf, &n);
	if (!Decrypt(pBuf, n)) {
		WipeOutAndDelete((char*)pBuf, nFileSize + 1);
		return NULL;
	}

	// •œ�†‰»•¶MD5SUM‚ÌŽæ“¾
	BYTE decriptsum[16];
	getMD5Sum(decriptsum, pBuf + 24, nDataSize);

	// �³‚µ‚­•œ�†‰»‚Å‚«‚½‚©‚Ìƒ`ƒFƒbƒN
	for (int i = 0; i < 16; i++) {
		if (pBuf[8 + i] != decriptsum[i]) {
			WipeOutAndDelete((char*)pBuf, nFileSize + 1);
			FatalError(ERROR_INVALID_PASSWORD, "Invalid Password");
			return NULL;
		}
	}
	pBuf[nDataSize + 24] = '\0';
	*pSize = nDataSize;

	// —Ìˆæ�ÄŠm•Û
	// —��”ƒf�[ƒ^‚ÆMD5SUM‚ð‚Ü‚Æ‚ß‚Ä•œ�†‰»‚·‚é‚½‚ß‚É1‚Â‚Ìƒoƒbƒtƒ@‚ÅŠm•Û‚µ‚½‚ª�A
	// delete‚ð�³‚µ‚­�s‚¦‚é‚æ‚¤‚É—Ìˆæ‚ð�ÄŠm•Û�AƒRƒs�[‚µ‚Ä•Ô‚·
	LPBYTE pData = new BYTE[nDataSize + 1];
	if (pData == NULL) {
		WipeOutAndDelete((char*)pBuf, nFileSize + 1);
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return NULL;
	}
	memcpy(pData, pBuf + 24, nDataSize);
	pData[nDataSize] = '\0';
	WipeOutAndDelete((char*)pBuf, nFileSize + 1);
	return pData;
}

LPBYTE CryptManager::LoadStdIn(LPDWORD pSize)
{
	size_t size = 0;
	size_t blocksize = 32768;

	LPBYTE data = new BYTE[blocksize];
	LPBYTE buf = data;

	// read until end of stdin
	while(!feof(stdin))
	{
		// read the block
		size_t readsize = fread(buf, 1, blocksize, stdin);
		// adjust size
		size +=readsize;
		// check, if eof, and exit
		if(readsize != blocksize)
			break;
		// we need bigger block 
		LPBYTE newdata = new BYTE[size+blocksize];
		memcpy(newdata, data, size);
		memset(data, 0xcb, size);
		delete[] data;
		data = newdata;
		buf = data+size;
	}
	*pSize = (DWORD) size;
	return data;
}


void WipeOutAndDelete(LPTSTR p, DWORD len)
{
	for (DWORD i = 0; i < len; i++) p[i] = TEXT('\0');
}


void WipeOutAndDelete(LPTSTR p)
{
	if (p == NULL) return;

	LPTSTR q = p;
	while (*q) {
		*q++ = TEXT('\0');
	}
	delete [] p;
}

#ifdef _WIN32_WCE
void WipeOutAndDelete(char *p, DWORD len)
{
	for (DWORD i = 0; i < len; i++) p[i] = TEXT('\0');
}

void WipeOutAndDelete(char *p)
{
	if (p == NULL) return;

	char *q = p;
	while (*q) {
		*q++ = TEXT('\0');
	}
	delete [] p;
}
#endif

extern "C" int CryptManager_Decrypt(unsigned char* pBuf, int len, const char* pKey)
{
	CryptManager mgr;
	BOOL bRet = mgr.Init(pKey);

	if(bRet)
	{
			unsigned char* pBufOriginal = pBuf;
			DWORD nFileSize = len;
			char version[5];
			DWORD n;
			DWORD nDataSize;

			memcpy(version, pBuf, 4);
			version[4] = '\0';
			if (strcmp(version, "BF01") != 0) {
				SetLastError(ERROR_INVALID_DATA);
				return FALSE;
			}

			// data size
			memcpy(&nDataSize, pBuf+4, sizeof(n));

			pBuf = pBuf + 4 + sizeof(nDataSize);
			n = nFileSize - 4 - sizeof(nDataSize);
			if (!mgr.Decrypt(pBuf, n)) {
				return FALSE;
			}

			// •œ�†‰»•¶MD5SUM‚ÌŽæ“¾
			BYTE decriptsum[16];
			getMD5Sum(decriptsum, pBuf + 24, nDataSize);

			// �³‚µ‚­•œ�†‰»‚Å‚«‚½‚©‚Ìƒ`ƒFƒbƒN
			for (int i = 0; i < 16; i++) {
				if (pBuf[8 + i] != decriptsum[i]) {
					FatalError(ERROR_INVALID_PASSWORD, "Invalid Password");
					return FALSE;
				}
			}
			pBuf[nDataSize + 24] = '\0';
			//*pSize = nDataSize;

			// —Ìˆæ�ÄŠm•Û
			// —��”ƒf�[ƒ^‚ÆMD5SUM‚ð‚Ü‚Æ‚ß‚Ä•œ�†‰»‚·‚é‚½‚ß‚É1‚Â‚Ìƒoƒbƒtƒ@‚ÅŠm•Û‚µ‚½‚ª�A
			// delete‚ð�³‚µ‚­�s‚¦‚é‚æ‚¤‚É—Ìˆæ‚ð�ÄŠm•Û�AƒRƒs�[‚µ‚Ä•Ô‚·
			//LPBYTE pData = new BYTE[nDataSize + 1];
			//if (pData == NULL) {
			//	WipeOutAndDelete((char*)pBuf, nFileSize + 1);
			//	SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			//	return NULL;
			//}
			LPBYTE pData = pBufOriginal;

			memmove(pData, pBuf + 24, nDataSize);
			memset(pData+nDataSize, 0, len-nDataSize);
			//pData[nDataSize] = '\0';
			//WipeOutAndDelete((char*)pBuf, nFileSize + 1);

			return TRUE;
	}

	return 1;
}

