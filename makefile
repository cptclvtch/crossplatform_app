windows: windows/main.c
	tcc -Wl,-subsystem=console windows/main.c -o windows/build/main.exe -I apis/VulkanSDK/1.3.231.1/Include -L apis/VulkanSDK/1.3.231.1/Lib -lvulkan-1

windows_debug: windows/main.c
	tcc -Wl,-subsystem=console windows/main.c -o windows/build/main.exe -I apis/VulkanSDK/1.3.231.1/Include -L apis/VulkanSDK/1.3.231.1/Lib -lvulkan-1 -g

run_windows: windows
	./windows/build/main.exe

run_windows_debug: windows_debug
	gdb ./windows/build/main.exe