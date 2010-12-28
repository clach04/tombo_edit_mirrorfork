#ifndef FF_FILE_H
#define FF_FILE_H

#include <stdio.h>

// WIN32 API�ł�64bit�̃t�@�C���T�C�Y���T�|�[�g���Ă��邪�A
// �ʏ�̃A�v���P�[�V�����ł�32bit�ŏ\���Ȃ̂Ńv���O�����̊ȗ����̂��߁A
// �����ł�32bit�ŏ�������B

class File {
protected:
	FILE* hFile;
	DWORD nSize;
public:

	// �R���X�g���N�^&�f�X�g���N�^
	File() : hFile(NULL) {}
	~File();

	//BOOL Open(LPCTSTR pFileName, DWORD nMode, DWORD nShareMode, DWORD nOpenMode);
	BOOL Open(LPCTSTR pFileName, bool bWrite);

	void Close();

	// �t�@�C���T�C�Y�̎擾
	DWORD FileSize() { return nSize; }

	// �t�@�C���|�C���^�̃V�[�N
	//BOOL Seek(DWORD nPos);
	// ���݂̃t�@�C���|�C���^�̎擾
	//DWORD CurrentPos();

	BOOL Read(LPBYTE pBuf, LPDWORD pSize);
	BOOL Write(const LPBYTE pBuf, DWORD nSize);

	//BOOL WriteUnicodeString(LPCWSTR p);

	BOOL SetEOF();
};

#endif
