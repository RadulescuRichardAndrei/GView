#include "png.hpp"

using namespace GView::Type::PNG;


PNGFile::PNGFile():selectionZoneInterface(nullptr) {}

bool PNGFile::Update()
{
    memset(&header, 0, sizeof(header));
    ihdrChunk = {};
    chunks.clear();

    auto& data = this->obj->GetData();

    CHECK(data.Copy<Header>(0, header), false, "Failed to copy PNG header");
    CHECK(header.magic == PNG_SIGNATURE, false, "Invalid PNG magic number");

    uint64 offset = sizeof(Header);
    while (offset < data.GetSize()) {
        Chunk chunk;
        CHECK(data.Copy<uint32>(offset, chunk.length), false, "Failed to copy chunk length");
        chunk.length = Endian::BigToNative(chunk.length);

        CHECK(data.Copy<uint32>(offset + 4, chunk.type), false, "Failed to copy chunk type");

        if (chunk.length > 0) {
            chunk.data.resize(chunk.length);
            CHECK(data.Copy(offset + 8, chunk.data), false, "Failed to copy chunk data");
        }

        CHECK(data.Copy<uint32>(offset + 8 + chunk.length, chunk.crc), false, "Failed to copy chunk CRC");

        if (chunk.IsType(CHUNK_TYPE_IHDR)) {
            CHECK(chunk.data.size() >= sizeof(IHDRChunk), false, "Invalid IHDR chunk size");
            memcpy(&ihdrChunk, chunk.data.data(), sizeof(IHDRChunk));
            ihdrChunk.width  = Endian::BigToNative(ihdrChunk.width);
            ihdrChunk.height = Endian::BigToNative(ihdrChunk.height);
        }

        chunks.push_back(chunk);

        if (chunk.IsType(CHUNK_TYPE_IEND)) {
            break;
        }

        offset += 12 + chunk.length;
    }

    bool hasIHDR = std::any_of(chunks.begin(), chunks.end(), [](const Chunk& c) { return c.IsType(CHUNK_TYPE_IHDR); });
    bool hasIEND = std::any_of(chunks.begin(), chunks.end(), [](const Chunk& c) { return c.IsType(CHUNK_TYPE_IEND); });
    CHECK(hasIHDR && hasIEND, false, "Missing IHDR or IEND chunk");

    return true;
}

bool PNGFile::LoadImageToObject(Image& img, uint32 index)
{
    Buffer buf;
    auto bf = obj->GetData().GetEntireFile();
    if (!bf.IsValid()) {
        buf = this->obj->GetData().CopyEntireFile();
        CHECK(buf.IsValid(), false, "Failed to copy the entire file");
        bf = (BufferView) buf;
    }

    CHECK(img.Create(bf), false, "Failed to create image object from PNG data");

    return true;
}