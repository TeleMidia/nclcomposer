#include "mediapreviewfactory.h"

MediaPreviewFactory::MediaPreviewFactory()
{
}

MediaPreviewFactory::~MediaPreviewFactory()
{

}

// This is very important, and will allow to Qt framework recognize your
// plugin
Q_EXPORT_PLUGIN2(MediaPreview, MediaPreviewFactory)
