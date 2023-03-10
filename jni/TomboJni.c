/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include "CryptManager.h"

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/HelloJni/HelloJni.java
 */
jstring
Java_org_efimov_tomboedit_NoteEditor_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello from JNI !");
}

jboolean
Java_org_efimov_tomboedit_NoteEditor_decrypt( JNIEnv* 	 env,
                                              jobject 	 thiz,
                                              jbyteArray cipherText,
                                              jbyteArray plainText,
                                              jstring password)
{
	jbyte *pCipher = (*env)->GetByteArrayElements(env, cipherText, 0);
	int cipherLen = (*env)->GetArrayLength(env, cipherText);
	plainText = (*env)->NewByteArray(env, cipherLen);
	jboolean isCopy = 0;
	const jbyte *pPassword = (*env)->GetStringUTFChars(env, password, &isCopy);

//	__android_log_print(ANDROID_LOG_ERROR, "TomboJni", "cipher=%p len=%d", pCipher, cipherLen);
//	__android_log_print(ANDROID_LOG_ERROR, "TomboJni", "pwd=%s", pPassword);

	jboolean bRet = JNI_TRUE;
	int iRet = CryptManager_Decrypt(pCipher, cipherLen, pPassword);

	if( iRet )
	{
		(*env)->SetByteArrayRegion(env, plainText, 0, cipherLen, pCipher);
		(*env)->ReleaseByteArrayElements(env, cipherText, pCipher, JNI_ABORT);
		(*env)->ReleaseByteArrayElements(env, plainText, pCipher, 0);
		(*env)->ReleaseStringUTFChars(env, password, pPassword);
	}

    return iRet != 0 ? JNI_TRUE: JNI_FALSE;
}
