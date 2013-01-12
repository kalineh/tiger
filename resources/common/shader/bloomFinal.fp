#version 120

uniform sampler2D texRaw;

uniform sampler2D texBlur0; // sharpest
uniform sampler2D texBlur1;
uniform sampler2D texBlur2;
uniform sampler2D texBlur3; // blurrest
uniform sampler2D texLensBlur; // blurrest

uniform vec4 data;
uniform vec4 lensFlareData;
uniform vec4 stepSize;
uniform vec4 randData;

vec3 ToneMap(vec3 color, float exposure, float whiteLevel)
{
	// Perform tone-mapping
	float Y = dot(vec3(0.30, 0.59, 0.11), color);
	float YD = exposure * (exposure/2.0f + 1.0) / (exposure + 1.0);
	color *= YD;
	
	return color;	
}

vec3 Uncharted2Tonemap(vec3 x)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;

   return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

float lumin(vec3 color)
{
	return dot( color, vec3( 0.3f, 0.59f, 0.11f ) );
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void)
{
	vec2 uv = gl_TexCoord[0].xy;	
	vec3 diffuse = texture2D(texRaw, uv).xyz;

	//vec3 luminVec = vec3(0.3f, 0.59f, 0.11f );
	//float lumin = dot( diffuse, luminVec );
	//diffuse = mix ( diffuse, vec3(1.0f,1.0f,0.0f), 0.2f * pow( 2.71f, -pow(lumin*5.0f-1.5f,2.0f)) );
	
	// 4 bloom textures
	vec3 bloom0 =  texture2D(texBlur0, uv).xyz;
	vec3 bloom1 =  texture2D(texBlur1, uv).xyz;
	vec3 bloom2 =  texture2D(texBlur2, uv).xyz;
	vec3 bloom3 =  texture2D(texBlur3, uv).xyz;
	vec3 bloomLensFlare =  texture2D(texLensBlur, uv).xyz;
	
	// weight the 4 bloom textures
	float bloomCoeff = data.x;
	vec4 bloomWeights = vec4(0.25f);
	vec3 bloom = (bloom0*bloomWeights.x + bloom1*bloomWeights.y + bloom2*bloomWeights.z + bloom3*bloomWeights.w)*bloomCoeff;
	
	// lens flare
	float lensFlareAmmount = lensFlareData.x;
	float lensFlarePower = lensFlareData.y;
	vec3 lensFlarePow;
	lensFlarePow.x = pow(bloomLensFlare.x, lensFlarePower);
	lensFlarePow.y = pow(bloomLensFlare.y, lensFlarePower);
	lensFlarePow.z = pow(bloomLensFlare.z, lensFlarePower);
	bloom += max(vec3(0.0f),lensFlarePow*lensFlareAmmount);
	
	
	// THIS PART IS JUST A HACK TO TEST THE STYLES FOR JAPAN DEMO
	{
		/*// chromatic aberation
		vec2 uvStep = vec2(0.002f, 0.0025f);
		vec3 diffuseL = texture2D(texRaw, uv - vec2(uvStep.x,0.0f) ).xyz;
		vec3 diffuseR = texture2D(texRaw, uv + vec2(uvStep.x,0.0f) ).xyz;
		vec3 diffuseT = texture2D(texRaw, uv - vec2(0.0f,uvStep.y) ).xyz;
		vec3 diffuseB = texture2D(texRaw, uv + vec2(0.0f,uvStep.y) ).xyz;
		
		vec3 chromaticL = mix( vec3(0.0f), vec3(0.0f,1.0f,1.0f), lumin(diffuseL));
		vec3 chromaticR = mix( vec3(0.0f), vec3(1.0f,0.0f,1.0f), lumin(diffuseR));
		vec3 chromaticT = mix( vec3(0.0f), vec3(1.0f,1.0f,0.0f), lumin(diffuseT));
		vec3 chromaticB = mix( vec3(0.0f), vec3(1.0f,0.0f,1.0f), lumin(diffuseB));
		
		diffuse = max(diffuse,max(chromaticT,(max(chromaticR,max(chromaticB, chromaticL) ) ) ) );
		*/
		
		float lumin = dot( diffuse, vec3( 0.3f, 0.59f, 0.11f ) );
		diffuse = mix( vec3(0.10f,0.10f,0.05f), diffuse, lumin );  
		
		// test
		//bloom *= mix( 0.8f,1.0f,sin(uv.y*1000.0f)*0.5f+0.5f);
		float noise = rand(uv + randData.xy );
		diffuse += mix(-0.0f,0.1f,noise);
		bloom *= mix(0.9f,1.2f,noise);
	}
	
	// tone-mapping
	float hdrExposure = data.y;
	float hdrWhiteLevel = data.z;
	vec3 diffuseToneMapped = ToneMap(diffuse, hdrExposure, hdrWhiteLevel );	

	
	float vignette = smoothstep(-0.7f,0.5f, 1.0f - length(uv*2.0f-vec2(1.0f)));
 
	gl_FragColor = vec4(diffuseToneMapped*vignette+bloom,1.0f);
	//gl_FragColor = vec4(diffuseToneMapped+bloom,1.0f);
	//gl_FragColor = vec4(diffuseToneMapped,1.0f);
	//gl_FragColor = vec4( ( (vec3(1.0f)-bloom)*diffuse + bloom)*vignette, 1.0f );
	//gl_FragColor = vec4(vignette,vignette,vignette,1.0f);
}