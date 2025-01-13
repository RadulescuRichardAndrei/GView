#pragma once

#include "GView.hpp"

namespace GView
{
namespace Type
{
namespace PNG
{
#pragma pack(push, 2)

    constexpr uint32 PNG_SIGNATURE = 0x89504E47; // PNG signature
    constexpr uint32 PNG_CHUNK_TYPE_IHDR = 0x49484452; // IHDR chunk type
    constexpr uint32 PNG_CHUNK_TYPE_IDAT = 0x49444154; // IDAT chunk type
    constexpr uint32 PNG_CHUNK_TYPE_IEND = 0x49454E44; // IEND chunk type

    struct Header {
        uint32 width;
        uint32 height;
        uint8 bitDepth;
        uint8 colorType;
        uint8 compressionMethod;
        uint8 filterMethod;
        uint8 interlaceMethod;
    };

    #pragma pack(pop)

    class PNGFile : public TypeInterface, public View::ImageViewer::LoadImageInterface
    {
    public:
        Header header{};

        Reference<GView::Utils::SelectionZoneInterface> selectionZoneInterface;

    public:
        PNGFile();
        virtual ~PNGFile() {}

        bool Update();

        std::string_view GetTypeName() override
        {
            return "PNG";
        }
        void RunCommand(std::string_view) override {}

        bool LoadImageToObject(Image& img, uint32 index) override;

        uint32 GetSelectionZonesCount() override
        {
            CHECK(selectionZoneInterface.IsValid(), 0, "");
            return selectionZoneInterface->GetSelectionZonesCount();
        }

        TypeInterface::SelectionZone GetSelectionZone(uint32 index) override
        {
            static auto d = TypeInterface::SelectionZone{ 0, 0 };
            CHECK(selectionZoneInterface.IsValid(), d, "");
            CHECK(index < selectionZoneInterface->GetSelectionZonesCount(), d, "");

            return selectionZoneInterface->GetSelectionZone(index);
        }

        bool UpdateKeys(KeyboardControlsInterface* interface) override
        {
            return true;
        }
    };

    namespace Panels
    {
        class Information : public AppCUI::Controls::TabPage
        {
            Reference<GView::Type::PNG::PNGFile> png;
            Reference<AppCUI::Controls::ListView> general;
            Reference<AppCUI::Controls::ListView> issues;

            void UpdateGeneralInformation();
            void UpdateIssues();
            void RecomputePanelsPositions();

        public:
            Information(Reference<GView::Type::PNG::PNGFile> png);

            void Update();
            virtual void OnAfterResize(int newWidth, int newHeight) override
            {
                RecomputePanelsPositions();
            }
        };
    }; // namespace Panels
} // namespace PNG
} // namespace Type
} // namespace GView
