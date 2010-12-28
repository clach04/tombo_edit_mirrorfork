#ifndef CRYPTMANAGER_H
#define CRYPTMANAGER_H

#ifdef __cplusplus

#include "Crypt.h"

class CryptManager {
	Crypt crypt;
	BYTE md5key[16];

public:
	~CryptManager();

	// ƒpƒXƒ��[ƒh‚ðŽw’è‚·‚é�B
	BOOL Init(const char *pKey);

	// pBuf‚É•½•¶‚ð�Ý’è‚µ‚ÄŒÄ‚Ô‚±‚Æ‚ÅpBuf‚ÉˆÃ�†•¶‚ð•Ô‚·�B
	BOOL Encrypt(LPBYTE pBuf, int len);

	// pBuf‚ÉˆÃ�†•¶‚ð�Ý’è‚µ‚ÄŒÄ‚Ô‚±‚Æ‚ÅpBuf‚É•½•¶‚ð•Ô‚·�B
	// len‚Í8‚Ì”{�”‚Å‚È‚¯‚ê‚Î‚È‚ç‚È‚¢�B
	BOOL Decrypt(LPBYTE pBuf, int len);

	// ƒeƒLƒXƒg‚ðŽó‚¯Žæ‚è�AˆÃ�†‰»‚µ‚Äƒtƒ@ƒCƒ‹‚É•Û‘¶‚·‚é
	BOOL EncryptAndStore(const LPBYTE pData, int nSize, LPCTSTR pFileName);

	// ˆÃ�†‰»‚³‚ê‚½ƒtƒ@ƒCƒ‹‚ð•œ�†‰»‚·‚é�B*pSize‚É‚Í–{“–‚ÌƒoƒCƒg�”‚ª“ü‚é�B
	// ‚Ü‚½�A–ß‚è’l‚Ìƒf�[ƒ^—ñ‚Ì�ÅŒã‚É‚ÍNULL‚ª•t‰Á‚³‚ê‚Ä‚¢‚é�B
	// –ß‚è’l‚Ìƒoƒbƒtƒ@‚Ínew‚³‚ê‚Ä‚¢‚é‚½‚ß�A‚¢‚ç‚È‚­‚È‚Á‚½’iŠK‚ÅŒÄ‚Ñ�o‚µŒ³‚ªdelete[]‚·‚é•K—v‚ª‚ ‚é�B
	LPBYTE LoadAndDecrypt(LPDWORD pSize, LPCTSTR pFileName);

	// read byte stream from std input
	LPBYTE LoadStdIn(LPDWORD pSize);

//	BOOL EncryptFile(LPCTSTR pSrc, LPCTSTR pDst);
//	BOOL DecryptFile(LPCTSTR pSrc, LPCTSTR pDst);
};

extern "C" {
	void getMD5Sum(byte *md5sum, byte *in, int len);
	int CryptManager_Decrypt(unsigned char* pBuf, int len, const char* pKey);
};

#else

	int CryptManager_Decrypt(unsigned char* pBuf, int len, const char* pKey);
#endif // __cplusplus


#endif

