if(assimp_POPULATED)
	target_link_libraries(Engine PRIVATE assimp)
    target_compile_definitions(Engine PRIVATE ASSIMP_LOADED=1)
endif()