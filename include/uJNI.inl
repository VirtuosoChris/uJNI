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
//  uJNI.inl
//
//
//  Created by Chris Pugh on 6/2/15.
//

template<class Arg1,class ...Args>
inline const std::string& compute_signature_string()
{
	static const std::string a=compute_signature_string<Arg1>()+compute_signature_string<Args...>();
	return a;
}


template<class Arg1>
inline const std::string& compute_signature_string_return()
{
    return compute_signature_string<Arg1>();
}

template<>
inline const std::string& compute_signature_string_return<void>()
{
    static const std::string rval = "V";
    return rval;
}


template<>
inline const std::string& compute_signature_string<jint>()
{
	static const std::string a = "I";
	return a;
}

template<>
inline const std::string& compute_signature_string<jboolean>()
{
	static const std::string a= "Z";
	return a;
}

template<>
inline const std::string& compute_signature_string<jbyte>()
{
    static const std::string a= "B";
    return a;
}

template<>
inline const std::string& compute_signature_string<jchar>()
{
	static const std::string a="C";
	return a;
}

template<>
inline const std::string& compute_signature_string<jshort>()
{
	static const std::string a= "S";
	return a;
}

template<>
inline const std::string& compute_signature_string<jlong>()
{
    static const std::string a= "J";
    return a;
}

template<>
inline const std::string& compute_signature_string<jfloat>()
{
    static const std::string a= "F";
    return a;
}

template<>
inline const std::string& compute_signature_string<jdouble>()
{
    static const std::string a= "D";
    return a;
}


template<>
inline const std::string& compute_signature_string<jobject>()
{
    static const std::string a= "Ljava/lang/Object;";
    return a;
}


template<>
inline const std::string& compute_signature_string<void>()
{
    static const std::string a= "";
    return a;
}

inline jmethodID getMethodID(JNIEnv* env, jclass classInstance, const std::string& methodStr, const std::string& signature)
{
    jmethodID method = env->GetMethodID(classInstance, methodStr.c_str(), signature.c_str());

    if(!method)
    {
        std::string err("Unable to get method : ");
        err += methodStr;
        err += " With Signature: " + signature;
        throwJavaException(env, classnames::java::lang::Exception::name, err.c_str() );
    }

    return method;
}


inline jmethodID getStaticMethodID(JNIEnv* env, jclass classInstance, const std::string& methodStr, const std::string& signature)
{
    jmethodID method = env->GetStaticMethodID(classInstance, methodStr.c_str(), signature.c_str());

    if(!method)
    {
        std::string err("Unable to get static method : ");
        err += methodStr;
        err += " With Signature: " + signature;
        throwJavaException(env, classnames::java::lang::Exception::name, err.c_str());
    }

    return method;
}


inline jfieldID getStaticFieldID(JNIEnv* env, jclass classInstance, const char* name, const char* signature)
{
    jfieldID fieldID = env->GetStaticFieldID(classInstance, name, signature);

    if(!fieldID)
    {
        std::string err("Unable to get static field : ");
        err += name;
        err += " With Signature: ";
        err += signature;
        throwJavaException(env, classnames::java::lang::Exception::name, err.c_str());
    }

    return fieldID;
}

inline jfieldID getFieldID(JNIEnv* env, jclass classInstance, const char* name, const char* signature)
{
    jfieldID fieldID = env->GetFieldID(classInstance, name, signature);

    if(!fieldID)
    {
        std::string err("Unable to get field : ");
        err += name;
        err += " With Signature: ";
        err += signature;
        throwJavaException(env, classnames::java::lang::Exception::name, err.c_str());
    }

    return fieldID;
}


template<typename... Args>
class JavaMethodCaller<jobject, Args...>
{
public:
    static jobject callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallObjectMethod(object.obj, method, args...);
    }
};

template<typename... Args>
class JavaMethodCaller<jfloat, Args...>
{
public:
    static jfloat callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallFloatMethod(object.obj, method, args...);
    }
};

template<typename... Args>
class JavaMethodCaller<jint, Args...>
{
public:
    static jint callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallIntMethod(object.obj, method, args...);
    }
};


template<typename... Args>
class JavaMethodCaller<jboolean, Args...>
{
public:
    static jboolean callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallBooleanMethod(object.obj, method, args...);
    }
};

template<typename... Args>
class JavaMethodCaller<jbyte, Args...>
{
public:
    static jbyte callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallByteMethod(object.obj, method, args...);
    }
};


template<typename... Args>
class JavaMethodCaller<jchar, Args...>
{
public:
    static jchar callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallCharMethod(object.obj, method, args...);
    }
};


template<typename... Args>
class JavaMethodCaller<jshort, Args...>
{
  public:
    static jshort callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallShortMethod(object.obj, method, args...);
    }
};


template<typename... Args>
class JavaMethodCaller<jlong, Args...>
{
public:
    static jlong callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallLongMethod(object.obj, method, args...);
    }
};


template<typename... Args>
class JavaMethodCaller<jdouble, Args...>
{
public:
    static jdouble callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallDoubleMethod(object.obj, method, args...);
    }
};


template<typename... Args>
class JavaMethodCaller<void, Args...>
{
public:
    static void callFunction(JNIEnv* env, const JavaObject& object, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getMethodID(env, object.javaClass.classInstance, methodStr, signature);
        return env->CallVoidMethod(object.obj, method, args...);
    }
};


template<class RETURNTYPE, typename... Args>
RETURNTYPE JavaObject::call(const std::string& method, Args... args)
{
    JNIEnv* env = getJNIEnv();
    std::string signature = makeSignature<RETURNTYPE, Args...>();
    JavaMethodCaller<RETURNTYPE, Args...>::callFunction(env, *this, method, signature, args...);
}

template<class RETURNTYPE>
RETURNTYPE JavaObject::call(const std::string& method)
{
    JNIEnv* env = getJNIEnv();
    std::string signature = makeSignature<RETURNTYPE, void>();
    JavaMethodCaller<RETURNTYPE>::callFunction(env, *this, method, signature);
}


template<class RETURNTYPE, typename... Args>
RETURNTYPE JavaClass::callStatic(const std::string& method, Args... args)
{
    JNIEnv* env = getJNIEnv();
    std::string signature = makeSignature<RETURNTYPE, Args...>();
    JavaStaticMethodCaller<RETURNTYPE, Args...>::callFunction(env, *this, method, signature, args...);
}


template<class RETURNTYPE>
RETURNTYPE JavaClass::callStatic(const std::string& method)
{
    JNIEnv* env = getJNIEnv();
    std::string signature = makeSignature<RETURNTYPE, void>();
    JavaStaticMethodCaller<RETURNTYPE>::callFunction(env, *this, method, signature);
}


template<typename... Args>
class JavaStaticMethodCaller<jobject, Args...>
{
public:
    static jobject callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env, javaClass.classInstance, methodStr.c_str(), signature.c_str());
        return env->CallStaticObjectMethod(javaClass.classInstance, method, args...);
    }
};

template<typename... Args>
class JavaStaticMethodCaller<jfloat, Args...>
{
public:
    static jfloat callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env, javaClass.classInstance, methodStr.c_str(), signature.c_str());
        return env->CallStaticFloatMethod(javaClass.classInstance, method, args...);
    }
};

template<typename... Args>
class JavaStaticMethodCaller<jint, Args...>
{
public:
    static jint callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env,javaClass.classInstance, methodStr.c_str(), signature.c_str());
        return env->CallStaticIntMethod(javaClass.classInstance, method, args...);
    }
};


template<typename... Args>
class JavaStaticMethodCaller<jboolean, Args...>
{
public:
    static jboolean callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env, javaClass.classInstance, methodStr.c_str(), signature.c_str());
        return env->CallStaticBooleanMethod(javaClass.classInstance, method, args...);
    }
};

template<typename... Args>
class JavaStaticMethodCaller<jbyte, Args...>
{
public:
    static jbyte callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env, javaClass.classInstance, methodStr.c_str(), signature.c_str());
        return env->CallStaticByteMethod(javaClass.classInstance, method, args...);
    }
};


template<typename... Args>
class JavaStaticMethodCaller<jchar, Args...>
{
public:
    static jchar callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env, javaClass.classInstance, methodStr.c_str(), signature.c_str());
        return env->CallStaticCharMethod(javaClass.classInstance, method, args...);
    }
};


template<typename... Args>
class JavaStaticMethodCaller<jshort, Args...>
{
public:
    static jshort callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env, javaClass.classInstance, methodStr.c_str(), signature.c_str());
        return env->CallStaticShortMethod(javaClass.classInstance, method, args...);
    }
};


template<typename... Args>
class JavaStaticMethodCaller<jlong, Args...>
{
public:
    static jlong callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env, javaClass.classInstance, methodStr.c_str(), signature.c_str());
        return env->CallStaticLongMethod(javaClass.classInstance, method, args...);
    }
};


template<typename... Args>
class JavaStaticMethodCaller<jdouble, Args...>
{
public:
    static jdouble callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env, javaClass.classInstance, methodStr.c_str(), signature.c_str());
        return env->CallStaticDoubleMethod(javaClass.classInstance, method, args...);
    }
};


template<typename... Args>
class JavaStaticMethodCaller<void, Args...>
{
public:
    static void callFunction(JNIEnv* env, const JavaClass& javaClass, const std::string& methodStr, const std::string& signature, Args... args)
    {
        jmethodID method = getStaticMethodID(env, javaClass.classInstance, methodStr.c_str(), signature.c_str());
        env->CallStaticVoidMethod(javaClass.classInstance, method, args...);
    }
};

template <>
inline int JavaClass::getStaticField<jint>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jint>().c_str());
    return env->GetStaticIntField(classInstance, fieldID);
}


template <>
inline jboolean JavaClass::getStaticField<jboolean>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jboolean>().c_str());
    return env->GetStaticBooleanField(classInstance, fieldID);
}


template <>
inline jbyte JavaClass::getStaticField<jbyte>(const char* name)
{

    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jbyte>().c_str());
    return env->GetStaticByteField(classInstance, fieldID);
}


template <>
inline jchar JavaClass::getStaticField<jchar>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jchar>().c_str());
    return env->GetStaticCharField(classInstance, fieldID);
}


template <>
inline jshort JavaClass::getStaticField<jshort>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jshort>().c_str());
    return env->GetStaticShortField(classInstance, fieldID);
}


template <>
inline jlong JavaClass::getStaticField<jlong>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jlong>().c_str());
    return env->GetStaticLongField(classInstance, fieldID);
}


template <>
inline jfloat JavaClass::getStaticField<jfloat>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jfloat>().c_str());
    return env->GetStaticFloatField(classInstance, fieldID);
}


template <>
inline jdouble JavaClass::getStaticField<jdouble>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jdouble>().c_str());
    return env->GetStaticDoubleField(classInstance, fieldID);
}


template<>
inline void JavaClass::setStaticField<jint>(const char* name, jint field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jint>().c_str());
    env->SetStaticIntField(classInstance, fieldID, field);
}

template<>
inline void JavaClass::setStaticField<jboolean>(const char* name, jboolean field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jboolean>().c_str());
    env->SetStaticBooleanField(classInstance, fieldID, field);
}

template<>
inline void JavaClass::setStaticField<jbyte>(const char* name, jbyte field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jbyte>().c_str());
    env->SetStaticByteField(classInstance, fieldID, field);
}

template<>
inline void JavaClass::setStaticField<jchar>(const char* name, jchar field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jchar>().c_str());
    env->SetStaticCharField(classInstance, fieldID, field);
}

template<>
inline void JavaClass::setStaticField<jshort>(const char* name, jshort field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jshort>().c_str());
    env->SetStaticShortField(classInstance, fieldID, field);
}

template<>
inline void JavaClass::setStaticField<jlong>(const char* name, jlong field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jlong>().c_str());
    env->SetStaticLongField(classInstance, fieldID, field);
}

template<>
inline void JavaClass::setStaticField<jfloat>(const char* name, jfloat field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jfloat>().c_str());
    env->SetStaticFloatField(classInstance, fieldID, field);
}

template<>
inline void JavaClass::setStaticField<jdouble>(const char* name, jdouble field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getStaticFieldID(env, classInstance, name, compute_signature_string<jdouble>().c_str());
    env->SetStaticDoubleField(classInstance, fieldID, field);
}

/* getter and setter for instance fields */

template <>
inline int JavaObject::getField<jint>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jint>().c_str());
    return env->GetIntField(obj, fieldID);
}


template <>
inline jboolean JavaObject::getField<jboolean>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jboolean>().c_str());
    return env->GetBooleanField(obj, fieldID);
}


template <>
inline jbyte JavaObject::getField<jbyte>(const char* name)
{

    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jbyte>().c_str());
    return env->GetByteField(obj, fieldID);
}


template <>
inline jchar JavaObject::getField<jchar>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jchar>().c_str());
    return env->GetCharField(obj, fieldID);
}


template <>
inline jshort JavaObject::getField<jshort>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jshort>().c_str());
    return env->GetShortField(obj, fieldID);
}


template <>
inline jlong JavaObject::getField<jlong>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jlong>().c_str());
    return env->GetLongField(obj, fieldID);
}


template <>
inline jfloat JavaObject::getField<jfloat>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jfloat>().c_str());
    return env->GetFloatField(obj, fieldID);
}


template <>
inline jdouble JavaObject::getField<jdouble>(const char* name)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jdouble>().c_str());
    return env->GetDoubleField(obj, fieldID);
}


template<>
inline void JavaObject::setField<jint>(const char* name, jint field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jint>().c_str());
    env->SetIntField(obj, fieldID, field);
}

template<>
inline void JavaObject::setField<jboolean>(const char* name, jboolean field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jboolean>().c_str());
    env->SetBooleanField(obj, fieldID, field);
}

template<>
inline void JavaObject::setField<jbyte>(const char* name, jbyte field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jbyte>().c_str());
    env->SetByteField(obj, fieldID, field);
}

template<>
inline void JavaObject::setField<jchar>(const char* name, jchar field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jchar>().c_str());
    env->SetCharField(obj, fieldID, field);
}

template<>
inline void JavaObject::setField<jshort>(const char* name, jshort field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jshort>().c_str());
    env->SetShortField(obj, fieldID, field);
}

template<>
inline void JavaObject::setField<jlong>(const char* name, jlong field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jlong>().c_str());
    env->SetLongField(obj, fieldID, field);
}

template<>
inline void JavaObject::setField<jfloat>(const char* name, jfloat field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jfloat>().c_str());
    env->SetFloatField(obj, fieldID, field);
}

template<>
inline void JavaObject::setField<jdouble>(const char* name, jdouble field)
{
    JNIEnv* env = getJNIEnv();
    jfieldID fieldID = getFieldID(env, javaClass.classInstance, name, compute_signature_string<jdouble>().c_str());
    env->SetDoubleField(obj, fieldID, field);
}

template<typename... Args>
JavaObject JavaClass::construct(Args... args)
{
    JNIEnv* env = getJNIEnv();
    std::string ctorSignature = makeSignature<void, Args...>();
    jmethodID ctorMethod = env->GetMethodID(classInstance, "<init>", ctorSignature.c_str());
    jobject localRef = env->NewObject(classInstance, ctorMethod, args...);
    jobject globalRef = env->NewGlobalRef(localRef);
    return JavaObject(globalRef);
}
