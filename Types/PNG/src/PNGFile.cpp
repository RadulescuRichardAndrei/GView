#include "png.hpp"

using namespace GView::Type::PNG;


PNGFile::PNGFile():selectionZoneInterface(nullptr) {}

bool PNGFile::Update()
{
    /*
    if (this->obj->IsValid() == false)
        return false;
    return true;
    */

    return true;
}

bool PNGFile::LoadImageToObject(Image& img, uint32 index)
{
    /*
    this->obj = GView::Object::Create(path);
    if (!this->obj.IsValid())
        return false;
    
    if (!this->obj->GetData().IsValid())
        return false;

    return Validate();
    */
    return true;
}

//bool PNGFile::Validate()
//{
//     
//    if (!this->obj.IsValid() || this->obj->GetData().GetSize() < 8)
//        return false;
//    
//    auto signature = this->obj->GetData().GetObject<uint64>(0);
//    return *signature == PNG_SIGNATURE;
// 
//    return true;
//}


//bool PNGFile::GetImageInfo(GView::View::ImageViewer::ImageInfo& info)
//{
//    if (!Validate())
//        return false;
//
//    // Parse PNG header to get width, height, and bit depth
//    auto data = this->obj->GetData();
//    uint32 width = data.GetUInt32(16);
//    uint32 height = data.GetUInt32(20);
//    uint8 bitDepth = data.GetUInt8(24);
//    uint8 colorType = data.GetUInt8(25);
//
//    info.width = width;
//    info.height = height;
//    info.bitsPerPixel = bitDepth * (colorType == 2 ? 3 : (colorType == 6 ? 4 : 1));
//    return true;
//}
//
//bool PNGFile::GetImageData(Image& img)
//{
//    if (!Validate())
//        return false;
//
//    auto data = this->obj->GetData();
//    uint32 width = data.GetUInt32(16);
//    uint32 height = data.GetUInt32(20);
//    uint8 bitDepth = data.GetUInt8(24);
//    uint8 colorType = data.GetUInt8(25);
//    uint8 compressionMethod = data.GetUInt8(26);
//    uint8 filterMethod = data.GetUInt8(27);
//    uint8 interlaceMethod = data.GetUInt8(28);
//
//    if (compressionMethod != 0 || filterMethod != 0 || interlaceMethod != 0)
//        return false;
//
//    uint8 bytesPerPixel = (bitDepth / 8) * (colorType == 2 ? 3 : (colorType == 6 ? 4 : 1));
//
//    uint32 offset = 33; // Start after IHDR chunk
//    uint32 idatOffset = 0;
//    uint32 idatSize = 0;
//    while (offset < data.GetSize() - 12)
//    {
//        uint32 chunkSize = data.GetUInt32(offset);
//        uint32 chunkType = data.GetUInt32(offset + 4);
//        if (chunkType == 0x54414449) // "IDAT"
//        {
//            idatOffset = offset + 8;
//            idatSize = chunkSize;
//            break;
//        }
//        offset += chunkSize + 12;
//    }
//
//    if (idatOffset == 0 || idatSize == 0)
//        return false;
//
//    img.Create(width, height, bytesPerPixel * 8);
//
//    // Decompress IDAT chunk (assumes no compression)
//    uint32 rowSize = width * bytesPerPixel + 1; // +1 for filter type byte
//    uint32 totalSize = rowSize * height;
//    if (totalSize > idatSize)
//        return false;
//
//    uint8* imgData = img.GetData();
//    const uint8* srcData = data.GetData() + idatOffset;
//
//    for (uint32 y = 0; y < height; y++)
//    {
//        uint8 filterType = *srcData++;
//        uint8* row = imgData + y * width * bytesPerPixel;
//
//        switch (filterType)
//        {
//        case 0:
//            memcpy(row, srcData, width * bytesPerPixel);
//            break;
//        case 1:
//            for (uint32 x = 0; x < width * bytesPerPixel; x++)
//            {
//                uint8 left = x < bytesPerPixel ? 0 : row[x - bytesPerPixel];
//                row[x] = srcData[x] + left;
//            }
//            break;
//        default:
//            return false;
//        }
//
//        srcData += width * bytesPerPixel;
//    }
//
//    return true;
// }
