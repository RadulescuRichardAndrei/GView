#include "png.hpp"

using namespace GView::Type::PNG;

PNGFile::PNGFile()
{
}

bool PNGFile::Update()
{
    memset(&header, 0, sizeof(header));
    auto& data = this->obj->GetData();

    uint32 signature;
    CHECK(data.Copy<uint32>(0, signature), false, "");
    CHECK(signature == PNG_SIGNATURE, false, "Invalid PNG signature");

    // Find and parse IHDR chunk
    uint64 offset = 8; // Skip signature
    uint32 length, type;
    CHECK(data.Copy<uint32>(offset, length), false, "");
    CHECK(data.Copy<uint32>(offset + 4, type), false, "");
    length = Endian::BigToNative(length);
    
    CHECK(type == PNG_CHUNK_TYPE_IHDR, false, "IHDR chunk not found");
    CHECK(data.Copy<Header>(offset + 8, header), false, "");

    // Convert endianness
    header.width = Endian::BigToNative(header.width);
    header.height = Endian::BigToNative(header.height);

    return true;
}

bool PNGFile::LoadImageToObject(Image& img, uint32 index)
{
    Buffer buf;
    auto bf = obj->GetData().GetEntireFile();
    if (!bf.IsValid()) {
        buf = this->obj->GetData().CopyEntireFile();
        CHECK(buf.IsValid(), false, "Failed to copy entire file");
        bf = (BufferView)buf;
    }
    CHECK(img.Create(bf), false, "");
    return true;
}