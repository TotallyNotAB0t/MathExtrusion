#ifndef __ENGINE_SHADER_PAIR__
#define __ENGINE_SHADER_PAIR__

#include <iostream>
namespace Ge
{
	class ShaderPair
	{
	public:
		ShaderPair(std::string f, std::string v,bool b,bool m = true);
		ShaderPair();
		std::string Frag;
		std::string Vert;
		bool back;
		bool multiSampling = true;
	};
}

#endif //__ENGINE_SHADER_PAIR__