#include <igGapCore.h>
#include <igCore/igCoreAll.h>
#include <igCore/igIGBFile.h>
#include <igCore/igRegistry.h>

#include <igGapAttrs.h>
#include <igAttrs/igTextureAttr.h>

#include <igGapGfx.h>
#include <igGfx/igImage.h>
#include <igGfx/igPngLoader.h>

#include <igGapSg.h>
#include <igSg/igSceneInfo.h>
#include <igSg/igAttrSet.h>

using namespace Gap;

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "Usage : %s <inputFile.jpg|bmp|png|tga|dds|igt> <outputFile.igb> [registryFile.ini]\n", argv[0]);
		return 1;
	}

	igAlchemy alchemy;
	igString inputFile = argv[1];
	igString outputFile = argv[2];

	Sg::igArkRegisterForIGBFiles();
	igImage::useDefaultFileTypes();
	igImage::addFileType("igt", igPngLoader::getClassMetaSafe());

	igImageRef image = igImage::instantiateRef();
	IG_GFX_TEXTURE_FILTER filter = IG_GFX_TEXTURE_FILTER_NEAREST;
	IG_GFX_TEXTURE_WRAP wrap = IG_GFX_TEXTURE_WRAP_REPEAT;

	if (!image->load(inputFile))
	{
		fprintf(stderr, "Failed to load image file %s\n", inputFile);
		return -1;
	}

	if (argc > 3)
	{
		igRegistryRef registry = igRegistry::instantiateRef();
		igString registryFile = argv[3];

		if (!registry->load(registryFile))
		{
			fprintf(stderr, "Failed to load registry file %s\n", registryFile);
			return -1;
		}

		igInt section = registry->findSection("image2igb", false);
		
		if (section < 0)
		{
			fprintf(stderr, "Could not find \"image2igb\" section in registry file %s\n", registryFile);
			return -3;
		}

		igBool linear, clamp;

		if (registry->getValue(section, "linear", linear) && linear)
			filter = IG_GFX_TEXTURE_FILTER_LINEAR;

		if (registry->getValue(section, "clamp", clamp) && clamp)
			wrap = IG_GFX_TEXTURE_WRAP_CLAMP;

		registry = NULL;
	}

	igSceneInfoRef sceneInfo = igSceneInfo::instantiateRef();
	igAttrSetRef sceneGraph = igAttrSet::instantiateRef();
	igTextureAttrRef textureAttribute = igTextureAttr::instantiateRef();
	igIGBFileRef file = igIGBFile::instantiateRef();

	textureAttribute->setImage(image);
	textureAttribute->setMagnificationFilter(filter);
	textureAttribute->setMinificationFilter(filter);
	textureAttribute->setWrapS(wrap);
	textureAttribute->setWrapT(wrap);
	sceneGraph->appendAttr(textureAttribute);

	sceneInfo->setSceneGraph(sceneGraph);
	sceneInfo->updateTextureList();
	file->appendInfo(sceneInfo);
	file->writeFile(outputFile);

	if (file->isEmpty())
	{
		fprintf(stderr, "Failed to write file %s\n", outputFile);
		return -2;
	}

	return 0;
}