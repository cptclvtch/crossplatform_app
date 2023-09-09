const char* desired_vk_extensions[] = {
                                        "VK_KHR_surface",
                                        #ifdef _WIN32
                                        "VK_KHR_win32_surface"
                                        #endif
                                        };