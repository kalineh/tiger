#include "matrix.h"

namespace funk
{
	
matrix matrix::inverse3x3() const
{
	float invdet = determinant3x3();

	if ( invdet == 0.f ) return matrix::Identity();

	invdet = 1.f / invdet;

	return matrix( v4( (    y.y * z.z - z.y * y.z   ) * invdet,
					   ( -( x.y * z.z - z.y * x.z ) ) * invdet,
					   (    x.y * y.z - y.y * x.z   ) * invdet, 0.f ),
				   v4( ( -( y.x * z.z - z.x * y.z ) ) * invdet,
					   (    x.x * z.z - z.x * x.z   ) * invdet,
					   ( -( x.x * y.z - y.x * x.z ) ) * invdet, 0.f ),
				   v4( (    y.x * z.y - z.x * y.y )   * invdet,
					   ( -( x.x * z.y - z.x * x.y ) ) * invdet,
					   (    x.x * y.y - y.x * x.y   ) * invdet, 0.f ),
				   v4(0.0f,0.0f,0.0f,1.0f) );
}

matrix matrix::inverseRT() const
{
	v3 translationInv = -getTranslation();

	matrix result = *this;
	result.cancelTranslation();
	result = result.transpose();

	result.w.x = translationInv.x;
	result.w.y = translationInv.x;
	result.w.z = translationInv.x;

	return result;
}

matrix matrix::LookAt( const v3 eyePos, const v3 target, const v3 up )
{
	v3 vecDir = normalize(target-eyePos);
	v3 vecRight = normalize(cross( up, vecDir ));
	v3 vecUp = normalize( cross(vecDir, vecRight) );

	return matrix( v4(vecRight), v4(vecUp), v4(vecDir), v4(0.0f,0.0f,0.0f,1.0f) );
}

}