#include "ShaderPair.hpp"

namespace Ge
{
	ShaderPair::ShaderPair(std::string f, std::string v, bool b,bool m, bool t)
	{
		this->Frag = f;
		this->Vert = v;
		this->back = b;
		this->multiSampling = m;
		this->transparency = t;
	}

	ShaderPair::ShaderPair(){}
}