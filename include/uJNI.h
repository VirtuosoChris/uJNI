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
//  uJNI.h
//
//  Created by Chris Pugh on 6/2/15.
//

#ifndef ____uJNI__
#define ____uJNI__

#include <jni.h>
#include <string>
#include <functional>

#include "uJNIClasses.h"

/* todos */
/* cache method and class handles */
/* handle array objects */
/* handle jobject return type and arguments to methods */
/* construction / conversion of java arrays */

void throwJavaException(JNIEnv* pEnv, const char* exceptionClass, const char* message);

std::string stringFromJavaString(JNIEnv* env, jstring str);

void forObjectInJavaList(JNIEnv* env, jobject listObject, std::function<void (jobject)> func);

template<class Arg1,class ...Args>
const std::string& compute_signature_string();

template<class Arg1>
const std::string& compute_signature_string_return();


template<class RETURNTYPE, typename... Args>
class JavaMethodCaller;

template<class RETURNTYPE, typename... Args>
class JavaStaticMethodCaller;

class JavaObject;

class JavaClass
{
public:

    jclass classInstance;

    JavaClass(const std::string& nameIn);
    JavaClass(jclass instanceIn);
    JavaClass(jobject obj);

    template<class RETURNTYPE, typename... Args>
    RETURNTYPE callStatic(const std::string& method, Args... args);

    template<class RETURNTYPE>
    RETURNTYPE callStatic(const std::string& method);

    template<typename... Args>
    JavaObject construct(Args... args);

    JavaClass superClass()const;

    ////\todo
    template<class RETURNTYPE, typename... Args>
    constexpr std::string makeSignature()
    {
        return std::string("(") + compute_signature_string<Args...>() + ')' + compute_signature_string_return<RETURNTYPE>();
    }

    template <class T>
    T getStaticField(const char* name);

    template<class T>
    void setStaticField(const char* name, T field);

    operator bool()const;
};


class JavaObject
{
public:

    jobject obj;
    JavaClass javaClass;

    JavaObject(jobject objIn);
    JavaObject(const JavaObject& objIn);

    template<class RETURNTYPE, typename... Args>
    RETURNTYPE call(const std::string& method, Args... args); ///< Call java function named by string "method".  Use <init> for constructor.

    template<class RETURNTYPE>
    RETURNTYPE call(const std::string& method);

    template <class T>
    T getField(const char* name);

    template<class T>
    void setField(const char* name, T field);

    int monitorEnter()const; ///< enter synchronized block on this object
    int monitorExit()const; ///< exit synchronized block on this object

    JavaObject& operator=(const JavaObject& objIn);
    bool operator==(const JavaObject& objectIn)const;
    operator bool()const;

    ~JavaObject();

    private:

    template<class RETURNTYPE, typename... Args>
    constexpr std::string makeSignature()
    {
        return std::string("(") +  compute_signature_string<Args...>() + ')' + compute_signature_string_return<RETURNTYPE>();
    }

};

JavaVM* jvm();

///jvm is sharable + safe across threads, env is not
JNIEnv* getJNIEnv(JavaVM* pJavaVM);

inline JNIEnv* getJNIEnv()
{
    return getJNIEnv(jvm());
}

void detachCurrentThreadFromJVM();


#include "uJNI.inl"


#endif /* defined(____uJNI__) */
