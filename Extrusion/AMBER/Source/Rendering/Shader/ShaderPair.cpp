#include "ShaderPair.hpp"

namespace Ge
{
	ShaderPair::ShaderPair(std::string f, std::string v, bool b,bool m)
	{
		this->Frag = f;
		this->Vert = v;
		this->back = b;
		this->multiSampling = m;
	}

	ShaderPair::ShaderPair(){}
}