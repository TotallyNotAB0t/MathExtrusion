"%VULKAN_SDK%\Bin\glslc.exe" shader.vert -o vert.spv
"%VULKAN_SDK%\Bin\glslc.exe" shader.frag -o frag.spv
"%VULKAN_SDK%\Bin\glslc.exe" shader_unlit.vert -o vert_unlit.spv
"%VULKAN_SDK%\Bin\glslc.exe" shader_unlit.frag -o frag_unlit.spv
"%VULKAN_SDK%\Bin\glslc.exe" skybox_vs.vert -o skybox_vs.spv
"%VULKAN_SDK%\Bin\glslc.exe" skybox_fs.frag -o skybox_fs.spv
"%VULKAN_SDK%\Bin\glslc.exe" Shadow.vert -o Shadow_vs.spv
"%VULKAN_SDK%\Bin\glslc.exe" Shadow.frag -o Shadow_fs.spv
"%VULKAN_SDK%\Bin\glslc.exe" shader_mirror.vert -o mirror_vs.spv
"%VULKAN_SDK%\Bin\glslc.exe" shader_mirror.frag -o mirror_fs.spv
"%VULKAN_SDK%\Bin\glslc.exe" compute_shader.comp -o compute_shader.comp.spv
pause

