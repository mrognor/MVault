#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>

// BIG_FILE_SIZE must be greater then FILE_BLOCK_SIZE
#define FILE_BLOCK_SIZE 4096
#define BIG_FILE_SIZE 10485760

namespace mvlt
{
    /**
        \defgroup CsvParserFunctions Csv parser functions
        \brief All functions for working with MVault and csv

        @{
    */

    /**
        \brief A class for reading files as a stream

        Small files will be fully loaded into RAM. A large file is considered to be a file larger than BIG_FILE_SIZE.
        Large files will be read in chunks, the size of FILE_BLOCK_SIZE. At the same time,
        there will be no difference in interaction for the user of the class.
    */
    class StreamFileReader
    {
    private:
        // Input file variable
        std::ifstream File;

        // Input file size
        std::streamsize FileSize = 0;

        // Position in file
        std::streamsize FilePos = 0;

        // Position if file chunk
        std::uint16_t ChunkPos = 1;

        // Current char
        char Char = 0;

        // Next char
        char NextChar = 0;

        // File chunk
        char DataChunk[FILE_BLOCK_SIZE] = {0};

        // Pointer to store little files
        char* Data = nullptr;

    public:
        /// \brief Default constructor
        StreamFileReader() noexcept = default;

        /**
            \brief Copy constructor

            \param [in] other object to copy
        */
        StreamFileReader(const StreamFileReader& other) noexcept = delete;

        /**
            \brief Move constructor

            \param [in] other object to move
        */
        StreamFileReader(StreamFileReader&& other) noexcept = delete;

        /**
            \brief Constructor

            \param [in] fileName file to open
        */
        explicit StreamFileReader(const std::string& fileName) noexcept;

        /**
            \brief Assignment operator

            \param [in] other object to copy

            \return new object with data from other
        */
        StreamFileReader& operator=(const StreamFileReader& other) noexcept = delete;

        /**
            \brief Move assignment operator

            \param [in] other object to move

            \return new object with data from other
        */
        StreamFileReader& operator=(StreamFileReader&& other) noexcept = delete;

        /**
            \brief Method for opening the file

            \param [in] fileName file to open

            \return true if the file was opened, otherwise returns false
        */
        bool Open(const std::string& fileName) noexcept;

        /**
            \brief Method for checking whether the file has been opened

            \return true if the file was opened, otherwise returns false
        */
        bool IsOpen() const noexcept;

        /**
            \brief A method for checking if a file has been read

            \return true if the file has been read, otherwise returns false
        */
        bool IsEnd() const noexcept;

        /**
            \brief A method for get file size

            \return file size
        */
        std::size_t GetFileSize() const noexcept;

        /**
            \brief A method for get current reading position

            \return current reading position
        */
        std::size_t GetCurrentPos() const noexcept;

        /**
            \brief Method for getting the current character

            \return current char
        */
        char GetChar() const noexcept;

        /**
            \brief Method for getting the next character

            \return next char
        */
        char GetNextChar() const noexcept;

        /**
            \brief A method for moving through a file

            \return true if the next character exists, otherwise false
        */
        bool Move() noexcept;

        /// \brief Method for closing a file
        void Close() noexcept;

        /// \brief Default destructor
        ~StreamFileReader() noexcept;
    };

    /**@} */
}