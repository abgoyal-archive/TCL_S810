

#ifndef DALVIK_DVMDEX_H_
#define DALVIK_DVMDEX_H_

#include "libdex/DexFile.h"

/* extern */
struct ClassObject;
struct HashTable;
struct InstField;
struct Method;
struct StringObject;


struct DvmDex {
    /* pointer to the DexFile we're associated with */
    DexFile*            pDexFile;

    /* clone of pDexFile->pHeader (it's used frequently enough) */
    const DexHeader*    pHeader;

    /* interned strings; parallel to "stringIds" */
    struct StringObject** pResStrings;

    /* resolved classes; parallel to "typeIds" */
    struct ClassObject** pResClasses;

    /* resolved methods; parallel to "methodIds" */
    struct Method**     pResMethods;

    /* resolved instance fields; parallel to "fieldIds" */
    /* (this holds both InstField and StaticField) */
    struct Field**      pResFields;

    /* interface method lookup cache */
    struct AtomicCache* pInterfaceCache;

    /* shared memory region with file contents */
    bool                isMappedReadOnly;
    MemMapping          memMap;

    /* lock ensuring mutual exclusion during updates */
    pthread_mutex_t     modLock;
};


int dvmDexFileOpenFromFd(int fd, DvmDex** ppDvmDex);

int dvmDexFileOpenPartial(const void* addr, int len, DvmDex** ppDvmDex);

void dvmDexFileFree(DvmDex* pDvmDex);


bool dvmDexChangeDex1(DvmDex* pDvmDex, u1* addr, u1 newVal);
bool dvmDexChangeDex2(DvmDex* pDvmDex, u2* addr, u2 newVal);


INLINE struct StringObject* dvmDexGetResolvedString(const DvmDex* pDvmDex,
    u4 stringIdx)
{
    assert(stringIdx < pDvmDex->pHeader->stringIdsSize);
    return pDvmDex->pResStrings[stringIdx];
}
INLINE struct ClassObject* dvmDexGetResolvedClass(const DvmDex* pDvmDex,
    u4 classIdx)
{
    assert(classIdx < pDvmDex->pHeader->typeIdsSize);
    return pDvmDex->pResClasses[classIdx];
}
INLINE struct Method* dvmDexGetResolvedMethod(const DvmDex* pDvmDex,
    u4 methodIdx)
{
    assert(methodIdx < pDvmDex->pHeader->methodIdsSize);
    return pDvmDex->pResMethods[methodIdx];
}
INLINE struct Field* dvmDexGetResolvedField(const DvmDex* pDvmDex,
    u4 fieldIdx)
{
    assert(fieldIdx < pDvmDex->pHeader->fieldIdsSize);
    return pDvmDex->pResFields[fieldIdx];
}

INLINE void dvmDexSetResolvedString(DvmDex* pDvmDex, u4 stringIdx,
    struct StringObject* str)
{
    assert(stringIdx < pDvmDex->pHeader->stringIdsSize);
    pDvmDex->pResStrings[stringIdx] = str;
}
INLINE void dvmDexSetResolvedClass(DvmDex* pDvmDex, u4 classIdx,
    struct ClassObject* clazz)
{
    assert(classIdx < pDvmDex->pHeader->typeIdsSize);
    pDvmDex->pResClasses[classIdx] = clazz;
}
INLINE void dvmDexSetResolvedMethod(DvmDex* pDvmDex, u4 methodIdx,
    struct Method* method)
{
    assert(methodIdx < pDvmDex->pHeader->methodIdsSize);
    pDvmDex->pResMethods[methodIdx] = method;
}
INLINE void dvmDexSetResolvedField(DvmDex* pDvmDex, u4 fieldIdx,
    struct Field* field)
{
    assert(fieldIdx < pDvmDex->pHeader->fieldIdsSize);
    pDvmDex->pResFields[fieldIdx] = field;
}

#endif  // DALVIK_DVMDEX_H_
