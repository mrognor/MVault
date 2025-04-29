#include "StreamFileReader.h"

namespace mvlt
{
    StreamFileReader::StreamFileReader() noexcept{}

    StreamFileReader::StreamFileReader(const std::string& fileName) noexcept
    {
        Open(fileName);
    }

    bool StreamFileReader::Open(const std::string& fileName) noexcept
    {
        File.open(fileName, std::ios::binary | std::ios::ate);
        if (!File.is_open()) return false;


        FileSize = File.tellg();
        File.seekg(0);

        if (FileSize > 0)
        {
            if (FileSize > BIG_FILE_SIZE)
            {
                File.read(DataChunk, FILE_BLOCK_SIZE);
                Char = DataChunk[0];
                NextChar = DataChunk[1];
            }
            else
            {
                Data = new char[FileSize];
                File.read(Data, FileSize);
                Char = Data[0];
                FilePos = 1;
                if (FileSize > 1)
                    NextChar = Data[1];
            }
        }
        
        return true; 
    }

    bool StreamFileReader::IsOpen() const noexcept
    {
        return File.is_open();
    }

    bool StreamFileReader::IsEnd() const noexcept
    {
        bool res = false;

        if (Data == nullptr)
        {
            if (FilePos + ChunkPos <= FileSize)
                res = true;
        }
        else
        {
            if (FilePos <= FileSize)
                res = true;
        }

        return res;
    }

    std::size_t StreamFileReader::GetFileSize() const noexcept
    {
        return FileSize;
    }

    std::size_t StreamFileReader::GetCurrentPos() const noexcept
    {
        if (FileSize == 0) return 0;
        else if (Data == nullptr) return FilePos + ChunkPos - 1;
        else return FilePos - 1;
    }

    char StreamFileReader::GetChar() const noexcept
    {
        return Char;
    }

    char StreamFileReader::GetNextChar() const noexcept
    {
        return NextChar;
    }

    bool StreamFileReader::Move() noexcept
    {
        bool res = false;

        if (Data == nullptr)
        {
            if (FilePos + ChunkPos <= FileSize)
            {
                ++ChunkPos;
                if (ChunkPos == FILE_BLOCK_SIZE)
                {
                    FilePos += FILE_BLOCK_SIZE;

                    if (FileSize - FilePos > FILE_BLOCK_SIZE)
                        File.read(DataChunk, FILE_BLOCK_SIZE);
                    else
                        File.read(DataChunk, FileSize - FilePos);

                    ChunkPos = 0;
                }

                Char = NextChar;
                if (FilePos + ChunkPos < FileSize) NextChar = DataChunk[ChunkPos];
                res = true;
            }
        }
        else
        {
            if (FilePos <= FileSize)
            {
                ++FilePos;
                Char = NextChar;
                if (FilePos < FileSize) NextChar = Data[FilePos];
                res = true;
            }
        }

        return res;
    }

    void StreamFileReader::Close() noexcept
    {
        File.close();
    }

    StreamFileReader::~StreamFileReader() noexcept
    {
        if (Data != nullptr) delete[] Data;
    }
}