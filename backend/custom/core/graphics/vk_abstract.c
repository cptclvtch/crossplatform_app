
const char** get_vk_extension_overlap(uint32_t* count, uint32_t use_all_available_extensions)
{
    VkResult error;
    
    //retrieve device extensions
    uint32_t device_count;

    error = vkEnumerateInstanceExtensionProperties(NULL, &device_count, NULL);
    if(error != VK_SUCCESS)
        return NULL;

    VkExtensionProperties* extension_properties = ALLOCATE_MEMORY(device_count, VkExtensionProperties);
    error = vkEnumerateInstanceExtensionProperties(NULL, &device_count, extension_properties);
    
    if(error != VK_SUCCESS)
    {
        PRINT_FN("Can't enumerate extensions. Error: %u\n", error);
        DEALLOCATE_MEMORY(extension_properties, device_count);
        return NULL;
    }

    //check and write down indices of overlapped extensions
    #include "desired_vk_extensions.c"

    uint32_t overlapped_count = 0;
    uint32_t desired_extension, device_extension;
    uint32_t* overlapped_indices = ALLOCATE_MEMORY(device_count, uint32_t);
    uint32_t desired_extensions_count = sizeof(desired_vk_extensions)/sizeof(const char*);

    for(device_extension = 0; device_extension < device_count; device_extension++)
    {
        if(use_all_available_extensions)
        {
            overlapped_indices[overlapped_count] = device_extension; 
            overlapped_count++;
            continue;
        }
        else
        for(desired_extension = 0; desired_extension < desired_extensions_count; desired_extension++)
        {    
            if(!strcmp(desired_vk_extensions[desired_extension], extension_properties[device_extension].extensionName))
            {
                overlapped_indices[overlapped_count] = device_extension; 
                overlapped_count++;
                continue;
            }
        }
    }

    //copy extension strings and return them
    PRINT_FN("Desired extensions overlap (%u/%u):\n",overlapped_count, desired_extensions_count);
    const char** overlapped_extensions = ALLOCATE_MEMORY(overlapped_count, const char*);
    for(desired_extension = 0; desired_extension < overlapped_count; desired_extension++)
    {
        overlapped_extensions[desired_extension] = extension_properties[overlapped_indices[desired_extension]].extensionName;
        PRINT_FN("%s\n",overlapped_extensions[desired_extension]);
    }
    DEALLOCATE_MEMORY(overlapped_indices, device_count);
    DEALLOCATE_MEMORY(extension_properties, device_count);

    *count = overlapped_count;
    return overlapped_extensions;
}

VkResult create_vk_instance()
{
    PRINT_FN("Creating Vulkan instance.\n");
    VkResult error;
    VkInstance instance;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    //optional info
    {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = APP_NAME;
        appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        createInfo.pApplicationInfo = &appInfo;
    }

    //set extensions
    uint32_t extension_count;
    //#warning Make sure extension names memory gets appropriately deallocated.
    const char** overlapped_extensions = get_vk_extension_overlap(&extension_count, 0);
    char extension[] = "VK_KHR_surface";
    createInfo.ppEnabledExtensionNames = &extension;//overlapped_extensions;
    createInfo.enabledExtensionCount = extension_count;

    //set validation layers
    createInfo.enabledLayerCount = 0;

    PRINT_FN("Crashes right here.\n");
    //create
    error = vkCreateInstance(&createInfo, NULL, &instance);
    PRINT_FN("Nevermind!\n");
    DEALLOCATE_MEMORY(overlapped_extensions, extension_count);

    if(error != VK_SUCCESS)
    {
        PRINT_FN("Can't create instance. Error: %u\n", error);
        return error;
    }
    return VK_SUCCESS;
}
