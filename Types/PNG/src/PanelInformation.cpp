#include "png.hpp"

using namespace GView::Type::PNG;
using namespace AppCUI::Controls;

Panels::Information::Information(Reference<GView::Type::PNG::PNGFile> _png) : TabPage("&Information")
{
    png     = _png;
    general = Factory::ListView::Create(this, "x:0,y:0,w:100%,h:10", { "n:Field,w:20", "n:Value,w:100" }, ListViewFlags::None);
    issues  = Factory::ListView::Create(this, "x:0,y:21,w:100%,h:10", { "n:Info,w:200" }, ListViewFlags::HideColumns);

    this->Update();
}

void Panels::Information::UpdateGeneralInformation()
{
    LocalString<256> tempStr;
    NumericFormatter n;

    general->DeleteAllItems();
    general->AddItem("File");

    // File size
    general->AddItem({ "Size", tempStr.Format("%s bytes", n.ToString(png->obj->GetData().GetSize(), { NumericFormatFlags::None, 10, 3, ',' }).data()) });

    // Image dimensions
    general->AddItem({ "Width", tempStr.Format("%u", png->ihdrChunk.width) });
    general->AddItem({ "Height", tempStr.Format("%u", png->ihdrChunk.height) });

    // Bit depth and color type
    general->AddItem({ "Bit Depth", tempStr.Format("%u", png->ihdrChunk.bitDepth) });
    general->AddItem({ "Color Type", tempStr.Format("%u", png->ihdrChunk.colorType) });

    // Compression, filter, and interlace methods
    general->AddItem({ "Compression Method", tempStr.Format("%u", png->ihdrChunk.compressionMethod) });
    general->AddItem({ "Filter Method", tempStr.Format("%u", png->ihdrChunk.filterMethod) });
    general->AddItem({ "Interlace Method", tempStr.Format("%u", png->ihdrChunk.interlaceMethod) });
}

void Panels::Information::UpdateIssues()
{
    issues->DeleteAllItems();

    // Example issue detection: Missing IHDR or IEND chunk
    bool hasIHDR = std::any_of(png->chunks.begin(), png->chunks.end(), [](const Chunk& c) { return c.IsType(CHUNK_TYPE_IHDR); });
    bool hasIEND = std::any_of(png->chunks.begin(), png->chunks.end(), [](const Chunk& c) { return c.IsType(CHUNK_TYPE_IEND); });

    if (!hasIHDR)
    {
        issues->AddItem("Missing IHDR chunk.");
    }

    if (!hasIEND)
    {
        issues->AddItem("Missing IEND chunk.");
    }

    // Add additional checks for PNG validity or potential issues here
}

void Panels::Information::RecomputePanelsPositions()
{
    int w = this->GetWidth();
    int h = this->GetHeight();

    if (!general.IsValid() || !issues.IsValid())
    {
        return;
    }

    issues->SetVisible(false);
    this->general->Resize(w, h);
}

void Panels::Information::Update()
{
    UpdateGeneralInformation();
    UpdateIssues();
    RecomputePanelsPositions();
}
