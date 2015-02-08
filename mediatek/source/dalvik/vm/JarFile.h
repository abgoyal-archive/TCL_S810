
#ifndef DALVIK_JARFILE_H_
#define DALVIK_JARFILE_H_

struct JarFile {
    ZipArchive  archive;
    //MemMapping  map;
    char*       cacheFileName;
    DvmDex*     pDvmDex;
};

int dvmJarFileOpen(const char* fileName, const char* odexOutputName,
    JarFile** ppJarFile, bool isBootstrap);

void dvmJarFileFree(JarFile* pJarFile);

/* pry the DexFile out of a JarFile */
INLINE DvmDex* dvmGetJarFileDex(JarFile* pJarFile) {
    return pJarFile->pDvmDex;
}

/* get full path of optimized DEX file */
INLINE const char* dvmGetJarFileCacheFileName(JarFile* pJarFile) {
    return pJarFile->cacheFileName;
}

enum DexCacheStatus {
    DEX_CACHE_ERROR = -2,
    DEX_CACHE_BAD_ARCHIVE = -1,
    DEX_CACHE_OK = 0,
    DEX_CACHE_STALE,
    DEX_CACHE_STALE_ODEX,
};

DexCacheStatus dvmDexCacheStatus(const char *fileName);

#endif  // DALVIK_JARFILE_H_
