#include "png.hpp"

using namespace GView::Type::PNG;

PNGFile::PNGFile()
{
}

bool PNGFile::Update()
{
    // Clear the header and chunks
    memset(&header, 0, sizeof(header));
    ihdrChunk = {};
    chunks.clear();

    auto& data = this->obj->GetData();

    // Check the PNG signature
    CHECK(data.Copy<Header>(0, header), false, "Failed to copy PNG header");
    CHECK(header.magic == PNG_MAGIC, false, "Invalid PNG magic number");

    // Start parsing chunks after the header
    uint64 offset = sizeof(Header);
    while (offset < data.GetSize())
    {
        Chunk chunk;
        // Read chunk length and type
        CHECK(data.Copy<uint32>(offset, chunk.length), false, "Failed to copy chunk length");
        chunk.length = Endian::BigToNative(chunk.length);

        CHECK(data.Copy<uint32>(offset + 4, chunk.type), false, "Failed to copy chunk type");

        // Read chunk data
        if (chunk.length > 0)
        {
            chunk.data.resize(chunk.length);
            CHECK(data.Copy(offset + 8, chunk.data.data(), chunk.length), false, "Failed to copy chunk data");
        }

        // Read CRC
        CHECK(data.Copy<uint32>(offset + 8 + chunk.length, chunk.crc), false, "Failed to copy chunk CRC");

        // Check for IHDR chunk
        if (chunk.IsType(CHUNK_TYPE_IHDR))
        {
            CHECK(chunk.data.size() >= sizeof(IHDRChunk), false, "Invalid IHDR chunk size");
            memcpy(&ihdrChunk, chunk.data.data(), sizeof(IHDRChunk));
            ihdrChunk.width = Endian::BigToNative(ihdrChunk.width);
            ihdrChunk.height = Endian::BigToNative(ihdrChunk.height);
        }

        // Add the chunk to the list
        chunks.push_back(chunk);

        // If it's the IEND chunk, stop parsing
        if (chunk.IsType(CHUNK_TYPE_IEND))
        {
            break;
        }

        // Move to the next chunk (length + type + data + CRC)
        offset += 12 + chunk.length;
    }

    // Ensure at least one IHDR and one IEND chunk are present
    bool hasIHDR = std::any_of(chunks.begin(), chunks.end(), [](const Chunk& c) { return c.IsType(CHUNK_TYPE_IHDR); });
    bool hasIEND = std::any_of(chunks.begin(), chunks.end(), [](const Chunk& c) { return c.IsType(CHUNK_TYPE_IEND); });
    CHECK(hasIHDR && hasIEND, false, "Missing IHDR or IEND chunk");

    return true;
}

bool PNGFile::LoadImageToObject(Image& img, uint32 index)
{
    Buffer buf;
    auto bf = obj->GetData().GetEntireFile();
    if (!bf.IsValid())
    {
        buf = this->obj->GetData().CopyEntireFile();
        CHECK(buf.IsValid(), false, "Failed to copy the entire file");
        bf = (BufferView)buf;
    }

    // Create the image object using the buffer
    CHECK(img.Create(bf), false, "Failed to create image object from PNG data");

    return true;
}
