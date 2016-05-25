// Copyright (c) 2015 Virtuoso Engine, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//
//  uJNI.cpp
//  
//
//  Created by Chris Pugh on 6/2/15.
//


#include "../include/uJNI.h"

#include <algorithm>

using namespace classnames;

JavaVM* myGlobalJavaVM = NULL;

JavaVM* jvm()
{
    return myGlobalJavaVM;
}


void detachCurrentThreadFromJVM()
{
    jvm()->DetachCurrentThread();
}

JavaClass loadClass(struct android_app* app, JNIEnv* env, const std::string& classname)
{
    jobject nativeActivity = app->activity->clazz;
    jclass acl = env->GetObjectClass(nativeActivity);
    jmethodID getClassLoader = env->GetMethodID(acl, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = env->CallObjectMethod(nativeActivity, getClassLoader);
    jclass classLoader = env->FindClass("java/lang/ClassLoader");
    jmethodID findClass = env->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    jstring strClassName = env->NewStringUTF(classname.c_str());
    jclass fireworksAudioClass = (jclass)(env->CallObjectMethod(cls, findClass, strClassName));
    env->DeleteLocalRef(strClassName);
    return JavaClass(fireworksAudioClass);
}

/*
/// called when the java code calls System.loadLibrary.  Cache the copy of the JVM object ptr.
jint JNI_OnLoad(JavaVM* pVM, void* reserved)
{
   myGlobalJavaVM = pVM;
   JNIEnv *lEnv;
   if (pVM->GetEnv((void**) &lEnv, JNI_VERSION_1_6) != JNI_OK)
   {
       return -1;   // A problem occured
   }

   return JNI_VERSION_1_6;
}*/


bool JavaObject::operator==(const JavaObject& objectIn)const
{
    return getJNIEnv()->IsSameObject(obj, objectIn.obj);
}


void forObjectInJavaList(JNIEnv* env, jobject listObject, std::function<void (jobject)> func)
{
    // Find "java/util/List" Class (Standard JAVA Class).
     jclass listClass = env->FindClass(java::util::List::name);
     if( listClass == NULL )
     {
         throwJavaException(env, java::lang::Exception::name, "Unable to get jclass for standard java list");
     }

     // Get "java.util.List.get(int location)" MethodID
     jmethodID getMethodID = env->GetMethodID( listClass, "get", "(I)Ljava/lang/Object;");
     if( getMethodID == NULL )
     {
         throwJavaException(env, java::lang::Exception::name, "Unable to get jmethodID for standard java list get(i)");
     }

      // Get "int java.util.List.size()" MethodID
     jmethodID sizeMethodID = env->GetMethodID( listClass, "size", "()I" );
     if( sizeMethodID == NULL )
     {
         throwJavaException(env, java::lang::Exception::name, "Unable to get jmethodID for standard java list size()");
     }

     // Call "int java.util.List.size()" method and get count of items in the list.
     int listItemsCount = (int)env->CallIntMethod(listObject, sizeMethodID );

     for( int i=0; i<listItemsCount; i++ )
     {
         // Call "java.util.List.get" method and get IdentParams object by index.
         jobject identParamsObject = env->CallObjectMethod( listObject, getMethodID, i);
         if( identParamsObject == NULL )
         {
             throwJavaException(env, java::lang::Exception::name, "forObjectInJavaList: Can't get Object from list");
         }

         func(identParamsObject);
     }
}


void throwJavaException(JNIEnv* pEnv, const char* exceptionClass, const char* message) //exceptionClass is fully qualified class name, eg com/packtpub/exception/NotExistingKeyException
{
    std::string exception_message(message);
    jclass lClass = pEnv->FindClass(exceptionClass);

    if (lClass != NULL)
    {
        pEnv->ThrowNew(lClass, message);
    }
    else
    {
        lClass = pEnv->FindClass(java::lang::Exception::name);
        exception_message+= std::string("\nUnable to throw exception of unknown type ") + exceptionClass + ": throwing java.lang.Exception instead";
        pEnv->ThrowNew(lClass, exception_message.c_str());
    }

    pEnv->DeleteLocalRef(lClass);

    throw std::runtime_error(exception_message.c_str());
}


std::string stringFromJavaString(JNIEnv* env, jstring str)
{
    const char* chars = env->GetStringUTFChars(str, NULL);
    std::string rval(chars);
    env->ReleaseStringUTFChars(str, chars);
    return rval;
}


JavaClass::JavaClass(const std::string& nameIn)
:
    classInstance(getJNIEnv()->FindClass(nameIn.c_str()))
{
    if(!classInstance)
    {
        std::string msg("Unable to find class : ");
        msg += nameIn;
        throwJavaException(getJNIEnv(), java::lang::Exception::name, msg.c_str());
    }
}


JavaClass::JavaClass(jobject obj)
{
    classInstance = getJNIEnv()->GetObjectClass(obj);
}


JavaObject::~JavaObject()
{
    getJNIEnv()->DeleteGlobalRef(obj);
}



JavaObject::JavaObject() : obj(0), javaClass((jclass)0)
{
    
}

JavaObject::JavaObject(jobject objIn)
:
    obj(getJNIEnv()->NewGlobalRef(objIn))
    ,javaClass(obj)
{
}


JavaObject::JavaObject(const JavaObject& objIn)
:javaClass(objIn.javaClass),
obj(getJNIEnv()->NewGlobalRef(objIn.obj))
{
}


JavaObject& JavaObject::operator=(const JavaObject& objIn)
{
    javaClass = objIn.javaClass;
    JNIEnv* env = getJNIEnv();
    env->DeleteGlobalRef(obj);
    obj = env->NewGlobalRef(objIn.obj);
}


JNIEnv* getJNIEnv(JavaVM* pJavaVM)
{
    JNIEnv* lEnv;
    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;  ///\todo
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    if(pJavaVM->GetEnv((void**)&lEnv, JNI_VERSION_1_6))
    {
        return lEnv;
    }

    if (pJavaVM->AttachCurrentThread(&lEnv, &lJavaVMAttachArgs) == JNI_OK)
    {
        return lEnv;
    }

    return NULL;
}


int JavaObject::monitorEnter()const
{
    getJNIEnv()->MonitorEnter(obj);
}


int JavaObject::monitorExit()const
{
    getJNIEnv()->MonitorExit(obj);
}


JavaObject::operator bool()const
{
    return javaClass && obj;
}


JavaClass::operator bool()const
{
    return classInstance;
}


JavaClass JavaClass::superClass()const
{
    return JavaClass(getJNIEnv()->GetSuperclass(classInstance));
}


JavaClass::JavaClass(jclass instanceIn) : classInstance(instanceIn)
{
}
