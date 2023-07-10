#pragma once

#include <cstring>
#include <string>
#include <deque>
#include <vector>
#include <memory>

#include "Engine/Graphics/Texture_MM7.h"
#include "Engine/Graphics/Sprites.h"

#include "Library/Image/Image.h"

#include "Utility/Memory/Blob.h"

class Sprite;
class LodReader;

namespace LOD {
#pragma pack(push, 1)
struct FileHeader {
    inline FileHeader() {
        memset(pSignature, 0, 4);
        memset(LodVersion, 0, 80);
        memset(LodDescription, 0, 80);
        memset(array_0000B0, 0, 80);

        LODSize = 0;
        dword_0000A8 = 0;
        uNumIndices = 0;
    }

    char pSignature[4];
    char LodVersion[80];
    char LodDescription[80];
    uint32_t LODSize;
    uint32_t dword_0000A8;
    uint32_t uNumIndices;
    char array_0000B0[80];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Directory {
    Directory();

    char pFilename[15];
    char field_F;
    uint32_t uOfsetFromSubindicesStart;
    uint32_t uDataSize;
    uint32_t dword_000018;
    uint16_t uNumSubIndices;
    uint16_t priority;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CompressedHeader {
    uint32_t uVersion;
    char pMagic[4];
    uint32_t uCompressedSize;
    uint32_t uDecompressedSize;
};
static_assert(sizeof(CompressedHeader) == 16);
#pragma pack(pop)

class File {
 public:
    File();
    virtual ~File();

    bool Open(const std::string &pFilename);
    void Close();

    Blob LoadRaw(const std::string &pContainer) const;
    Blob LoadCompressedTexture(const std::string &pContainer);
    Blob LoadCompressed(const std::string &pContainer);
    bool DoesContainerExist(const std::string &filename);

    std::string GetSubNodeName(size_t index) const { return pSubIndices[index].pFilename; }
    size_t GetSubNodesCount() const { return uNumSubDirs; }
    int GetSubNodeIndex(const std::string &name) const;

 protected:
    FILE *FindContainer(const std::string &filename, size_t *data_size = nullptr) const;
    virtual bool OpenFile(const std::string &filePath);
    bool LoadHeader();
    bool LoadSubIndices(const std::string &sFolder);
    virtual void ResetSubIndices();

 protected:
    FILE *pFile;
    std::string pLODPath;
    bool isFileOpened;

    struct FileHeader header;

    std::vector<Directory> pRoot;
    std::string pContainerName;
    unsigned int uOffsetToSubIndex;

    unsigned int uNumSubDirs;
    struct Directory *pSubIndices;
};

class WriteableFile : public File {
 public:
    WriteableFile();
    bool LoadFile(const std::string &filePath, bool bWriting);
    unsigned int Write(const std::string &file_name, const void *pDirData, size_t size, int a4);
    unsigned int Write(const std::string &file_name, const Blob &data);
    void CloseWriteFile();
    int CreateTempFile();
    bool FixDirectoryOffsets();
    bool _4621A7();
    int CreateNewLod(LOD::FileHeader *pHeader, const std::string &root_name, const std::string &Source);

    void AllocSubIndicesAndIO(unsigned int uNumSubIndices, unsigned int uBufferSize);
    void FreeSubIndexAndIO();
    bool AppendDirectory(const std::string &file_name, const void *pData, size_t data_size);

    void ClearSubNodes() { uNumSubDirs = 0; }

 protected:
    virtual void ResetSubIndices();

 protected:
    uint8_t * pIOBuffer;
    unsigned int uIOBufferSize;
    FILE *pOutputFileHandle;
    unsigned int uLODDataSize;
};
};  // namespace LOD

class LODFile_IconsBitmaps {
 public:
    LODFile_IconsBitmaps();
    ~LODFile_IconsBitmaps();

    bool open(const std::string &pFilename, const std::string &pFolderName);

    void releaseUnreserved();
    void reserveLoadedTextures();

    Texture_MM7 *loadTexture(const std::string &pContainer, bool useDummyOnError = true);

    Blob LoadCompressedTexture(const std::string &pContainer); // TODO(captainurist): doesn't belong here.

 private:
    int LoadTextureFromLOD(struct Texture_MM7 *pOutTex, const std::string &pContainer);

 private:
    std::unique_ptr<LodReader> _reader;
    int _reservedCount = 0;
    std::deque<Texture_MM7> _textures;
};

#pragma pack(push, 1)
struct LODSpriteHeader {
    inline LODSpriteHeader() {
        uHeight = 0;
        uPaletteId = 0;
        word_1A = 0;
    }

    char pName[12] {};         // 0
    uint32_t uSpriteSize = 0;        // C
    uint16_t uWidth = 0;         // 10  SW width (as opposed to Sprite::BufferWidth)
    uint16_t uHeight = 0;        // 12  SW height
    uint16_t uPaletteId = 0;     // 14
    uint16_t word_16 = 0;        // 16
    uint16_t uTexturePitch = 0;  // 18
    uint16_t word_1A = 0;        // 1a  flags - 1024 delete bitmap
    uint32_t uDecompressedSize = 0;  // 1c
};
#pragma pack(pop)

struct LODSprite : public LODSpriteHeader {
    void Release();

    GrayscaleImage bitmap;
};

class LODFile_Sprites {
 public:
    LODFile_Sprites();
    ~LODFile_Sprites();

    bool open(const std::string &pFilename, const std::string &folder);

    void releaseUnreserved();
    void reserveLoadedSprites();

    Sprite *loadSprite(const std::string &pContainerName);

 private:
    bool LoadSpriteFromFile(LODSprite *pSpriteHeader, const std::string &pContainer);

 private:
    std::unique_ptr<LodReader> _reader;
    int _reservedCount = 0;
    std::deque<Sprite> _sprites;
};

extern LODFile_IconsBitmaps *pIcons_LOD;
extern LODFile_IconsBitmaps *pIcons_LOD_mm6;
extern LODFile_IconsBitmaps *pIcons_LOD_mm8;

extern LODFile_IconsBitmaps *pBitmaps_LOD;
extern LODFile_IconsBitmaps *pBitmaps_LOD_mm6;
extern LODFile_IconsBitmaps *pBitmaps_LOD_mm8;

extern LODFile_Sprites *pSprites_LOD;
extern LODFile_Sprites *pSprites_LOD_mm6;
extern LODFile_Sprites *pSprites_LOD_mm8;

extern LOD::WriteableFile *pSave_LOD;
extern LOD::File *pGames_LOD;
