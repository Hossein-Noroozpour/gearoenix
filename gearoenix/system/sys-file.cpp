#include "sys-file.hpp"
#include "sys-app.hpp"
#include "sys-log.hpp"
#ifdef IN_IOS
#import <Foundation/Foundation.h>
//#import "../apple/sys-apl.mm"
#elif defined(IN_ANDROID)
#include <android_native_app_glue.h>
#endif

void gearoenix::system::File::check_endian_compatibility()
{
    unsigned int system_endian = 1;
    uint8_t resource_endian;
    read(resource_endian);
    is_endian_compatible = (resource_endian == ((uint8_t*)(&system_endian))[0]);
    //    LOGE(std::string("endian is: ") + std::to_string(is_endian_compatible));
}

gearoenix::system::File::File(system::Application* sys_app, const std::string& name)
#ifdef USE_STD_FILE
    : sys_app(sys_app)
{
    std::string file_path = name;
#ifdef IN_IOS
    @autoreleasepool {
//        NSString *f_name = [NSString stringWithCString:name.c_str() encoding:[NSString defaultCStringEncoding]];
        NSString *path = [[NSBundle mainBundle] pathForResource:@"data" ofType:@"gx3d"];
        file_path = std::string([path fileSystemRepresentation]);
    }
#endif
    file.open(file_path, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        LOGF("Error in opening assets file.");
    }
#elif defined(IN_ANDROID)
{
    file = AAssetManager_open(sys_app->get_android_app()->activity->assetManager,
        "data.gx3d", AASSET_MODE_BUFFER);
    if (file == nullptr) {
        LOGF(std::string("asset not found!"));
    }
#else
#error "Unimplemented!"
#endif
    check_endian_compatibility();
}

gearoenix::system::File::~File() {}

bool gearoenix::system::File::get_endian_compatibility() const
{
    return is_endian_compatible;
}

unsigned int gearoenix::system::File::read(void* data, size_t length)
{
#ifdef IN_ANDROID
    return static_cast<unsigned int>(AAsset_read(file, data, length));
#elif defined(USE_STD_FILE)
    file.read(reinterpret_cast<char*>(data), length);
    return static_cast<unsigned int>(file.gcount());
#else
#error "Error not implemented yet!"
#endif
}

void gearoenix::system::File::seek(unsigned int offset)
{
#if defined(USE_STD_FILE)
    file.seekg(offset, std::ios::beg);
#elif defined(IN_ANDROID)
    AAsset_seek(file, offset, SEEK_SET);
#else
#error "Error not implemented yet!"
#endif
}

unsigned int gearoenix::system::File::tell()
{
#if defined(USE_STD_FILE)
    return file.tellg();
#elif defined(IN_ANDROID)
#error "Not implemented yet"
#else
#error "Error not implemented yet!"
#endif
}
