#pragma once


namespace BlendMode
{
	enum Type
	{
		SourceAlpha,
		// Use the source alpha value integrally in output

		DestAlpha,
		// Use the destination alpha value integrally in the output

		Copy,
		// No blending, perform a raw copy
	};
}


