#pragma once

#include <core/engine/enginesubsystem.hpp>
#include <core/compute/program.hpp> // Kernel, Buffer
#include <core/filesystem/resource.hpp> // basic_resource
#include <utility>
#include <core/data/image.hpp>

/**
 * @file context.hpp
 */


namespace legion::core::compute
{
    /** @class Context
     *  @brief Wraps a cl_context in a neater interface.
     */
    class Context : public EngineSubSystem<Context>
    {
        AllowPrivateOnInit;
        AllowPrivateOnShutdown;
        SubSystemInstance(Context);
    private:
        /**
         * @brief Initializes the OpenCL context.
         */
        static void onInit();

        /**
         * @brief Destroys the OpenCL context.
         */
        static void onShutdown();

    public:
        /**
         * @brief Creates a Program from a basic resource (can be file or buffer).
         * @param resource The Buffer you want to create the Program from
         *         You can for instance use fs::view.get()
         * @return A Program Object that supports execution
         */
        static Program createProgram(const filesystem::basic_resource& resource);


        /**
         * @brief Creates an OpenCL Native Buffer (GPU or similar) with a host buffer.
         * @param container A container filled with any arbitrary Data, you must make
         *         sure that the alignment corresponds to the Device Buffer.
         * @param type The type of Buffer you want to create can be either READ, WRITE
         *         or READ | WRITE @see buffer_type
         * @param name (optional) The name of the Buffer, this is useful for
         *         automatically entering the buffer into a kernel
         * @return a Native Buffer Object
        */
        template <class T>
        static Buffer createBuffer(std::vector<T>& container, buffer_type type, std::string name = "")
        {
            return Buffer(
                instance.m_context,
                reinterpret_cast<byte*>(container.data()), // data as void-ptr
                container.size() * sizeof(T),  // size with data as void-ptr
                type,
                std::forward<std::string>(name)
            );
        }

        static Buffer createBuffer(byte* data, size_type size, buffer_type type, std::string name = "")
        {
            return Buffer(instance.m_context, data, size, type, std::forward<std::string>(name));
        }

        static Buffer createImage(image& img, buffer_type type, std::string name = "")
        {
            size_type width = static_cast<size_type>(img.resolution().x);
            size_type height = static_cast<size_type>(img.resolution().y);
            size_type depth = 0;

            cl_image_format fmt;

            switch (img.format()) {
            case channel_format::eight_bit: fmt.image_channel_data_type = CL_UNORM_INT8; break;
            case channel_format::sixteen_bit: fmt.image_channel_data_type = CL_UNORM_INT16; break;
            case channel_format::float_hdr: fmt.image_channel_data_type = CL_FLOAT; break;
            case channel_format::depth_stencil:
            default:
            {
                log::warn("Buffer::createImage invalid Image format!");
                fmt.image_channel_data_type = CL_UNORM_INT8;
            }
            }

            switch (img.components())
            {
            case image_components::grey: fmt.image_channel_order = CL_R; break;
            case image_components::grey_alpha: fmt.image_channel_order = CL_RA; break;
            case image_components::rgb: fmt.image_channel_order = CL_RGB; break;
            case image_components::rgba: fmt.image_channel_order = CL_RGBA; break;
            case image_components::depth:
            case image_components::depth_stencil:
            case image_components::stencil:
            default:
            {
                log::warn("Buffer::createImage invalid Image Components!");
                fmt.image_channel_order = CL_RGBA;
            }
            }

            return Buffer(instance.m_context, img.data(), width, height, depth, CL_MEM_OBJECT_IMAGE2D, &fmt, type, name);
        }

        static Buffer createImageFromOpenGLImage(uint target, uint texture, buffer_type type, std::string name = "", uint mip_level = 0)
        {
            return Buffer(instance.m_context, target, texture, mip_level, type, std::move(name));
        }

        static Buffer createImageFromOpenGLBuffer(uint bufferid, buffer_type type, std::string name = "")
        {
            return Buffer(instance.m_context, bufferid, type, false, std::move(name));
        }
        static Buffer createImageFromOpenGLRenderBuffer(uint bufferid, buffer_type type, std::string name = "")
        {
            return Buffer(instance.m_context, bufferid, type, true, std::move(name));
        }


        /**
         * @brief Returns the OpenCL device-id associated with this context
         */
        static cl_device_id getDeviceId()
        {
            return instance.m_device_id;
        }

    private:
        cl_context m_context = nullptr;
        cl_platform_id m_platform_id = nullptr;
        cl_device_id m_device_id = nullptr;
    };

    ReportSubSystem(Context);
}
